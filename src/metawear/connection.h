/**
 * @copyright MbientLab License 
 * @file connection.h
 * @brief Methods and types for communicating with the metawear board
 */
#pragma once

#include <stdint.h>

#include "data.h"
#include "datasignal_fwd.h"
#include "dllmarker.h"
#include "metawearboard_fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/** High QWORD of the MetaWear gatt service UUID */
const uint64_t METAWEAR_GATT_SERVICE_UUID_HIGH= 0x326a900085cb9195;
/** Low QWORD of the MetaWear gatt service uuid */
const uint64_t METAWEAR_GATT_SERVICE_UUID_LOW=  0xd9dd464cfbbae75a;

/** High QWORD of the MetaWear command gatt characteristic */
const uint64_t METAWEAR_GATT_CHAR_COMMAND_UUID_HIGH= 0x326a900185cb9195;
/** Low QWORD of the MetaWear command gatt characteristic */
const uint64_t METAWEAR_GATT_CHAR_COMMAND_UUID_LOW=  0xd9dd464cfbbae75a;

/** High QWORD of the MetaWear notification gatt characteristic */
const uint64_t METAWEAR_GATT_CHAR_NOTIFY_UUID_HIGH= 0x326a900685cb9195;
/** Low QWORD of the MetaWear notification gatt characteristic */
const uint64_t METAWEAR_GATT_CHAR_NOTIFY_UUID_LOW=  0xd9dd464cfbbae75a;

/**
 * Container wrapping functions that communicate with the MetaWear board
 * @author Eric Tsai
 */
typedef struct {
    /**
     * Writes the command to the MetaWear Gatt service.  All commands are directed to this function
     * @param board     Pointer to the board the command is for
     * @param command   Byte array holding the command
     * @param len       Length of the array
     */
    void (*send_command)(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len);
    /**
     * Processes data received from the sensors and directly the values to the appropriate consumers
     * @param signal    Pointer to the data signal the message is from
     * @param data      Pointer to the message container
     */
    void (*received_sensor_data)(const MblMwDataSignal* signal, const MblMwData* data);
} MblMwConnection;

/**
 * Sets the MblMwConnection the library uses to communicate with the board
 * @param conn        Function that writes to the MetaWear command characteristic
 */
METAWEAR_API void mbl_mw_connection_init(const MblMwConnection* conn);

#ifdef __cplusplus
}
#endif
