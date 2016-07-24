#include "metawear/processor/accumulator.h"
#include "metawear/processor/counter.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "processor_private_common.h"

#include <cstdlib>

using std::malloc;

const uint8_t MODE_ACCUMULATE = 0;
const uint8_t MODE_COUNT = 1;

struct AccumulatorConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t mode:3;
};

static inline int32_t create_accumulator_processor(MblMwDataSignal *source, uint8_t n_channels, uint8_t channel_size, 
        DataProcessorType accum_type, MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);
    new_processor->set_channel_attr(n_channels, channel_size);
    if (accum_type == DataProcessorType::COUNTER) {
        new_processor->is_signed= 0;
        new_processor->converter = FirmwareConverter::DEFAULT;
        new_processor->interpreter = DataInterpreter::UINT32;
    }
    create_processor_state_signal(new_processor, new_processor->interpreter);

    AccumulatorConfig *config = (AccumulatorConfig*) malloc(sizeof(AccumulatorConfig));
    *((uint8_t*) config)= 0;
    config->output= (new_processor->length() - 1);
    config->input= (source->length() - 1);
    config->mode= accum_type == DataProcessorType::ACCUMULATOR ? MODE_ACCUMULATE : MODE_COUNT;
    create_processor(source, config, sizeof(AccumulatorConfig), accum_type, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_accumulator_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    return create_accumulator_processor(source, source->n_channels, source->channel_size, DataProcessorType::ACCUMULATOR, 
            processor_created);
}
int32_t mbl_mw_dataprocessor_accumulator_create_size(MblMwDataSignal *source, uint8_t output_size,
        MblMwFnDataProcessor processor_created) {
    return create_accumulator_processor(source, source->n_channels, output_size, DataProcessorType::ACCUMULATOR, processor_created);
}
int32_t mbl_mw_dataprocessor_set_accumulator_state(MblMwDataProcessor *accumulator, float new_running_sum) {
    if (accumulator->type == DataProcessorType::ACCUMULATOR) {
        uint32_t scaled_sum= (uint32_t) number_to_firmware_converters.at(accumulator->converter)(accumulator, new_running_sum);
        set_processor_state(accumulator, &scaled_sum, sizeof(scaled_sum));
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_counter_create(MblMwEvent *source, MblMwFnDataProcessor processor_created) {
    return mbl_mw_dataprocessor_counter_create_size(source, 1, processor_created);
}
int32_t mbl_mw_dataprocessor_counter_create_size(MblMwEvent *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    if (MblMwDataSignal* src_signal= dynamic_cast<MblMwDataSignal*>(source)) {
        return create_accumulator_processor(src_signal, 1, 1, DataProcessorType::COUNTER, processor_created);
    }

    MblMwDataSignal event_signal(*source);
    return create_accumulator_processor(&event_signal, 1, 1, DataProcessorType::COUNTER, processor_created);
}
int32_t mbl_mw_dataprocessor_counter_set_state(MblMwDataProcessor *counter, uint32_t new_count) {
    if (counter->type == DataProcessorType::COUNTER) {
        set_processor_state(counter, &new_count, sizeof(new_count));
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
