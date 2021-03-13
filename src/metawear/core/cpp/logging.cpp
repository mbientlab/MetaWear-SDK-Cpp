#include "metawear/core/logging.h"
#include "metawear/core/datasignal.h"
#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/platform/cpp/async_creator.h"
#include "metawear/platform/cpp/threadpool.h"
#include "metawear/processor/cpp/dataprocessor_config.h"
#include "metawear/processor/cpp/dataprocessor_private.h"
#include "metawear/processor/cpp/dataprocessor_register.h"

#include "anonymous_datasignal_private.h"
#include "datasignal_private.h"
#include "logging_private.h"
#include "logging_register.h"
#include "metawearboard_def.h"
#include "register.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <vector>

using namespace std;
using namespace std::chrono;

#define GET_LOGGER_STATE(board) static_pointer_cast<LoggerState>(board->logger_state)

const uint8_t REVISION_EXTENDED_LOGGING= 2, MMS_REVISION= 3, ENTRY_ID_MASK= 0x1f, RESET_UID_MASK= 0x7, 
        LOG_ENTRY_SIZE= (uint8_t) sizeof(uint32_t), ROOT_SIGNAL_INDEX= 0xff;
const double TICK_TIME_STEP= (48.0 / 32768.0) * 1000.0;         ///< milliseconds

const ResponseHeader 
    LOGGING_TIME_RESPONSE_HEADER(MBL_MW_MODULE_LOGGING, READ_REGISTER(ORDINAL(LoggingRegister::TIME))),
    LOGGING_LENGTH_RESPONSE_HEADER(MBL_MW_MODULE_LOGGING, READ_REGISTER(ORDINAL(LoggingRegister::LENGTH))),
    LOGGING_READOUT_NOTIFY_HEADER(MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_NOTIFY)),
    LOGGING_READOUT_PAGE_COMPLETED_HEADER(MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PAGE_COMPLETED));

static int32_t logging_response_readout_progress(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);

struct TimeReference {
    int64_t epoch;
    uint8_t reset_uid;

    TimeReference(uint8_t** state_stream, uint8_t format);
    TimeReference(int64_t epoch, uint8_t reset_uid);
    void serialize(vector<uint8_t>& state) const;
};

struct MblMwDataLogger;
struct LoggerState : public AsyncCreator {
    unordered_map<uint8_t, TimeReference> log_time_references;
    unordered_map<uint8_t, uint32_t> latest_tick, rollback_timestamps;
    unordered_map<uint8_t, MblMwDataLogger*> data_loggers;
    unordered_map<const MblMwDataLogger*, string> identifiers;
    unordered_map<ResponseHeader, uint8_t> placeholder;
    unordered_map<ResponseHeader, int8_t> nRemainingLoggers;
    vector<MblMwAnonymousDataSignal*> anonymous_signals;
    stack<uint8_t> fuser_ids;
    stack<tuple<MblMwDataSignal*, ProcessorEntry>> fuser_configs;
    MblMwDataLogger* next_logger;
    MblMwLogDownloadHandler log_download_handler;
    MblMwRawLogDownloadHandler raw_log_download_handler;
    float log_download_notify_progress;
    uint32_t n_log_entries;
    uint8_t latest_reset_uid, queryLogId;
    bool state_signal;

    LoggerState();

    void clear_data_loggers();
};

struct MblMwDataLogger : public MblMwAnonymousDataSignal {
    MblMwDataLogger(uint8_t** state_stream, uint8_t format, MblMwMetaWearBoard* board);
    MblMwDataLogger(MblMwDataSignal* source, void *context, MblMwFnDataLoggerPtr logger_ready);

