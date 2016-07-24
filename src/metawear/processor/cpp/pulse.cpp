#include "metawear/processor/pulse.h"

#include "processor_private_common.h"

#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memcpy;

struct PulseDetectorConfig {
    uint8_t length;
    uint8_t trigger_mode;
    uint8_t output_mode;
    uint8_t threshold[4];
    uint8_t width[2];
};

int32_t mbl_mw_dataprocessor_pulse_create(MblMwDataSignal *source, MblMwPulseOutput output, float threshold,
        uint16_t width, MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);

    switch (output) {
    case MBL_MW_PULSE_OUTPUT_WIDTH:
        new_processor->set_channel_attr(1, 2);
        new_processor->interpreter = DataInterpreter::UINT32;
        new_processor->converter = FirmwareConverter::DEFAULT;
        break;
    case MBL_MW_PULSE_OUTPUT_AREA:
        new_processor->set_channel_attr(1, 4);
        break;
   case MBL_MW_PULSE_OUTPUT_ON_DETECTION:
        new_processor->is_signed = 0;
        new_processor->set_channel_attr(1, 1);
        new_processor->interpreter = DataInterpreter::UINT32;
        new_processor->converter = FirmwareConverter::DEFAULT;
        break;
    default:
        break;
    }
    
    int32_t scaled_threshold= (int32_t) number_to_firmware_converters.at(source->converter)(source, threshold);

    PulseDetectorConfig *config = (PulseDetectorConfig*) malloc(sizeof(PulseDetectorConfig));
    config->length= source->length() - 1;
    config->trigger_mode= 0;
    config->output_mode= output;
    memcpy(((uint8_t*)(config)) + 3, &scaled_threshold, sizeof(scaled_threshold));
    memcpy(((uint8_t*)(config)) + 7, &width, sizeof(width));
    create_processor(source, config, sizeof(PulseDetectorConfig), DataProcessorType::PULSE, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_pulse_modify(MblMwDataProcessor *pulse, float threshold, uint16_t width) {
    if (pulse->type == DataProcessorType::PULSE) {
        int32_t scaled_threshold= (int32_t) number_to_firmware_converters.at(pulse->converter)(pulse, threshold);
        memcpy(((uint8_t*)(pulse->config)) + 3, &scaled_threshold, sizeof(scaled_threshold));
        memcpy(((uint8_t*)(pulse->config)) + 7, &width, sizeof(width));

        modify_processor_configuration(pulse, sizeof(PulseDetectorConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
