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

//@{
/** Indices for component values of the color adc data signal, used with mbl_mw_datasignal_get_component */
const uint8_t MBL_MW_CD_TCS34725_ADC_CLEAR_INDEX = 0, 
        MBL_MW_CD_TCS34725_ADC_RED_INDEX = 1, 
        MBL_MW_CD_TCS34725_ADC_GREEN_INDEX = 2,
        MBL_MW_CD_TCS34725_ADC_BLUE_INDEX = 3;
//@}

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
 * This signal represents the amount of each color present (C,R,G,B) as a percentage
 * @param board         Board the sensor resides on
 * MblMwTcs34725ColorAdc is return signal data type 
 */
METAWEAR_API MblMwDataSignal* mbl_mw_cd_tcs34725_get_adc_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the integration time - 2.4ms to 614.4ms in 2.4ms increments
 * The RGBC integration time impacts both the resolution and the sensitivity of the RGBC reading.
 * Integration of all four channels occurs simultaneously and upon completion of the conversion cycle, the results are transferred to the color data registers
 * @param board         Board to modify
 * @param time          New integration time to use, between [2.4, 614.4] milliseconds
 */
METAWEAR_API void mbl_mw_cd_tcs34725_set_integration_time(MblMwMetaWearBoard *board, float time);
/**
 * Sets the analog gain scale
 * RGBC gain control for the RGBC photodiodes - 1x, 4x, 16x, 60x
 * See MblMwColorDetectorTcs34725Gain for allowed values.
 * @param board         Board to modify
 * @param gain          New gain scale to use
 */
METAWEAR_API void mbl_mw_cd_tcs34725_set_gain(MblMwMetaWearBoard *board, MblMwColorDetectorTcs34725Gain gain);
/**
 * Writes the configuration to the sensor
 * Applies the INTEGRATION TIME, and GAIN values set in set_*().
 * @param board         Board the sensor resides on
 */
METAWEAR_API void mbl_mw_cd_tcs34725_write_config(const MblMwMetaWearBoard *board);
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

#ifdef	__cplusplus
}
#endif
