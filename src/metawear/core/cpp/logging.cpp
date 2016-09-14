#include "metawear/core/logging.h"
#include "metawear/core/datasignal.h"
#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/platform/cpp/async_creator.h"
#include "metawear/platform/cpp/threadpool.h"

#include "datasignal_private.h"
#include "logging_private.h"
#include "logging_register.h"
#include "metawearboard_def.h"
#include "register.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::time_point;
using std::forward_as_tuple;
using std::free;
using std::find_if;
using std::make_shared;
using std::malloc;
using std::memcpy;
using std::min;
using std::piecewise_construct;
using std::queue;
using std::static_pointer_cast;
using std::unordered_map;
using std::unordered_set;
using std::vector;

#define GET_LOGGER_STATE(board) static_pointer_cast<LoggerState>(board->logger_state)

const uint8_t REVISION_EXTENDED_LOGGING= 2, ENTRY_ID_MASK= 0x1f, RESET_UID_MASK= 0x7, LOG_ENTRY_SIZE= (uint8_t) sizeof(uint32_t);
const float TICK_TIME_STEP= (48.f / 32768.f) * 1000.f;         ///< milliseconds

static int32_t logging_response_readout_progress(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);

struct MblMwDataLogger {
    MblMwDataLogger(uint8_t** state_stream, MblMwMetaWearBoard* board);
    MblMwDataLogger(MblMwDataSignal* source, MblMwFnDataLoggerPtr logger_ready);

    void add_entry_id(uint8_t id);
    void process_log_data(const MblMwMetaWearBoard* board, uint8_t id, int64_t epoch, uint32_t data);
    void serialize(vector<uint8_t>& state) const;

    inline uint8_t get_id() const {
        return entry_ids.front();
    }

    MblMwDataSignal *source;
    uint8_t n_req_entries;
    MblMwFnData data_handler;
    MblMwFnDataLoggerPtr logger_ready;
    vector<uint8_t> entry_ids;
    unordered_map<uint8_t, queue<uint32_t>> entries;
};

struct TimeReference {
    time_point<system_clock> timestamp;
    uint32_t tick;
    uint8_t reset_uid;

    TimeReference(uint8_t** state_stream);
    TimeReference(uint32_t tick, uint8_t reset_uid);
    void serialize(vector<uint8_t>& state) const;
};

struct LoggerState : public AsyncCreator {
    unordered_map<uint8_t, TimeReference> log_time_references;
    unordered_map<uint8_t, uint32_t> latest_tick;
    unordered_map<uint8_t, MblMwDataLogger*> data_loggers;
    MblMwDataLogger* next_logger;
    MblMwLogDownloadHandler log_download_handler;
    float log_download_notify_progress;
    uint32_t n_log_entries;


    LoggerState();

    void clear_data_loggers();
};

static int32_t logging_response_time_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    if (board->initialized_timeout != nullptr) {
        board->initialized_timeout->cancel();
    }

    auto state = GET_LOGGER_STATE(board);
    uint32_t tick;

    memcpy(&tick, response + 2, 4);
    state->log_time_references.erase(response[6]);
    state->log_time_references.emplace(piecewise_construct, forward_as_tuple(response[6]), forward_as_tuple(tick, response[6]));

    board->initialized(board, MBL_MW_STATUS_OK);
    return MBL_MW_STATUS_OK;
}

static int32_t logging_response_entry_id_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    GET_LOGGER_STATE(board)->next_logger->add_entry_id(response[2]);
    return 0;
}

static int32_t logging_response_length_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state= GET_LOGGER_STATE(board);
    uint8_t payload_size= len - 2;
    
    memcpy(&state->n_log_entries, response + 2, payload_size);
    
    // If there are no entires we won't get any responses, so end the download now
    // by forcing a callback on the readout progress with 0 remaining entries
    if (state->n_log_entries == 0) {
        // Just recycle the response buffer since we know it a 32bit 0 entry
        return logging_response_readout_progress(board, response, len);
    }
    
    uint32_t n_entries_notify= state->n_log_entries * state->log_download_notify_progress;
    vector<uint8_t> command= { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT) };
    command.insert(command.end(), response + 2, response + len);
    command.insert(command.end(), (uint8_t*) &n_entries_notify, (uint8_t*) &n_entries_notify + 4);
    send_command(board, command.data(), (uint8_t) command.size());
    
    return 0;
}