    virtual void subscribe(void *context, MblMwFnData data_handler);
    virtual const char* get_identifier() const {
        auto state = GET_LOGGER_STATE(source->owner);

        if (!state->identifiers.count(this)) {
            stringstream stream;
            MblMwDataSignal* current = source;
            MblMwDataProcessor* processor;
            stack<MblMwDataSignal*> parts;

            do {
                parts.push(current);
                if ((processor = dynamic_cast<MblMwDataProcessor*>(current)) != nullptr) {
                    current = processor->parent();
                } else if (current->header.module_id == MBL_MW_MODULE_DATA_PROCESSOR && CLEAR_READ(current->header.register_id) == ORDINAL(DataProcessorRegister::STATE)) {
                    ResponseHeader header(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), current->header.data_id);
                    current = dynamic_cast<MblMwDataProcessor*>(current->owner->module_events.at(header))->parent();
                } else {
                    current = nullptr;
                }
            } while(current != nullptr);

            bool first = true;
            if ((processor = dynamic_cast<MblMwDataProcessor*>(parts.top())) != nullptr) {
                processor->input->create_uri(stream);
                first = false;
            }
            while(!parts.empty()) {
                if (!first) {
                    stream << ":";
                }
                parts.top()->create_uri(stream);

                first = false;
                parts.pop();
            }

            state->identifiers.emplace(this, stream.str());
        }
        return state->identifiers[this].c_str();
    }

    void add_entry_id(uint8_t id, bool anonymous);
    void process_log_data(uint8_t id, int64_t epoch, uint32_t data);
    void serialize(vector<uint8_t>& state) const;

    inline uint8_t get_id() const {
        return entry_ids.front();
    }

    MblMwDataSignal* source;
    uint8_t n_req_entries;
    void *data_context;
    MblMwFnData data_handler;
    void *logger_context;
    MblMwFnDataLoggerPtr logger_ready;
    vector<uint8_t> entry_ids;
    unordered_map<uint8_t, queue<uint32_t>> entries;
};

static int32_t logging_response_entry_id_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    GET_LOGGER_STATE(board)->next_logger->add_entry_id(response[2], false);
    return 0;
}

static MblMwDataSignal* guessLogSource(MblMwMetaWearBoard* board, ResponseHeader& key, uint8_t offset, uint8_t length) {
    key.disable_silent();

    vector<MblMwDataSignal*> possible;
    auto source = dynamic_cast<MblMwDataSignal*>(board->module_events.at(key));

    possible.push_back(source);
    possible.insert(possible.end(), source->components.begin(), source->components.end());
    
    MblMwDataSignal* original = nullptr;
    bool multiple = false;
    for(auto it: possible) {
        if (it->length() > 4) {
            original = it;
            multiple = true;
        }
    }

    if (multiple) {
        auto state = GET_LOGGER_STATE(board);
        if (offset == 0 && length > LOG_ENTRY_SIZE) {
            return original;
        }
        if (!state->placeholder.count(key) && length == LOG_ENTRY_SIZE) {
            state->placeholder[key] = length;
            return original;
        }
        if (state->placeholder.count(key)) {
            uint8_t newLength = (uint8_t) (state->placeholder[key] + length);
            if (newLength == original->length()) {
                state->placeholder.erase(key);
            }
            return original;
        }
    }

    for(auto it: possible) {
        if (it->offset == offset && it->length() == length) {
            return it;
        }
    }
    return nullptr;
}

static void queue_next_logger(MblMwMetaWearBoard* board) {
    auto state = GET_LOGGER_STATE(board);
    state->timeout->cancel();

    state->queryLogId++;
    if (state->queryLogId < board->module_info.at(MBL_MW_MODULE_LOGGING).extra[0]) {
        state->pending_fns.push([=](void) -> void {
            state->timeout= ThreadPool::schedule([state, board](void) -> void {
                board->anon_signals_created(board->anon_signals_context, board, nullptr, MBL_MW_STATUS_ERROR_TIMEOUT);
                state->create_next(true);
            }, board->time_per_response);
    
            uint8_t command[3]= {MBL_MW_MODULE_LOGGING, READ_REGISTER(ORDINAL(LoggingRegister::TRIGGER)), state->queryLogId};
            SEND_COMMAND;
        });
    } else {
        vector<uint8_t> sorted;
        for (auto it: state->data_loggers) {
            sorted.push_back(it.first);
        }
        sort(sorted.begin(), sorted.end());

        state->anonymous_signals.clear();
        for(auto it: sorted) {
            auto val = state->data_loggers[it];
            auto pos = find(begin(state->anonymous_signals), end(state->anonymous_signals), val);
            if (pos == end(state->anonymous_signals)) {
                state->anonymous_signals.push_back(val);
            }
        }

        board->anon_signals_created(board->anon_signals_context, board, state->anonymous_signals.data(), (uint32_t) state->anonymous_signals.size());
    }
    state->create_next(true);
}

