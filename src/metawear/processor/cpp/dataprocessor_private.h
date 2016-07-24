#pragma once

#include <stdint.h>
#include <vector>

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/processor/dataprocessor_fwd.h"

const uint8_t PROCESSOR_MAX_LENGTH = 4;

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
    MblMwDataProcessor(uint8_t** state_stream, MblMwMetaWearBoard *owner);
    MblMwDataProcessor(const MblMwDataSignal& signal);
    virtual ~MblMwDataProcessor();

    virtual void subscribe();
    virtual void unsubscribe();
    virtual void serialize(std::vector<uint8_t>& state) const;

    MblMwDataProcessor* parent() const;

    uint8_t parent_id;
    MblMwDataSignal* state;
    void* config;
    uint8_t config_size;
    std::vector<uint8_t> consumers;
    DataProcessorType type;
};

void init_dataprocessor_module(MblMwMetaWearBoard* board);
void free_dataprocessor_module(void* state);
void create_processor(MblMwDataSignal* source, void* config, uint8_t size, DataProcessorType type, 
        MblMwDataProcessor* new_processor, MblMwFnDataProcessor processor_created);
void create_processor_state_signal(MblMwDataProcessor* processor, DataInterpreter interpreter);
void set_processor_state(MblMwDataProcessor *processor, void* new_state, uint8_t size);
void modify_processor_configuration(MblMwDataProcessor *processor, uint8_t size);

namespace std {

template <>
struct hash<DataProcessorType> {
    size_t operator()(const DataProcessorType& key) const;
};

}
