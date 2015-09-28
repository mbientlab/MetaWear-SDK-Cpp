/**
 * @copyright MbientLab License 
 * @file debug.h
 * @brief Debug functions, for advanced use
 */
#pragma once

#include <stdint.h>
#include "dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Constructs the command that triggers a soft reset
 * @param   command     Byte array (len 2) for the function to write the command to
 */
METAWEAR_API void mbl_mw_debug_reset(uint8_t command[2]);
/**
 * Constructs the command that restarts the board in bootloader mode
 * @param   command     Byte array (len 2) for the function to write the command to
 */
METAWEAR_API void mbl_mw_debug_jump_to_bootloader(uint8_t command[2]);
/**
 * Constructs the command that has the board terminate the connection
 * @param   command     Byte array (len 2) for the function to write the command to
 */
METAWEAR_API void mbl_mw_debug_disconnect(uint8_t command[2]);
/**
 * Constructs the command that restarts the board after performing garbage collection
 * @param   command     Byte array (len 2) for the function to write the command to
 */
METAWEAR_API void mbl_mw_debug_reset_after_gc(uint8_t command[2]);

#ifdef	__cplusplus
}
#endif