static void log_source_discovered(shared_ptr<LoggerState> state, MblMwDataSignal* source, uint8_t offset) {
    if (state->state_signal) {
        source = dynamic_cast<MblMwDataProcessor*>(source)->state;
    }

    MblMwDataProcessor const* processor = dynamic_cast<MblMwDataProcessor*>(source);
    if (processor != nullptr) {
        MblMwDataSignal const* last;
        while(processor != nullptr) {
            last = processor->input;
            processor = dynamic_cast<const MblMwDataProcessor*>(last);
        }
    }

    if (!state->nRemainingLoggers.count(source->header) && source->length() > LOG_ENTRY_SIZE) {
        state->nRemainingLoggers[source->header] = (int8_t) ceil((float) (source->length() / LOG_ENTRY_SIZE));
    }

    MblMwDataLogger* logger = nullptr;
    for(auto it: state->data_loggers) {
        auto other = it.second->source;
        if (other->header == source->header && other->offset == source->offset && 
                other->is_signed == source->is_signed && other->n_channels == source->n_channels && other->channel_size == source->channel_size) {
            logger = it.second;
            break;
        }
    }

    if (logger == nullptr || (offset != 0 && !state->nRemainingLoggers.count(source->header))) {
        logger = new MblMwDataLogger(source, nullptr, nullptr);
    }
    logger->add_entry_id(state->queryLogId, true);
    state->data_loggers[state->queryLogId] = logger;

    if (state->nRemainingLoggers.count(source->header)) {
        state->nRemainingLoggers[source->header]--;
        if (state->nRemainingLoggers[source->header] < 0) {
            state->nRemainingLoggers.erase(source->header);
        }
    }

    queue_next_logger(source->owner);
}

static void processor_synced(MblMwMetaWearBoard* board, stack<ProcessorEntry>& entries) {
    auto type = guessLogSource(board, entries.top().source, entries.top().offset, entries.top().length);
    auto state = GET_LOGGER_STATE(board);
    auto fill_processor = [board](MblMwDataSignal* parent, const ProcessorEntry& entry) {
        auto next = MblMwDataProcessor::transform(parent, entry.config);
        next->header.data_id = entry.id;

        if (board->module_events.count(next->header)) {
            auto temp = board->module_events.at(next->header);
            delete next;

            next = dynamic_cast<MblMwDataProcessor*>(temp);
        } else {
            board->module_events.emplace(next->header, next);
        }

        if (parent->header.module_id == MBL_MW_MODULE_DATA_PROCESSOR && CLEAR_READ(parent->header.register_id) == ORDINAL(DataProcessorRegister::NOTIFY)) {
            next->parent_id = parent->header.data_id;
        }
        if (next->state != nullptr) {
            next->state->header.data_id = entry.id;
            board->module_events.emplace(next->state->header, next->state);
        }

        return next;
    };

    while(!entries.empty()) {
        auto config = entries.top().config;
        if (config[0] == 0x1b) {
            for(uint8_t i = 0; i < (config[1] & 0x1f); i++) {
                state->fuser_ids.push(config[i + 2]);
            }
            state->fuser_configs.push(make_tuple(type, entries.top()));
        } else {
            type = fill_processor(type, entries.top());
        }
        entries.pop();
    }

    if (state->fuser_ids.empty()) {
        while(!state->fuser_configs.empty()) {
            auto top = state->fuser_configs.top();
            type = MblMwDataProcessor::transform(get<0>(top), get<1>(top).config);
            type->header.data_id = get<1>(top).id;

            fill_processor(type, get<1>(top));
            state->fuser_configs.pop();
        }
        log_source_discovered(GET_LOGGER_STATE(board), type, 0);
    } else {
        auto id = state->fuser_ids.top();
        state->fuser_ids.pop();

        sync_processor_chain(board, id, processor_synced);
    }
}

