/**
 * @copyright MbientLab License 
 * @file ambientlight_ltr329.h
 * @brief Functions for interacting with the LTR329 ambient light sensor.
 * This sensor is only available on MetaWear RPro boards.
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "sensor_defs.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_ALS_LTR329_MODULE= 0x14;       ///< Module id for the LTR329 ambient light sensor
const uint8_t MBL_MW_ALS_LTR329_DATA= 0x3;          ///< Register id for the LTR329 illuminance data

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
 * Container representing LTR329 sensor configuration
 */
typedef struct MblMwAlsLtr329Config MblMwAlsLtr329Config;

/**
 * Retrieves the attributes for LTR329 illuminance data 
 * @return Pointer to the LTR329 illuminance data source
 */
METAWEAR_API const DataSource* mbl_mw_als_ltr329_get_data_source();

/**
 * Instantiates an MblMwAlsLtr329Config struct with a 1x gain, 100ms integration time, and 500ms measurement rate.  
 * The user is responsible for deallocating the memory by calling mbl_mw_als_ltr329_free_config()
 * @return Pointer to the newly instantiated MblMwAlsLtr329Config struct
 */
METAWEAR_API MblMwAlsLtr329Config* mbl_mw_als_ltr329_create_config();
/**
 * Frees the memory allocated for a MblMwAlsLtr329Config struct
 * @param config    Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_als_ltr329_free_config(MblMwAlsLtr329Config *config);
/**
 * Sets the sensor gain
 * @param config    Pointer to the MblMwAlsLtr329Config struct to modify
 * @param gain      Sensor gain value to set
 */
METAWEAR_API void mbl_mw_als_ltr329_set_gain(MblMwAlsLtr329Config *config, MblMwAlsLtr329Gain gain);
/**
 * Sets the sensor integration time
 * @param config                Pointer to the MblMwAlsLtr329Config struct to modify
 * @param integration_time      Integration time value to set
 */
METAWEAR_API void mbl_mw_als_ltr329_set_integration_time(MblMwAlsLtr329Config *config, MblMwAlsLtr329IntegrationTime integration_time);
/**
 * Sets the sensor measurement rate
 * @param config                Pointer to the MblMwAlsLtr329Config struct to modify
 * @param measurement_rate      Measurement rate value to set
 */
METAWEAR_API void mbl_mw_als_ltr329_set_measurement_rate(MblMwAlsLtr329Config *config, MblMwAlsLtr329MeasurementRate measurement_rate);
/**
 * Constructs the command that writes the configuration to the LTR329 sensor
 * @param command   Byte array (len 4) for the function to write the command to
 * @param config    Pointer to the sensor configuration
 */
METAWEAR_API void mbl_mw_als_ltr329_write_config(uint8_t command[4], const MblMwAlsLtr329Config *config);

/**
 * Constructs the command that starts the sensor
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_als_ltr329_start(uint8_t command[3]);
/**
 * Constructs the command that stops the sensor
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_als_ltr329_stop(uint8_t command[3]);

/**
 * Extracts the illuminance data from a response sent by the LTR329 sensor in milli lux (mlx)
 * @param data_mlx  Pointer to where the function should write the illuminance data to
 * @param response  Response received from the LTR329 sensor
 * @return #STATUS_OK if the response contains valid illuminance data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_als_ltr329_get_illuminance_data(uint32_t *data_mlx, uint8_t response[6]);

#ifdef	__cplusplus
}
#endif
