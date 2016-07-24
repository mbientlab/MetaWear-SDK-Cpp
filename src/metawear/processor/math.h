/**
 * @copyright MbientLab License
 * @file math.h
 * @brief Performs arithmetic on sensor data
 */
#pragma once

#include "processor_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Available operations for the math processor
 */
typedef enum {
    MBL_MW_MATH_OP_ADD= 1,          ///< Computes input + rhs 
    MBL_MW_MATH_OP_MULTIPLY,        ///< Computes input * rhs 
    MBL_MW_MATH_OP_DIVIDE,          ///< Computes input / rhs
    MBL_MW_MATH_OP_MODULUS,         ///< Computes input % rhs
    MBL_MW_MATH_OP_EXPONENT,        ///< Computes input ^ rhs
    MBL_MW_MATH_OP_SQRT,            ///< Computes sqrt(input)
    MBL_MW_MATH_OP_LSHIFT,          ///< Computes input << rhs
    MBL_MW_MATH_OP_RSHIFT,          ///< Computes input >> rhs
    MBL_MW_MATH_OP_SUBTRACT,        ///< Computes input - rhs
    MBL_MW_MATH_OP_ABS_VALUE,       ///< Computes |input|
    MBL_MW_MATH_OP_CONSTANT         ///< Replaces input with rhs
} MblMwMathOperation;

/**
 * Create a math processor where signed/unsigned operation is inferred.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Math operation to compute
 * @param rhs                   Right hand side of the operation that requires 2 inputs
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_math_create(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created);
/**
 * Create a math processor using signed operations.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Math operation to compute
 * @param rhs                   Right hand side of the operation that requires 2 inputs
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_math_create_signed(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created);
/**
 * Create a math processor using unsigned operations.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Math operation to compute
 * @param rhs                   Right hand side of the operation that requires 2 inputs
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_math_create_unsigned(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created);
/**
 * Modify the configuration of a math processor, changing the right side value of the operation
 * @param math                  Math processor to modify
 * @param rhs                   New right hand side of the operation
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-math processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_math_modify_rhs(MblMwDataProcessor *math, float rhs);
/**
 * Modify the configuration of a math processor for a feedback or feedforward loop
 * @param math                  Math processor to modify
 * @param rhs_signal            Data signal supplying the rhs value of the operation
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-math processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_math_modify_rhs_signal(MblMwDataProcessor *math, MblMwDataSignal* rhs_signal);

#ifdef __cplusplus
}
#endif
