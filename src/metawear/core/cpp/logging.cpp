#include "metawear/core/logging.h"

#include "metawear/core/datasignal.h"

#include "datasignal_private.h"
#include "logging_private.h"
#include "logging_register.h"
#include "metawearboard_def.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <stack>
#include <unordered_map>
#include <tuple>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::time_point;
using std::forward_as_tuple;
using std::free;
using std::find_if;
using std::malloc;
using std::memcpy;
using std::min;
using std::piecewise_construct;
using std::queue;
using std::stack;
using std::unordered_map;
using std::vector;

const uint8_t REVISION_EXTENDED_LOGGING= 2, ENTRY_ID_MASK= 0x1f, LOG_ENTRY_SIZE= (uint8_t) sizeof(uint32_t);
const float TICK_TIME_STEP= (48.f / 32768.f) * 1000.f;         ///< milliseconds

class Loggable {
public:
    Loggable(MblMwDataSignal* source, MblMwFnData data_handler, MblMwFnVoid logger_ready);

    void add_entry_id(uint8_t id);
    void process_log_data(const MblMwMetaWearBoard* board, uint8_t id, int64_t epoch, uint32_t data);

    MblMwDataSignal *source;
    uint8_t n_req_entries;
private:
    MblMwFnData data_handler;
    MblMwFnVoid logger_ready;
    vector<uint8_t> entry_ids;
    unordered_map<uint8_t, queue<uint32_t>> entries;
};

struct TimeReference {
    time_point<system_clock> timestamp;
    uint32_t tick;
    uint8_t reset_uid;

    TimeReference(uint32_t tick, uint8_t reset_uid);
};

struct LoggerState {
    unordered_map<uint8_t, TimeReference> log_time_references;
    unordered_map<uint8_t, uint32_t> latest_tick;
    unordered_map<uint8_t, Loggable*> data_loggers;
    vector<Loggable> spawned_loggers;
    size_t pending_index;

    LoggerState();
};

static int32_t logging_response_entry_id_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state= (LoggerState*) board->logger_state;
    state->spawned_loggers.at(state->pending_index).add_entry_id(response[2]);
    return 0;
}

static int32_t logging_response_length_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint8_t payload_size= len - 2;

    memcpy(&board->n_log_entries, response + 2, payload_size);
    uint32_t n_entries_notify= board->n_log_entries * board->log_download_notify_progress;

    vector<uint8_t> command= { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT) };
    command.insert(command.end(), response + 2, response + len);
    command.insert(command.end(), (uint8_t*) &n_entries_notify, (uint8_t*) &n_entries_notify + 4);
    send_command(board, command.data(), (uint8_t) command.size());
    
    return 0;
}

static int32_t logging_response_readout_notify(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto parse_response= [&board, &response](uint8_t offset) -> void {
        auto state= (LoggerState*) board->logger_state;
        uint8_t entry_id= response[offset] & ENTRY_ID_MASK, reset_uid= (response[offset] & ~ENTRY_ID_MASK) >> 5;
        uint32_t entry_tick, data;

        memcpy(&entry_tick, response + offset + 1, 4);
        TimeReference reference= state->log_time_references.at(reset_uid);
        auto timestamp_copy(reference.timestamp);
        milliseconds time_offset((int64_t) ((entry_tick - reference.tick) * TICK_TIME_STEP));
        timestamp_copy+= time_offset;

        memcpy(&data, response + offset + 5, 4);

        if (!state->latest_tick.count(entry_id) || state->latest_tick.at(entry_id) < entry_tick) {
            state->latest_tick[entry_id]= entry_tick;
            if (state->data_loggers.count(entry_id)) {
                state->data_loggers.at(entry_id)->process_log_data(board, entry_id, 
                        duration_cast<milliseconds>(timestamp_copy.time_since_epoch()).count(), data);
            } else if (board->log_download_handler.received_unknown_entry != nullptr) {
                board->log_download_handler.received_unknown_entry(entry_id, 
                         duration_cast<milliseconds>(timestamp_copy.time_since_epoch()).count(), (const uint8_t*) &data, sizeof(data));
            }
        }
    };

    parse_response(2);
    if (len == 20) {
        parse_response(11);
    }

    return 0;
}

