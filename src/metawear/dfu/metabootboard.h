/**
 * @copyright MbientLab License
 * @file metabootboard.h
 */
#pragma once

#include <stdint.h>

#include "metabootboard_fwd.h"

#include "metawear/platform/btle_connection.h"

#ifdef __cplusplus
extern "C" {
#endif

/** UUIDs for the MetaWear DFU characteristic */
const MblMwGattChar DFU_PACKET_CHAR = { 0x000015301212EFDE, 0x1523785FEABCD123, 0x000015321212EFDE, 0x1523785FEABCD123 };
const MblMwGattChar DFU_CONTROL_POINT_CHAR = { 0x000015301212EFDE, 0x1523785FEABCD123, 0x000015311212EFDE, 0x1523785FEABCD123 };

/**
 * Wrapper class containing functions for receiving callbacks throughout the DFU process
 */
typedef struct {
    void (*on_dfu_started)();
    void (*on_dfu_cancelled)();
    void (*on_transfer_percentage)(int32_t percentage);
    void (*on_successful_file_transferred)();
    void (*on_error)(const char *errorMessage);
} MblMwDfuDelegate;

/**
 * Creates an instance of the MblMwMetaWearBoard struct
 * @param connection    Connection struct the new MblMwMetaWearBoard variable will use for btle communication
 * @return Pointer to the newly created struct
 */
METAWEAR_API MblMwMetaBootBoard* mbl_mw_metabootboard_create(const MblMwBtleConnection *connection, const MblMwDfuDelegate *delegate);
/**
 * Frees the memory allocated for the struct
 * @param board     Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_metabootboard_free(MblMwMetaBootBoard *board);

/**
 * Starts the DFU process and updloads the given file to the device
 * @param filename     path to firmware bin file
 */
METAWEAR_API void mbl_mw_metabootboard_perform_dfu(MblMwMetaBootBoard *board, const char *filename);

/**
 * Handles changes from the MetaWear notify characteristic.  All characteristic changes from the notify characteristic must be forwarded
 * to this function
 * @param board         Board the characteristic change is from
 * @param value         Byte array containing the new characteristic value
 * @param len           Length of the array
 * @see METAWEAR_SERVICE_NOTIFY_CHAR
 */
METAWEAR_API void mbl_mw_dfu_control_point_char_changed(MblMwMetaBootBoard *board, const uint8_t *value, uint8_t len);

#ifdef __cplusplus
}
#endif
