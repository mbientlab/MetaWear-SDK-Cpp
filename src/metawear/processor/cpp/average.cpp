#include "metawear/processor/average.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "processor_private_common.h"

#include <cstdlib>

using std::calloc;

struct AverageConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t :1;
    uint8_t mode:1;
    uint8_t :2;
    uint8_t size;
    uint8_t count;
};

const uint8_t HPF_REVISION = 2;

static int32_t apply_average(MblMwDataSignal *source, bool use_hpf, uint8_t size, MblMwFnDataProcessor processor_created) {
    bool has_hpf = source->owner->module_info.at(MBL_MW_MODULE_DATA_PROCESSOR).revision >= HPF_REVISION;

    if (!has_hpf && source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    AverageConfig *config = (AverageConfig*) calloc(1, sizeof(AverageConfig));
    config->output= source->length() - 1;
    config->input= source->length() - 1;
    config->size= size;
    config->count = source->n_channels - 1;

    if (has_hpf && use_hpf) {
        config->mode = 1;
    }

    create_processor(source, config, sizeof(AverageConfig) - (has_hpf ? 0 : 1), DataProcessorType::AVERAGE, new MblMwDataProcessor(*source), 
            processor_created);
    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_average_create(MblMwDataSignal *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    return mbl_mw_dataprocessor_lowpass_create(source, size, processor_created);
}

int32_t mbl_mw_dataprocessor_highpass_create(MblMwDataSignal *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    return apply_average(source, true, size, processor_created);
}

int32_t mbl_mw_dataprocessor_lowpass_create(MblMwDataSignal *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    return apply_average(source, false, size, processor_created);
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
        bool has_hpf = average->owner->module_info.at(MBL_MW_MODULE_DATA_PROCESSOR).revision >= HPF_REVISION;
        ((AverageConfig*) average->config)->size= size;
        modify_processor_configuration(average, sizeof(AverageConfig) - (has_hpf ? 0 : 1));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
