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
    MBL_MW_ACC_MMA8452Q_RANGE_2G= 0,  ///< +/- 2g
    MBL_MW_ACC_MMA8452Q_RANGE_4G,     ///< +/- 4g
    MBL_MW_ACC_MMA8452Q_RANGE_8G      ///< +/- 8g
} MblMwAccMma8452qRange;

/**
 * Available output data rates on the MMA8452Q accelerometer
 */
typedef enum {
    MBL_MW_ACC_MMA8452Q_ODR_800Hz= 0,
    MBL_MW_ACC_MMA8452Q_ODR_400Hz,
    MBL_MW_ACC_MMA8452Q_ODR_200Hz,
    MBL_MW_ACC_MMA8452Q_ODR_100Hz,
    MBL_MW_ACC_MMA8452Q_ODR_50Hz,
    MBL_MW_ACC_MMA8452Q_ODR_12_5Hz,
    MBL_MW_ACC_MMA8452Q_ODR_6_25Hz,
    MBL_MW_ACC_MMA8452Q_ODR_1_56Hz
} MblMwAccMma8452qOdr;

/**
 * Available cutoff frequencies for the MMA8452Q high pass filter
 */
typedef enum {
    MBL_MW_ACC_MMA8452Q_CUTOFF_FREQ_HIGHEST = 0,
    MBL_MW_ACC_MMA8452Q_CUTOFF_FREQ_HIGH = 1,
    MBL_MW_ACC_MMA8452Q_CUTOFF_FREQ_MEDIUM = 2,
    MBL_MW_ACC_MMA8452Q_CUTOFF_FREQ_LOW = 3
} MblMwAccMma8452qCutoffFreq;

/**
 * Retrieves the data signal representing acceleration data for the MMA8452Q accelerometer
 * This signal is timestamp,x,y,z acc data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's MMA8452Q acceleration data signal
 * MblMwCartesianFloat is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_mma8452q_get_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_acc_mma8452q_get_packed_acceleration_data_signal instead.
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * Variant of acceleration data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z acc data (it packs three acc data points in one timestamp)
 * @return Pointer to the data singal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_mma8452q_get_packed_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the output data rate
 * The ODR sets the output data frequency in Hz.
 * @param board     Calling object
 * @param odr       Output data rate value to set
 */
METAWEAR_API void mbl_mw_acc_mma8452q_set_odr(MblMwMetaWearBoard *board, MblMwAccMma8452qOdr odr);
/**
 * Sets the acceleration range 
 * The range is in units of Gs 
 * @param board     Calling object
 * @param range     Acceleration range value to set
 */
METAWEAR_API void mbl_mw_acc_mma8452q_set_range(MblMwMetaWearBoard *board, MblMwAccMma8452qRange range);
/**
 * Sets the cutoff frequency for the high-pass filter
 * The high-pass filter cutoff frequency can be set by the user to four different frequencies which are dependent on the output data rate (ODR)
 * Cutoff frequency is set to 16 Hz @ 800 Hz by default. See the MM8452Q datasheet for available options.
 * @param board     Calling object
 * @param frequency     Hpf cutoff frequency, set to 0 to disable the high pass filter
 */
METAWEAR_API void mbl_mw_acc_mma8452q_set_high_pass_cutoff(MblMwMetaWearBoard *board, float frequency);
/**
 * Writes the acceleration settings to the sensor
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_mma8452q_write_acceleration_config(const MblMwMetaWearBoard *board);
/**
 * Switches the accelerometer to active mode.  
 * When in active mode, the accelerometer cannot be configured
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_mma8452q_start(const MblMwMetaWearBoard *board);
/**
 * Switches the accelerometer to standby mode
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_mma8452q_stop(const MblMwMetaWearBoard *board);
/**
 * Enables acceleration sampling
 * The board will start gathering data from the accelerometer
  * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_mma8452q_enable_acceleration_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables acceleration sampling
 * The board will stop gathering data from the accelerometer
 * @param board      Calling object
 */
METAWEAR_API void mbl_mw_acc_mma8452q_disable_acceleration_sampling(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing data from the orientation detection algorithm
 * This signal is static orientation detection (portrait/landscape, up/down, left/right, back/front position)
 * @param board     Calling object
 * @return Pointer to MMA8452Q's orientation detection data signal
 * MblMwSensorOrientation is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_mma8452q_get_orientation_detection_data_signal(const MblMwMetaWearBoard* board);
/**
 * Set the orientation calculation mode
 * @param board         Calling object
 * @param delay         Time, in milliseconds, for which the sensor's orientation must remain in the new position 
 *                      before a position change is triggered
 */
METAWEAR_API void mbl_mw_acc_mma8452q_set_orientation_delay(MblMwMetaWearBoard *board, uint16_t delay);
/**
 * Enables orientation detection
 * The board will start gathering orientation data from the accelerometer
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_mma8452q_enable_orientation_detection(const MblMwMetaWearBoard *board);
/**
 * Disables orientation detection
 * The board will stop gathering orientation data from the accelerometer
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_mma8452q_disable_orientation_detection(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
