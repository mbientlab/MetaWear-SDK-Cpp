/**
 * @copyright MbientLab License
 * @file sample.h
 * @brief Holds data until a certain amount has been collected
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Create a sample delay processor.  A pointer representing the processor will be passed back 
 * to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param bin_size              Number of samples to hold before letting data through
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_sample_create(MblMwDataSignal *source, uint8_t bin_size,
        void *context, MblMwFnDataProcessor processor_created);
/**
 * Modify the bin size of a sample delay processor
 * @param sample_delay          Sample processor to modify
 * @param bin_size              Number of samples to hold before letting data through
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non sample delay processor was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_sample_modify_bin_size(MblMwDataProcessor *sample_delay, uint8_t bin_size);

#ifdef	__cplusplus
}
#endif
