/**
 * @copyright MbientLab License
 * @file accumulator.h
 * @brief Tallies a running sum of the input
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Create an accumulator whose output is the same size as the input.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_accumulator_create(MblMwDataSignal *source, void *context, MblMwFnDataProcessor processor_created);
/**
 * Create an accumulator with a specific output size.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param output_size           Output size, between [1, 4] bytes
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_accumulator_create_size(MblMwDataSignal *source, uint8_t output_size, void *context, MblMwFnDataProcessor processor_created);
/**
 * Overwrites the current running sum with a new value
 * @param accumulator           Accumulator processor to modify
 * @param new_running_sum       New running sum of the accumulator
 * @return  MBL_MW_STATUS_OK if processor state was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if a non-accumulator 
 *          was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_set_accumulator_state(MblMwDataProcessor *accumulator, float new_running_sum);

#ifdef	__cplusplus
}
#endif
