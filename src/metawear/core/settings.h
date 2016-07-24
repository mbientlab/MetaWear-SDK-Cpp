/**
 * @copyright MbientLab License
 * @file settings.h
 * @brief Configures Bluetooth Low Energy advertisement and connection settings
 */
#pragma once

#include <stdint.h>

#include "datasignal_fwd.h"
#include "event_fwd.h"
#include "metawearboard_fwd.h"

#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Retrieves an event pointer representing a disconnect event
 * @param board         Board the event is fired on
 * @return Pointer to the disconnect event
 */
METAWEAR_API MblMwEvent* mbl_mw_settings_get_disconnect_event(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing battery state
 * @param board         Board the battery state is from
 * @return Pointer to the battery state signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_settings_get_battery_state_data_signal(const MblMwMetaWearBoard *board);

/**
 * Sets the advertisement name
 * @param board             Board to modify
 * @param device_name       Byte array containing the device name, max 8 ASCII characters
 * @param len               Length of the array
 */
METAWEAR_API void mbl_mw_settings_set_device_name(const MblMwMetaWearBoard *board, const uint8_t *device_name, uint8_t len);
/**
 * Sets the advertising interval
 * @param board         Board to set the ad interval
 * @param interval      Advertisement interval, between [0, 65535] milliseconds
 * @param timeout       Advertisement timeout, between [0, 180] seconds where 0 indicates no timeout
 */
METAWEAR_API void mbl_mw_settings_set_ad_interval(const MblMwMetaWearBoard *board, uint16_t interval, uint8_t timeout);
/**
 * Sets advertising transmitting power.  If a non valid value is set, the nearest valid value will be used instead
 * @param board         Board to set the TX power
 * @param tx_power      Valid values are: 4, 0, -4, -8, -12, -16, -20, -30
 */
METAWEAR_API void mbl_mw_settings_set_tx_power(const MblMwMetaWearBoard *board, int8_t tx_power);
/**
 * Starts advertising
 * @param board         Board to start btle advertisement
 */
METAWEAR_API void mbl_mw_settings_start_advertising(const MblMwMetaWearBoard *board);
/**
 * Sets scan response
 * @param board         Board to modify
 * @param response      Scan response as a byte array
 * @param len           Length of the array
 */
METAWEAR_API void mbl_mw_settings_set_scan_response(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
/**
 * Sets connection parameters
 * @param board                 Board to modify
 * @param min_conn_interval     Connection interval lower bound, min 7.5ms
 * @param max_conn_interval     Connection interval upper bound, max 4000ms
 * @param latency               Number of connection intervals to skip, betwen [0, 1000]
 * @param timeout               Max time between data exchanges until the connection is considered to be lost, between [10, 32000]ms
 */
METAWEAR_API void mbl_mw_settings_set_connection_parameters(const MblMwMetaWearBoard *board, float min_conn_interval, float max_conn_interval, 
        uint16_t latency, uint16_t timeout);

#ifdef	__cplusplus
}
#endif
