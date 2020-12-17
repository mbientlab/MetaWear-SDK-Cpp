#include "dataprocessor_config.h"
#include "dataprocessor_private.h"
#include "dataprocessor_register.h"
#include "metawear/processor/dataprocessor.h"

#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include "metawear/platform/cpp/async_creator.h"
#include "metawear/platform/cpp/threadpool.h"

#include "metawear/processor/comparator.h"
#include "metawear/processor/delta.h"
#include "metawear/processor/math.h"
#include "metawear/processor/pulse.h"
#include "metawear/processor/threshold.h"
#include "metawear/processor/time.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <unordered_map>
#include <vector>

using namespace std;

#define GET_DATAPROCESSOR_STATE(board) static_pointer_cast<DataProcessorState>(board->dp_state)

const uint8_t NO_PARENT = -1;
const ResponseHeader DATAPROCESSOR_RESPONSE_HEADER(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY));

unordered_map<DataProcessorType, uint8_t> type_to_id= {
    { DataProcessorType::ACCUMULATOR, 0x2 },
    { DataProcessorType::AVERAGE, 0x3 },
    { DataProcessorType::BUFFER, 0xf },
    { DataProcessorType::COMPARATOR, 0x6 },
    { DataProcessorType::COUNTER, 0x2 },
    { DataProcessorType::DELTA, 0xc },
    { DataProcessorType::MATH, 0x9 },
    { DataProcessorType::PASSTHROUGH, 0x1 },
    { DataProcessorType::PULSE, 0xb },
    { DataProcessorType::RMS, 0x7 },
    { DataProcessorType::RSS, 0x7 },
    { DataProcessorType::SAMPLE, 0xa },
    { DataProcessorType::THRESHOLD, 0xd },
    { DataProcessorType::TIME, 0x8 },
    { DataProcessorType::ACCOUNTER, 0x11 },
    { DataProcessorType::PACKER, 0x10 },
    { DataProcessorType::FUSER, 0x1b}
};

struct DataProcessorState : public AsyncCreator {
    void *processor_context;
    MblMwFnDataProcessor processor_callback;
    MblMwDataProcessor* next_processor;
    ProcessorEntriesHandler sync_handler;
    stack<ProcessorEntry> chain;
    uint8_t entry_id;
};


static void create_processor_state_signal(MblMwDataProcessor* processor, DataInterpreter interpreter) {
    ResponseHeader state_header(MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::STATE)));
    processor->state= new MblMwDataSignal(state_header, processor->owner, interpreter, processor->n_channels, 
            processor->channel_size, processor->is_signed, 0);
}

