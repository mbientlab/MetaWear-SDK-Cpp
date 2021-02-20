/**
 * @copyright MbientLab License
 * @file buffer.h
 * @brief Captures input data which can be retrieved at a later point in time
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Create a buffer processor.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_buffer_create(MblMwDataSignal *source, void *context, MblMwFnDataProcessor processor_created);

#ifdef	__cplusplus
}
#endif
