/**
 * @copyright MbientLab License 
 * @file debug.h
 * @brief Testing and debug functions, for advanced users only
 */
#pragma once

#include <stdint.h>

#include "data.h"
#include "metawearboard_fwd.h"
#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Issues a soft reset
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_debug_reset(const MblMwMetaWearBoard *board);
/**
 * Restarts the board in bootloader mode
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_debug_jump_to_bootloader(const MblMwMetaWearBoard *board);
/**
 * Instructs the board to terminate the connection
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_debug_disconnect(const MblMwMetaWearBoard *board);
/**
 * Restarts the board after performing garbage collection
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_debug_reset_after_gc(const MblMwMetaWearBoard *board);

/**
 * Enables/disables stack overflow assertion.  Function will do nothing if feature is unsupported.
 * @param board     Calling object
 * @param enable    0 to disable, non-zero value to enable
 */
METAWEAR_API void mbl_mw_debug_set_stack_overflow_assertion(const MblMwMetaWearBoard *board, uint8_t enable);
/**
 * Reads the current stack state.  If feature is unspported, nullptr will be passed to the `handler` parameter
 * @param board     Calling object
 * @param handler   Callback function for handling the received data
 */
METAWEAR_API void mbl_mw_debug_read_stack_overflow_state(const MblMwMetaWearBoard *board, MblMwFnData handler);

/**
 * Reads the internal queues' current usage statistics; data is returned as a byte array.  
 * If feature is unspported, nullptr will be passed to the `handler` parameter
 * @param board     Calling object
 * @param handler   Callback function for handling the received data
 */
METAWEAR_API void mbl_mw_debug_read_schedule_queue_usage(const MblMwMetaWearBoard *board, MblMwFnData handler);

#ifdef	__cplusplus
}
#endif
