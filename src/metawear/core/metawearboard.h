/**
 * @copyright MbientLab License
 * @file metawearboard.h
 */
#pragma once

#include <stdint.h>

#include "callback.h"
#include "connection.h"
#include "dllmarker.h"
#include "metawearboard_fwd.h"
#include "module.h"

const int32_t MBL_MW_MODULE_TYPE_NA = -1;           ///< Constant signifying a module is not available

#ifdef __cplusplus
extern "C" {
#endif

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
 * Initialize the board.  This function is non-blocking and will alert the caller when the operation is complete.
 * @param board         Board to initialize
 * @param initialized   Callback function to be executed when the board is initialized
 */
METAWEAR_API void mbl_mw_metawearboard_initialize(MblMwMetaWearBoard *board, MblMwFnVoid initialized);
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
METAWEAR_API int32_t mbl_mw_metawearboard_lookup_module(const MblMwMetaWearBoard *board, uint8_t module);

#ifdef __cplusplus
}
#endif
