#include "dataprocessor_private.h"

#include "metawear/core/status.h"
#include "metawear/processor/time_delay.h"

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

void mbl_mw_dataprocessor_create_time_delay(MblMwDataSignal *source, MblMwTimeMode mode, uint32_t period, 
        MblMwFnDataProcessor processor_created) {
    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);

    if (mode == MBL_MW_TIME_DIFFERENTIAL) {
        if (source->convertor == ResponseConvertor::UINT32) {
            new_processor->convertor = ResponseConvertor::INT32;
        }
    }
    TimeDelayConfig *config = (TimeDelayConfig*) malloc(sizeof(TimeDelayConfig));
    *((uint8_t*) config)= 0;
    config->length= source->length() - 1;
    config->mode= mode;
    memcpy(((uint8_t*)(config)) + 1, &period, sizeof(period));
    create_processor(source, config, sizeof(TimeDelayConfig), DataProcessorType::TIME, new_processor, processor_created);
}

int32_t mbl_mw_dataprocessor_time_delay_modify_period(MblMwDataProcessor *time_delay, uint32_t period) {
    if (time_delay->type == DataProcessorType::TIME) {
        memcpy(((uint8_t*)(time_delay->config)) + 1, &period, sizeof(period));
        modify_processor_configuration(time_delay, sizeof(TimeDelayConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
