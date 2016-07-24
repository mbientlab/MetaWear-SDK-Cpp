#include "metawear/processor/rms.h"
#include "metawear/processor/rss.h"

#include "processor_private_common.h"

#include <cstdlib>

using std::malloc;

const int8_t MODE_RMS= 0;
const int8_t MODE_RSS= 1;

struct CombinerConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t count:3;
    uint8_t is_signed:1;
    uint8_t mode;
};

static inline int32_t create_combiner(MblMwDataSignal *source, DataProcessorType combiner_type,
        MblMwFnDataProcessor processor_created) {
    if (source->n_channels <= 1) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);
    new_processor->is_signed= 0;
    new_processor->offset= 0;
    new_processor->set_channel_attr(1, source->channel_size);

    switch (source->interpreter) {
    case DataInterpreter::BOSCH_ACCELERATION:
        new_processor->interpreter= DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS;
        break;
    case DataInterpreter::MMA8452Q_ACCELERATION:
        new_processor->interpreter = DataInterpreter::MMA8452Q_ACCELERATION_UNSIGNED_SINGLE_AXIS;
        break;
    case DataInterpreter::BMI160_ROTATION:
        new_processor->interpreter = DataInterpreter::BMI160_ROTATION_UNSIGNED_SINGLE_AXIS;
        break;
    case DataInterpreter::BMM150_B_FIELD:
        new_processor->interpreter = DataInterpreter::BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS;
        break;
    case DataInterpreter::TCS34725_COLOR_ADC:
        new_processor->interpreter = DataInterpreter::UINT32;
        break;
    default:
        break;
    }

    CombinerConfig *config = (CombinerConfig*) malloc(sizeof(CombinerConfig));
    config->output= source->channel_size - 1;
    config->input= source->channel_size - 1;
    config->count= source->n_channels - 1;
    config->is_signed= source->is_signed;
    config->mode= combiner_type == DataProcessorType::RMS ? MODE_RMS : MODE_RSS;
    create_processor(source, config, sizeof(CombinerConfig), combiner_type, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_rms_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    return create_combiner(source, DataProcessorType::RMS, processor_created);
}

int32_t mbl_mw_dataprocessor_rss_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    return create_combiner(source, DataProcessorType::RSS, processor_created);
}
