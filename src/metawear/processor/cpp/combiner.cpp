#include "dataprocessor_private.h"

#include "metawear/core/data.h"
#include "metawear/processor/combiner.h"
#include "metawear/sensor/cpp/accelerometer_bmi160_register.h"
#include "metawear/sensor/cpp/accelerometer_mma8452q_register.h"
#include "metawear/sensor/cpp/gyro_bmi160_register.h"

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

static inline void create_combiner(MblMwDataSignal *source, DataProcessorType combiner_type, 
        MblMwFnDataProcessor processor_created) {

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);
    new_processor->is_signed= 0;
    new_processor->offset= 0;
    new_processor->set_channel_attr(1, source->channel_size);

    switch (source->convertor) {
    case ResponseConvertor::BMI160_ACCELERATION:
        new_processor->convertor= ResponseConvertor::BMI160_ACCELERATION_SINGLE_AXIS;
        break;
    case ResponseConvertor::MMA8452Q_ACCELERATION:
        new_processor->convertor = ResponseConvertor::MMA8452Q_ACCELERATION_SINGLE_AXIS;
        break;
    case ResponseConvertor::BMI160_ROTATION:
        new_processor->convertor = ResponseConvertor::BMI160_ROTATION_SINGLE_AXIS;
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
}

void mbl_mw_dataprocessor_create_rms(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    create_combiner(source, DataProcessorType::RMS, processor_created);
}

void mbl_mw_dataprocessor_create_rss(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    create_combiner(source, DataProcessorType::RSS, processor_created);
}
