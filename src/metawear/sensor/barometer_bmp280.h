/**
 * @copyright MbientLab License 
 * @file barometer_bmp280.h
 * @brief Functions for interacting with the BMP280 pressure sensor.
 * @details This sensor is only available on MetaWear RPro boards.
 */
#pragma once

#include <stdint.h>

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"
#include "metawear/core/metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

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
 * Retrieves the data signal representing BMP280 pressure data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMP280 pressure data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_baro_bmp280_get_pressure_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing BMP280 altitude data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMP280 altitude data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_baro_bmp280_get_altitude_data_signal(const MblMwMetaWearBoard *board);

/**
 * Set the oversampling mode
 * @param board     Pointer to the board to modify
 * @param oversampling      Oversampling value to set
 */
METAWEAR_API void mbl_mw_baro_bmp280_set_oversampling(MblMwMetaWearBoard *board, MblMwBaroBmp280Oversampling oversampling);
/**
 * Set the iir filter coefficient
 * @param board     Pointer to the board to modify
 * @param iir_filter    IIR filter value to set
 */
METAWEAR_API void mbl_mw_baro_bmp280_set_iir_filter(MblMwMetaWearBoard *board, MblMwBaroBmp280IirFilter iir_filter);
/**
 * Set the standby time
 * @param board     Pointer to the board to modify
 * @param standby_time      Standby time value to set
 */
METAWEAR_API void mbl_mw_baro_bmp280_set_standby_time(MblMwMetaWearBoard *board, MblMwBaroBmp280StandbyTime standby_time);
/**
 * Writes the configuration to the sensor
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_baro_bmp280_write_config(const MblMwMetaWearBoard *board);

/**
 * Start pressure and altitude sensing
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_baro_bmp280_start(const MblMwMetaWearBoard *board);
/**
 * Stop pressure and altitude sensing
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_baro_bmp280_stop(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
