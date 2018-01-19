/**
 * @copyright MbientLab License
 * @file threshold.h
 * @brief Allows data through that crosses a boundary
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Output modes of the processor
 */
typedef enum {
    MBL_MW_THRESHOLD_MODE_ABSOLUTE = 0,         ///< Return the data as is
    MBL_MW_THRESHOLD_MODE_BINARY                ///< Return 1 if data > bounday, -1 if data < boundary
} MblMwThresholdMode;

/**
 * Create a threshold processor.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param mode                  Processor output mode
 * @param boundary              Limit that triggers an event when data crosses it
 * @param hysteresis            Min distance between the limit and value to signal a successful crossing
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_threshold_create(MblMwDataSignal *source, MblMwThresholdMode mode, float boundary,
        float hysteresis, void *context, MblMwFnDataProcessor processor_created);
/**
 * Modifies the threshold processor configuration
 * @param threshold             Threshold processor to modify
 * @param boundary              Limit that triggers an event when data crosses it
 * @param hysteresis            Min distance between the limit and value to signal a successful crossing
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-threshold processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_threshold_modify_boundary(MblMwDataProcessor *threshold, float boundary, 
        float hysteresis);

#ifdef	__cplusplus
}
#endif
