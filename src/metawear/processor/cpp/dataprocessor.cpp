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

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <unordered_map>
#include <vector>

using std::find;
using std::forward_as_tuple;
using std::free;
using std::function;
using std::malloc;
using std::make_shared;
using std::memcpy;
using std::piecewise_construct;
using std::static_pointer_cast;
using std::unordered_map;
using std::vector;

#define GET_DATAPROCESSOR_STATE(board) static_pointer_cast<DataProcessorState>(board->dp_state)

const uint8_t NO_PARENT = -1;
const ResponseHeader DATAPROCESSOR_RESPONSE_HEADER(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY));

static unordered_map<DataProcessorType, uint8_t> type_to_id= {
    {DataProcessorType::ACCUMULATOR, 0x2},
    {DataProcessorType::AVERAGE, 0x3},
    {DataProcessorType::BUFFER, 0xf},
    {DataProcessorType::COMPARATOR, 0x6},
    {DataProcessorType::COUNTER, 0x2},
    {DataProcessorType::DELTA, 0xc},
    {DataProcessorType::MATH, 0x9},
    {DataProcessorType::PASSTHROUGH, 0x1},
    {DataProcessorType::PULSE, 0xb},
    {DataProcessorType::RMS, 0x7},
    {DataProcessorType::RSS, 0x7},
    {DataProcessorType::SAMPLE, 0xa},
    {DataProcessorType::THRESHOLD, 0xd},
    {DataProcessorType::TIME, 0x8}
};

struct DataProcessorState : public AsyncCreator {
    MblMwFnDataProcessor processor_callback;
    MblMwDataProcessor* next_processor;
};

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
        parent_id(NO_PARENT), state(nullptr), config(nullptr) {
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
    state->processor_callback(state->next_processor);
    state->create_next(true);

    return MBL_MW_STATUS_OK;
}

void init_dataprocessor_module(MblMwMetaWearBoard* board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY)),
        forward_as_tuple(response_handler_data_with_id));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::ADD)),
        forward_as_tuple(dataprocessor_created));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::STATE))),
        forward_as_tuple(response_handler_data_with_id));

    if (!board->dp_state) {
        board->dp_state = make_shared<DataProcessorState>();
    }
}

void free_dataprocessor_module(void* state) {
    delete (DataProcessorState*) state;
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
    return board->module_events.count(map_key) ? dynamic_cast<MblMwDataProcessor*>(board->module_events.at(map_key)) : nullptr;
}

void create_processor(MblMwDataSignal *source, void* config, uint8_t size, DataProcessorType type, MblMwDataProcessor* new_processor, 
        MblMwFnDataProcessor processor_created) {
    new_processor->offset = 0;
    new_processor->header.module_id = MBL_MW_MODULE_DATA_PROCESSOR;
    new_processor->header.register_id = ORDINAL(DataProcessorRegister::NOTIFY);
    new_processor->config= config;
    new_processor->config_size= size;
    new_processor->type= type;
    if (MblMwDataProcessor* src_processor= dynamic_cast<MblMwDataProcessor*>(source)) {
        new_processor->parent_id= src_processor->header.data_id;
    }
    
    auto state = GET_DATAPROCESSOR_STATE(source->owner);
    state->pending_fns.push([state, new_processor, processor_created, source](void) -> void {
        state->next_processor= new_processor;
        state->processor_callback= processor_created;

        vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::ADD), source->header.module_id, 
                source->header.register_id, source->header.data_id, source->get_data_ubyte(), type_to_id.at(new_processor->type) };
        command.insert(command.end(), (uint8_t*) new_processor->config, ((uint8_t*) new_processor->config) + new_processor->config_size);

        state->timeout= ThreadPool::schedule([state](void) -> void {
            if (state->next_processor->state != nullptr) {
                delete state->next_processor->state;
            }
            state->next_processor->remove = false;
            delete state->next_processor;
            state->processor_callback(nullptr);

            state->create_next(true);
        }, source->owner->time_per_response);
        send_command(source->owner, command.data(), (uint8_t) command.size());
    });
    state->create_next(false);
}

void create_processor_state_signal(MblMwDataProcessor* processor, DataInterpreter interpreter) {
    ResponseHeader state_header(MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::STATE)));
    processor->state= new MblMwDataSignal(state_header, processor->owner, interpreter, processor->n_channels, 
            processor->channel_size, processor->is_signed, 0);
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

namespace std {

size_t hash<DataProcessorType>::operator()(const DataProcessorType& key) const {
    return static_cast<uint8_t>(key);
}

}