static int32_t logging_response_read_entry_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state = GET_LOGGER_STATE(board);

    if (len > 2) { 
        uint8_t offset = (uint8_t) (response[5] & 0x1f), length = (uint8_t) (((response[5] >> 5) & 0x3) + 1);
        ResponseHeader key(response[2], response[3], response[4]);

        state->state_signal = key.module_id == MBL_MW_MODULE_DATA_PROCESSOR && CLEAR_READ(key.register_id) == ORDINAL(DataProcessorRegister::STATE);
        if (key.module_id == MBL_MW_MODULE_DATA_PROCESSOR && (key.register_id == ORDINAL(DataProcessorRegister::NOTIFY) || state->state_signal)) {
            sync_processor_chain(board, key.data_id, processor_synced);
        } else {
            log_source_discovered(state, guessLogSource(board, key, offset, length), offset);
        }
    } else {
        queue_next_logger(board);
    }

    return 0;
}

static int32_t logging_response_length_received(MblMwMetaWearBoard *board, uint32_t log_entries) {
    auto state= GET_LOGGER_STATE(board);    
    state->n_log_entries = log_entries;
    
    // If there are no entires we won't get any responses, so end the download now
    // by forcing a callback on the readout progress with 0 remaining entries
    if (state->n_log_entries == 0) {
        state->latest_tick.clear();
        state->rollback_timestamps.clear();
        uint8_t readoutResponse[6] = {0};
        return logging_response_readout_progress(board, readoutResponse, sizeof(readoutResponse));
    }
    
    uint32_t n_entries_notify= state->n_log_entries * state->log_download_notify_progress;
    vector<uint8_t> command= { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT) };
    command.insert(command.end(), (uint8_t*) &state->n_log_entries, (uint8_t*) &state->n_log_entries + 4);
    command.insert(command.end(), (uint8_t*) &n_entries_notify, (uint8_t*) &n_entries_notify + 4);
    send_command(board, command.data(), (uint8_t) command.size());
    
    return 0;
}

static TimeReference& mbl_mw_logger_lookup_reset_uid(const MblMwMetaWearBoard* board, uint8_t reset_uid) {
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
        forward_as_tuple(0, reset_uid));
    return logger_state->log_time_references.at(reset_uid);
}

static int64_t calculate_epoch_inner(shared_ptr<LoggerState> state, uint32_t tick, TimeReference& reference) {
    state->latest_tick[reference.reset_uid]= tick;
    return reference.epoch + static_cast<int64_t>(round((double)tick * TICK_TIME_STEP));
}

static int32_t logging_response_readout_notify(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto parse_response= [&board, &response](uint8_t offset) -> void {
        auto state= GET_LOGGER_STATE(board);
        uint8_t entry_id= response[offset] & ENTRY_ID_MASK, reset_uid= (response[offset] & ~ENTRY_ID_MASK) >> 5;
        auto entry_tick = (uint32_t*) (response + offset + 1), data = entry_tick + 1;

        if (!state->rollback_timestamps.count(reset_uid) || state->rollback_timestamps.at(reset_uid) < *entry_tick) {
            auto realtime = calculate_epoch_inner(state, *entry_tick, mbl_mw_logger_lookup_reset_uid(board, reset_uid));
            if (state->data_loggers.count(entry_id)) {
                state->data_loggers.at(entry_id)->process_log_data(entry_id, realtime, *data);
            } else if (state->log_download_handler.received_unknown_entry != nullptr) {
                state->log_download_handler.received_unknown_entry(state->log_download_handler.context, entry_id, realtime, (const uint8_t*) data, sizeof(*data));
            }
        }
    };

    parse_response(2);
    if (len == 20) {
        parse_response(11);
    }

    return 0;
}

