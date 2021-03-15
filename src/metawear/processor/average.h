/**
 * @copyright MbientLab License
 * @file average.h
 * @brief Computes a running average of the input
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @deprecated As of v0.10.0, use mbl_mw_dataprocessor_lowpass_create
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_average_create(MblMwDataSignal *source, uint8_t size, void *context, MblMwFnDataProcessor processor_created);
/**
 * Create a high-pass filter
 * Uses the averager to compute the difference of the current value from a running average of the previous (amount of) "size" samples.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param size                  Number of previous data samples to compare against, Recommended
 *                              to be a power of 2 for faster computation.
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_highpass_create(MblMwDataSignal *source, uint8_t size, void *context, MblMwFnDataProcessor processor_created);
/**
 * Create a low-pass filter 
 * Uses the averager to create a moving average.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param size                  Number of previous data samples to compare against, Recommended
 *                              to be a power of 2 for faster computation.
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_lowpass_create(MblMwDataSignal *source, uint8_t size, void *context, MblMwFnDataProcessor processor_created);
/**
 * Resets the running average (averager current value)
 * @param average       Average processor to reset
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-average processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_average_reset(MblMwDataProcessor *average);
/**
 * Modifies the sample size of the average processor
 * The sample size is the number of previous data samples to compare against
 * Recommended to be a power of 2 for faster computation.
 * @param average       Average processor to modify
 * @param size          New sample size to use
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-average processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_average_modify_size(MblMwDataProcessor *average, uint8_t size);

#ifdef	__cplusplus
}
#endif
