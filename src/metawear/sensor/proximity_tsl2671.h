/**
 * @copyright MbientLab License (LICENSE.md)
 * @file proximity_tsl2671.h
 * @brief Communicates with the TSL2671 proximity detector
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Photodiodes the detector should use for proximity detection
 */
typedef enum {
    MBL_MW_PROXIMITY_TSL2671_CHANNEL_0= 1,              ///< Use channel 0, which is responsive to infrared and visible light
    MBL_MW_PROXIMITY_TSL2671_CHANNEL_1,                 ///< Use channel 1, which is responsive to primarily infrared light
    MBL_MW_PROXIMITY_TSL2671_CHANNEL_BOTH               ///< Use both channel 0 and 1
} MblMwProximityTsl2671Channel;

/**
 * Amount of current driving the IR transmitter
 */
typedef enum {
    MBL_MW_PROXIMITY_TSL2671_CURRENT_100mA= 0,
    MBL_MW_PROXIMITY_TSL2671_CURRENT_50mA,
    MBL_MW_PROXIMITY_TSL2671_CURRENT_25mA,
    MBL_MW_PROXIMITY_TSL2671_CURRENT_12_5mA
} MblMwProximityTsl2671Current;

/**
 * Retrieves the data signal representing ADC values for the light's colors
 * @param board         Board the sensor resides on
 * @return Pointer to the data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_proximity_tsl2671_get_adc_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the integration time
 * @param board         Board to modify
 * @param time          New integration time to use, between [2.72, 693.6] milliseconds
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_set_integration_time(MblMwMetaWearBoard *board, float time);
/**
 * Sets the pulse count.  Sensitivity increase by the sqrt of pulse count.
 * @param board         Board to modify
 * @param n_pulses      Number of pulses to use for proximity detection, between [1, 255]
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_set_n_pulses(MblMwMetaWearBoard *board, uint8_t n_pulses);
/**
 * Sets the photodiode that responds to light
 * @param board         Board to modify
 * @param channel       New receiver channel to use
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_set_receiver_channel(MblMwMetaWearBoard *board, MblMwProximityTsl2671Channel channel);
/**
 * Sets the current driving the light transmitter.  For boards powered by the CR2032 battery, it is recommended that the current be 25mA or less.
 * @param board         Board to modify
 * @param current       New driver current to use
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_set_transmitter_current(MblMwMetaWearBoard *board, MblMwProximityTsl2671Current current);
/**
 * Writes the configuration to the sensor
 * @param board         Board the sensor resides on
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_write_config(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
