/**
 * @copyright MbientLab License
 * @file fuser.h
 * @brief Combine data from multiple data sources into 1 data packet
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Create a fuser processor.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param ops                   Array of data signals to combine into 1 message
 * @param n_ops                 Number of items in the array
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_fuser_create(MblMwDataSignal *source, MblMwDataSignal** ops, uint32_t n_ops,
        void *context, MblMwFnDataProcessor processor_created);

#ifdef	__cplusplus
}
#endif
