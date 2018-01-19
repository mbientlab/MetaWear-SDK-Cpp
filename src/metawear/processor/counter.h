/**
 * @copyright MbientLab License
 * @file counter.h
 * @brief Counts the number of times an event was fired
 */
#pragma once

#include <stdint.h>

#include "dataprocessor_fwd.h"
#include "metawear/core/event_fwd.h"
#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Create a counter with an output size of 1 byte.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Event the processor is counting
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_counter_create(MblMwEvent *source, void *context, MblMwFnDataProcessor processor_created);
/**
 * Create a counter with a specific output size.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Event the processor is counting
 * @param size                  Output size, between [1, 4] bytes
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_counter_create_size(MblMwEvent *source, uint8_t size, void *context, MblMwFnDataProcessor processor_created);
/**
 * Overwrites the current count with a new value
 * @param counter           Counter processor to modify
 * @param new_count         New count value
 * @return  MBL_MW_STATUS_OK if processor state was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if a non-counter 
 *          was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_counter_set_state(MblMwDataProcessor *counter, uint32_t new_count);

#ifdef	__cplusplus
}
#endif
