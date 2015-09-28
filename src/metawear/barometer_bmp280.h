/**
 * @copyright MbientLab License 
 * @file barometer_bmp280.h
 * @brief Functions for interacting with the BMP280 pressure sensor.
 * This sensor is only available on MetaWear RPro boards.
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "sensor_defs.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_BARO_BMP280_MODULE= 0x12;              ///< Module id for the BMP280 barometer
const uint8_t MBL_MW_BARO_BMP280_PRESSURE_DATA= 0x1;        ///< Register id for BMP280 pressure data
const uint8_t MBL_MW_BARO_BMP280_ALTITUDE_DATA= 0x2;        ///< Register id for BMP280 altitude data

/**
 * Supported oversampling modes on the BMP280 sensor
 */
typedef enum  {
    MBL_MW_BARO_BMP280_OVERSAMPLE_SKIP= 0,
    MBL_MW_BARO_BMP280_OVERSAMPLE_ULTRA_LOW_POWER,
    MBL_MW_BARO_BMP280_OVERSAMPLE_LOW_POWER,
    MBL_MW_BARO_BMP280_OVERSAMPLE_STANDARD,
    MBL_MW_BARO_BMP280_OVERSAMPLE_HIGH,
    MBL_MW_BARO_BMP280_OVERSAMPLE_ULTRA_HIGH
} MblMwBaroBmp280Oversampling;

/**
 * Supported filter modes on the BMP280 sensor
 */
typedef enum {
    MBL_MW_BARO_BMP280_IIR_FILTER_OFF= 0,
    MBL_MW_BARO_BMP280_IIR_FILTER_AVG_2,
    MBL_MW_BARO_BMP280_IIR_FILTER_AVG_4,
    MBL_MW_BARO_BMP280_IIR_FILTER_AVG_8,
    MBL_MW_BARO_BMP280_IIR_FILTER_AVG_16
} MblMwBaroBmp280IirFilter;

/**
 * Supported stand by times on the BMP280 sensor
 */
typedef enum {
    MBL_MW_BARO_BMP280_STANDBY_TIME_0_5MS= 0,
    MBL_MW_BARO_BMP280_STANDBY_TIME_62_5MS,
    MBL_MW_BARO_BMP280_STANDBY_TIME_125MS,
    MBL_MW_BARO_BMP280_STANDBY_TIME_250MS,
    MBL_MW_BARO_BMP280_STANDBY_TIME_500MS,
    MBL_MW_BARO_BMP280_STANDBY_TIME_1000MS,
    MBL_MW_BARO_BMP280_STANDBY_TIME_2000MS,
    MBL_MW_BARO_BMP280_STANDBY_TIME_4000MS
} MblMwBaroBmp280StandbyTime;

/**
 * Container for BMP280 configuration
 */
typedef struct MblMwBaroBmp280Config MblMwBaroBmp280Config;

/**
 * Retrieves the attributes for BMP280 pressure data 
 * @return Pointer to the BMP280 pressure data source
 */
METAWEAR_API const DataSource* mbl_mw_baro_bmp280_get_pressure_data_source();
/**
 * Retrieves the attributes for BMP280 altitude data 
 * @return Pointer to the BMP280 altitude data source
 */
METAWEAR_API const DataSource* mbl_mw_baro_bmp280_get_altitude_data_source();

/**
 * Instantiates an MblMwBaroBmp280Config struct with standard oversampling, no IIR filter, and a standby time of 0.5ms. 
 * The user is responsible for deallocating the memory by calling mbl_mw_als_ltr329_free_config()
 * @return Pointer to the newly instantiated MblMwAlsLtr329Config struct
 */
METAWEAR_API MblMwBaroBmp280Config* mbl_mw_baro_bmp280_create_config();
/**
 * Frees the memory allocated for a MblMwBaroBmp280Config struct
 * @param config    Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_baro_bmp280_free_config(MblMwBaroBmp280Config *config);
/**
 * Set the oversampling mode
 * @param config            Pointer to the MblMwBaroBmp280Config struct to modify
 * @param oversampling      Oversampling value to set
 */
METAWEAR_API void mbl_mw_baro_bmp280_set_oversampling(MblMwBaroBmp280Config *config, MblMwBaroBmp280Oversampling oversampling);
/**
 * Set the iir filter coefficient
 * @param config        Pointer to the MblMwBaroBmp280Config struct to modify
 * @param iir_filter    IIR filter value to set
 */
METAWEAR_API void mbl_mw_baro_bmp280_set_iir_filter(MblMwBaroBmp280Config *config, MblMwBaroBmp280IirFilter iir_filter);
/**
 * Set the standby time
 * @param config            Pointer to the MblMwBaroBmp280Config struct to modify
 * @param standby_time      Standby time value to set
 */
METAWEAR_API void mbl_mw_baro_bmp280_set_standby_time(MblMwBaroBmp280Config *config, MblMwBaroBmp280StandbyTime standby_time);
/**
 * Constructs the command that writes the configuration to the BMP280 sensor
 * @param command       Byte array (len 4) for the function to write the command to
 * @param config        Pointer to the sensor configuration
 */
METAWEAR_API void mbl_mw_baro_bmp280_write_config(uint8_t command[4], MblMwBaroBmp280Config *config);

/**
 * Constructs the command to start the sensor
 * @param command       Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_baro_bmp280_start(uint8_t command[4]);
/**
 * Constructs the command to stop the sensor
 * @param command       Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_baro_bmp280_stop(uint8_t command[4]);

/**
 * Extracts the pressure data from a response sent by the BMP280 sensor in pascals (pa)
 * @param data_pa   Pointer to where the function should write the pressure data to
 * @param response  Response received from the BMP280 sensor
 * @return #STATUS_OK if the response contains valid pressure data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_baro_bmp280_get_pressure_data(float *data_pa, uint8_t response[6]);
/**
 * Extracts the pressure data from a response sent by the BMP280 sensor in meters (m)
 * @param data_m    Pointer to where the function should write the altitude data to
 * @param response  Response received from the BMP280 sensor
 * @return #STATUS_OK if the response contains valid pressure data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_baro_bmp280_get_altitude_data(float *data_m, uint8_t response[6]);

#ifdef	__cplusplus
}
#endif
