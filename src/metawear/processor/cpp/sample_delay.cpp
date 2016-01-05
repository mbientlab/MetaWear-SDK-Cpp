#include "dataprocessor_private.h"

#include "metawear/core/status.h"
#include "metawear/processor/sample_delay.h"

#include <cstdlib>

using std::malloc;

struct SampleDelayConfig {
    uint8_t length;
    uint8_t bin_size;
};

void mbl_mw_dataprocessor_create_sample_delay(MblMwDataSignal *source, uint8_t bin_size, MblMwFnDataProcessor processor_created) {
    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);

    SampleDelayConfig *config = (SampleDelayConfig*) malloc(sizeof(SampleDelayConfig));
    config->length= source->length() - 1;
    config->bin_size= bin_size;
    create_processor(source, config, sizeof(SampleDelayConfig), DataProcessorType::SAMPLE, new_processor, processor_created);
}

int32_t mbl_mw_dataprocessor_sample_delay_modify_bin_size(MblMwDataProcessor *sample_delay, uint8_t bin_size) {
    if (sample_delay->type == DataProcessorType::SAMPLE) {
        SampleDelayConfig* current_config = (SampleDelayConfig*) sample_delay->config;
        current_config->bin_size= bin_size;
        modify_processor_configuration(sample_delay, sizeof(SampleDelayConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
