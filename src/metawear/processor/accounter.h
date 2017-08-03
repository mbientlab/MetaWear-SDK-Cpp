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
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_accounter_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created);

#ifdef	__cplusplus
}
#endif