MblMwDataProcessor* MblMwDataProcessor::transform(const MblMwDataSignal* input, uint8_t id, const void* config) {
    auto processor = new MblMwDataProcessor(*input);
    
    switch(id) {
        case 1: {
            processor->type = DataProcessorType::PASSTHROUGH;
            processor->config_size = sizeof(PassthroughConfig);
            create_processor_state_signal(processor, DataInterpreter::UINT32);
            break;
        }
        case 2: {
            auto accumulator = (AccumulatorConfig*) config;

            processor->type = accumulator->mode == ACCUMULATOR_SUM ? DataProcessorType::ACCUMULATOR : DataProcessorType::COUNTER;
            processor->config_size = sizeof(AccumulatorConfig);

            if (processor->type == DataProcessorType::COUNTER) {
                processor->is_signed= 0;
                processor->converter = FirmwareConverter::DEFAULT;
                processor->interpreter = DataInterpreter::UINT32;
                processor->set_channel_attr(1, accumulator->output + 1);
            } else {
                processor->set_channel_attr(input->n_channels, (accumulator->output + 1) / input->n_channels);
            }
            create_processor_state_signal(processor, processor->interpreter);
            
            break;
        }
        case 3: {
            bool has_hpf = input->owner->module_info.at(MBL_MW_MODULE_DATA_PROCESSOR).revision >= HPF_REVISION;

            processor->type = DataProcessorType::AVERAGE;
            processor->config_size = sizeof(AverageConfig) - (has_hpf ? 0 : 1);
            break;
        }
        case 6: {
            if (!(input->owner->firmware_revision < MULTI_COMPARE)) {
                auto compare = (MultiComparatorConfig*) config;

                processor->config_size = sizeof(MultiComparatorConfig);
                if (compare->mode == MBL_MW_COMPARATOR_MODE_ZONE || compare->mode == MBL_MW_COMPARATOR_MODE_BINARY) {
                    processor->interpreter= DataInterpreter::UINT32;
                    processor->converter= FirmwareConverter::DEFAULT;
                    processor->is_signed= 0;
                    processor->set_channel_attr(1, 1);
                }
            } else {
                auto compare = (ComparatorConfig*) config;
                processor->config_size = sizeof(ComparatorConfig);
                processor->is_signed = compare->is_signed;
            }
            processor->type = DataProcessorType::COMPARATOR;
            break;
        }
        case 7: {
            auto combiner = (CombinerConfig*) config;

            processor->config_size = sizeof(CombinerConfig);
            processor->is_signed= 0;
            processor->offset= 0;
            processor->set_channel_attr(1, input->channel_size);
            
            switch (input->interpreter) {
            case DataInterpreter::BOSCH_ACCELERATION:
                processor->interpreter= DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS;
                break;
            case DataInterpreter::MMA8452Q_ACCELERATION:
                processor->interpreter = DataInterpreter::MMA8452Q_ACCELERATION_UNSIGNED_SINGLE_AXIS;
                break;
            case DataInterpreter::BMI160_ROTATION:
                processor->interpreter = DataInterpreter::BMI160_ROTATION_UNSIGNED_SINGLE_AXIS;
                break;
            case DataInterpreter::BMM150_B_FIELD:
                processor->interpreter = DataInterpreter::BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS;
                break;
            case DataInterpreter::TCS34725_COLOR_ADC:
                processor->interpreter = DataInterpreter::UINT32;
                break;
            default:
                break;
            }

            processor->type = combiner->mode == COMBINER_RMS ? DataProcessorType::RMS : DataProcessorType::RSS;
            break;
        }
        case 8: {
            auto time = (TimeDelayConfig*) config;

            processor->config_size = sizeof(TimeDelayConfig);
            if (time->mode == MBL_MW_TIME_DIFFERENTIAL) {
                processor->make_signed();
            }
            
            processor->type = DataProcessorType::TIME;
            break;
        }
        case 9: {
            auto math = (MathConfig*) config;

            processor->config_size = sizeof(MathConfig) - (input->owner->firmware_revision < MULTI_CHANNEL_MATH ? 1 : 0);

            switch (math->operation) {
            // Use the is_signed value to determine if data will be signed/unsigned
            case MBL_MW_MATH_OP_ADD:
            case MBL_MW_MATH_OP_MULTIPLY:
            case MBL_MW_MATH_OP_DIVIDE:
            case MBL_MW_MATH_OP_MODULUS:
                if (math->is_signed) {
                    processor->make_signed();
                } else {
                    processor->make_unsigned();
                }
                break;
            // data always signed for subtract
            case MBL_MW_MATH_OP_SUBTRACT:
                processor->make_signed();
                break;
            // data always unsigned for sqrt & abs
            case MBL_MW_MATH_OP_SQRT:
            case MBL_MW_MATH_OP_ABS_VALUE:
                processor->make_unsigned();
                break;
            default:
                break;
            }

            switch (math->operation) {
            case MBL_MW_MATH_OP_EXPONENT:
            case MBL_MW_MATH_OP_SQRT:
            case MBL_MW_MATH_OP_LSHIFT:
            case MBL_MW_MATH_OP_RSHIFT:
                if (processor->interpreter != DataInterpreter::INT32 && processor->interpreter != DataInterpreter::UINT32) {
                    processor->interpreter = DataInterpreter::BYTE_ARRAY;
                }
                processor->converter = FirmwareConverter::DEFAULT;
                break;
            case MBL_MW_MATH_OP_CONSTANT:
                processor->interpreter = DataInterpreter::INT32;
                processor->converter = FirmwareConverter::DEFAULT;
            default:
                break;
            }
        
            uint32_t* rhs = (uint32_t*) &(math->rhs);
            switch (math->operation) {
            case MBL_MW_MATH_OP_RSHIFT:
                processor->channel_size= max(1, (int) (input->channel_size - (*rhs / 8)));
                break;
            case MBL_MW_MATH_OP_LSHIFT:
                processor->channel_size= min(4, input->channel_size + (uint8_t) ceil(*rhs / 8));
                break;
            default:
                processor->channel_size= 4;
            }

            processor->type = DataProcessorType::MATH;
            break;
        }
        case 0xa: {
            processor->type = DataProcessorType::SAMPLE;
            processor->config_size = sizeof(SampleDelayConfig);
            break;
        }
        case 0xb: {
            auto pulse = (PulseDetectorConfig*) config;

            processor->config_size = sizeof(PulseDetectorConfig);

            switch (pulse->output_mode) {
            case MBL_MW_PULSE_OUTPUT_WIDTH:
                processor->set_channel_attr(1, 2);
                processor->interpreter = DataInterpreter::UINT32;
                processor->converter = FirmwareConverter::DEFAULT;
                break;
            case MBL_MW_PULSE_OUTPUT_AREA:
                processor->set_channel_attr(1, 4);
                break;
            case MBL_MW_PULSE_OUTPUT_ON_DETECTION:
                processor->is_signed = 0;
                processor->set_channel_attr(1, 1);
                processor->interpreter = DataInterpreter::UINT32;
                processor->converter = FirmwareConverter::DEFAULT;
                break;
            default:
                break;
            }

            processor->type = DataProcessorType::PULSE;
            break;
        }
        case 0xc: {
            auto delta = (DeltaConfig*) config;

            processor->config_size = sizeof(DeltaConfig);
            switch (delta->mode) {
            case MBL_MW_DELTA_MODE_DIFFERENTIAL:
                processor->make_signed();
                break;
            case MBL_MW_DELTA_MODE_BINARY:
                processor->is_signed = 1;
                processor->interpreter = DataInterpreter::INT32;
                processor->set_channel_attr(1, 1);
                processor->converter = FirmwareConverter::DEFAULT;
                break;
            default:
                break;
            }
            processor->type = DataProcessorType::DELTA;

            create_processor_state_signal(processor, processor->interpreter);

            break;
        }
        case 0xd: {
            auto threshold = (ThresholdConfig*) config;

            processor->config_size = sizeof(ThresholdConfig);
            if (threshold->mode == MBL_MW_THRESHOLD_MODE_BINARY) {
                processor->is_signed = 1;
                processor->interpreter = DataInterpreter::INT32;
                processor->set_channel_attr(1, 1);
                processor->converter = FirmwareConverter::DEFAULT;
            }

            processor->type = DataProcessorType::THRESHOLD;
            break;
        }
        case 0xf: {
            processor->type = DataProcessorType::BUFFER;
            processor->config_size = sizeof(BufferConfig);
            create_processor_state_signal(processor, processor->interpreter);
            break;
        }
        case 0x10: {
            processor->type = DataProcessorType::PACKER;
            processor->config_size = sizeof(PackerConfig);
            break;
        }
        case 0x11: {
            auto accounter = (AccounterConfig*) config;
            processor->config_size = sizeof(AccounterConfig);
            processor->channel_size+= accounter->length + 1;
            processor->type = DataProcessorType::ACCOUNTER;
            break;
        }
        case 0x1b: {
            auto fuse = (FuseConfig*) config;
            processor->config_size = sizeof(FuseConfig);
            processor->channel_size = input->length();
            processor->n_channels = 1;
            processor->type = DataProcessorType::FUSER;
            processor->interpreter = DataInterpreter::FUSED_DATA;
            processor->converter = FirmwareConverter::DEFAULT;

            for(uint8_t i = 0; i < fuse->count; i++) {
                auto value = lookup_processor(input->owner, fuse->references[i]);
                processor->channel_size+= value->length();
            }

            break;
        }
    }

    processor->config = malloc(processor->config_size);
    memcpy(processor->config, config, processor->config_size);
    return processor;
}

