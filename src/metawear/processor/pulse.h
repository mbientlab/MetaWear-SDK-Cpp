/**
 * @copyright MbientLab License
 * @file pulse.h
 * @brief Detects and quantifies a pulse over the input values
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
    MBL_MW_PULSE_OUTPUT_WIDTH = 0,      ///< Return number of samples in the pulse
    MBL_MW_PULSE_OUTPUT_AREA,           ///< Return a sum of all data points in the pulse
    MBL_MW_PULSE_OUTPUT_PEAK,           ///< Return the highest value in the pulse
    MBL_MW_PULSE_OUTPUT_ON_DETECTION    ///< Return a 0x01 as soon as a pulse is detected
} MblMwPulseOutput;

/**
 * Create a pulse detector. 
 * Detects and quantifies a pulse over the input values using the threshold and width specified.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param output                Output type of the processor
 * @param threshold             Value the data must exceed for a valid pulse
 * @param width                 Number of samples that must exceed the threshold for a valid pulse
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_pulse_create(MblMwDataSignal *source, MblMwPulseOutput output,
            float threshold, uint16_t width, void *context, MblMwFnDataProcessor processor_created);
/**
 * Modify the configuration of a pulse detector
 * @param pulse             Pulse detector to modify
 * @param threshold         New threshold of the detector
 * @param width             New width of the detector
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non pulse detector was passed in
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_pulse_modify(MblMwDataProcessor *pulse, float threshold, uint16_t width);

#ifdef	__cplusplus
}
#endif
