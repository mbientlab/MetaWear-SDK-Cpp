#pragma once

#include <stdint.h>
#include <vector>

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
    MblMwDataProcessor(const MblMwDataSignal& signal);
    virtual ~MblMwDataProcessor();

    MblMwDataProcessor* parent;
    MblMwDataSignal* state;
    void* config;
    std::vector<MblMwDataProcessor*> consumers;
    DataProcessorType type;
};

void create_processor(MblMwDataSignal* source, void* config, uint8_t size, DataProcessorType type, 
        MblMwDataProcessor* new_processor, MblMwFnDataProcessor processor_created);
MblMwDataSignal* create_processor_state_signal(MblMwDataProcessor* processor, DataInterpreter interpreter);
void set_processor_state(MblMwDataProcessor *processor, void* new_state, uint8_t size);
void modify_processor_configuration(MblMwDataProcessor *processor, uint8_t size);

namespace std {

template <>
struct hash<DataProcessorType> {
    size_t operator()(const DataProcessorType& key) const;
};

}
