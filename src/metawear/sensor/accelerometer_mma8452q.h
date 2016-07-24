/**
 * @copyright MbientLab License 
 * @file accelerometer_mma8452q.h
 * @brief Functions for interacting with the MMA8452Q accelerometer.
 * @details This sensor is only available on MetaWear R boards.
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_MODULE_ACC_TYPE_MMA8452Q = 0;          ///< Constant identifying the MMA8452Q accelerometer type

/*
typedef enum {
    MBL_MW_ACC_MMA8452Q_AXIS_X,
    MBL_MW_ACC_MMA8452Q_AXIS_Y,
    MBL_MW_ACC_MMA8452Q_AXIS_Z
} MblMWMma8452qAxis;

typedef enum {
    MBL_MW_ACC_MMA8452Q_FREE_FALL,
    MBL_MW_ACC_MMA8452Q_MOTION
} MblMwAccMma8452qMovementType;
*/

/**
 * Available g-ranges on the MMA8452Q accelerometer
 */
typedef enum {
    MBL_MW_ACC_MMA8452Q_FSR_2G= 0,  ///< +/- 2g
    MBL_MW_ACC_MMA8452Q_FSR_4G,     ///< +/- 4g
    MBL_MW_ACC_MMA8452Q_FSR_8G      ///< +/- 8g
} MblMwAccMma8452qRange;

/**
 * Available output data rates on the MMA8452Q accelerometer
 */
typedef enum {
    MBL_MW_ACC_MMA8452Q_ODR_800HZ= 0,
    MBL_MW_ACC_MMA8452Q_ODR_400HZ,
    MBL_MW_ACC_MMA8452Q_ODR_200HZ,
    MBL_MW_ACC_MMA8452Q_ODR_100HZ,
    MBL_MW_ACC_MMA8452Q_ODR_50HZ,
    MBL_MW_ACC_MMA8452Q_ODR_12_5HZ,
    MBL_MW_ACC_MMA8452Q_ODR_6_25HZ,
    MBL_MW_ACC_MMA8452Q_ODR_1_56HZ
} MblMwAccMma8452qOdr;

/**
 * Retrieves the data signal representing acceleration data for the MMA8452Q accelerometer
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's MMA8452Q acceleration data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_mma8452q_get_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves a special data signal representing high frequency (>100Hz) acceleration data for the MMA8452Q accelerometer.  This signal is only for 
 * streaming and cannot use logging nor data processing.  To use those features with an acceleration data signal, use the signal from
 * mbl_mw_acc_mma8452q_get_acceleration_data_signal.
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to a high frequency data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard *board);

/**
 * Sets the output data rate
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to set
 */
METAWEAR_API void mbl_mw_acc_mma8452q_set_odr(MblMwMetaWearBoard *board, MblMwAccMma8452qOdr odr);
/**
 * Sets the acceleration range 
 * @param board     Pointer to the board to modify
 * @param range     Acceleration range value to set
 */
METAWEAR_API void mbl_mw_acc_mma8452q_set_range(MblMwMetaWearBoard *board, MblMwAccMma8452qRange range);
/**
 * Writes the acceleration settings to the sensor
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_write_acceleration_config(const MblMwMetaWearBoard *board);

/**
 * Switches the accelerometer to active mode.  When in active mode, the accelerometer cannot be configured
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_start(const MblMwMetaWearBoard *board);
/**
 * Switches the accelerometer to standby mode
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_stop(const MblMwMetaWearBoard *board);

/**
 * Enables acceleration sampling
  * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_enable_acceleration_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables acceleration sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_disable_acceleration_sampling(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
