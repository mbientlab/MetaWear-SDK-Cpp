#include "metawear/processor/math.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "processor_private_common.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memcpy;
using std::memset;
using std::int32_t;
using std::uint8_t;

struct MathConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t is_signed:1;
    uint8_t :3;
    uint8_t operation;
    uint8_t rhs[4];
    uint8_t n_channels;
};

static inline int32_t create_math(MblMwDataSignal *source, MblMwMathOperation op, float rhs, uint8_t is_signed,
        MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH && source->owner->firmware_revision < MULTI_CHANNEL_MATH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);
    new_processor->set_channel_attr(1, 4);

    switch (op) {
    case MBL_MW_MATH_OP_SUBTRACT:
        new_processor->is_signed= 1;
        break;
    case MBL_MW_MATH_OP_SQRT:
    case MBL_MW_MATH_OP_ABS_VALUE:
        new_processor->is_signed= 0;
        break;
    default:
        break;
    }

    switch (op) {
    case MBL_MW_MATH_OP_ADD:
    case MBL_MW_MATH_OP_MULTIPLY:
    case MBL_MW_MATH_OP_DIVIDE:
    case MBL_MW_MATH_OP_SUBTRACT:
    case MBL_MW_MATH_OP_MODULUS:
        if (is_signed) {
            if (source->interpreter == DataInterpreter::UINT32) {
                new_processor->interpreter = DataInterpreter::INT32;
            }
        }
        break;
    case MBL_MW_MATH_OP_SQRT:
    case MBL_MW_MATH_OP_ABS_VALUE:
        if (source->interpreter == DataInterpreter::INT32) {
            new_processor->interpreter = DataInterpreter::UINT32;
        }
        break;
    default:
        break;
    }

    int32_t scaled_rhs;
    switch (op) {
    case MBL_MW_MATH_OP_ADD:
    case MBL_MW_MATH_OP_SUBTRACT:
    case MBL_MW_MATH_OP_MODULUS:
        scaled_rhs= (int32_t) source->number_to_firmware(source, rhs);
        break;
    default:
        scaled_rhs= (int32_t) rhs;
        break;
    }
 
    MathConfig *config = (MathConfig*) malloc(sizeof(MathConfig));
    *((uint8_t*) config)= 0;
    config->output= new_processor->length() - 1;
    config->input= source->length() - 1;
    config->is_signed= is_signed;
    config->operation= op;
    config->n_channels= 0;

    memcpy(((uint8_t*)(config)) + 2, &scaled_rhs, sizeof(scaled_rhs) - 
        (source->owner->firmware_revision < MULTI_CHANNEL_MATH ? 1 : 0));
    
    create_processor(source, config, sizeof(MathConfig), DataProcessorType::MATH, new_processor, processor_created);
    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_math_create(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created) {
    if (source->is_signed) {
        return mbl_mw_dataprocessor_math_create_signed(source, op, rhs, processor_created);
    } else {
        return mbl_mw_dataprocessor_math_create_unsigned(source, op, rhs, processor_created);
    }
}

int32_t mbl_mw_dataprocessor_math_create_signed(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created) {
    return create_math(source, op, rhs, 1, processor_created);
}

int32_t mbl_mw_dataprocessor_math_create_unsigned(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created) {
    return create_math(source, op, rhs, 0, processor_created);
}

int32_t mbl_mw_dataprocessor_math_modify_rhs(MblMwDataProcessor *math, float rhs) {
    if (math->type == DataProcessorType::MATH) {
        MathConfig* current_config= (MathConfig*) math->config;

        int32_t scaled_rhs;
        switch (current_config->operation) {
        case MBL_MW_MATH_OP_ADD:
        case MBL_MW_MATH_OP_SUBTRACT:
        case MBL_MW_MATH_OP_MODULUS:
            scaled_rhs= (int32_t) math->number_to_firmware(math, rhs);
            break;
        default:
            scaled_rhs= (int32_t) rhs;
            break;
        }
        memcpy(((uint8_t*)(current_config)) + 2, &scaled_rhs, sizeof(scaled_rhs));

        modify_processor_configuration(math, sizeof(MathConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_math_modify_rhs_signal(MblMwDataProcessor *math, MblMwDataSignal* rhs_signal) {
    if (math->type == DataProcessorType::MATH) {
        MathConfig *current_config= (MathConfig*) math->config;

        memset(((uint8_t*)(current_config)) + 2, 0, sizeof(current_config->rhs));

        EventDataParameter signal_data_token= {rhs_signal->length(), rhs_signal->offset, 4};
        math->owner->data_token= &signal_data_token;

        modify_processor_configuration(math, sizeof(MathConfig));
        math->owner->data_token= nullptr;
        
        return MBL_MW_STATUS_OK;
    }

    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
