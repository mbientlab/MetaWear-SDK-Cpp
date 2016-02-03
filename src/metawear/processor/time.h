/**
 * @copyright MbientLab License
 * @file time.h
 * @brief Periodically allow data through
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Output modes for the processor
 */
typedef enum {
    MBL_MW_TIME_ABSOLUTE = 0,           ///< Returns the data as is
    MBL_MW_TIME_DIFFERENTIAL            ///< Returns the difference between the current and previous value
} MblMwTimeMode;

/**
 * Creates a time delay processor.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param mode                  Operation mode of the processor
 * @param period                How often to allow data through, in milliseconds
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_time_create(MblMwDataSignal *source, MblMwTimeMode mode, uint32_t period,
        MblMwFnDataProcessor processor_created);
/**
 * Modify the configuration of the time delay processor
 * @param time_delay            Time delay processor to modify
 * @param period                How often to allow data through, in milliseconds
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_time_modify_period(MblMwDataProcessor *time_delay, uint32_t period);

#ifdef	__cplusplus
}
#endif