static int32_t logging_response_readout_progress(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    if (board->log_download_handler.received_progress_update != nullptr) {
        uint32_t entries_left;
        memcpy(&entries_left, response + 2, min(len - 2, 4));

        board->log_download_handler.received_progress_update(entries_left, board->n_log_entries);
    }
    return 0;
}

static int32_t logging_response_page_completed(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint8_t command[2]= { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PAGE_CONFIRM) };
    SEND_COMMAND;
    return 0;
}

TimeReference::TimeReference(uint32_t tick, uint8_t reset_uid) : tick(tick), reset_uid(reset_uid) {
    timestamp= system_clock::now();
}

Loggable::Loggable(MblMwDataSignal* source, MblMwFnData data_handler, MblMwFnVoid logger_ready) : 
        source(source), n_req_entries((source->length() - 1) / sizeof(uint32_t) + 1), data_handler(data_handler), logger_ready(logger_ready) {
}

LoggerState::LoggerState() : pending_index(0) {

}


void Loggable::add_entry_id(uint8_t id) {
    entry_ids.push_back(id);
    entries.emplace(id, queue<uint32_t>());

    if ((uint8_t) entry_ids.size() == n_req_entries) {
        auto state= (LoggerState*) source->owner->logger_state;
        state->pending_index++;

        for(auto it: entry_ids) {
            state->data_loggers[it]= this;
        }
        logger_ready();
    }
}

void Loggable::process_log_data(const MblMwMetaWearBoard* board, uint8_t id, int64_t epoch, uint32_t data) {
    entries.at(id).push(data);
    
    bool ready= true;
    for(auto it: entries) {
        ready&= !it.second.empty();
    }
    if (ready) {
        vector<uint8_t> merged;
        for(auto it: entry_ids) {
            uint8_t *data= (uint8_t*) &entries.at(it).front();
            merged.insert(merged.end(), data, data + 4);
            entries.at(it).pop();
        }

        MblMwData* data = data_response_converters.at(source->interpreter)(board, merged.data(), (uint8_t) merged.size());
        data->epoch= epoch;

        data_handler(data);

        free(data->value);
        free(data);
    }
}

void init_logging(MblMwMetaWearBoard *board) {
    board->logger_state= new LoggerState;
    board->responses.emplace(LOGGING_RESPONSE_TRIGGER_HEADER, logging_response_entry_id_received);
    board->responses.emplace(LOGGING_RESPONSE_LENGTH_HEADER, logging_response_length_received);
    board->responses.emplace(LOGGING_RESPONSE_NOTIFY_HEADER, logging_response_readout_notify);
    board->responses.emplace(LOGGING_RESPONSE_PROGRESS_HEADER, logging_response_readout_progress);
    board->responses.emplace(LOGGING_RESPONSE_PAGE_COMPLETED_HEADER, logging_response_page_completed);
}

void tear_down_logging(MblMwMetaWearBoard *board) {
    delete (LoggerState*) board->logger_state;
}

void received_time_data(MblMwMetaWearBoard *board, const uint8_t* value, uint8_t length) {
    auto state= (LoggerState*) board->logger_state;
    uint32_t tick;

    memcpy(&tick, value + 2, 4);
    state->log_time_references.emplace(piecewise_construct, forward_as_tuple(value[6]), forward_as_tuple(tick, value[6]));
}

void mbl_mw_logging_start(const MblMwMetaWearBoard* board, uint8_t overwrite) {
    uint8_t command[3]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::CIRCULAR_BUFFER), (uint8_t) (overwrite ? 1 : 0)};
    SEND_COMMAND;

    command[1]= ORDINAL(LoggingRegister::ENABLE);
    command[2]= 0x1;
    SEND_COMMAND;
}

