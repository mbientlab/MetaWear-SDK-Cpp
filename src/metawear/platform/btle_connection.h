/**
 * @copyright MbientLab License 
 * @file btle_connection.h
 * @brief Platform agnostic wrappers for Bluetooth Low Energy communication
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"

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

/**
 * Wrapper class containing functions for communicating with the MetaWear through a Bluetooth Low Energy connection.
 */
typedef struct {
    /** 
     * Writes the characteristic and value to the device
     * @param caller            Object using this function pointer
     * @param characteristic    Gatt characteristic to write
     * @param value             Value to write as a byte array
     * @param length            Length of the byte array
     */
    void (*write_gatt_char)(const void* caller, const MblMwGattChar* characteristic, const uint8_t* value, uint8_t length);
    /**
     * Reads the value of the characteristic from the device
     * @param caller                Object using this function pointer
     * @param characteristic        Gatt characteristic to read
     */
    void (*read_gatt_char)(const void* caller, const MblMwGattChar* characteristic);
} MblMwBtleConnection;

#ifdef __cplusplus
}
#endif
