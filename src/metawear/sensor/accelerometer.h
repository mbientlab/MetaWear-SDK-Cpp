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

/**
 * Retrieves the data signal representing acceleration data
 * @param board     Board to retrieve the signal from
 * @return Pointer to the acceleration data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_get_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves a special data signal representing high frequency (>100Hz) acceleration data.  This signal is only for 
 * streaming and cannot use logging nor data processing.  To use those features with an acceleration data signal, use the signal from
 * mbl_mw_acc_get_acceleration_data_signal.
 * @param board     Board to retrieve the signal from
 * @return Pointer to a high frequency data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard *board);

/**
 * Sets the output data rate.  If an invalid odr is used, the closest valid value will be used.
 * @param board     Board to configure
 * @param odr       Output data rate, in Hz
 */
METAWEAR_API float mbl_mw_acc_set_odr(MblMwMetaWearBoard *board, float odr);
/**
 * Sets the full scale range.  IF an invalid range is used, the closet valid value will be used.
 * @param board     Board to configure
 * @param range     Sampling range, in g's
 */
METAWEAR_API float mbl_mw_acc_set_range(MblMwMetaWearBoard *board, float range);
/**
 * Writes the acceleration settings to the board
 * @param board     Board to configure
 */
METAWEAR_API void mbl_mw_acc_write_acceleration_config(const MblMwMetaWearBoard* board);

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
 * @param board     Board to enable acceleration sampling on
 */
METAWEAR_API void mbl_mw_acc_enable_acceleration_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables acceleration sampling
 * @param board     Board to disable acceleration sampling on
 */
METAWEAR_API void mbl_mw_acc_disable_acceleration_sampling(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