static int32_t raw_logging_response_readout_notify(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto parse_response= [&board, &response](uint8_t offset) -> void {
        auto state= GET_LOGGER_STATE(board);
        uint8_t entry_id = response[offset] & ENTRY_ID_MASK;
        uint8_t reset_uid = (response[offset] & ~ENTRY_ID_MASK) >> 5;
        uint32_t entry_tick;
        memcpy(&entry_tick, (response + offset + 1), 4);
        uint32_t data;
        memcpy(&data, (response + offset + 5), 4);
        if (state->raw_log_download_handler.received_entry != nullptr) {
            state->raw_log_download_handler.received_entry(state->raw_log_download_handler.context, entry_id, reset_uid, entry_tick, data);
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
    uint32_t entries_left;
    memcpy(&entries_left, response + 2, min(len - 2, 4));

    if (entries_left == 0) {
        state->latest_tick.clear();
        state->rollback_timestamps.clear();
    }
    if (state->log_download_handler.received_progress_update != nullptr) {
        state->log_download_handler.received_progress_update(state->log_download_handler.context, entries_left, state->n_log_entries);
    }
    if (state->raw_log_download_handler.received_progress_update != nullptr) {
        state->raw_log_download_handler.received_progress_update(state->raw_log_download_handler.context, entries_left, state->n_log_entries);
    }
    return 0;
}

static int32_t logging_response_page_completed(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint8_t command[2]= { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PAGE_CONFIRM) };
    SEND_COMMAND;
    return 0;
}

static int32_t raw_logging_response_page_completed(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state= GET_LOGGER_STATE(board);
    if (state->raw_log_download_handler.logging_page_completed != nullptr) {
        state->raw_log_download_handler.logging_page_completed(state->raw_log_download_handler.context, board, [](const MblMwMetaWearBoard* board) {
            uint8_t command[2]= { MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PAGE_CONFIRM) };
            SEND_COMMAND;
        });
    }
    return 0;
}

TimeReference::TimeReference(uint8_t** state_stream, uint8_t format) {
    if (format <= ORDINAL(SerializationFormat::SIGNAL_COMPONENT)) {
        milliseconds epoch(*((int64_t*) *state_stream));
        auto timestamp= time_point<system_clock, milliseconds>{epoch};
        *state_stream += sizeof(int64_t);

        uint32_t tick;
        memcpy(&tick, *state_stream, sizeof(uint32_t));
        *state_stream += sizeof(uint32_t);

        reset_uid = **state_stream;

        (*state_stream)++;

        this->epoch = duration_cast<milliseconds>(timestamp.time_since_epoch()).count() - static_cast<int64_t>(round((double)tick * TICK_TIME_STEP));
    } else {
        memcpy(&epoch, *state_stream, sizeof(int64_t));
        *state_stream += sizeof(int64_t);

        reset_uid = **state_stream;

        (*state_stream)++;
    }
}

TimeReference::TimeReference(int64_t epoch, uint8_t reset_uid) : epoch(epoch), reset_uid(reset_uid) {
}

void TimeReference::serialize(vector<uint8_t>& state) const {
    state.insert(state.end(), (uint8_t*) &epoch, ((uint8_t*) &epoch) + sizeof(epoch));
    state.push_back(reset_uid);
}

MblMwDataLogger::MblMwDataLogger(uint8_t** state_stream, uint8_t format, MblMwMetaWearBoard* board) : 
        data_context(nullptr), data_handler(nullptr), logger_context(nullptr), logger_ready(nullptr) {
    uint8_t signal_index;
    
    if (format >= ORDINAL(SerializationFormat::SIGNAL_COMPONENT)) {
        signal_index = **state_stream;
        (*state_stream)++;
    } else {
        signal_index = ROOT_SIGNAL_INDEX;
    }

    ResponseHeader source_header(state_stream);
    source_header.disable_silent();

    auto root = dynamic_cast<MblMwDataSignal*>(board->module_events.at(source_header));
    source = (signal_index == ROOT_SIGNAL_INDEX) ? root : root->components.at(signal_index);

    uint8_t n_entry_ids = **state_stream;
    for (uint8_t i = 0; i < n_entry_ids; i++) {
        uint8_t id = *(++(*state_stream));
        entry_ids.push_back(id);
        entries.emplace(id, queue<uint32_t>());
    }

    (*state_stream)++;
}

