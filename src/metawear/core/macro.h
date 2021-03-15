/**
 * @copyright MbientLab License
 * @file macro.h
 * @brief Firmware feature that saves MetaWear commands to the on-board flash memory
 */
#pragma once

#include "metawearboard_fwd.h"
#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Begin macro recording.  
 * Used to command the board on boot. Commands will survive a reset.
 * For example, on boot, flash the LED red.
 * For example, when the user drops the metawear (freefall event), vibrate the coin motor.
 * For example, renaming the device permanently.
 * Every MetaWear command issued will be recorded to the flash memory.
 * @param board             Calling object
 * @param exec_on_boot      True if the commands should be executed when the board powers on
 */
METAWEAR_API void mbl_mw_macro_record(MblMwMetaWearBoard *board, uint8_t exec_on_boot);
/**
 * Ends macro recording.  
 * An numerical id representing the macro will be passed to the callback function when the operation is complete.
 * @param board                 Calling object
 * @param context               Pointer to additional data for the callback function
 * @param commands_recorded     Callback function to be executed when the commands are recorded
 */
METAWEAR_API void mbl_mw_macro_end_record(MblMwMetaWearBoard *board, void *context, MblMwFnBoardPtrInt commands_recorded);
/**
 * Execute the commands corresponding to the macro ID
 * @param board     Calling object
 * @param id        Numerical ID of the macro to execute
 */
METAWEAR_API void mbl_mw_macro_execute(MblMwMetaWearBoard *board, uint8_t id);
/**
 * Remove all macros on the flash memory.  
 * The erase operation will not be performed until you disconnect from the board.  
 * If you wish to reset the board after the erase operation, use the mbl_mw_debug_reset_after_gc method.
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_macro_erase_all(MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
