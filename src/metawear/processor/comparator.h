/**
 * @copyright MbientLab License
 * @file comparator.h
 * @brief Only allows data through that satisfies a comparison operation
 */
#pragma once

#include "processor_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Available operations for the processor
 */
typedef enum {
    MBL_MW_COMPARATOR_OP_EQ = 0,        ///< Equals to
    MBL_MW_COMPARATOR_OP_NEQ,           ///< Not equals to
    MBL_MW_COMPARATOR_OP_LT,            ///< Less than
    MBL_MW_COMPARATOR_OP_LTE,           ///< Less than or equals to
    MBL_MW_COMPARATOR_OP_GT,            ///< Greater than
    MBL_MW_COMPARATOR_OP_GTE,           ///< Greater than or equals to
} MblMwComparatorOperation;

/**
 * Create a comparator processor where signed/unsigned is inferred.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute
 * @param reference             Reference value to compare the input to
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_create(MblMwDataSignal *source, MblMwComparatorOperation op,
        float reference, MblMwFnDataProcessor processor_created);
/**
 * Create a comparator processor specifically for a signed comparison.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute
 * @param reference             Reference value to compare the input to
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_create_signed(MblMwDataSignal *source, MblMwComparatorOperation op,
        float reference, MblMwFnDataProcessor processor_created);
/**
 * Create a comparator processor specifically for an unsigned comparison.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute
 * @param reference             Reference value to compare the input to
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_create_unsigned(MblMwDataSignal *source, MblMwComparatorOperation op,
        float reference, MblMwFnDataProcessor processor_created);
/**
 * Modifies the comparator processor, changing the operation and reference value
 * @param comparator            Comparator processor to modify
 * @param op                    New comparison operation
 * @param reference             New reference value
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-comparator processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, 
        float reference);
/**
 * Modifies the comparator processor for a feedback or feedforward loop 
 * @param comparator            Comparator processor to modify
 * @param op                    New comparison operation
 * @param reference_signal      Data signal output to be used for the reference value
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-comparator processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_modify_signal(MblMwDataProcessor *comparator, MblMwComparatorOperation op, 
        MblMwDataSignal *reference_signal);

#ifdef __cplusplus
}
#endif
