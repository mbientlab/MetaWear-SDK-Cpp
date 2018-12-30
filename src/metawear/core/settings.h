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

//@{
///< Indices for component values of the battery data signal, to be used with mbl_mw_datasignal_get_component
const uint8_t MBL_MW_SETTINGS_BATTERY_VOLTAGE_INDEX = 0,
        MBL_MW_SETTINGS_BATTERY_CHARGE_INDEX = 1;
//@}

/**
 * Types of BLE addresses
 */
const uint8_t MBL_MW_ADDRESS_TYPE_PUBLIC = 0,
        MBL_MW_ADDRESS_TYPE_RANDOM_STATIC = 1,
        MBL_MW_ADDRESS_TYPE_PRIVATE_RESOLVABLE = 2,
        MBL_MW_ADDRESS_TYPE_PRIVATE_NON_RESOLVABLE = 3;

//@{
///< Values returned if the power or charge statuses are not supported
const int32_t MBL_MW_SETTINGS_POWER_STATUS_UNSUPPORTED = -1,
        MBL_MW_SETTINGS_CHARGE_STATUS_UNSUPPORTED = -1;
//@}

/**
 * Whitelist filter modes
 */
typedef enum {
    MBL_MW_WHITELIST_FILTER_ALLOW_FROM_ANY = 0,
    MBL_MW_WHITELIST_FILTER_SCAN_REQUESTS = 1,
    MBL_MW_WHITELIST_FILTER_CONNECTION_REQUESTS = 2,
    MBL_MW_WHITELIST_FILTER_SCAN_AND_CONNECTION_REQUESTS = 3
} MblMwWhitelistFilter;

typedef enum {
    MBL_MW_BLE_AD_TYPE_CONNECTED_UNDIRECTED = 0,
    MBL_MW_BLE_AD_TYPE_CONNECTED_DIRECTED = 1
} MblMwBleAdType;

/**
 * BLE MAC Address
 */
typedef struct {
    uint8_t address_type;       ///< Types of BLE address: MBL_MW_ADDRESS_TYPE_*
    uint8_t address[6];         ///< MAC Address - Little Endian (LSB First)
} MblMwBtleAddress;

/**
 * Retrieves an event pointer representing a disconnect event
 * @param board         Board the event is fired on
 * @return Pointer to the disconnect event
 */
METAWEAR_API MblMwEvent* mbl_mw_settings_get_disconnect_event(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing battery state
 * @param board         Calling object
 * @return Pointer to the battery state signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_settings_get_battery_state_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing the device GAP (MAC) address
 * @param board         Calling object
 * @return Pointer to the mac signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_settings_get_mac_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing the power status
 * @param board         Calling object
 * @return Pointer to the power status signal, nullptr if unsupported  
 */
METAWEAR_API MblMwDataSignal* mbl_mw_settings_get_power_status_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing the charge status
 * @param board         Calling object
 * @return Pointer to the charge status signal, nullptr if unsupported
 */
METAWEAR_API MblMwDataSignal* mbl_mw_settings_get_charge_status_data_signal(const MblMwMetaWearBoard* board);

/**
 * Sets the advertisement name
 * @param board             Board to modify
 * @param device_name       Byte array containing the device name, max 8 ASCII characters
 * @param len               Length of the array
 */
METAWEAR_API void mbl_mw_settings_set_device_name(const MblMwMetaWearBoard *board, const uint8_t *device_name, uint8_t len);
/**
 * @deprecated In v0.14.0, use <code>mbl_mw_settings_set_ad_parameters</code> instead
 */
METAWEAR_API void mbl_mw_settings_set_ad_interval(const MblMwMetaWearBoard *board, uint16_t interval, uint8_t timeout);
/**
 * Configures Bluetooth LE ad parameters
 * @param board         Calling object
 * @param interval      Advertisement interval, between [0, 65535] milliseconds
 * @param timeout       Advertisement timeout, between [0, 180] seconds where 0 indicates no timeout
 * @param type          Advertisement type, ignored if unsupported on current firmware
 */
METAWEAR_API void mbl_mw_settings_set_ad_parameters(const MblMwMetaWearBoard *board, uint16_t interval, uint8_t timeout, MblMwBleAdType type);
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
/**
 * Adds MAC Addresses for Whitelist filtering
 * @param board         Board to modify
 * @param index         Whitelist MAC address in range [1, 8], must start at 1 and go in increasing order
 * @param address       Address to add
 */
METAWEAR_API void mbl_mw_settings_add_whitelist_address(const MblMwMetaWearBoard *board, uint8_t index, const MblMwBtleAddress *address);
/**
 * Adds MAC Addresses for Whitelist filtering
 * @param board         Board to modify
 * @param index         Whitelist MAC address in range [1, 8], must start at 1 and go in increasing order
 * @param address       Address to add
 */
METAWEAR_API MblMwDataSignal* mbl_mw_settings_get_whitelist_data_signal(MblMwMetaWearBoard* board, uint8_t index);
/**
 * Sets connection parameters
 * @param board          Board to modify
 * @param mode           Whitelist filter mode
 */
METAWEAR_API void mbl_mw_settings_set_whitelist_filter_mode(const MblMwMetaWearBoard *board, MblMwWhitelistFilter mode);

/**
 * Reads the current power status if available.  The callback function will be called with:  
 * 1    - power source is attached  
 * 0    - no power source atached  
 * -1   - feature not supported  
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param handler       Callback function that is executed when the task is finished
 */
METAWEAR_API void mbl_mw_settings_read_current_power_status(MblMwMetaWearBoard* board, void* context, MblMwFnBoardPtrInt handler);
/**
 * Reads the current charge status.  The callback function will be called with:  
 * 1    - battery is charging  
 * 0    - battery is not charging  
 * -1   - feature not supported
 */
METAWEAR_API void mbl_mw_settings_read_current_charge_status(MblMwMetaWearBoard* board, void* context, MblMwFnBoardPtrInt handler);

#ifdef	__cplusplus
}
#endif
