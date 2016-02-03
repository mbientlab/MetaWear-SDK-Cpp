#include "metawear/processor/average.h"

#include "processor_private_common.h"

#include <cstdlib>

using std::malloc;

struct AverageConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t :4;
    uint8_t size;
};

int32_t mbl_mw_dataprocessor_average_create(MblMwDataSignal *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    AverageConfig *config = (AverageConfig*) malloc(sizeof(AverageConfig));
    *((uint8_t*) config)= 0;
    config->output= source->length() - 1;
    config->input= source->length() - 1;
    config->size= size;

    create_processor(source, config, sizeof(AverageConfig), DataProcessorType::AVERAGE, new MblMwDataProcessor(*source), 
            processor_created);
    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_average_reset(MblMwDataProcessor *average) {
    if (average->type == DataProcessorType::AVERAGE) {
        set_processor_state(average, nullptr, 0);
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_average_modify_size(MblMwDataProcessor *average, uint8_t size) {
    if (average->type == DataProcessorType::AVERAGE) {
        ((AverageConfig*) average->config)->size= size;
        modify_processor_configuration(average, sizeof(AverageConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
