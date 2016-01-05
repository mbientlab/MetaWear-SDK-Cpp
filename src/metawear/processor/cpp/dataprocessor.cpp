#include "dataprocessor_private.h"
#include "dataprocessor_register.h"

#include "metawear/core/cpp/connection_def.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <cstdlib>
#include <unordered_map>
#include <vector>

using std::free;
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
    send_command_wrapper(source->owner, enable_command, sizeof(enable_command));

    uint8_t enable_notify[3]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY), 1};
    send_command_wrapper(source->owner, enable_notify, sizeof(enable_notify));
}

static void data_processor_unsubscribe(MblMwDataSignal *source) {
    uint8_t disable_command[4]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY_ENABLE), 
            source->header.data_id, 0};
    send_command_wrapper(source->owner, disable_command, sizeof(disable_command));
}

MblMwDataProcessor::MblMwDataProcessor(const ResponseHeader& header, MblMwMetaWearBoard *owner, ResponseConvertor convertor, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset) : 
        MblMwDataSignal(header, owner, convertor, n_channels, channel_size, is_signed, offset) {
    subscribe= data_processor_subscribe;
    unsubscribe= data_processor_unsubscribe;
}

MblMwDataProcessor::MblMwDataProcessor(const MblMwDataSignal& signal) : MblMwDataSignal(signal.header, signal.owner, 
        signal.convertor, signal.n_channels, signal.channel_size, signal.is_signed, signal.offset, signal.number_to_firmware, 
        data_processor_subscribe, data_processor_unsubscribe) {
    config= nullptr;
}

MblMwDataProcessor::~MblMwDataProcessor() {
    free(config);
    config= nullptr;
}

void create_processor(MblMwDataSignal *source, void* config, uint8_t size, DataProcessorType type, 
        MblMwDataProcessor* new_processor, MblMwFnDataProcessor processor_created) {
    new_processor->offset = 0;
    new_processor->header.module_id = MBL_MW_MODULE_DATA_PROCESSOR;
    new_processor->header.register_id = ORDINAL(DataProcessorRegister::NOTIFY);
    new_processor->config= config;
    new_processor->type= type;
    source->owner->pending_dataprocessors.push(new_processor);

    vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::ADD), source->header.module_id, 
            source->header.register_id, source->header.data_id, source->get_data_ubyte(), type_to_id.at(type) };

    source->owner->dataprocessor_callbacks.push(processor_created);
    command.insert(command.end(), (uint8_t*) config, ((uint8_t*) config) + size);

    send_command_wrapper(source->owner, command.data(), (uint8_t) command.size());
}

void set_processor_state(MblMwDataProcessor *processor, void* new_state, uint8_t size) {
    vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::STATE), 
            processor->header.data_id };
    if (new_state != nullptr) {
        command.insert(command.end(), (uint8_t*) new_state, ((uint8_t*) new_state) + size);
    }
    send_command_wrapper(processor->owner, command.data(), (uint8_t) command.size());
}

void modify_processor_configuration(MblMwDataProcessor *processor, uint8_t size) {
    vector<uint8_t> command = { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::PARAMETER), 
            processor->header.data_id, type_to_id.at(processor->type)};
    command.insert(command.end(), (uint8_t*) processor->config, ((uint8_t*) processor->config) + size);
    send_command_wrapper(processor->owner, command.data(), (uint8_t) command.size());
}

namespace std {

size_t hash<DataProcessorType>::operator()(const DataProcessorType& key) const {
    return static_cast<uint8_t>(key);
}

}