MblMwDataProcessor* MblMwDataProcessor::transform(const MblMwDataSignal* input, const std::vector<uint8_t>& config) {
    return MblMwDataProcessor::transform(input, config[0], config.data() + 1);
}

MblMwDataProcessor::MblMwDataProcessor(uint8_t** state_stream, MblMwMetaWearBoard *owner) : MblMwDataSignal(state_stream, owner) {
    parent_id = **state_stream;

    config_size = *(++(*state_stream));
    config = malloc(config_size);
    memcpy(config, ++(*state_stream), config_size);

    *state_stream += config_size;
    uint8_t n_consumers = **state_stream;
    for (uint8_t i = 0; i < n_consumers; i++) {
        consumers.push_back(*(++(*state_stream)));
    }

    type = static_cast<DataProcessorType>(*(++(*state_stream)));

    (*state_stream)++;
}

MblMwDataProcessor::MblMwDataProcessor(const MblMwDataSignal& signal) : MblMwDataSignal(signal.header, signal.owner, 
        signal.interpreter, signal.converter, signal.n_channels, signal.channel_size, signal.is_signed, signal.offset), 
        parent_id(NO_PARENT), state(nullptr), input(&signal), config(nullptr) {
    offset = 0;
    header.module_id = MBL_MW_MODULE_DATA_PROCESSOR;
    header.register_id = ORDINAL(DataProcessorRegister::NOTIFY);

    if (const MblMwDataProcessor* source= dynamic_cast<const MblMwDataProcessor*>(&signal)) {
        parent_id= source->header.data_id;
    }
}

