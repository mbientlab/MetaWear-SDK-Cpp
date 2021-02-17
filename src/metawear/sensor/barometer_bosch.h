/**
 * @copyright MbientLab License 
 * @file barometer_bosch.h
 * @brief Interacts with the supported Bosch barometers, currenly BMP280 and BME280
 * @details The BMP280 and BME280 sensors are identical except for the standby times.  Functions that are generic for 
 * both barometers have "bosch" in the name whereas functions specific to either accelerometer are contain their model 
 * i.e. "bmp280" or "bme280".
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_MODULE_BARO_TYPE_BMP280 = 0;            ///< Constant identifying the BMP280 barometer module type
const uint8_t MBL_MW_MODULE_BARO_TYPE_BME280 = 1;            ///< Constant identifying the BME280 barometer module type

/**
 * Supported oversampling modes on the Bosch barometers
 */
typedef enum  {
    MBL_MW_BARO_BOSCH_OVERSAMPLING_SKIP= 0,
    MBL_MW_BARO_BOSCH_OVERSAMPLING_ULTRA_LOW_POWER,
    MBL_MW_BARO_BOSCH_OVERSAMPLING_LOW_POWER,
    MBL_MW_BARO_BOSCH_OVERSAMPLING_STANDARD,
    MBL_MW_BARO_BOSCH_OVERSAMPLING_HIGH,
    MBL_MW_BARO_BOSCH_OVERSAMPLING_ULTRA_HIGH
} MblMwBaroBoschOversampling;

/**
 * Supported filter modes on the Bosch barometers
 */
typedef enum {
    MBL_MW_BARO_BOSCH_IIR_FILTER_OFF= 0,
    MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_2,
    MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_4,
    MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_8,
    MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_16
} MblMwBaroBoschIirFilter;

/**
 * Supported stand by times on the BMP280 barometer
 */
typedef enum {
    MBL_MW_BARO_BMP280_STANDBY_TIME_0_5ms= 0,
    MBL_MW_BARO_BMP280_STANDBY_TIME_62_5ms,
    MBL_MW_BARO_BMP280_STANDBY_TIME_125ms,
    MBL_MW_BARO_BMP280_STANDBY_TIME_250ms,
    MBL_MW_BARO_BMP280_STANDBY_TIME_500ms,
    MBL_MW_BARO_BMP280_STANDBY_TIME_1000ms,
    MBL_MW_BARO_BMP280_STANDBY_TIME_2000ms,
    MBL_MW_BARO_BMP280_STANDBY_TIME_4000ms
} MblMwBaroBmp280StandbyTime;

/**
 * Supported stand by times on the BME280 barometer
 */
typedef enum {
    MBL_MW_BARO_BME280_STANDBY_TIME_0_5ms= 0,
    MBL_MW_BARO_BME280_STANDBY_TIME_62_5ms,
    MBL_MW_BARO_BME280_STANDBY_TIME_125ms,
    MBL_MW_BARO_BME280_STANDBY_TIME_250ms,
    MBL_MW_BARO_BME280_STANDBY_TIME_500ms,
    MBL_MW_BARO_BME280_STANDBY_TIME_1000ms,
    MBL_MW_BARO_BME280_STANDBY_TIME_10ms,
    MBL_MW_BARO_BME280_STANDBY_TIME_20ms
} MblMwBaroBme280StandbyTime;

/**
 * Retrieves the data signal representing pressure data from a Bosch barometer
 * This signal represent the pressure in Pascals
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's pressure data signal
 * uInt32 is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_baro_bosch_get_pressure_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing a single read from a Bosch barometer
 * This signal represent the pressure in Pascals
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's pressure data signal
 * uInt32 is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_baro_bosch_get_pressure_read_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing altitude data from a Bosch barometer
 * This signal represent the altitude in Meters
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's altitude data signal
 * Int32 is return signal data type (can be negative)
 */
METAWEAR_API MblMwDataSignal* mbl_mw_baro_bosch_get_altitude_data_signal(const MblMwMetaWearBoard *board);
/**
 * Set the oversampling mode
 * Settings available from ultra low power to ultra high resolution to adapt the sensor to the target application. 
 * See MblMwBaroBoschOversampling for allowed values.
 * The settings are predefined combinations of pressure measurement oversampling and temperature measurement oversampling. 
 * @param board     Pointer to the board to modify
 * @param oversampling      Oversampling value to set
 */
METAWEAR_API void mbl_mw_baro_bosch_set_oversampling(MblMwMetaWearBoard *board, MblMwBaroBoschOversampling oversampling);
/**
 * Set the iir filter coefficient
 * The built-in IIR filter minimizes short-term disturbances in the output data caused by the slamming of a door or window. 
 * The filter coefficient ranges from 0 (off) to 16. 
 * See MblMwBaroBoschIirFilter for allowed values.
 * @param board     Pointer to the board to modify
 * @param iir_filter    IIR filter value to set
 */
METAWEAR_API void mbl_mw_baro_bosch_set_iir_filter(MblMwMetaWearBoard *board, MblMwBaroBoschIirFilter iir_filter);
/**
 * Set the standby time for the BMP280 barometer
 * When the sensor operates in normal mode, it perpetualy cycles between an active measurement period and an inactive standby period. 
 * See MblMwBaroBmp280StandbyTime for allowed values.
 * @param board     Pointer to the board to modify
 * @param standby_time      Standby time value to set
 */
METAWEAR_API void mbl_mw_baro_bmp280_set_standby_time(MblMwMetaWearBoard *board, MblMwBaroBmp280StandbyTime standby_time);
/**
 * Set the standby time for the BME280 barometer
 * When the sensor operates in normal mode, it continuously cycles between an active measurement period and an inactive standby period. 
 * See MblMwBaroBme280StandbyTime for allowed values.
 * @param board     Pointer to the board to modify
 * @param standby_time      Standby time value to set
 */
METAWEAR_API void mbl_mw_baro_bme280_set_standby_time(MblMwMetaWearBoard *board, MblMwBaroBme280StandbyTime standby_time);
/**
 * Set the standby time.  If an invalid standby time is used, the closest valid value will be chosen
 * When the sensor operates in normal mode, it continuously cycles between an active measurement period and an inactive standby period. 
 * @param board                 Pointer to the board to modify
 * @param standby_time_ms       Standby time value to set, in milliseconds
 * @return Selected standby time, in milliseconds, or -1 if barometer not present
 */
METAWEAR_API float mbl_mw_baro_bosch_set_standby_time(MblMwMetaWearBoard *board, float standby_time_ms);
/**
 * Writes the configuration to the sensor
 * Applies the STANDBY, IIR and OVERSAMPLING values set in set_standbytime(), set_oversampling(), and set_iir_filter().
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_baro_bosch_write_config(const MblMwMetaWearBoard *board);
/**
 * Start pressure and altitude sensing
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_baro_bosch_start(const MblMwMetaWearBoard *board);
/**
 * Stop pressure and altitude sensing
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_baro_bosch_stop(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
