/**
 * @copyright MbientLab License 
 * @file gyro_bmi160.h
 * @brief Functions for interacting with the BMI160 gyro.
 * @details This sensor is only available on MetaWear RG and RPro boards.
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Available output data rates on the BMI160 gyro
 */
typedef enum {
    MBL_MW_GYRO_BMI160_ODR_25HZ= 6,
    MBL_MW_GYRO_BMI160_ODR_50HZ,
    MBL_MW_GYRO_BMI160_ODR_100HZ,
    MBL_MW_GYRO_BMI160_ODR_200HZ,
    MBL_MW_GYRO_BMI160_ODR_400HZ,
    MBL_MW_GYRO_BMI160_ODR_800HZ,
    MBL_MW_GYRO_BMI160_ODR_1600HZ,
    MBL_MW_GYRO_BMI160_ODR_3200HZ
} MblMwGyroBmi160Odr;

/**
 * Available degrees per second ranges on the BMI160 gyro
 */
typedef enum {
    MBL_MW_GYRO_BMI160_FSR_2000DPS= 0,      ///< +/-2000 degrees per second
    MBL_MW_GYRO_BMI160_FSR_1000DPS,         ///< +/-1000 degrees per second
    MBL_MW_GYRO_BMI160_FSR_500DPS,          ///< +/-500 degrees per second
    MBL_MW_GYRO_BMI160_FSR_250DPS,          ///< +/-250 degrees per second
    MBL_MW_GYRO_BMI160_FSR_125DPS           ///< +/-125 degrees per second
} MblMwGyroBmi160Range;

/**
 * Retrieves the data signal representing BMI160 rotation data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 rotation data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi160_get_rotation_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves a special data signal representing high frequency (>100Hz) rotation data for the BMI160 IMU.  This signal is only for streaming and cannot 
 * use logging nor data processing.  To use those features with a rotation data signal, use the signal from 
 * mbl_mw_gyro_bmi160_get_rotation_data_signal.
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to a high frequency data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal(const MblMwMetaWearBoard *board);

/**
 * Sets the output data rate 
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_gyro_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwGyroBmi160Odr odr);
/**
 * Sets the rotation range
 * @param board     Pointer to the board to modify
 * @param range     New rotation range
 */
METAWEAR_API void mbl_mw_gyro_bmi160_set_range(MblMwMetaWearBoard *board, MblMwGyroBmi160Range range);
/**
 * Writes the configuration to the sendor
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_write_config(const MblMwMetaWearBoard *board);

/**
 * Switches the gyro to active mode.  While in active mode, the gyro cannot be configured
 */
METAWEAR_API void mbl_mw_gyro_bmi160_start(const MblMwMetaWearBoard *board);
/**
 * Switches the gyro to standby mode.
 */
METAWEAR_API void mbl_mw_gyro_bmi160_stop(const MblMwMetaWearBoard *board);

/**
 * Enables rotation sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_enable_rotation_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables rotation sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_disable_rotation_sampling(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
