/**
 * @copyright MbientLab License
 * @file rms.h
 * @brief Computes the root mean square of the input
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Create an rms processor.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_rms_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created);

#ifdef	__cplusplus
}
#endif
