#pragma once

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_MATH_OP_ADD= 1,
    MBL_MW_MATH_OP_MULTIPLY,
    MBL_MW_MATH_OP_DIVIDE,
    MBL_MW_MATH_OP_MODULUS,
    MBL_MW_MATH_OP_EXPONENT,
    MBL_MW_MATH_OP_SQRT,
    MBL_MW_MATH_OP_LSHIFT,
    MBL_MW_MATH_OP_RSHIFT,
    MBL_MW_MATH_OP_SUBTRACT,
    MBL_MW_MATH_OP_ABS_VALUE
} MblMwMathOperation;

METAWEAR_API void mbl_mw_dataprocessor_create_math(MblMwDataSignal *source, MblMwMathOperation op, float rhs, 
        MblMwFnDataProcessor processor_created);
METAWEAR_API void mbl_mw_dataprocessor_create_math_signed(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created);
METAWEAR_API void mbl_mw_dataprocessor_create_math_unsigned(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_math_modify_rhs(MblMwDataProcessor *math, float rhs);
METAWEAR_API int32_t mbl_mw_dataprocessor_math_modify_rhs_signal(MblMwDataProcessor *math, MblMwDataSignal* rhs_signal);

#ifdef __cplusplus
}
#endif
