/**
 * @copyright MbientLab License
 * @file accounter.h
 * @brief Adds additional information to the payload to facilitate packet reconstruction
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Adds a system timer (timestamp) to the input signal. 
 * Not typically used.
 * Adds additional information to the BTLE packet in the form of a counter
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_accounter_create(MblMwDataSignal *source, void *context, MblMwFnDataProcessor processor_created);

/**
 * Adds a simple counter (1,2,3...) to the input signal.  
 * Great to make sure the packets are coming in order.
 * The count value is accessed through the MblMwData struct's <code>extra</code> field.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_accounter_create_count(MblMwDataSignal *source, void *context, MblMwFnDataProcessor processor_created);

#ifdef	__cplusplus
}
#endif
