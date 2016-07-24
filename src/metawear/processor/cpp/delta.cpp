#include "metawear/processor/delta.h"

#include "processor_private_common.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memcpy;
using std::uint8_t;

struct DeltaConfig {
    uint8_t length:2;
    uint8_t is_signed:1;
    uint8_t mode:3;
    uint8_t magnitude[4];
};

int32_t mbl_mw_dataprocessor_delta_create(MblMwDataSignal *source, MblMwDeltaMode mode, float magnitude,
        MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);

    switch (mode) {
    case MBL_MW_DELTA_MODE_DIFFERENTIAL:
        new_processor->is_signed = 1;
        if (source->interpreter == DataInterpreter::UINT32) {
            new_processor->interpreter = DataInterpreter::INT32;
        }
        break;
    case MBL_MW_DELTA_MODE_BINARY:
        new_processor->is_signed = 1;
        new_processor->interpreter = DataInterpreter::INT32;
        new_processor->set_channel_attr(1, 1);
        new_processor->converter = FirmwareConverter::DEFAULT;
        break;
    default:
        break;
    }
    create_processor_state_signal(new_processor, new_processor->interpreter);    

    uint32_t scaled_magnitude= (uint32_t) number_to_firmware_converters.at(source->converter)(source, magnitude);

    DeltaConfig *config = (DeltaConfig*) malloc(sizeof(DeltaConfig));
    *((uint8_t*) config)= 0;
    config->length= source->length() - 1;
    config->is_signed= source->is_signed;
    config->mode= mode;
    memcpy(((uint8_t*)(config)) + 1, &scaled_magnitude, sizeof(scaled_magnitude));
    create_processor(source, config, sizeof(DeltaConfig), DataProcessorType::DELTA, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_delta_set_reference(MblMwDataProcessor *delta, float previous_value) {
    if (delta->type == DataProcessorType::DELTA) {
        int32_t scaled_previous_value= (int32_t) number_to_firmware_converters.at(delta->converter)(delta, previous_value);
        set_processor_state(delta, &scaled_previous_value, sizeof(scaled_previous_value));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_delta_modify_magnitude(MblMwDataProcessor *delta, float magnitude) {
    if (delta->type == DataProcessorType::DELTA) {
        uint32_t scaled_magnitude= (uint32_t) number_to_firmware_converters.at(delta->converter)(delta, magnitude);
        memcpy(((uint8_t*)(delta->config)) + 1, &scaled_magnitude, sizeof(scaled_magnitude));

        modify_processor_configuration(delta, sizeof(DeltaConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
