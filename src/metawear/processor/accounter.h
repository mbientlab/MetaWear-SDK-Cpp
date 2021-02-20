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
 * Create an accounter for the input signal.  A pointer representing the processor will be passed back
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_accounter_create(MblMwDataSignal *source, void *context, MblMwFnDataProcessor processor_created);

/**
 * Create an accounter in code mode for the input signal.  The count value is accessed through the MblMwData struct's <code>extra</code> field.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_accounter_create_count(MblMwDataSignal *source, void *context, MblMwFnDataProcessor processor_created);

#ifdef	__cplusplus
}
#endif
