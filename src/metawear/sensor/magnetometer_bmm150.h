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
    MWL_MW_MAG_BMM_150_PP_LOW_POWER= 0,
    MWL_MW_MAG_BMM_150_PP_REGULAR,
    MWL_MW_MAG_BMM_150_PP_ENHANCED_REGULAR,
    MWL_MW_MAG_BMM_150_PP_HIGH_ACCURACY
} MblMwMagBmm150PowerPreset;

/**
 * Retrieves the data signal representing B field strength
 * @param board         Board the magnetometer resides on
 * @return Pointer to the data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_mag_bmm150_get_b_field_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the power mode to one of the recommended presets
 * @param board         Board to modify
 * @param preset        New preset power mode to use
 */
METAWEAR_API void mbl_mw_mag_bmm150_set_power_preset(const MblMwMetaWearBoard *board, MblMwMagBmm150PowerPreset preset);
/**
 * Enable B field sampling
 * @param board         Board the magnetometer resides on
 */
METAWEAR_API void mbl_mw_mag_bmm150_enable_b_field_sampling(const MblMwMetaWearBoard *board);
/**
 * Disable B field sampling
 * @param board         Board the magnetometer resides on
 */
METAWEAR_API void mbl_mw_mag_bmm150_disable_b_field_sampling(const MblMwMetaWearBoard *board);
/**
 * Switches the magnetometer into normal mode
 * @param board         Board the magnetometer resides on
 */
METAWEAR_API void mbl_mw_mag_bmm150_start(const MblMwMetaWearBoard *board);
/**
 * Switches the magnetometer into sleep mode
 * @param board         Board the magnetometer resides on
 */
METAWEAR_API void mbl_mw_mag_bmm150_stop(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