static TimeReference mbl_mw_logger_lookup_reset_uid(const MblMwMetaWearBoard* board, uint8_t reset_uid) {
    auto logger_state = GET_LOGGER_STATE(board);
    if (logger_state->log_time_references.count(reset_uid)) {
        return logger_state->log_time_references.at(reset_uid);
    }
    // No valid reset uid time base found.  This means we had multiple unexpected resets,
    // so come up with a best guess by working backwards to find a previous reset uid base.
    for (uint8_t prev_uid = (reset_uid - 1) & RESET_UID_MASK; prev_uid != reset_uid; prev_uid = (prev_uid - 1) & RESET_UID_MASK) {
        if (logger_state->log_time_references.count(prev_uid)) {
            // Copy the previous one
            TimeReference prev_reference = logger_state->log_time_references.at(prev_uid);
            prev_reference.reset_uid = reset_uid;
            logger_state->log_time_references.emplace(reset_uid, prev_reference);
            return logger_state->log_time_references.at(reset_uid);
        }
    }
    // Nothing to go on just create a new one
    logger_state->log_time_references.emplace(piecewise_construct, forward_as_tuple(reset_uid), 
        forward_as_tuple(reset_uid, 0));
    return logger_state->log_time_references.at(reset_uid);
}

static int32_t logging_response_readout_notify(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto parse_response= [&board, &response](uint8_t offset) -> void {
        auto state= GET_LOGGER_STATE(board);
        uint8_t entry_id= response[offset] & ENTRY_ID_MASK, reset_uid= (response[offset] & ~ENTRY_ID_MASK) >> 5;
        uint32_t entry_tick, data;

        memcpy(&entry_tick, response + offset + 1, 4);
        TimeReference reference = mbl_mw_logger_lookup_reset_uid(board, reset_uid);
        auto timestamp_copy(reference.timestamp);
        milliseconds time_offset((int64_t) ((int32_t)(entry_tick - reference.tick) * TICK_TIME_STEP));
        timestamp_copy+= time_offset;

        memcpy(&data, response + offset + 5, 4);

        if (!state->latest_tick.count(entry_id) || state->latest_tick.at(entry_id) < entry_tick) {
            state->latest_tick[entry_id]= entry_tick;
            if (state->data_loggers.count(entry_id)) {
                state->data_loggers.at(entry_id)->process_log_data(board, entry_id, 
                        duration_cast<milliseconds>(timestamp_copy.time_since_epoch()).count(), data);
            } else if (state->log_download_handler.received_unknown_entry != nullptr) {
                state->log_download_handler.received_unknown_entry(entry_id, 
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
    auto state= GET_LOGGER_STATE(board);
    if (state->log_download_handler.received_progress_update != nullptr) {
        uint32_t entries_left;
        memcpy(&entries_left, response + 2, min(len - 2, 4));

        state->log_download_handler.received_progress_update(entries_left, state->n_log_entries);
    }
    return 0;
}

static int32_t logging_response_page_completed(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint8_t command[2]= { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PAGE_CONFIRM) };
    SEND_COMMAND;
    return 0;
}

TimeReference::TimeReference(uint8_t** state_stream) {
    milliseconds epoch(*((int64_t*) *state_stream));
    timestamp= time_point<system_clock, milliseconds>{epoch};
    *state_stream += sizeof(int64_t);

    memcpy(&tick, *state_stream, sizeof(uint32_t));
    *state_stream += sizeof(uint32_t);

    reset_uid = **state_stream;

    (*state_stream)++;
}

TimeReference::TimeReference(uint32_t tick, uint8_t reset_uid) : tick(tick), reset_uid(reset_uid) {
    timestamp= system_clock::now();
}

void TimeReference::serialize(vector<uint8_t>& state) const {
    int64_t epoch= duration_cast<milliseconds>(timestamp.time_since_epoch()).count();

    state.insert(state.end(), (uint8_t*) &epoch, ((uint8_t*) &epoch) + sizeof(epoch));
    state.insert(state.end(), (uint8_t*) &tick, ((uint8_t*) &tick) + sizeof(tick));
    state.push_back(reset_uid);
}

MblMwDataLogger::MblMwDataLogger(uint8_t** state_stream, MblMwMetaWearBoard* board) : data_handler(nullptr), logger_ready(nullptr) {
    ResponseHeader source_header(state_stream);
    source_header.disable_silent();
    source = dynamic_cast<MblMwDataSignal*>(board->module_events.at(source_header));

    uint8_t n_entry_ids = **state_stream;
    for (uint8_t i = 0; i < n_entry_ids; i++) {
        uint8_t id = *(++(*state_stream));
        entry_ids.push_back(id);
        entries.emplace(id, queue<uint32_t>());
    }

    (*state_stream)++;
}

MblMwDataLogger::MblMwDataLogger(MblMwDataSignal* source, MblMwFnDataLoggerPtr logger_ready) :
        source(source), n_req_entries((source->length() - 1) / sizeof(uint32_t) + 1), data_handler(nullptr), logger_ready(logger_ready) {
}

void MblMwDataLogger::add_entry_id(uint8_t id) {
    entry_ids.push_back(id);
    entries.emplace(id, queue<uint32_t>());

    if ((uint8_t) entry_ids.size() == n_req_entries) {
        auto state= GET_LOGGER_STATE(source->owner);

        state->timeout->cancel();

        for(auto it: entry_ids) {
            state->data_loggers[it]= this;
        }
        state->next_logger = nullptr;

        logger_ready(this);

        state->create_next(true);
    }
}

void MblMwDataLogger::process_log_data(const MblMwMetaWearBoard* board, uint8_t id, int64_t epoch, uint32_t data) {
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

        MblMwFnData unhandled_callback;
        if (data_handler != nullptr) {
            data_handler(data);
        } else if ((unhandled_callback= GET_LOGGER_STATE(board)->log_download_handler.received_unhandled_entry) != nullptr) {
            unhandled_callback(data);
        }

        free(data->value);
        free(data);
    }
}

void MblMwDataLogger::serialize(vector<uint8_t>& state) const {
    source->header.serialize(state);

    state.push_back((uint8_t) entry_ids.size());
    state.insert(state.end(), entry_ids.begin(), entry_ids.end());
}

LoggerState::LoggerState() : next_logger(nullptr) { }

void LoggerState::clear_data_loggers() {
    unordered_set<MblMwDataLogger*> unique_loggables;
    for (auto it : data_loggers) {
        unique_loggables.insert(it.second);
    }

    for(auto it: unique_loggables) {
        delete it;
    }

    data_loggers.clear();
}

void init_logging(MblMwMetaWearBoard *board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, READ_REGISTER(ORDINAL(LoggingRegister::TIME))),
        forward_as_tuple(logging_response_time_received));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::TRIGGER)),
        forward_as_tuple(logging_response_entry_id_received));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, READ_REGISTER(ORDINAL(LoggingRegister::LENGTH))),
        forward_as_tuple(logging_response_length_received));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_NOTIFY)),
        forward_as_tuple(logging_response_readout_notify));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PROGRESS)),
        forward_as_tuple(logging_response_readout_progress));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PAGE_COMPLETED)),
        forward_as_tuple(logging_response_page_completed));

    if (!board->logger_state) {
        board->logger_state = make_shared<LoggerState>();
    }
}

