#include "metawear/processor/time.h"

#include "processor_private_common.h"

#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memcpy;

struct TimeDelayConfig {
    uint8_t length:3;
    uint8_t mode:3;
    uint8_t :2;
    uint8_t period[4];
};

int32_t mbl_mw_dataprocessor_time_create(MblMwDataSignal *source, MblMwTimeMode mode, uint32_t period,
        MblMwFnDataProcessor processor_created) {
    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);

    if (mode == MBL_MW_TIME_DIFFERENTIAL) {
        new_processor->is_signed = 1;
        if (source->interpreter == DataInterpreter::UINT32) {
            new_processor->interpreter = DataInterpreter::INT32;
        }
    }

    TimeDelayConfig *config = (TimeDelayConfig*) malloc(sizeof(TimeDelayConfig));
    *((uint8_t*) config)= 0;
    config->length= source->length() - 1;
    config->mode= mode;
    memcpy(((uint8_t*)(config)) + 1, &period, sizeof(period));
    create_processor(source, config, sizeof(TimeDelayConfig), DataProcessorType::TIME, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_time_modify_period(MblMwDataProcessor *time_delay, uint32_t period) {
    if (time_delay->type == DataProcessorType::TIME) {
        memcpy(((uint8_t*)(time_delay->config)) + 1, &period, sizeof(period));
        modify_processor_configuration(time_delay, sizeof(TimeDelayConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
