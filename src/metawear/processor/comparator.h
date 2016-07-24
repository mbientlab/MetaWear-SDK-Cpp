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
    MBL_MW_COMPARATOR_OP_GTE            ///< Greater than or equals to
} MblMwComparatorOperation;

/**
 * Output modes for the comparator, only supported in firmware v1.2.3 and higher
 */
typedef enum {
    MBL_MW_COMPARATOR_MODE_ABSOLUTE = 0,        ///< Return the data as is
    MBL_MW_COMPARATOR_MODE_REFERENCE,           ///< Return the reference value for the satisfied comparison
    MBL_MW_COMPARATOR_MODE_ZONE,                ///< Return the position of the reference value satisfying the comparison, n + 1 for not satisfied
    MBL_MW_COMPARATOR_MODE_BINARY               ///< Return 1 if any reference values satisfy the comparison, 0 if none do
} MblMwComparatorMode;

/**
 * Create a comparator processor where signed/unsigned is inferred.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute
 * @param reference             Reference value to compare the input to
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_create(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, 
        MblMwFnDataProcessor processor_created);
/**
 * Create a comparator processor specifically for a signed comparison.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute
 * @param reference             Reference value to compare the input to
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_create_signed(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, 
        MblMwFnDataProcessor processor_created);
/**
 * Create a comparator processor specifically for an unsigned comparison.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute
 * @param reference             Reference value to compare the input to
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_create_unsigned(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, 
        MblMwFnDataProcessor processor_created);

/**
 * Modifies the comparator processor, changing the operation and reference value
 * @param comparator            Comparator processor to modify
 * @param op                    New comparison operation
 * @param reference             New reference value
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-comparator processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, float reference);
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

/**
 * Create a multi-value comparator where a signed/unsigned comparison is inferred.  This feature is only available on firmware v1.2.3 and later.  
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input
 * @param op                    Comparison operation to execute
 * @param mode                  Processor output mode
 * @param references            Array of reference values to compare against
 * @param references_length     Number of elements in the references array
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_multi_comparator_create(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, 
        float references[], uint8_t references_length, MblMwFnDataProcessor processor_created);
/**
 * Create a multi-value comparator for signed comparisons.  This feature is only available on firmware v1.2.3 and later.  A pointer representing the 
 * processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input
 * @param op                    Comparison operation to execute
 * @param mode                  Processor output mode
 * @param references            Array of reference values to compare against
 * @param references_length     Number of elements in the references array
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_multi_comparator_create_signed(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, 
        float references[], uint8_t references_length, MblMwFnDataProcessor processor_created);
/**
 * Create a multi-value comparator for unsigned comparisons.  This feature is only available on firmware v1.2.3 and later.  A pointer representing the 
 * processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input
 * @param op                    Comparison operation to execute
 * @param mode                  Processor output mode
 * @param references            Array of reference values to compare against
 * @param references_length     Number of elements in the references array
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_multi_comparator_create_unsigned(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, 
        float references[], uint8_t references_length, MblMwFnDataProcessor processor_created);

/**
 * Modifies the multi-value comparator, changing the operation and reference values.  This feature is only available on firmware v1.2.3 and later.
 * @param comparator            Comparator processor to modify
 * @param op                    New comparison operation
 * @param references            Array of new reference values
 * @param references_length     Number of elements in the references array
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-comparator processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_multi_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, float references[],
        uint8_t references_length);

#ifdef __cplusplus
}
#endif