void tear_down_logging(void *state, bool preserve_memory) {
    auto logging_state= (LoggerState*) state;

    if (logging_state != nullptr) {
        logging_state->clear_data_loggers();
        if (!preserve_memory) {
            delete logging_state;
        }
    }
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
    auto state= GET_LOGGER_STATE(board);
    state->log_download_notify_progress= n_notifies ? 1.0 / n_notifies : 0;

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
        memcpy(&state->log_download_handler, handler, sizeof(MblMwLogDownloadHandler));

        command[1]= ORDINAL(LoggingRegister::READOUT_PROGRESS);
        command[2]= 0x1;
        SEND_COMMAND;
    } else {
        state->log_download_handler.received_progress_update= nullptr;
        state->log_download_handler.received_unknown_entry= nullptr;
        state->log_download_handler.received_unhandled_entry = nullptr;
    }

    command[1]= READ_REGISTER(ORDINAL(LoggingRegister::LENGTH));
    send_command(board, command, 2);
}

uint8_t mbl_mw_logger_get_id(const MblMwDataLogger* logger) {
    return logger->get_id();
}

MblMwDataSignal* mbl_mw_logger_get_signal(const MblMwDataLogger* logger) {
    return logger->source;
}

MblMwDataLogger* mbl_mw_logger_lookup_id(const MblMwMetaWearBoard* board, uint8_t id) {
    auto logger_state = GET_LOGGER_STATE(board);
    return logger_state->data_loggers.count(id) ? logger_state->data_loggers.at(id) : nullptr;
}

