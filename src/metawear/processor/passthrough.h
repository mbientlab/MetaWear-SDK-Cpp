/**
 * @copyright MbientLab License
 * @file passthrough.h
 * @brief Gate that only allows data though based on a user configured internal state
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Operation modes for the processor
 */
typedef enum {
    MBL_MW_PASSTHROUGH_ALL = 0,             ///< Allow all data through
    MBL_MW_PASSTHROUGH_CONDITIONAL,         ///< Only allow data through if count > 0
    MBL_MW_PASSTHROUGH_COUNT                ///< Only allow a fixed number of data samples through
} MblMwPassthroughMode;

/**
 * Create a passthrough processor.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param mode                  Processor's operation mode
 * @param count                 Internal count to initial the processor with
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_passthrough_create(MblMwDataSignal *source, MblMwPassthroughMode mode, uint16_t count,
        MblMwFnDataProcessor processor_created);
/**
 * Modify the internal count of the passthrough processor
 * @param passthrough           Passthrough processor to modify
 * @param new_count             New internal count
 * @return MBL_MW_STATUS_OK if processor state was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-passthrough processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_passthrough_set_count(MblMwDataProcessor *passthrough, uint16_t new_count);
/**
 * Modify the passthrough configuration
 * @param passthrough           Passthrough processor to update
 * @param mode                  New operation mode to use
 * @param count                 New initial count
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-passthrough processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_passthrough_modify(MblMwDataProcessor *passthrough, MblMwPassthroughMode mode, uint16_t count);

#ifdef	__cplusplus
}
#endif
