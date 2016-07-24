/**
 * @copyright MbientLab License 
 * @file ambientlight_ltr329.h
 * @brief Functions for interacting with the LTR329 ambient light sensor.
 * @details This sensor is only available on MetaWear RPro boards.
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Controls the range and resolution of illuminance values
 */
typedef enum {
    MBL_MW_ALS_LTR329_GAIN_1X= 0,   ///< Illuminance range between [1, 64k] lux (default)
    MBL_MW_ALS_LTR329_GAIN_2X,      ///< Illuminance range between [0.5, 32k] lux
    MBL_MW_ALS_LTR329_GAIN_4X,      ///< Illuminance range between [0.25, 16k] lux
    MBL_MW_ALS_LTR329_GAIN_8X,      ///< Illuminance range between [0.125, 8k] lux
    MBL_MW_ALS_LTR329_GAIN_48X,     ///< Illuminance range between [0.02, 1.3k] lux
    MBL_MW_ALS_LTR329_GAIN_96X      ///< Illuminance range between [0.01, 600] lux
} MblMwAlsLtr329Gain;

/**
 * Measurement time for each cycle
 */
typedef enum {
    MBL_MW_ALS_LTR329_TIME_100MS= 0,    ///< Default setting
    MBL_MW_ALS_LTR329_TIME_50MS,
    MBL_MW_ALS_LTR329_TIME_200MS,
    MBL_MW_ALS_LTR329_TIME_400MS,
    MBL_MW_ALS_LTR329_TIME_150MS,
    MBL_MW_ALS_LTR329_TIME_250MS,
    MBL_MW_ALS_LTR329_TIME_300MS,
    MBL_MW_ALS_LTR329_TIME_350MS
} MblMwAlsLtr329IntegrationTime;

/**
 * How frequently to update the illuminance data.
 */
typedef enum {
    MBL_MW_ALS_LTR329_RATE_50MS= 0,
    MBL_MW_ALS_LTR329_RATE_100MS,
    MBL_MW_ALS_LTR329_RATE_200MS,
    MBL_MW_ALS_LTR329_RATE_500MS,       ///< Default setting
    MBL_MW_ALS_LTR329_RATE_1000MS,
    MBL_MW_ALS_LTR329_RATE_2000MS
} MblMwAlsLtr329MeasurementRate;

/**
 * Retrieves the data signal representing LTR329 illuminance data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's LTR329 illuminance data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_als_ltr329_get_illuminance_data_signal(const MblMwMetaWearBoard *board);

/**
 * Sets the sensor gain
 * @param board     Pointer to the board to modify
 * @param gain      Sensor gain value to set
 */
METAWEAR_API void mbl_mw_als_ltr329_set_gain(MblMwMetaWearBoard *board, MblMwAlsLtr329Gain gain);
/**
 * Sets the sensor integration time
 * @param board     Pointer to the board to modify
 * @param integration_time      Integration time value to set
 */
METAWEAR_API void mbl_mw_als_ltr329_set_integration_time(MblMwMetaWearBoard *board, MblMwAlsLtr329IntegrationTime integration_time);
/**
 * Sets the sensor measurement rate
 * @param board     Pointer to the board to modify
 * @param measurement_rate      Measurement rate value to set
 */
METAWEAR_API void mbl_mw_als_ltr329_set_measurement_rate(MblMwMetaWearBoard *board, MblMwAlsLtr329MeasurementRate measurement_rate);
/**
 * Writes the configuration to the LTR329 sensor
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_als_ltr329_write_config(const MblMwMetaWearBoard *board);

/**
 * Starts illuminance sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_als_ltr329_start(const MblMwMetaWearBoard *board);
/**
 * Stops illuminance sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_als_ltr329_stop(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
