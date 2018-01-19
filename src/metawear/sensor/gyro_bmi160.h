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

//@{
/** Indices for component values of the rotation data signal, used with mbl_mw_datasignal_get_component */
const uint8_t MBL_MW_GYRO_ROTATION_X_AXIS_INDEX = 0, 
        MBL_MW_GYRO_ROTATION_Y_AXIS_INDEX = 1, 
        MBL_MW_GYRO_ROTATION_Z_AXIS_INDEX = 2;
//@}

/**
 * Available output data rates on the BMI160 gyro
 */
typedef enum {
    MBL_MW_GYRO_BMI160_ODR_25Hz= 6,
    MBL_MW_GYRO_BMI160_ODR_50Hz,
    MBL_MW_GYRO_BMI160_ODR_100Hz,
    MBL_MW_GYRO_BMI160_ODR_200Hz,
    MBL_MW_GYRO_BMI160_ODR_400Hz,
    MBL_MW_GYRO_BMI160_ODR_800Hz,
    MBL_MW_GYRO_BMI160_ODR_1600Hz,
    MBL_MW_GYRO_BMI160_ODR_3200Hz
} MblMwGyroBmi160Odr;

/**
 * Available degrees per second ranges on the BMI160 gyro
 */
typedef enum {
    MBL_MW_GYRO_BMI160_RANGE_2000dps= 0,      ///< +/-2000 degrees per second
    MBL_MW_GYRO_BMI160_RANGE_1000dps,         ///< +/-1000 degrees per second
    MBL_MW_GYRO_BMI160_RANGE_500dps,          ///< +/-500 degrees per second
    MBL_MW_GYRO_BMI160_RANGE_250dps,          ///< +/-250 degrees per second
    MBL_MW_GYRO_BMI160_RANGE_125dps           ///< +/-125 degrees per second
} MblMwGyroBmi160Range;

/**
 * Retrieves the data signal representing BMI160 rotation data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 rotation data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi160_get_rotation_data_signal(const MblMwMetaWearBoard *board);
/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_gyro_bmi160_get_packed_rotation_data_signal instead.
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal(const MblMwMetaWearBoard *board);
/**
 * Variant of rotation data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * @return Pointer to the data singal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi160_get_packed_rotation_data_signal(const MblMwMetaWearBoard *board);

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
 * Pulls the current gyro output data rate and data range from the sensor
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
METAWEAR_API void mbl_mw_gyro_bmi160_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed);

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