MblMwDataProcessor::~MblMwDataProcessor() {
    free(config);
    config= nullptr;

    if (remove) {
        uint8_t command[3]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::REMOVE), header.data_id };
        send_command(owner, command, sizeof(command));
    }
}

void MblMwDataProcessor::subscribe() {
    uint8_t enable_command[4] = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY_ENABLE), header.data_id, 1 };
    send_command(owner, enable_command, sizeof(enable_command));

    uint8_t enable_notify[3] = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), 1 };
    send_command(owner, enable_notify, sizeof(enable_notify));
}

void MblMwDataProcessor::unsubscribe() {
    uint8_t disable_command[4] = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY_ENABLE), header.data_id, 0 };
    send_command(owner, disable_command, sizeof(disable_command));
}

MblMwDataProcessor* MblMwDataProcessor::parent() const {
    ResponseHeader parent_header(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), parent_id);
    return owner->module_events.count(parent_header) ?
        dynamic_cast<MblMwDataProcessor*>(owner->module_events.at(parent_header)) :
        nullptr;
}

void MblMwDataProcessor::serialize(vector<uint8_t>& state) const {
    MblMwDataSignal::serialize(state);

    state.push_back(parent_id);

    state.push_back((uint8_t) config_size);
    state.insert(state.end(), (uint8_t*) config, ((uint8_t*) config) + config_size);

    state.push_back((uint8_t) consumers.size());
    state.insert(state.end(), consumers.begin(), consumers.end());

    state.push_back(static_cast<uint8_t>(type));
}

static const char* type_to_uri(DataProcessorType type, void* config) {
    switch(type) {
    case DataProcessorType::ACCUMULATOR:
        return "accumulate";
    case DataProcessorType::AVERAGE:
        return ((AverageConfig*) config)->mode == USE_HPF ? "high-pass" : "low-pass";
    case DataProcessorType::BUFFER:
        return "buffer";
    case DataProcessorType::COMPARATOR:
        return "comparison";
    case DataProcessorType::COUNTER:
        return "count";
    case DataProcessorType::DELTA:
        return "differential";
    case DataProcessorType::MATH:
        return "math";
    case DataProcessorType::PASSTHROUGH:
        return "passthrough";
    case DataProcessorType::PULSE:
        return "pulse";
    case DataProcessorType::RMS:
        return "rms";
    case DataProcessorType::RSS:
        return "rss";
    case DataProcessorType::SAMPLE:
        return "delay";
    case DataProcessorType::THRESHOLD:
        return "threshold";
    case DataProcessorType::TIME:
        return "time";
    case DataProcessorType::ACCOUNTER:
        return "account";
    case DataProcessorType::PACKER:
        return "packer";
    case DataProcessorType::FUSER:
        return "fuser";
    default:
        return nullptr;
    }
}

