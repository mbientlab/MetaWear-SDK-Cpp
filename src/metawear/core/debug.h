/**
 * @copyright MbientLab License 
 * @file debug.h
 * @brief Testing and debug functions, for advanced users only
 */
#pragma once

#include <stdint.h>

#include "data.h"
#include "metawearboard_fwd.h"
#include "datasignal_fwd.h"
#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Issues a soft reset
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_debug_reset(const MblMwMetaWearBoard *board);
/**
 * Restarts the board in bootloader mode
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_debug_jump_to_bootloader(const MblMwMetaWearBoard *board);
/**
 * Instructs the board to terminate the connection
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_debug_disconnect(const MblMwMetaWearBoard *board);
/**
 * Restarts the board after performing garbage collection
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_debug_reset_after_gc(const MblMwMetaWearBoard *board);
/**
 * Places the board in a powered down state after the next reset.  When in power save mode, press the switch 
 * or plug in the USB charger to wake the board up.
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_debug_enable_power_save(const MblMwMetaWearBoard *board);

/**
 * Enables/disables stack overflow assertion.  Function will do nothing if feature is unsupported.
 * @param board     Calling object
 * @param enable    0 to disable, non-zero value to enable
 */
METAWEAR_API void mbl_mw_debug_set_stack_overflow_assertion(const MblMwMetaWearBoard *board, uint8_t enable);
/**
 * Reads the current stack state.  If feature is unspported, nullptr will be passed to the `handler` parameter
 * @param board     Calling object
 * @param context   Pointer to additional data for the callback function
 * @param handler   Callback function for handling the received data
 */
METAWEAR_API void mbl_mw_debug_read_stack_overflow_state(const MblMwMetaWearBoard *board, void *context, MblMwFnData handler);

/**
 * Reads the internal queues' current usage statistics; data is returned as a byte array.  
 * If feature is unspported, nullptr will be passed to the `handler` parameter
 * @param board     Calling object
 * @param context   Pointer to additional data for the callback function
 * @param handler   Callback function for handling the received data
 */
METAWEAR_API void mbl_mw_debug_read_schedule_queue_usage(const MblMwMetaWearBoard *board, void *context, MblMwFnData handler);

/**
 * Creates a synthetic notification internally to the MetaWear system.  Useful for testing.
 * @param board     Calling object
 * @param value     Value to spoof: [Module ID, Register ID, Notifcation En, Optional Index, Data...]
 * @param lenght    Size of the value array
 */
METAWEAR_API void mbl_mw_debug_spoof_notification(const MblMwMetaWearBoard *board, const uint8_t *value, uint8_t length);

/**
 * Sends a raw command directly to the MetaWear.  Useful for testing.
 * @param board     Calling object
 * @param value     Value to send: [Module ID, Register ID, Optional Index, Data...]
 * @param lenght    Size of the value array
 */
METAWEAR_API void mbl_mw_debug_send_command(const MblMwMetaWearBoard *board, const uint8_t *value, uint8_t length);

/**
 * Retrieves a data signal representing the key register value.  This is a simple
 * 4 byte scratch register.
 * @param board     Board to receive data from
 */
METAWEAR_API MblMwDataSignal* mbl_mw_debug_get_key_register_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the key register value.  This is a simple 4 byte scratch register.
 * @param board     Board to receive data from
 */
METAWEAR_API void mbl_mw_debug_set_key_register(const MblMwMetaWearBoard *board, uint32_t value);

#ifdef	__cplusplus
}
#endif
