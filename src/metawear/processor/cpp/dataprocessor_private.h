#pragma once

#include <stdint.h>

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/cpp/datasignal_private.h"

enum class DataProcessorType : uint8_t {
    ACCUMULATOR,
    AVERAGE,
    BUFFER,
    COMPARATOR,
    COUNTER,
    DELTA,
    MATH,
    PASSTHROUGH,
    PULSE,
    RMS,
    RSS,
    SAMPLE,
    THRESHOLD,
    TIME
};

struct MblMwDataProcessor : public MblMwDataSignal {
    MblMwDataProcessor(const ResponseHeader& header, MblMwMetaWearBoard *owner, ResponseConvertor convertor, uint8_t n_channels, 
            uint8_t channel_size, uint8_t is_signed, uint8_t offset);
    MblMwDataProcessor(const MblMwDataSignal& signal);
    ~MblMwDataProcessor();

    void* config;
    DataProcessorType type;
};

void create_processor(MblMwDataSignal* source, void* config, uint8_t size, DataProcessorType type, 
        MblMwDataProcessor* new_processor, MblMwFnDataProcessor processor_created);
void set_processor_state(MblMwDataProcessor *processor, void* new_state, uint8_t size);
void modify_processor_configuration(MblMwDataProcessor *processor, uint8_t size);

namespace std {

template <>
struct hash<DataProcessorType> {
    size_t operator()(const DataProcessorType& key) const;
};

}