void MblMwDataProcessor::create_uri(std::stringstream& uri) const {
    uri << type_to_uri(type, config) << "?id=" << (int) header.data_id;
}

void create_dataprocessor_state_uri(const MblMwDataSignal* signal, stringstream& uri) {
    ResponseHeader header(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), signal->header.data_id);
    auto processor = dynamic_cast<MblMwDataProcessor*>(signal->owner->module_events.at(header));

    uri << type_to_uri(processor->type, processor->config) << "-state?id=" << (int) processor->header.data_id;
}

static int32_t dataprocessor_created(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state = GET_DATAPROCESSOR_STATE(board);

    state->timeout->cancel();
    state->next_processor->header.data_id = response[2];

    auto proc_parent = state->next_processor->parent();
    if (proc_parent != nullptr) {
        proc_parent->consumers.push_back(response[2]);
    }

    if (state->next_processor->state != nullptr) {
        state->next_processor->state->header.data_id = response[2];
        state->next_processor->owner->module_events[state->next_processor->state->header] = state->next_processor->state;
        state->next_processor->state = nullptr;
    }

    state->next_processor->owner->module_events.emplace(state->next_processor->header, state->next_processor);
    state->processor_callback(state->processor_context, state->next_processor);
    state->create_next(true);

    return MBL_MW_STATUS_OK;
}

static int32_t dataprocessor_config_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state = GET_DATAPROCESSOR_STATE(board);
    state->timeout->cancel();
    
    ProcessorEntry entry = {
        state->entry_id,
        static_cast<uint8_t>(response[5] & 0x1f),
        static_cast<uint8_t>(((response[5] >> 5) & 0x7) + 1),
        ResponseHeader(response[2], response[3], response[4])
    };
    entry.config.insert(entry.config.end(), response + 6, response + len);

    state->chain.push(entry);

    if (response[2] == MBL_MW_MODULE_DATA_PROCESSOR && response[3] == ORDINAL(DataProcessorRegister::NOTIFY)) {
        state->entry_id = response[4];
        state->pending_fns.push([=](void) -> void {
            state->timeout= ThreadPool::schedule([state, board](void) -> void {
                board->anon_signals_created(board->anon_signals_context, board, nullptr, MBL_MW_STATUS_ERROR_TIMEOUT);
                state->create_next(true);
            }, board->time_per_response);
    
            uint8_t command[3] = { MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::ADD)), response[4] };
            SEND_COMMAND;
        });
    } else { 
        state->sync_handler(board, state->chain);
    }
    state->create_next(true);

    return MBL_MW_STATUS_OK;
}

void init_dataprocessor_module(MblMwMetaWearBoard* board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY)),
        forward_as_tuple(response_handler_data_with_id));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::ADD)),
        forward_as_tuple(dataprocessor_created));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::ADD))),
        forward_as_tuple(dataprocessor_config_received));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::STATE))),
        forward_as_tuple(response_handler_data_with_id));

    if (!board->dp_state) {
        board->dp_state = make_shared<DataProcessorState>();
    }
}

void free_dataprocessor_module(void* state) {
    delete (DataProcessorState*) state;
}

void disconnect_dataprocessor(MblMwMetaWearBoard* board) {
    auto state = GET_DATAPROCESSOR_STATE(board);
    if (state != nullptr) {
        state->pending_fns.clear();
    }
}

MblMwDataSignal* mbl_mw_dataprocessor_get_state_data_signal(const MblMwDataProcessor* processor) {
    ResponseHeader state_header(MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::STATE)), processor->header.data_id);
    GET_DATA_SIGNAL_BOARD(processor->owner, state_header);
}

