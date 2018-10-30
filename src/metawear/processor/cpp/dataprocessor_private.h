#pragma once

#include <stdint.h>
#include <unordered_map>
#include <sstream>
#include <stack>
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
    TIME,
    ACCOUNTER,
    PACKER,
    FUSER
};

extern std::unordered_map<DataProcessorType, uint8_t> type_to_id;

struct ProcessorEntry {
    uint8_t id, offset, length;
    ResponseHeader source;
    std::vector<uint8_t> config;
};

typedef void(*ProcessorEntriesHandler)(MblMwMetaWearBoard* board, std::stack<ProcessorEntry>& entries);

struct MblMwDataProcessor : public MblMwDataSignal {
    static MblMwDataProcessor* transform(const MblMwDataSignal* input, uint8_t id, const void* config);
    static MblMwDataProcessor* transform(const MblMwDataSignal* input, const std::vector<uint8_t>& config);
    
    MblMwDataProcessor(uint8_t** state_stream, MblMwMetaWearBoard *owner);
    MblMwDataProcessor(const MblMwDataSignal& signal);
    virtual ~MblMwDataProcessor();

    virtual void subscribe();
    virtual void unsubscribe();

    virtual void serialize(std::vector<uint8_t>& state) const;
    virtual void create_uri(std::stringstream& uri) const;
    MblMwDataProcessor* parent() const;

    uint8_t parent_id;
    MblMwDataSignal* state;
    const MblMwDataSignal* input;
    void* config;
    uint8_t config_size;
    std::vector<uint8_t> consumers;
    DataProcessorType type;
};

void init_dataprocessor_module(MblMwMetaWearBoard* board);
void free_dataprocessor_module(void* state);
void create_processor(MblMwDataSignal* source, MblMwDataProcessor* processor, void *context, MblMwFnDataProcessor processor_created);
void set_processor_state(MblMwDataProcessor *processor, void* new_state, uint8_t size);
void modify_processor_configuration(MblMwDataProcessor *processor, uint8_t size);
void sync_processor_chain(MblMwMetaWearBoard* board, uint8_t id, ProcessorEntriesHandler handler);
void disconnect_dataprocessor(MblMwMetaWearBoard* board);
MblMwDataProcessor* lookup_processor(const MblMwMetaWearBoard* board, uint8_t id);

namespace std {

template <>
struct hash<DataProcessorType> {
    size_t operator()(const DataProcessorType& key) const;
};

}
