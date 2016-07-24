/**
 * @copyright MbientLab License 
 * @file debug.h
 * @brief Debug functions, for advanced use
 */
#pragma once

#include <stdint.h>
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

#ifdef	__cplusplus
}
#endif