void mbl_mw_dataprocessor_remove(MblMwDataProcessor *processor) {
    if (processor->parent_id != NO_PARENT) {
        auto parent_consumers= &processor->parent()->consumers;
        parent_consumers->erase(find(parent_consumers->begin(), parent_consumers->end(), processor->header.data_id));
    }

    function<void (MblMwDataProcessor *current)> remove_inner= [&remove_inner](MblMwDataProcessor *current) -> void {
        for(auto it: current->consumers) {
            ResponseHeader consumer_header(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), it);
            remove_inner(dynamic_cast<MblMwDataProcessor*>(current->owner->module_events.at(consumer_header)));
        }

        auto state_signal = mbl_mw_dataprocessor_get_state_data_signal(current);
        if (state_signal != nullptr) {
            state_signal->header.disable_silent();
            current->owner->module_events.erase(state_signal->header);
            delete state_signal;
        }

        current->owner->module_events.erase(current->header);
        delete current;
    };

    remove_inner(processor);
}

uint8_t mbl_mw_dataprocessor_get_id(const MblMwDataProcessor* processor) {
    return processor->header.data_id;
}

MblMwDataProcessor* mbl_mw_dataprocessor_lookup_id(const MblMwMetaWearBoard* board, uint8_t id) {
    ResponseHeader map_key(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), id);
    return dynamic_cast<MblMwDataProcessor*>(board->module_events.at(map_key));
}

void create_processor(MblMwDataSignal* source, MblMwDataProcessor* processor, void *context, MblMwFnDataProcessor processor_created) {
    auto state = GET_DATAPROCESSOR_STATE(processor->owner);
    state->pending_fns.push([state, processor, context, processor_created, source](void) -> void {
        state->next_processor= processor;
        state->processor_context= context;
        state->processor_callback= processor_created;

        vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::ADD), source->header.module_id, 
                source->header.register_id, source->header.data_id, source->get_data_ubyte(), type_to_id.at(processor->type) };
        command.insert(command.end(), (uint8_t*) processor->config, ((uint8_t*) processor->config) + processor->config_size);

        state->timeout= ThreadPool::schedule([state](void) -> void {
            if (state->next_processor->state != nullptr) {
                delete state->next_processor->state;
            }
            state->next_processor->remove = false;
            delete state->next_processor;
            state->processor_callback(state->processor_context, nullptr);

            state->create_next(true);
        }, source->owner->time_per_response);
        send_command(source->owner, command.data(), (uint8_t) command.size());
    });
    state->create_next(false);
}

void set_processor_state(MblMwDataProcessor *processor, void* new_state, uint8_t size) {
    vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::STATE), processor->header.data_id };
    if (new_state != nullptr) {
        command.insert(command.end(), (uint8_t*) new_state, ((uint8_t*) new_state) + size);
    }
    send_command(processor->owner, command.data(), (uint8_t) command.size());
}

void modify_processor_configuration(MblMwDataProcessor *processor, uint8_t size) {
    vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::PARAMETER), processor->header.data_id, 
            type_to_id.at(processor->type)};
    command.insert(command.end(), (uint8_t*) processor->config, ((uint8_t*) processor->config) + size);
    send_command(processor->owner, command.data(), (uint8_t) command.size());
}

void sync_processor_chain(MblMwMetaWearBoard* board, uint8_t id, ProcessorEntriesHandler handler) {
    auto state = GET_DATAPROCESSOR_STATE(board);

    while(!state->chain.empty()) {
        state->chain.pop();
    }
    state->entry_id = id;
    state->sync_handler = handler;

    state->pending_fns.push([=](void) -> void {
        state->timeout= ThreadPool::schedule([state, board](void) -> void {
            board->anon_signals_created(board->anon_signals_context, board, nullptr, MBL_MW_STATUS_ERROR_TIMEOUT);
            state->create_next(true);
        }, board->time_per_response);

        uint8_t command[3] = { MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::ADD)), id };
        SEND_COMMAND;
    });
    state->create_next(false);
}

MblMwDataProcessor* lookup_processor(const MblMwMetaWearBoard* board, uint8_t id) {
    ResponseHeader key = {MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), id};
    return dynamic_cast<MblMwDataProcessor*>(board->module_events.at(key));
}

namespace std {

size_t hash<DataProcessorType>::operator()(const DataProcessorType& key) const {
    return static_cast<uint8_t>(key);
}

}
