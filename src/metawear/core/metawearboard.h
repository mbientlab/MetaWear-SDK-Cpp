/**
 * @copyright MbientLab License
 * @file metawearboard.h
 * @brief Functions for communicating with an MblMwMetaWearBoard object
 */
#pragma once

#include <stdint.h>

#include "metawearboard_fwd.h"
#include "module.h"

#include "metawear/platform/btle_connection.h"
#include "metawear/platform/dllmarker.h"

/** Constant signifying a module is not available */
const int32_t MBL_MW_MODULE_TYPE_NA = -1;

/** UUIDs for the MetaWear notification characteristic */
const MblMwGattChar METAWEAR_SERVICE_NOTIFY_CHAR = { 0x326a900085cb9195, 0xd9dd464cfbbae75a, 0x326a900685cb9195, 0xd9dd464cfbbae75a };

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @deprecated As of v0.5.0 and will be removed in v1.0.0.  Use mbl_mw_metawearboard_notify_char_changed instead.
 */
METAWEAR_API int32_t mbl_mw_connection_notify_char_changed(MblMwMetaWearBoard *board, const uint8_t *value, uint8_t len);
/**
 * Handles changes from the MetaWear notify characteristic.  All characteristic changes from the notify characteristic must be forwarded
 * to this function
 * @param board         Board the characteristic change is from
 * @param value         Byte array containing the new characteristic value
 * @param len           Length of the array
 * @see METAWEAR_SERVICE_NOTIFY_CHAR
 */
METAWEAR_API int32_t mbl_mw_metawearboard_notify_char_changed(MblMwMetaWearBoard *board, const uint8_t *value, uint8_t len);

/**
 * @deprecated As of v0.5.0 and will be removed in v1.0.0.  Use mbl_mw_metawearboard_char_read instead.
 */
METAWEAR_API void mbl_mw_connection_char_read(MblMwMetaWearBoard *board, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);
/**
 * Handles responses from a gatt characteristic read issued by a MblMwMetaWearBoard object.  All characteristic values read must be forwarded to this function.
 * @param board             Board the response is from
 * @param characteristic    Characteristic that was read
 * @param value             Byte array containing the characteristic value
 * @param length            Length of the array
 */
METAWEAR_API void mbl_mw_metawearboard_char_read(MblMwMetaWearBoard *board, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);

/**
 * Creates an instance of the MblMwMetaWearBoard struct
 * @param connection    Connection struct the new MblMwMetaWearBoard variable will use for btle communication
 * @return Pointer to the newly created struct
 */
METAWEAR_API MblMwMetaWearBoard* mbl_mw_metawearboard_create(const MblMwBtleConnection *connection);
/**
 * Frees the memory allocated for the struct
 * @param board     Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_metawearboard_free(MblMwMetaWearBoard *board);
/**
 * Sets how long the API should wait before a required response is received.  You should increase this value if operations such as 
 * API initialization, creating timer, loggers, and data processors, and recording commands consistently time out.
 * @param board                 Board to configure
 * @param response_time_ms      How long to wait, in milliseconds, capped at 4000ms (4s)
 */
METAWEAR_API void mbl_mw_metawearboard_set_time_for_response(MblMwMetaWearBoard* board, uint16_t response_time_ms);
/**
 * Initialize the API's internal state.  This function is non-blocking and will alert the caller when the operation is complete.
 * @param board         Board to initialize
 * @param initialized   Callback function to be executed when the board is initialized
 */
METAWEAR_API void mbl_mw_metawearboard_initialize(MblMwMetaWearBoard *board, MblMwFnBoardPtrInt initialized);
/**
 * Removes all data processors and timers from the MetaWear board
 * @param board         Board to tear down
 */
METAWEAR_API void mbl_mw_metawearboard_tear_down(MblMwMetaWearBoard *board);
/**
 * Checks if the board is initialized
 * @param board     Board to check
 * @return Zero if not initialized, non-zero if it is
 */
METAWEAR_API int32_t mbl_mw_metawearboard_is_initialized(const MblMwMetaWearBoard *board);

/**
 * Checks module type i.e. what kind of accelerometer is being used  
 * @param board         Board to check
 * @param module        Module to lookup
 * @return Module type used by the board, MBL_MW_MODULE_TYPE_NA if module is not available
 * @see MBL_MW_MODULE_ACC_TYPE_MMA8452Q
 * @see MBL_MW_MODULE_ACC_TYPE_BMI160
 */
METAWEAR_API int32_t mbl_mw_metawearboard_lookup_module(const MblMwMetaWearBoard *board, MblMwModule module);

/**
* Serializes the API state.  The memory allocated by the function must be freed by calling mbl_mw_memory_free.
* @param board         Board to serialize
* @param size          Pointer to where the size of the returned byte array will be written to
* @return Byte array of the serialized state
*/
METAWEAR_API uint8_t* mbl_mw_metawearboard_serialize(const MblMwMetaWearBoard* board, uint32_t* size);
/**
* Deserializes API state.  This function must be executed before calling mbl_mw_metawearboard_initialize.
* @param board         Board to deserialize
* @param state         Byte array holding the the information state
* @param size          Byte array size
*/
METAWEAR_API void mbl_mw_metawearboard_deserialize(MblMwMetaWearBoard* board, uint8_t* state, uint32_t size);
#ifdef __cplusplus
}
#endif
