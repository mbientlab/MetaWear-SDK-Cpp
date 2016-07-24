#include "metawear/processor/threshold.h"

#include "processor_private_common.h"

#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memcpy;

struct ThresholdConfig {
    uint8_t length:2;
    uint8_t is_signed:1;
    uint8_t mode:3;
    uint8_t boundary[4];
    uint8_t hysteresis[2];
};

int32_t mbl_mw_dataprocessor_threshold_create(MblMwDataSignal *source, MblMwThresholdMode mode, float boundary,
        float hysteresis, MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);

    if (mode == MBL_MW_THRESHOLD_MODE_BINARY) {
        new_processor->is_signed = 1;
        new_processor->interpreter = DataInterpreter::INT32;
        new_processor->set_channel_attr(1, 1);
        new_processor->converter = FirmwareConverter::DEFAULT;
    }

    int32_t scaled_boundary= (int32_t) number_to_firmware_converters.at(source->converter)(source, boundary);
    uint16_t scaled_hysteresis= (uint16_t) number_to_firmware_converters.at(source->converter)(source, hysteresis);

    ThresholdConfig *config = (ThresholdConfig*) malloc(sizeof(ThresholdConfig));
    *((uint8_t*) config)= 0;
    config->length= source->length() - 1;
    config->is_signed= source->is_signed;
    config->mode= mode;
    memcpy(((uint8_t*)(config)) + 1, &scaled_boundary, sizeof(scaled_boundary));
    memcpy(((uint8_t*)(config)) + 5, &scaled_hysteresis, sizeof(scaled_hysteresis));

    create_processor(source, config, sizeof(ThresholdConfig), DataProcessorType::THRESHOLD, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_threshold_modify_boundary(MblMwDataProcessor *threshold, float boundary, float hysteresis) {
    if (threshold->type == DataProcessorType::THRESHOLD) {
        int32_t scaled_boundary= (int32_t) number_to_firmware_converters.at(threshold->converter)(threshold, boundary);
        uint16_t scaled_hysteresis= (uint16_t) number_to_firmware_converters.at(threshold->converter)(threshold, hysteresis);

        memcpy(((uint8_t*)(threshold->config)) + 1, &scaled_boundary, sizeof(scaled_boundary));
        memcpy(((uint8_t*)(threshold->config)) + 5, &scaled_hysteresis, sizeof(scaled_hysteresis));

        modify_processor_configuration(threshold, sizeof(ThresholdConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