void mbl_mw_logger_remove(MblMwDataLogger* loggable) {
    auto state = GET_LOGGER_STATE(loggable->source->owner);

    sort(loggable->entry_ids.begin(), loggable->entry_ids.end());
    for (auto it : loggable->entry_ids) {
        state->data_loggers.erase(it);
        uint8_t command[3] = { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::REMOVE), it };
        SEND_COMMAND_BOARD(loggable->source->owner);
    }

    delete loggable;
}

void mbl_mw_logger_subscribe(MblMwDataLogger* loggable, MblMwFnData received_data) {
    loggable->data_handler = received_data;
}

void mbl_mw_datasignal_log(MblMwDataSignal *signal, MblMwFnDataLoggerPtr logger_ready) {
    auto state= GET_LOGGER_STATE(signal->owner);

    state->pending_fns.push([=](void) -> void {
        state->next_logger= new MblMwDataLogger(signal, logger_ready);
        state->timeout= ThreadPool::schedule([state, logger_ready](void) -> void {
            delete state->next_logger;
            state->next_logger = nullptr;

            logger_ready(nullptr);
            state->create_next(true);
        }, state->next_logger->n_req_entries * signal->owner->time_per_response);

        auto entries= state->next_logger->n_req_entries;
        uint8_t remainder= signal->length();
        for(uint8_t i= 0; i < entries; i++, remainder-= LOG_ENTRY_SIZE) {
            uint8_t entry_size= min(remainder, LOG_ENTRY_SIZE), entry_offset= LOG_ENTRY_SIZE * i + signal->offset;
            uint8_t command[6]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::TRIGGER), signal->header.module_id, signal->header.register_id, 
                    signal->header.data_id, (uint8_t) ((entry_size - 1) << 5 | entry_offset)};
            SEND_COMMAND_BOARD(signal->owner);
        }
    });
    state->create_next(false);
}

void serialize_logging(const MblMwMetaWearBoard* board, std::vector<uint8_t>& state) {
    auto logger_state= GET_LOGGER_STATE(board);

    {
        vector<uint8_t> sorted_keys;
        for (auto it : logger_state->log_time_references) {
            sorted_keys.push_back(it.first);
        }
        sort(sorted_keys.begin(), sorted_keys.end());

        state.push_back((uint8_t)logger_state->log_time_references.size());
        for (auto it : sorted_keys) {
            logger_state->log_time_references.at(it).serialize(state);
        }
    }

    {
        vector<uint8_t> sorted_keys;
        unordered_set<MblMwDataLogger*> unique_loggers;
        for (auto it : logger_state->data_loggers) {
            sorted_keys.push_back(it.first);
            unique_loggers.insert(it.second);
        }
        sort(sorted_keys.begin(), sorted_keys.end());

        state.push_back((uint8_t) unique_loggers.size());
        unique_loggers.clear();
        for (auto it : sorted_keys) {
            auto current = logger_state->data_loggers.at(it);
            if (!unique_loggers.count(current)) {
                unique_loggers.insert(current);
                current->serialize(state);
            }
        }
    }
}

void deserialize_logging(MblMwMetaWearBoard* board, uint8_t** state_stream) {
    if (board->logger_state) {
        GET_LOGGER_STATE(board)->clear_data_loggers();
    } else {
        board->logger_state = make_shared<LoggerState>();
    }

    auto saved_log_state = GET_LOGGER_STATE(board);

    uint8_t n_refs = **state_stream;
    (*state_stream)++;
    for (uint8_t i = 0; i < n_refs; i++) {
        TimeReference reference(state_stream);
        saved_log_state->log_time_references.emplace(reference.reset_uid, reference);
    }

    uint8_t n_loggers = **state_stream;
    (*state_stream)++;
    for (uint8_t i = 0; i < n_loggers; i++) {
        MblMwDataLogger* saved_loggable = new MblMwDataLogger(state_stream, board);

        for (auto it : saved_loggable->entry_ids) {
            saved_log_state->data_loggers[it] = saved_loggable;
        }
    }
}