MblMwDataLogger::MblMwDataLogger(MblMwDataSignal* source, void *context, MblMwFnDataLoggerPtr logger_ready) :
        source(source), n_req_entries((source->length() - 1) / sizeof(uint32_t) + 1), 
        data_context(nullptr), data_handler(nullptr), logger_context(context), logger_ready(logger_ready) {
}

void MblMwDataLogger::subscribe(void *context, MblMwFnData data_handler) {
    this->data_context = context;
    this->data_handler = data_handler;
}

void MblMwDataLogger::add_entry_id(uint8_t id, bool anonymous) {
    entry_ids.push_back(id);
    entries.emplace(id, queue<uint32_t>());

    if (!anonymous && (uint8_t) entry_ids.size() == n_req_entries) {
        auto state= GET_LOGGER_STATE(source->owner);

        state->timeout->cancel();

        for(auto it: entry_ids) {
            state->data_loggers[it]= this;
        }
        state->next_logger = nullptr;

        logger_ready(logger_context, this);

        state->create_next(true);
    }
}

void MblMwDataLogger::process_log_data(uint8_t id, int64_t epoch, uint32_t data) {
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

        MblMwData* data = data_response_converters.at(source->interpreter)(true, source, merged.data(), (uint8_t) merged.size());
        data->epoch= epoch;

        MblMwFnData unhandled_callback;
        if (data_handler != nullptr) {
            data_handler(data_context, data);
        } else if ((unhandled_callback= GET_LOGGER_STATE(source->owner)->log_download_handler.received_unhandled_entry) != nullptr) {
            unhandled_callback(GET_LOGGER_STATE(source->owner)->log_download_handler.context, data);
        }

        free_data(source, data);
    }
}

void MblMwDataLogger::serialize(vector<uint8_t>& state) const {
    ResponseHeader copy(source->header);
    copy.disable_silent();

    auto root = dynamic_cast<MblMwDataSignal*>(source->owner->module_events.at(copy));
    if (source == root) {
        state.push_back(ROOT_SIGNAL_INDEX);
    } else {
        uint8_t i = 0;
        for (auto it : root->components) {
            if (it == source) break;
            i++;
        }

        state.push_back(i);
    }
    source->header.serialize(state);

    state.push_back((uint8_t)entry_ids.size());
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
    identifiers.clear();
}

void init_logging(MblMwMetaWearBoard *board) {
    if (!board->module_events.count(LOGGING_TIME_RESPONSE_HEADER)) {
        board->module_events[LOGGING_TIME_RESPONSE_HEADER] = new MblMwDataSignal(LOGGING_TIME_RESPONSE_HEADER, board, 
            DataInterpreter::LOGGING_TIME, 1, 5, 0, 0);
    }
    board->responses[LOGGING_TIME_RESPONSE_HEADER] = response_handler_data_no_id;

    if (!board->module_events.count(LOGGING_LENGTH_RESPONSE_HEADER)) {
        board->module_events[LOGGING_LENGTH_RESPONSE_HEADER] = new MblMwDataSignal(LOGGING_LENGTH_RESPONSE_HEADER, board, 
            DataInterpreter::UINT32, 1, 4, 0, 0);
    }
    board->responses[LOGGING_LENGTH_RESPONSE_HEADER] = response_handler_data_no_id;  

    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::TRIGGER)),
        forward_as_tuple(logging_response_entry_id_received));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, READ_REGISTER(ORDINAL(LoggingRegister::TRIGGER))),
        forward_as_tuple(logging_response_read_entry_id));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PROGRESS)),
        forward_as_tuple(logging_response_readout_progress));

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

