#include "processor_private_common.h"

#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/version.h"

#include "metawear/processor/comparator.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memcpy;
using std::memset;
using std::min;
using std::uint8_t;

#define SCALE_MULTI_COMP_REFERENCE(signal, type) for(uint8_t i= 0; i < references_length; i++) {\
            type scaled= (type) number_to_firmware_converters.at(signal->converter)(signal, references[i]);\
            memcpy(config->references + offset, &scaled, sizeof(type));\
            offset+= sizeof(type);\
        }\

const Version MULTI_COMPARE(1, 2, 3);

struct ComparatorConfig {
    uint8_t is_signed;
    uint8_t operation;
    uint8_t :8;
    uint8_t reference[4];
};

struct MultiComparatorConfig {
    uint8_t is_signed:1;
    uint8_t length:2;
    uint8_t operation:3;
    uint8_t mode:2;
    uint8_t references[12];
};

static inline int32_t create_multi_comparator(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, float references[], 
        uint8_t references_length, uint8_t is_signed, MblMwFnDataProcessor processor_created) {
    if (source->owner->firmware_revision < MULTI_COMPARE || source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MultiComparatorConfig* config= (MultiComparatorConfig*) malloc(sizeof(MultiComparatorConfig));
    config->is_signed= is_signed;
    config->length= source->length() - 1;
    config->operation= op;
    config->mode= mode;

    uint8_t offset= 0;
    switch(source->length()) {
    case 1:
        SCALE_MULTI_COMP_REFERENCE(source, int8_t);
        break;
    case 2:
        SCALE_MULTI_COMP_REFERENCE(source, int16_t);
        break;
    case 4:
        SCALE_MULTI_COMP_REFERENCE(source, int32_t);
        break;
    }

    MblMwDataProcessor *new_processor= new MblMwDataProcessor(*source);
    if (mode == MBL_MW_COMPARATOR_MODE_ZONE || mode == MBL_MW_COMPARATOR_MODE_BINARY) {
        new_processor->interpreter= DataInterpreter::UINT32;
        new_processor->converter= FirmwareConverter::DEFAULT;
        new_processor->is_signed= 0;
        new_processor->set_channel_attr(1, 1);
    }

    create_processor(source, config, 1 + offset, DataProcessorType::COMPARATOR, new_processor, processor_created);
    return MBL_MW_STATUS_OK;
}

static inline int32_t create_comprator(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, uint8_t is_signed, 
        MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    if (source->owner->firmware_revision < MULTI_COMPARE) {
        ComparatorConfig* config = (ComparatorConfig*) malloc(sizeof(ComparatorConfig));
        *((uint8_t*) config + 2)= 0;
        config->is_signed= is_signed;
        config->operation= op;

        int32_t scaled_reference= (int32_t) number_to_firmware_converters.at(source->converter)(source, reference);
        memcpy(((uint8_t*)(config)) + 3, &scaled_reference, sizeof(scaled_reference));

        create_processor(source, config, sizeof(ComparatorConfig), DataProcessorType::COMPARATOR, new MblMwDataProcessor(*source), processor_created);
        return MBL_MW_STATUS_OK;
    } else {
        return create_multi_comparator(source, op, MBL_MW_COMPARATOR_MODE_ABSOLUTE, &reference, 1, is_signed, processor_created);
    }
}

int32_t mbl_mw_dataprocessor_comparator_create(MblMwDataSignal *source, MblMwComparatorOperation op, float reference,
        MblMwFnDataProcessor processor_created) {
    if (source->is_signed) {
        return create_comprator(source, op, reference, 1, processor_created);
    } else {
        return create_comprator(source, op, reference, 0, processor_created);
    }
}

int32_t  mbl_mw_dataprocessor_comparator_create_signed(MblMwDataSignal *source, MblMwComparatorOperation op, float reference,
        MblMwFnDataProcessor processor_created) {
    return create_comprator(source, op, reference, 1, processor_created);
}

int32_t mbl_mw_dataprocessor_comparator_create_unsigned(MblMwDataSignal *source, MblMwComparatorOperation op, float reference,
        MblMwFnDataProcessor processor_created) {
    return create_comprator(source, op, reference, 0, processor_created);
}

int32_t mbl_mw_dataprocessor_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, float reference) {
    if (comparator->owner->firmware_revision < MULTI_COMPARE) {
        return mbl_mw_dataprocessor_multi_comparator_modify(comparator, op, &reference, 1);
    }

    if (comparator->type == DataProcessorType::COMPARATOR) {
        int32_t scaled_reference= (int32_t) number_to_firmware_converters.at(comparator->converter)(comparator, reference);
        memcpy(((uint8_t*)(comparator->config)) + 3, &scaled_reference, sizeof(scaled_reference));
        ((ComparatorConfig*) comparator->config)->operation= op;

        modify_processor_configuration(comparator, sizeof(ComparatorConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_comparator_modify_signal(MblMwDataProcessor *comparator, MblMwComparatorOperation op, MblMwDataSignal *reference_signal) {
    if (comparator->type == DataProcessorType::COMPARATOR) {
        if (comparator->owner->firmware_revision < MULTI_COMPARE) {
            ComparatorConfig* current_config= (ComparatorConfig*) comparator->config;

            memset(((uint8_t*)(current_config)) + 3, 0, sizeof(current_config->reference));
            current_config->operation= op;

            EventDataParameter signal_data_token= {reference_signal->length(), reference_signal->offset, 5};

            set_data_token(comparator->owner, &signal_data_token);
            modify_processor_configuration(comparator, sizeof(ComparatorConfig));
            clear_data_token(comparator->owner);

            return MBL_MW_STATUS_OK;
        } else {
            MultiComparatorConfig* config= (MultiComparatorConfig*) malloc(sizeof(MultiComparatorConfig));

            *((uint8_t*) config)= *((uint8_t*) comparator->config);
            config->operation= op;

            uint8_t real_length= min((uint8_t) (config->length + 1), reference_signal->length());
            free(comparator->config);
            comparator->config= config;
            comparator->config_size= 1 + real_length;
            memset(config->references, 0, real_length);

            EventDataParameter signal_data_token= {real_length, reference_signal->offset, 3};

            set_data_token(comparator->owner, &signal_data_token);
            modify_processor_configuration(comparator, comparator->config_size);
            clear_data_token(comparator->owner);

            return MBL_MW_STATUS_OK;
        }
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_multi_comparator_create(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, float references[], 
        uint8_t references_length, MblMwFnDataProcessor processor_created) {
    if (source->is_signed) {
        return create_multi_comparator(source, op, mode, references, references_length, 1, processor_created);
    } else {
        return create_multi_comparator(source, op, mode, references, references_length, 0, processor_created);
    }
}

int32_t mbl_mw_dataprocessor_multi_comparator_create_signed(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, 
        float references[], uint8_t references_length, MblMwFnDataProcessor processor_created) {
    return create_multi_comparator(source, op, mode, references, references_length, 1, processor_created);
}

int32_t mbl_mw_dataprocessor_multi_comparator_create_unsigned(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, 
        float references[], uint8_t references_length, MblMwFnDataProcessor processor_created) {
    return create_multi_comparator(source, op, mode, references, references_length, 0, processor_created);
}

int32_t mbl_mw_dataprocessor_multi_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, float references[], 
        uint8_t references_length) {
    if (comparator->type == DataProcessorType::COMPARATOR) {
        MultiComparatorConfig* config= (MultiComparatorConfig*) malloc(sizeof(MultiComparatorConfig));

        *((uint8_t*) config)= *((uint8_t*) comparator->config);
        config->operation= op;

        uint8_t offset= 0;
        switch(config->length) {
        case 0:
            SCALE_MULTI_COMP_REFERENCE(comparator, int8_t);
            break;
        case 1:
            SCALE_MULTI_COMP_REFERENCE(comparator, int16_t);
            break;
        case 3:
            SCALE_MULTI_COMP_REFERENCE(comparator, int32_t);
            break;
        }

        free(comparator->config);
        comparator->config= config;
        comparator->config_size= 1 + offset;

        modify_processor_configuration(comparator, comparator->config_size);
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
