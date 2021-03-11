/**
 * @copyright MbientLab License
 * @file accelerometer.h
 * @brief Generic class providing high level access to an accelerometer
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

//@{
/** Indices for component values of the acceleration data signal, used with mbl_mw_datasignal_get_component */
const uint8_t MBL_MW_ACC_ACCEL_X_AXIS_INDEX = 0, 
        MBL_MW_ACC_ACCEL_Y_AXIS_INDEX = 1, 
        MBL_MW_ACC_ACCEL_Z_AXIS_INDEX = 2;
//@}

/**
 * Retrieves the data signal representing acceleration data
 * This signal is timestamp,x,y,z acc data
 * @param board     Board to retrieve the signal from
 * @return Pointer to the acceleration data signal
 * MblMwCartesianFloat is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_get_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_acc_get_packed_acceleration_data_signal instead.
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * Variant of acceleration data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z acc data (it packs three acc data points in one timestamp)
 * @return Pointer to the data signal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_get_packed_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the output data rate.  If an invalid odr is used, the closest valid value will be used.
 * The ODR sets the output data frequency in Hz.
 * @param board     Board to configure
 * @param odr       Output data rate, in Hz
 */
METAWEAR_API float mbl_mw_acc_set_odr(MblMwMetaWearBoard *board, float odr);
/**
 * Sets the full scale range.  IF an invalid range is used, the closet valid value will be used.
 * The range is in units of Gs 
 * @param board     Board to configure
 * @param range     Sampling range, in g's
 */
METAWEAR_API float mbl_mw_acc_set_range(MblMwMetaWearBoard *board, float range);
/**
 * Writes the acceleration settings to the board
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Board to configure
 */
METAWEAR_API void mbl_mw_acc_write_acceleration_config(const MblMwMetaWearBoard* board);
/**
 * Pulls the current accelerometer output data rate and data range from the sensor
 * Reads the ODR and RANGE values set in the sensor.
 * This is a debug function, the data is return in the context ptr as AccBmi160Config->acc/AccBmi270Config->acc/Mma8452qConfig->acc
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
METAWEAR_API void mbl_mw_acc_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed);
/**
 * Switches the accelerometer to active mode
 * @param board     Board the accelerometer is on
 */
METAWEAR_API void mbl_mw_acc_start(const MblMwMetaWearBoard *board);
/**
 * Switches the accelerometer to standby mode
 * @param board     Board the accelerometer is on
 */
METAWEAR_API void mbl_mw_acc_stop(const MblMwMetaWearBoard *board);
/**
 * Enables acceleration sampling
 * The board will start gathering data from the accelerometer
 * @param board     Board to enable acceleration sampling on
 */
METAWEAR_API void mbl_mw_acc_enable_acceleration_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables acceleration sampling
 * The board will stop gathering data from the accelerometer
 * @param board     Board to disable acceleration sampling on
 */
METAWEAR_API void mbl_mw_acc_disable_acceleration_sampling(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