void disconnect_logging(MblMwMetaWearBoard* board) {
    auto state = GET_LOGGER_STATE(board);

    if (state != nullptr) {
        state->pending_fns.clear();
        for(auto it: state->latest_tick) {
            state->rollback_timestamps[it.first] = it.second;
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

void mbl_mw_logging_flush_page(const MblMwMetaWearBoard* board) {
    if (board->module_info.at(MBL_MW_MODULE_LOGGING).revision == MMS_REVISION) {
        uint8_t command[3]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::PAGE_FLUSH), 1};
        SEND_COMMAND;
    }
}

void mbl_mw_logging_clear_entries(const MblMwMetaWearBoard* board) {
    if (board->module_info.at(MBL_MW_MODULE_LOGGING).revision == REVISION_EXTENDED_LOGGING) {
        uint8_t command[3]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::READOUT_PAGE_COMPLETED), 1};
        SEND_COMMAND;
    }

    uint8_t command[6]= {MBL_MW_MODULE_LOGGING, ORDINAL(LoggingRegister::REMOVE_ENTRIES), 0xff, 0xff, 0xff, 0xff};
    SEND_COMMAND;
}

void mbl_mw_logging_download_common(MblMwMetaWearBoard* board, uint8_t n_notifies) {
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
    
    MblMwDataSignal *signal = mbl_mw_logging_get_length_data_signal(board);
    mbl_mw_datasignal_subscribe(signal, board, [](void *context, const MblMwData* data) {
        MblMwMetaWearBoard* board = static_cast<MblMwMetaWearBoard*>(context);
        mbl_mw_datasignal_unsubscribe(mbl_mw_logging_get_length_data_signal(board));
        logging_response_length_received(board, *static_cast<uint32_t*>(data->value));
    });
    mbl_mw_datasignal_read(signal);
}

void mbl_mw_logging_download(MblMwMetaWearBoard* board, uint8_t n_notifies, const MblMwLogDownloadHandler* handler) {
    board->responses[LOGGING_READOUT_NOTIFY_HEADER] = logging_response_readout_notify;
    board->responses[LOGGING_READOUT_PAGE_COMPLETED_HEADER] = logging_response_page_completed;
    
    auto state= GET_LOGGER_STATE(board);
    uint8_t command[3]= {MBL_MW_MODULE_LOGGING};
    
    if (handler != nullptr) {
        state->log_download_handler = *handler;

        command[1]= ORDINAL(LoggingRegister::READOUT_PROGRESS);
        command[2]= 0x1;
        SEND_COMMAND;
    } else {
        memset(&state->log_download_handler, 0, sizeof(MblMwLogDownloadHandler));
    }
    memset(&state->raw_log_download_handler, 0, sizeof(MblMwRawLogDownloadHandler));
    
    mbl_mw_logging_download_common(board, n_notifies);
}

