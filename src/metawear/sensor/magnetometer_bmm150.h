/**
 * @copyright MbientLab License (LICENSE.md)
 * @file magnetometer_bmm150.h
 * @brief Communicates with the BMM150 magnetometer
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

//@{
/** Indices for component values of the bfield data signal, used with mbl_mw_datasignal_get_component */
const uint8_t MBL_MW_MAG_BFIELD_X_AXIS_INDEX = 0, 
        MBL_MW_MAG_BFIELD_Y_AXIS_INDEX = 1, 
        MBL_MW_MAG_BFIELD_Z_AXIS_INDEX = 2;
//@}

/**
 * Preset power modes recommended by Bosch
 * <table>
 *     <tr>
 *         <th>Setting</th>
 *         <th>ODR</th>
 *         <th>Average Current</th>
 *         <th>Noise</th>
 *     </tr>
 *     <tr>
 *         <td>LOW_POWER</td>
 *         <td>10Hz</td>
 *         <td>170&#956;A</td>
 *         <td>1.0&#956;T (xy axis), 1.4&#956;T (z axis)</td>
 *     </tr>
 *     <tr>
 *         <td>REGULAR</td>
 *         <td>10Hz</td>
 *         <td>0.5mA</td>
 *         <td>0.6&#956;T</td>
 *     </tr>
 *     <tr>
 *         <td>ENHANCED_REGULAR</td>
 *         <td>10Hz</td>
 *         <td>0.8mA</td>
 *         <td>0.5&#956;T</td>
 *     </tr>
 *     <tr>
 *         <td>HIGH_ACCURACY</td>
 *         <td>20Hz</td>
 *         <td>4.9mA</td>
 *         <td>0.3&#956;T</td>
 *     </tr>
 * </table>
 */
typedef enum {
    MBL_MW_MAG_BMM150_PRESET_LOW_POWER= 0,
    MBL_MW_MAG_BMM150_PRESET_REGULAR,
    MBL_MW_MAG_BMM150_PRESET_ENHANCED_REGULAR,
    MBL_MW_MAG_BMM150_PRESET_HIGH_ACCURACY
} MblMwMagBmm150Preset;

/**
 * Available output data rates for the BMM150 magnetometer
 */
typedef enum {
    MBL_MW_MAG_BMM150_ODR_10Hz = 0,
    MBL_MW_MAG_BMM150_ODR_2Hz,
    MBL_MW_MAG_BMM150_ODR_6Hz,
    MBL_MW_MAG_BMM150_ODR_8Hz,
    MBL_MW_MAG_BMM150_ODR_15Hz,
    MBL_MW_MAG_BMM150_ODR_20Hz,
    MBL_MW_MAG_BMM150_ODR_25Hz,
    MBL_MW_MAG_BMM150_ODR_30Hz
} MblMwMagBmm150Odr;

/**
 * Retrieves the data signal representing B field strength in uT (micro Teslas)
 * This signal is timestamp,x,y,z mag data
 * @param board         Calling object
 * @return Pointer to the data signal
 * MblMwCartesianFloat is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_mag_bmm150_get_b_field_data_signal(const MblMwMetaWearBoard *board);
/**
 * Variant of B field data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z mag data (it packs three mag data points in one timestamp)
 * @param board         Calling object
 * @return Pointer to the data signal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_mag_bmm150_get_packed_b_field_data_signal(const MblMwMetaWearBoard *board);
/**
 * Manually configure the sensor, only for advanced users.  
 * It is recommended that users use one of the preset configurations.
 * @param board         Calling object
 * @param xy_reps       Repetitions on the x/y-axis
 * @param z_reps        Repetitions on the z-axis
 * @param odr           Sensor data rate
 */
METAWEAR_API void mbl_mw_mag_bmm150_configure(const MblMwMetaWearBoard *board, uint16_t xy_reps, uint16_t z_reps, MblMwMagBmm150Odr odr);
/**
 * Sets the power mode to one of the recommended presets
 * The BMM150 magnetometer part has four power modes, see MblMwMagBmm150Preset for values
 * @param board         Calling object
 * @param preset        New preset power mode to use
 */
METAWEAR_API void mbl_mw_mag_bmm150_set_preset(const MblMwMetaWearBoard *board, MblMwMagBmm150Preset preset);
/**
 * Enable B field sampling
 * The board will start gathering data from the magnetometer
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_mag_bmm150_enable_b_field_sampling(const MblMwMetaWearBoard *board);
/**
 * Disable B field sampling
 * The board will stop gathering data from the magnetometer
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_mag_bmm150_disable_b_field_sampling(const MblMwMetaWearBoard *board);
/**
 * Switches the magnetometer into normal mode
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_mag_bmm150_start(const MblMwMetaWearBoard *board);
/**
 * Switches the magnetometer into sleep mode
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_mag_bmm150_stop(const MblMwMetaWearBoard *board);
/**
 * Puts the magnetometer in suspend mode.  This function will not issuee the command unless the 
 * board is running minimum firmware v1.3.4.
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_mag_bmm150_suspend(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
