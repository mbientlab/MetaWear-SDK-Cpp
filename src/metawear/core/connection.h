/**
 * @copyright MbientLab License 
 * @file connection.h
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "metawearboard_fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UUIDs identifying a gatt characteristic and its parent service
 */
typedef struct {
    uint64_t service_uuid_high;         ///< High 64 bits of the parent service uuid
    uint64_t service_uuid_low;          ///< Low 64 bits of the parent service uuid
    uint64_t uuid_high;                 ///< High 64 bits of the characteristic uuid
    uint64_t uuid_low;                  ///< Low 64 bits of the characteristic uuid
} MblMwGattChar;

/** UUIDs for the MetaWear notification characteristic */
const MblMwGattChar METAWEAR_SERVICE_NOTIFY_CHAR = { 0x326a900085cb9195, 0xd9dd464cfbbae75a, 0x326a900685cb9195, 0xd9dd464cfbbae75a };

/**
 * Wrapper class containing functions for communicating with the MetaWear through a Bluetooth Low Energy connection.
 */
typedef struct {
    /** 
     * Writes the characteristic and value to the device
     * @param characteristic    Gatt characteristic to write
     * @param value             Value to write as a byte array
     * @param length            Length of the byte array
     */
    void (*write_gatt_char)(const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);
    /**
     * Reads the value of the characteristic from the device
     * @param characteristic        Gatt characteristic to read
     */
    void (*read_gatt_char)(const MblMwGattChar *characteristic);
} MblMwBtleConnection;

/**
 * Handles changes from the MetaWear notify characteristic.  All characteristic changes from the notify characteristic must be forwarded 
 * to this function
 * @param board         Board the characteristic change is from
 * @param value         Byte array containing the new characteristic value
 * @param len           Length of the array
 * @see METAWEAR_SERVICE_NOTIFY_CHAR
 */
METAWEAR_API int32_t mbl_mw_connection_notify_char_changed(MblMwMetaWearBoard *board, const uint8_t *value, uint8_t len);
/**
 * Handles responses from reading gatt characteristics.  All characteristic values read must be forwaded to this function.
 * @param board             Board the response is from
 * @param characteristic    Characteristic that was read
 * @param value             Byte array containing the characteristic value
 * @param length            Length of the array
 */
METAWEAR_API void mbl_mw_connection_char_read(MblMwMetaWearBoard *board, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);

#ifdef __cplusplus
}
#endif
