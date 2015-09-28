/**
 * @copyright MbientLab License 
 * @file ibeacon.h
 * @brief Configures iBeacon mode
 */
#pragma once

#include <stdint.h>
#include "dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Constructs the command that sets the iBeacon advertising major number
 * @param command   Byte array (len 4) for the function to write the command to
 * @param major     New advertising major number
 */
METAWEAR_API void mbl_mw_ibeacon_set_major(uint8_t command[4], uint16_t major);
/**
 * Constructs the command that sets the iBeacon advertising minor number
 * @param command   Byte array (len 4) for the function to write the command to
 * @param minor     New advertising minor number
 */
METAWEAR_API void mbl_mw_ibeacon_set_minor(uint8_t command[4], uint16_t minor);
/**
 * Constructs the command that sets the iBeacon advertising period
 * @param command   Byte array (len 4) for the function to write the command to
 * @param period    New advertising period, in milliseconds
 */
METAWEAR_API void mbl_mw_ibeacon_set_period(uint8_t command[4], uint16_t period);
/**
 * Constructs the command that sets the iBeacon advertising transmitting power
 * @param command   Byte array (len 3) for the function to write the command to
 * @param tx_power     New advertising transmitting power
 */
METAWEAR_API void mbl_mw_ibeacon_set_tx_power(uint8_t command[3], int8_t tx_power);
/**
 * Constructs the command that sets the iBeacon advertising receiving power
 * @param command   Byte array (len 3) for the function to write the command to
 * @param rx_power     New advertising receiving power
 */
METAWEAR_API void mbl_mw_ibeacon_set_rx_power(uint8_t command[3], int8_t rx_power);
/**
 * Constructs the command that sets the iBeacon advertising UUID
 * @param command   Byte array (len 18) for the function to write the command to
 * @param ad_uuid    Byte representation of the UUID in little endian ordering
 */
METAWEAR_API void mbl_mw_ibeacon_set_uuid(uint8_t command[18], uint8_t ad_uuid[16]);

/**
 * Constructs the command that enables iBeacon mode.  You will need to disconnect from the board to 
 * advertise as an iBeacon
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_ibeacon_enable(uint8_t command[3]);
/**
 * Constructs the command that disables iBeacon mode
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_ibeacon_disable(uint8_t command[3]);

#ifdef	__cplusplus
}
#endif
