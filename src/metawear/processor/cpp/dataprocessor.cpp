#include "dataprocessor_private.h"
#include "dataprocessor_register.h"
#include "metawear/processor/dataprocessor.h"

#include "metawear/core/cpp/metawearboard_def.h"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <unordered_map>
#include <vector>

using std::find;
using std::free;
using std::function;
using std::unordered_map;
using std::vector;

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

static void data_processor_subscribe(MblMwDataSignal *source) {
    uint8_t enable_command[4]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY_ENABLE), 
            source->header.data_id, 1};
    send_command(source->owner, enable_command, sizeof(enable_command));

    uint8_t enable_notify[3]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), 1};
    send_command(source->owner, enable_notify, sizeof(enable_notify));
}

static void data_processor_unsubscribe(MblMwDataSignal *source) {
    uint8_t disable_command[4]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY_ENABLE), 
            source->header.data_id, 0};
    send_command(source->owner, disable_command, sizeof(disable_command));
}

MblMwDataProcessor::MblMwDataProcessor(const MblMwDataSignal& signal) : MblMwDataSignal(signal.header, signal.owner, 
        signal.interpreter, signal.n_channels, signal.channel_size, signal.is_signed, signal.offset, signal.number_to_firmware, 
        data_processor_subscribe, data_processor_unsubscribe), parent(nullptr), state(nullptr), config(nullptr) {
}

MblMwDataProcessor::~MblMwDataProcessor() {
    delete state;
    state = nullptr;

    free(config);
    config= nullptr;

    if (remove) {
        uint8_t command[3]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::REMOVE), header.data_id };
        send_command(owner, command, sizeof(command));
    }
}

const MblMwDataSignal* mbl_mw_dataprocessor_get_state_data_signal(MblMwDataProcessor *processor) {
    return processor->state;
}

void mbl_mw_dataprocessor_read_state(MblMwDataProcessor *processor) {
    uint8_t command[3]= { MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::STATE)), processor->header.data_id };
    send_command(processor->owner, command, sizeof(command));
}

void mbl_mw_dataprocessor_remove(MblMwDataProcessor *processor) {
    if (processor->parent != nullptr) {
        auto parent_consumers= &processor->parent->consumers;
        parent_consumers->erase(find(parent_consumers->begin(), parent_consumers->end(), processor));
    }

    function<void (MblMwDataProcessor *current)> remove_inner= [&remove_inner](MblMwDataProcessor *current) -> void {
        for(auto it: current->consumers) {
            remove_inner(it);
        }

        current->owner->sensor_data_signals.erase(current->header);
        delete current;
    };

    remove_inner(processor);
}

void create_processor(MblMwDataSignal *source, void* config, uint8_t size, DataProcessorType type, 
        MblMwDataProcessor* new_processor, MblMwFnDataProcessor processor_created) {
    new_processor->offset = 0;
    new_processor->header.module_id = MBL_MW_MODULE_DATA_PROCESSOR;
    new_processor->header.register_id = ORDINAL(DataProcessorRegister::NOTIFY);
    new_processor->config= config;
    new_processor->type= type;

    source->owner->pending_dataprocessors.push(new_processor);
    if (MblMwDataProcessor* src_processor= dynamic_cast<MblMwDataProcessor*>(source)) {
        new_processor->parent= src_processor;
        src_processor->consumers.push_back(new_processor);
    }

    vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::ADD), source->header.module_id, 
            source->header.register_id, source->header.data_id, source->get_data_ubyte(), type_to_id.at(type) };

    source->owner->dataprocessor_callbacks.push(processor_created);
    command.insert(command.end(), (uint8_t*) config, ((uint8_t*) config) + size);

    send_command(source->owner, command.data(), (uint8_t) command.size());
}

MblMwDataSignal* create_processor_state_signal(MblMwDataProcessor* processor, DataInterpreter interpreter) {
    ResponseHeader state_header = ResponseHeader(MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::STATE)));
    return new MblMwDataSignal(state_header, processor->owner, interpreter, processor->n_channels, processor->channel_size, processor->is_signed, 0);
}

void set_processor_state(MblMwDataProcessor *processor, void* new_state, uint8_t size) {
    vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::STATE), 
            processor->header.data_id };
    if (new_state != nullptr) {
        command.insert(command.end(), (uint8_t*) new_state, ((uint8_t*) new_state) + size);
    }
    send_command(processor->owner, command.data(), (uint8_t) command.size());
}

void modify_processor_configuration(MblMwDataProcessor *processor, uint8_t size) {
    vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::PARAMETER), 
            processor->header.data_id, type_to_id.at(processor->type)};
    command.insert(command.end(), (uint8_t*) processor->config, ((uint8_t*) processor->config) + size);
    send_command(processor->owner, command.data(), (uint8_t) command.size());
}

namespace std {

size_t hash<DataProcessorType>::operator()(const DataProcessorType& key) const {
    return static_cast<uint8_t>(key);
}

}
