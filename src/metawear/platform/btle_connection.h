/**
 * @copyright MbientLab License 
 * @file btle_connection.h
 * @brief Platform agnostic wrappers for Bluetooth Low Energy communication
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Write types for the GATT characteristic
 */
typedef enum {
    MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE = 0,
    MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE
} MblMwGattCharWriteType;

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
 * Definition for callback functions that accept a void pointer and byte array
 * @param caller        Object the callback is designated for
 * @param start         Pointer to the beginning of the byte array
 * @param length        Length of the byte array
 */
typedef int32_t(*MblMwFnIntVoidPtrArray)(const void* caller, const uint8_t* start, uint8_t length);
/**
 * Definition for callback functions that accept a void pointer and an int32
 * @param caller        Object the callback is designated for
 * @param value         Additional value passed to the function for context specific callbacks
 */
typedef void(*MblMwFnVoidVoidPtrInt)(const void* caller, int32_t value);

/**
 * Wrapper class containing functions for communicating with the MetaWear through a Bluetooth Low Energy connection.
 */
typedef struct {
    /**
     * Provides the calling function the ability to pass any context specific data required
     */
    void *context;
    /** 
     * Writes the characteristic and value to the device
     * @param context           Pointer to the <code>context</code> field
     * @param caller            Object using this function pointer
     * @param characteristic    Gatt characteristic to write
     * @param value             Value to write as a byte array
     * @param length            Length of the byte array
     */
    void (*write_gatt_char)(void *context, const void* caller, MblMwGattCharWriteType writeType, const MblMwGattChar* characteristic, 
            const uint8_t* value, uint8_t length);
    /**
     * Reads the value of the characteristic from the device
     * @param context               Pointer to the <code>context</code> field
     * @param caller                Object using this function pointer
     * @param characteristic        Gatt characteristic to read
     * @param handler               Callback function to handle the received value
     */
    void (*read_gatt_char)(void *context, const void* caller, const MblMwGattChar* characteristic, MblMwFnIntVoidPtrArray handler);
    /**
     * Enables notifications for characeristic changes
     * @param context               Pointer to the <code>context</code> field
     * @param caller                Object using this function pointer
     * @param characteristic        Characteristic to enable notifications for
     * @param handler               Callback function for handling characteristic notifications
     * @param ready                 Callback function to handle when the enable notify task is completed
     */
    void (*enable_notifications)(void *context, const void* caller, const MblMwGattChar* characteristic, MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready);
    /**
     * Register a handler for disconnect events
     * @param context               Pointer to the <code>context</code> field
     * @param caller                Object using this function pointer
     * @param handler               Handler to respond to the disconnect event
     */
    void (*on_disconnect)(void *context, const void* caller, MblMwFnVoidVoidPtrInt handler);
} MblMwBtleConnection;

#ifdef __cplusplus
}
#endif
