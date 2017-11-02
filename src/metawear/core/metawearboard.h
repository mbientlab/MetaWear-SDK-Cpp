/**
 * @copyright MbientLab License
 * @file metawearboard.h
 * @brief Functions for communicating with an MblMwMetaWearBoard object
 */
#pragma once

#include <stdint.h>

#include "anonymous_datasignal_fwd.h"
#include "metawearboard_fwd.h"
#include "model.h"
#include "module.h"

#include "metawear/platform/btle_connection.h"
#include "metawear/platform/dllmarker.h"

/** Constant signifying a module is not available */
const int32_t MBL_MW_MODULE_TYPE_NA = -1;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @deprecated As of v0.5.0 and will be removed in v1.0.0.  Use mbl_mw_metawearboard_notify_char_changed instead.
 */
METAWEAR_API int32_t mbl_mw_connection_notify_char_changed(MblMwMetaWearBoard *board, const uint8_t *value, uint8_t len);
/**
 * @deprecated As of v0.9.0.  Handling characeristic changes now setup through the MblMwBtleConnection struct
 */
METAWEAR_API int32_t mbl_mw_metawearboard_notify_char_changed(MblMwMetaWearBoard *board, const uint8_t *value, uint8_t len);

/**
 * @deprecated As of v0.5.0 and will be removed in v1.0.0.  Use mbl_mw_metawearboard_char_read instead.
 */
METAWEAR_API void mbl_mw_connection_char_read(MblMwMetaWearBoard *board, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);
/**
 * @deprecated As of v0.9.0.  Handling gatt characteristic reads now setup through the MblMwBtleConnection struct
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
 * @param response_time_ms      How long to wait for a response, from [0, 4000]ms.  Use 0ms for indefinite timeout
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
 * Determines the board model of the currently connected device.  Only call this function after the board has been initialized.
 * @return Board model, MBL_MW_MODEL_NA if unable to determine
 */
METAWEAR_API MblMwModel mbl_mw_metawearboard_get_model(const MblMwMetaWearBoard* board);
/**
 * Determines the board model of the currently connected device.  Only call this function after the board has been initialized.
 * @return Friendly name representing the board model 
 */
METAWEAR_API const char* mbl_mw_metawearboard_get_model_name(const MblMwMetaWearBoard* board);

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
 * @return MBL_MW_STATUS_OK if successful, MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT if failed
 */
METAWEAR_API int32_t mbl_mw_metawearboard_deserialize(MblMwMetaWearBoard* board, uint8_t* state, uint32_t size);

/**
 * Reads the current state of the board and creates anonymous data signals based on what data is being logged,  If this task failed, a 
 * null pointer will be passed into the `anonymous_signals` parameter
 * @param board         Calling object
 * @param created       Callback function to be executed once the task is completed.
 */
METAWEAR_API void mbl_mw_metawearboard_create_anonymous_datasignals(MblMwMetaWearBoard* board, MblMwFnAnonSignalArray created);

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
 * Starts the DFU process and updloads the given file to the device
 * @param board         Calling object
 * @param delegate      Struct the function will forward DFU progress updates to
 * @param filename      Path to firmware bin file
 */
METAWEAR_API void mbl_mw_metawearboard_perform_dfu(MblMwMetaWearBoard *board, const MblMwDfuDelegate *delegate, const char *filename);

#ifdef __cplusplus
}
#endif