void mbl_mw_logging_raw_download(MblMwMetaWearBoard* board, uint8_t n_notifies, const MblMwRawLogDownloadHandler* handler) {
    board->responses[LOGGING_READOUT_NOTIFY_HEADER] = raw_logging_response_readout_notify;
    board->responses[LOGGING_READOUT_PAGE_COMPLETED_HEADER] = raw_logging_response_page_completed;
    
    auto state= GET_LOGGER_STATE(board);
    uint8_t command[3]= {MBL_MW_MODULE_LOGGING};
    
    if (handler != nullptr) {
        state->raw_log_download_handler = *handler;
        
        command[1]= ORDINAL(LoggingRegister::READOUT_PROGRESS);
        command[2]= 0x1;
        SEND_COMMAND;
    } else {
        memset(&state->raw_log_download_handler, 0, sizeof(MblMwRawLogDownloadHandler));
    }
    memset(&state->log_download_handler, 0, sizeof(MblMwLogDownloadHandler));

    mbl_mw_logging_download_common(board, n_notifies);
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

void mbl_mw_logger_subscribe(MblMwDataLogger* loggable, void *context, MblMwFnData received_data) {
    loggable->data_context = context;
    loggable->data_handler = received_data;
}

const char* mbl_mw_logger_generate_identifier(const MblMwDataLogger* signal) {
    return signal->get_identifier();
}

void mbl_mw_datasignal_log(MblMwDataSignal *signal, void *context, MblMwFnDataLoggerPtr logger_ready) {
    auto state= GET_LOGGER_STATE(signal->owner);

    state->pending_fns.push([=](void) -> void {
        state->next_logger= new MblMwDataLogger(signal, context, logger_ready);
        state->timeout= ThreadPool::schedule([context, state, logger_ready](void) -> void {
            delete state->next_logger;
            state->next_logger = nullptr;

            logger_ready(context, nullptr);
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

void deserialize_logging(MblMwMetaWearBoard* board, uint8_t format, uint8_t** state_stream) {
    if (board->logger_state) {
        GET_LOGGER_STATE(board)->clear_data_loggers();
    } else {
        board->logger_state = make_shared<LoggerState>();
    }

    auto saved_log_state = GET_LOGGER_STATE(board);

    uint8_t n_refs = **state_stream;
    (*state_stream)++;
    for (uint8_t i = 0; i < n_refs; i++) {
        TimeReference reference(state_stream, format);
        saved_log_state->log_time_references.emplace(reference.reset_uid, reference);
    }

    uint8_t n_loggers = **state_stream;
    (*state_stream)++;
    for (uint8_t i = 0; i < n_loggers; i++) {
        MblMwDataLogger* saved_loggable = new MblMwDataLogger(state_stream, format, board);

        for (auto it : saved_loggable->entry_ids) {
            saved_log_state->data_loggers[it] = saved_loggable;
        }
    }
}

int64_t calculate_epoch(const MblMwMetaWearBoard* board, uint32_t tick) {
    return calculate_epoch_inner(GET_LOGGER_STATE(board), tick, mbl_mw_logger_lookup_reset_uid(board, GET_LOGGER_STATE(board)->latest_reset_uid));
}

void query_active_loggers(MblMwMetaWearBoard* board) {
    auto state= GET_LOGGER_STATE(board);
    state->clear_data_loggers();
    state->anonymous_signals.clear();
    state->queryLogId = 0;

    state->pending_fns.push([=](void) -> void {
        state->timeout= ThreadPool::schedule([state, board](void) -> void {            
            board->anon_signals_created(board->anon_signals_context, board, nullptr, MBL_MW_STATUS_ERROR_TIMEOUT);
            state->create_next(true);
        }, board->time_per_response);

        uint8_t command[3]= {MBL_MW_MODULE_LOGGING, READ_REGISTER(ORDINAL(LoggingRegister::TRIGGER)), state->queryLogId};
        SEND_COMMAND;
    });
    state->create_next(false);
}

MblMwDataSignal* mbl_mw_logging_get_length_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(LOGGING_LENGTH_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_logging_get_time_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(LOGGING_TIME_RESPONSE_HEADER);
}

uint8_t mbl_mw_logging_get_latest_reset_uid(const MblMwMetaWearBoard* board) {
    return GET_LOGGER_STATE(board)->latest_reset_uid;
}

void mbl_mw_logging_set_latest_reset_uid(const MblMwMetaWearBoard* board, uint8_t reset_uid) {
    GET_LOGGER_STATE(board)->latest_reset_uid = reset_uid;
}

int64_t mbl_mw_logging_get_reference_time(const MblMwMetaWearBoard *board, uint8_t reset_uid) {
    auto logger_state = GET_LOGGER_STATE(board);
    if (logger_state->log_time_references.count(reset_uid)) {
        return logger_state->log_time_references.at(reset_uid).epoch;
    }
    return -1;
}

void mbl_mw_logging_set_reference_time(const MblMwMetaWearBoard *board, uint8_t reset_uid, int64_t reference_epoch) {
    auto logger_state = GET_LOGGER_STATE(board);
    if (logger_state->log_time_references.count(reset_uid)) {
        logger_state->log_time_references.at(reset_uid).epoch = reference_epoch;
    } else {
        logger_state->log_time_references.emplace(piecewise_construct, forward_as_tuple(reset_uid), 
        forward_as_tuple(reference_epoch, reset_uid));
    }
}
