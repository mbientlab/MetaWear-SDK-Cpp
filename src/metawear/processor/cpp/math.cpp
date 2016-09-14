#include "metawear/processor/math.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/version.h"

#include "processor_private_common.h"

#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>

using std::ceil;
using std::malloc;
using std::max;
using std::memcpy;
using std::memset;
using std::min;
using std::int32_t;
using std::uint8_t;

const Version MULTI_CHANNEL_MATH(1, 1, 0), CONST_MATH_OP(1, 1, 0);

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
    if ((source->n_channels > 1 && source->owner->firmware_revision < MULTI_CHANNEL_MATH) || 
            (op == MBL_MW_MATH_OP_CONSTANT && source->owner->firmware_revision < CONST_MATH_OP)) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);

    switch (op) {
    // Use the is_signed value to determine if data will be signed/unsigned
    case MBL_MW_MATH_OP_ADD:
    case MBL_MW_MATH_OP_MULTIPLY:
    case MBL_MW_MATH_OP_DIVIDE:
    case MBL_MW_MATH_OP_MODULUS:
        if (is_signed) {
            new_processor->make_signed();
        } else {
            new_processor->make_unsigned();
        }
        break;
    // data always signed for subtract
    case MBL_MW_MATH_OP_SUBTRACT:
        new_processor->make_signed();
        break;
    // data always unsigned for sqrt & abs
    case MBL_MW_MATH_OP_SQRT:
    case MBL_MW_MATH_OP_ABS_VALUE:
        new_processor->make_unsigned();
        break;
    default:
        break;
    }

    switch (op) {
    case MBL_MW_MATH_OP_EXPONENT:
    case MBL_MW_MATH_OP_SQRT:
    case MBL_MW_MATH_OP_LSHIFT:
    case MBL_MW_MATH_OP_RSHIFT:
        if (new_processor->interpreter != DataInterpreter::INT32 && new_processor->interpreter != DataInterpreter::UINT32) {
            new_processor->interpreter = DataInterpreter::BYTE_ARRAY;
        }
        if (new_processor->converter != FirmwareConverter::DEFAULT) {
            new_processor->converter = FirmwareConverter::DEFAULT;
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
    case MBL_MW_MATH_OP_CONSTANT:
        scaled_rhs= (int32_t) number_to_firmware_converters.at(source->converter)(source, rhs);
        break;
    default:
        scaled_rhs= (int32_t) rhs;
        break;
    }

    switch (op) {
    case MBL_MW_MATH_OP_RSHIFT:
        new_processor->channel_size= max(1, source->channel_size - (scaled_rhs / 8));
        break;
    case MBL_MW_MATH_OP_LSHIFT:
        new_processor->channel_size= min(4, source->channel_size + (uint8_t) ceil(scaled_rhs / 8));
        break;
    default:
        new_processor->channel_size= 4;
    }

 
    MathConfig *config = (MathConfig*) malloc(sizeof(MathConfig));
    *((uint8_t*) config)= 0;
    config->output= new_processor->channel_size - 1;
    config->input= source->channel_size - 1;
    config->is_signed= is_signed;
    config->operation= op;
    config->n_channels= source->n_channels - 1;

    memcpy(((uint8_t*)(config)) + 2, &scaled_rhs, sizeof(scaled_rhs) - (source->owner->firmware_revision < MULTI_CHANNEL_MATH ? 1 : 0));
    
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
            scaled_rhs= (int32_t) number_to_firmware_converters.at(math->converter)(math, rhs);
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

        set_data_token(math->owner, &signal_data_token);
        modify_processor_configuration(math, sizeof(MathConfig));
        clear_data_token(math->owner);
        
        return MBL_MW_STATUS_OK;
    }

    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
