/**
 * @copyright MbientLab License
 * @file event.h
 * @brief Functions for the MblMwEvent type
 */

#pragma once

#include "event_fwd.h"
#include "metawearboard_fwd.h"

#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Retrieves the MblMwMetaWearBoard the event belongs to
 * @param event     Event to lookup
 * @return Pointer to the owner
 */
METAWEAR_API MblMwMetaWearBoard* mbl_mw_event_get_owner(const MblMwEvent *event);
/**
 * Enables command recording.  
 * Commands can be used to execute functions on the sensor as a result of an event.
 * For example, when the time counts to 30 (the event), take a temperature reading (the command).
 * All MetaWear commands called after this point will be executed when the owning event is fired
 * @param event     Event to record commands for
 */
METAWEAR_API void mbl_mw_event_record_commands(MblMwEvent *event);
/**
 * Ends command recording.  
 * This function is non-blocking and will asynchronously alert the caller when the operation is completed.
 * @param event                 Event to end recording for
 * @param context               Pointer to additional data for the callback function
 * @param commands_recorded     Callback function to be executed when commands have been recorded
 */
METAWEAR_API void mbl_mw_event_end_record(MblMwEvent *event, void *context, MblMwFnEventPtrInt commands_recorded);
/**
 * Remove all recorded events from the board.
 * @param board                 Calling object
 */
METAWEAR_API void mbl_mw_event_remove_all(MblMwMetaWearBoard* board);

#ifdef	__cplusplus
}
#endif