void mbl_mw_logging_stop(const MblMwMetaWearBoard* board) {
    uint8_t command[3]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::ENABLE), 0};
    SEND_COMMAND;
}

void mbl_mw_logging_clear_entries(const MblMwMetaWearBoard* board) {
    if (board->module_info.at(MBL_MW_MODULE_LOGGING).revision == REVISION_EXTENDED_LOGGING) {
        uint8_t command[3]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PAGE_COMPLETED), 1};
        SEND_COMMAND;
    }

    uint8_t command[6]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::REMOVE_ENTRIES), 0xff, 0xff, 0xff, 0xff};
    SEND_COMMAND;
}

void mbl_mw_logging_download(MblMwMetaWearBoard* board, uint8_t n_notifies, const MblMwLogDownloadHandler* handler) {
    board->log_download_notify_progress= n_notifies ? 1.0 / n_notifies : 0;

    uint8_t command[3]= {MBL_MW_MODULE_LOGGING};
    if (board->module_info.at(MBL_MW_MODULE_LOGGING).revision == REVISION_EXTENDED_LOGGING) {
        command[1]= ORDINAL(LoggingRegister::READOUT_PAGE_COMPLETED);
        command[2]= 0x1;
        SEND_COMMAND;
    }
    
    command[1]= ORDINAL(LoggingRegister::READOUT_NOTIFY);
    command[2]= 0x1;
    SEND_COMMAND;

    if (handler != nullptr) {
        memcpy(&board->log_download_handler, handler, sizeof(MblMwLogDownloadHandler));

        command[1]= ORDINAL(LoggingRegister::READOUT_PROGRESS);
        command[2]= 0x1;
        SEND_COMMAND;
    } else {
        board->log_download_handler.received_progress_update= nullptr;
        board->log_download_handler.received_unknown_entry= nullptr;
    }

    command[1]= READ_REGISTER(ORDINAL(LoggingRegister::LENGTH));
    send_command(board, command, 2);
}

void mbl_mw_datasignal_log(MblMwDataSignal *signal, MblMwFnData received_data, MblMwFnVoid logger_ready) {
    auto state= (LoggerState*) signal->owner->logger_state;
    uint8_t remainder= signal->length();

    state->spawned_loggers.emplace_back(signal, received_data, logger_ready);

    auto entries= state->spawned_loggers.back().n_req_entries;
    for(uint8_t i= 0; i < entries; i++, remainder-= LOG_ENTRY_SIZE) {
        uint8_t entry_size= min(remainder, LOG_ENTRY_SIZE), entry_offset= LOG_ENTRY_SIZE * i + signal->offset;
        uint8_t command[6]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::TRIGGER), signal->header.module_id, signal->header.register_id, 
                signal->header.data_id, (uint8_t) ((entry_size - 1) << 5 | entry_offset)};
        SEND_COMMAND_BOARD(signal->owner);
    }
}

void mbl_mw_datasignal_remove_logger(MblMwDataSignal *signal) {
    queue<uint8_t> logger_ids;
    auto state= (LoggerState*) signal->owner->logger_state;

    for(auto it: state->data_loggers) {
        if (it.second->source == signal) {
            uint8_t command[3]= { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::REMOVE_ENTRIES), it.first };
            SEND_COMMAND_BOARD(signal->owner);

            logger_ids.push(it.first);
        }
    }

    while(!logger_ids.empty()) {
        uint8_t id= logger_ids.front();

        auto pos= find_if(state->spawned_loggers.begin(), state->spawned_loggers.end(), [&state, &id](const Loggable& elem) -> bool {
            return ((void*) &elem == (void*) state->data_loggers.at(id));
        });
        if (pos != state->spawned_loggers.end()) {
            state->spawned_loggers.erase(pos);
        }

        logger_ids.pop();
    }

    state->pending_index= state->spawned_loggers.size();
}
