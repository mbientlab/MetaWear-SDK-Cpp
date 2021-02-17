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
 * Retrieves the data signal representing ADC values for the proximity of an object to the MetaWear
 * @param board         Board the sensor resides on
 * @return Pointer to the data signal
 * UINT32 is return signal data type 
 */
METAWEAR_API MblMwDataSignal* mbl_mw_proximity_tsl2671_get_adc_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the integration time
 * The proximity integration time (PTIME) is the period of time that the internal ADC converts the analog signal to a digital count. 
 * It is recommend that this be set to a minimum of PTIME = 0xFF or 2.72 ms.
 * @param board         Board to modify
 * @param time          New integration time to use, between [2.72, 693.6] milliseconds
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_set_integration_time(MblMwMetaWearBoard *board, float time);
/**
 * Sets the pulse count.  Sensitivity increase by the sqrt of pulse count.
 * The proximity pulse count register sets the number of proximity pulses that will be transmitted. 
 * PPULSE defines the number of pulses to be transmitted at a 62.5-kHz rate.
 * While the value can be programmed up to 255 pulses, the practical limit of the device is 32 pulses. 
 * It is recommended that 32 or fewer pulses be used to achieve maximum signal-to-noise ratio.
 * @param board         Board to modify
 * @param n_pulses      Number of pulses to use for proximity detection, between [1, 255]
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_set_n_pulses(MblMwMetaWearBoard *board, uint8_t n_pulses);
/**
 * Sets the photodiode that responds to light to be used
 * Channel 0 photodiode (CH0), which is responsive to both visible and infrared light
 * Channel 1 photodiode (CH1), which is responsive primarily to infrared light
 * See MblMwProximityTsl2671Channel for allowed values
 * @param board         Board to modify
 * @param channel       New receiver channel to use
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_set_receiver_channel(MblMwMetaWearBoard *board, MblMwProximityTsl2671Channel channel);
/**
 * Sets the current driving the light transmitter.  
 * An internal LED driver can be configured to provide a constant current sink of 12.5 mA, 25 mA, 50 mA, or 100 mA of current.
 * For boards powered by the CR2032 battery, it is recommended that the current be 25mA or less.
 * See MblMwProximityTsl2671Current for allowed values
 * @param board         Board to modify
 * @param current       New driver current to use
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_set_transmitter_current(MblMwMetaWearBoard *board, MblMwProximityTsl2671Current current);
/**
 * Writes the configuration to the sensor
 * Applies the INTEGRATION TIME, RECEIVER CHANNEL, PULSES and CURRENT values set in set_*().
 * @param board         Board the sensor resides on
 */
METAWEAR_API void mbl_mw_proximity_tsl2671_write_config(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
