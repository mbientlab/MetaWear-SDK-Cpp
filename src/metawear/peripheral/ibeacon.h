/**
 * @copyright MbientLab License 
 * @file ibeacon.h
 * @brief Configures iBeacon mode
 */
#pragma once

#include "peripheral_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Sets the iBeacon advertising major number
 * @param board     Pointer to the board to send the command to
 * @param major     New advertising major number
 */
METAWEAR_API void mbl_mw_ibeacon_set_major(const MblMwMetaWearBoard *board, uint16_t major);
/**
 * Sets the iBeacon advertising minor number
 * @param board     Pointer to the board to send the command to
 * @param minor     New advertising minor number
 */
METAWEAR_API void mbl_mw_ibeacon_set_minor(const MblMwMetaWearBoard *board, uint16_t minor);
/**
 * Sets the iBeacon advertising period
 * @param board     Pointer to the board to send the command to
 * @param period    New advertising period, in milliseconds
 */
METAWEAR_API void mbl_mw_ibeacon_set_period(const MblMwMetaWearBoard *board, uint16_t period);
/**
 * Sets the iBeacon advertising transmitting power
 * @param board         Pointer to the board to send the command to
 * @param tx_power      New advertising transmitting power
 */
METAWEAR_API void mbl_mw_ibeacon_set_tx_power(const MblMwMetaWearBoard *board, int8_t tx_power);
/**
 * Sets the iBeacon advertising receiving power
 * @param board         Pointer to the board to send the command to
 * @param rx_power      New advertising receiving power
 */
METAWEAR_API void mbl_mw_ibeacon_set_rx_power(const MblMwMetaWearBoard *board, int8_t rx_power);
/**
 * Sets the iBeacon advertising UUID
 * @param board         Pointer to the board to send the command to
 * @param ad_uuid       Byte representation of the UUID in little endian ordering
 */
METAWEAR_API void mbl_mw_ibeacon_set_uuid(const MblMwMetaWearBoard *board, uint8_t ad_uuid[16]);

/**
 * Enables iBeacon mode.  You will need to disconnect from the board to 
 * advertise as an iBeacon
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_ibeacon_enable(const MblMwMetaWearBoard *board);
/**
 * Disables iBeacon mode
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_ibeacon_disable(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
