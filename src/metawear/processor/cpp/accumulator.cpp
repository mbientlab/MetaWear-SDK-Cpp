#include "dataprocessor_private.h"

#include "metawear/processor/accumulator.h"
#include "metawear/core/status.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <cstdlib>

using std::malloc;

const uint8_t MODE_ACCUMULATE = 0;
const uint8_t MODE_COUNT = 1;

struct AccumulatorConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t mode:3;
};

static inline void create_accumulator_processor(MblMwDataSignal *source, uint8_t n_channels, uint8_t channel_size, 
        DataProcessorType accum_type, MblMwFnDataProcessor processor_created) {
    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);
    new_processor->set_channel_attr(n_channels, channel_size);
    if (accum_type == DataProcessorType::COUNTER) {
        new_processor->is_signed= 0;
        new_processor->number_to_firmware = number_to_firmware_default;
        new_processor->convertor = ResponseConvertor::UINT32;
    }

    AccumulatorConfig *config = (AccumulatorConfig*) malloc(sizeof(AccumulatorConfig));
    *((uint8_t*) config)= 0;
    config->output= (new_processor->length() - 1);
    config->input= (source->length() - 1);
    config->mode= accum_type == DataProcessorType::ACCUMULATOR ? MODE_ACCUMULATE : MODE_COUNT;

    create_processor(source, config, sizeof(AccumulatorConfig), accum_type, new_processor, processor_created);
}

void mbl_mw_dataprocessor_create_accumulator(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    create_accumulator_processor(source, source->n_channels, source->channel_size, DataProcessorType::ACCUMULATOR, 
            processor_created);
}
void mbl_mw_dataprocessor_create_accumulator_size(MblMwDataSignal *source, uint8_t output_size, 
        MblMwFnDataProcessor processor_created) {
    create_accumulator_processor(source, source->n_channels, output_size, DataProcessorType::ACCUMULATOR, processor_created);
}
int32_t mbl_mw_dataprocessor_set_accumulator_state(MblMwDataProcessor *accumulator, float new_running_sum) {
    if (accumulator->type == DataProcessorType::ACCUMULATOR) {
        uint32_t scaled_sum= (uint32_t) accumulator->number_to_firmware(accumulator, new_running_sum);
        set_processor_state(accumulator, &scaled_sum, sizeof(scaled_sum));
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

void mbl_mw_dataprocessor_create_counter(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    create_accumulator_processor(source, 1, 1, DataProcessorType::COUNTER, processor_created);
}
void mbl_mw_dataprocessor_create_counter_size(MblMwDataSignal *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    create_accumulator_processor(source, 1, size, DataProcessorType::COUNTER, processor_created);
}
int32_t mbl_mw_dataprocessor_set_counter_state(MblMwDataProcessor *counter, uint32_t new_count) {
    if (counter->type == DataProcessorType::COUNTER) {
        set_processor_state(counter, &new_count, sizeof(new_count));
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
