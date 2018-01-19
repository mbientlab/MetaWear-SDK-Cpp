/**
 * @copyright MbientLab License
 * @file delta.h
 * @brief Only allows data through that is a min distance from a reference value
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Output modes of the data processor
 */
typedef enum {
    MBL_MW_DELTA_MODE_ABSOLUTE = 0,         ///< Return the data as is
    MBL_MW_DELTA_MODE_DIFFERENTIAL,         ///< Return the difference between the input and the reference value
    MBL_MW_DELTA_MODE_BINARY                ///< Return 1 if input > reference, -1 if input < reference
} MblMwDeltaMode;

/**
 * Create a delta processor.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param mode                  Output mode of the processor
 * @param magnitude             Min distance from the reference value to allow the input to pass
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_delta_create(MblMwDataSignal *source, MblMwDeltaMode mode, float magnitude,
        void *context, MblMwFnDataProcessor processor_created);
/**
 * Sets the reference value of the processor.
 * @param delta                 Delta processor to modify
 * @param previous_value        Min distance from the reference value to allow the input to pass
 * @return MBL_MW_STATUS_OK if processor state was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-delta processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_delta_set_reference(MblMwDataProcessor *delta, float previous_value);
/**
 * Modifies the magnitude that allows data through
 * @param delta             Delta processor to modify
 * @param magnitude         Min distance from the reference value to allow the input to pass
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-delta processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_delta_modify_magnitude(MblMwDataProcessor *delta, float magnitude);

#ifdef	__cplusplus
}
#endif
