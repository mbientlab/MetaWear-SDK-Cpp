/**
 * @copyright MbientLab License (LICENSE.md)
 * @file colordetector_tcs34725.h
 * @brief Communicates with the TCS34725 color detector
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Available analog gain scale values
 */
typedef enum {
    MBL_MW_CD_TCS34725_GAIN_1X= 0,
    MBL_MW_CD_TCS34725_GAIN_4X,
    MBL_MW_CD_TCS34725_GAIN_16X,
    MBL_MW_CD_TCS34725_GAIN_60X
} MblMwColorDetectorTcs34725Gain;

/**
 * Retrieves the data signal representing detected ADC values for clear, red, green, and blue
 * @param board         Board the sensor resides on
 */
METAWEAR_API MblMwDataSignal* mbl_mw_cd_tcs34725_get_adc_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the integration time
 * @param board         Board to modify
 * @param time          New integration time to use, between [2.4, 614.4] milliseconds
 */
METAWEAR_API void mbl_mw_cd_tcs34725_set_integration_time(MblMwMetaWearBoard *board, float time);
/**
 * Sets the analog gain scale
 * @param board         Board to modify
 * @param gain          New gain scale to use
 */
METAWEAR_API void mbl_mw_cd_tcs34725_set_gain(MblMwMetaWearBoard *board, MblMwColorDetectorTcs34725Gain gain);
/**
 * Enable the illuminator LED
 * @param board         Board to modify
 */
METAWEAR_API void mbl_mw_cd_tcs34725_enable_illuminator_led(MblMwMetaWearBoard *board);
/**
 * Disable the illuminator LED
 * @param board          Board to modify
 */
METAWEAR_API void mbl_mw_cd_tcs34725_disable_illuminator_led(MblMwMetaWearBoard *board);
/**
 * Writes the configuration to the sensor
 * @param board         Board the sensor resides on
 */
METAWEAR_API void mbl_mw_cd_tcs34725_write_config(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
