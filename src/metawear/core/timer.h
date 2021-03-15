/**
 * @copyright MbientLab License
 * @file timer.h
 * @brief On board timer for scheduling MetaWear commands
 */
#pragma once

#include <stdint.h>

#include "metawearboard_fwd.h"
#include "timer_fwd.h"

#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Creates a timer that will run for a set number of repetitions.  
 * A pointer representing the timer will be passed to the user through a callback function
 * @param board             Board the timer belongs to
 * @param period            How often to 
 * @param repetitions       Number of events the timer will fire
 * @param delay             Zero if the tiemr should immediately fire, non-zero to delay the first event
 * @param context           Pointer to additional data for the callback function
 * @param received_timer    Callback function to be executed when the timer is created
 */
METAWEAR_API void mbl_mw_timer_create(MblMwMetaWearBoard *board, uint32_t period, uint16_t repetitions, uint8_t delay, void *context, MblMwFnTimerPtr received_timer);
/**
 * Creates a timer that will run indefinitely.  
 * A pointer representing the timer will be passed to the user through a callback function
 * @param board             Board the timer belongs to
 * @param period            How often to 
 * @param delay             Zero if the tiemr should immediately fire, non-zero to delay the first event
 * @param context           Pointer to additional data for the callback function
 * @param received_timer    Callback function to be executed when the timer is created
 */
METAWEAR_API void mbl_mw_timer_create_indefinite(MblMwMetaWearBoard *board, uint32_t period, uint8_t delay, void *context, MblMwFnTimerPtr received_timer);
/**
 * Retrieves the id value identifying the timer
 * @param timer             Timer to lookup
 * @return Numerical id of the timer
 */
METAWEAR_API uint8_t mbl_mw_timer_get_id(const MblMwTimer* timer);
/**
 * Looks up the MblMwTimer object corresponding to the id
 * @param board             Board to search on
 * @param id                Numerical id to lookup
 * @return Timer object identified by the id, null if no object is found
 */
METAWEAR_API MblMwTimer* mbl_mw_timer_lookup_id(const MblMwMetaWearBoard* board, uint8_t id);
/**
 * Starts a timer
 * @param timer     Timer to start
 */
METAWEAR_API void mbl_mw_timer_start(const MblMwTimer* timer);
/**
 * Stops a timer
 * @param timer     Timer to stop
 */
METAWEAR_API void mbl_mw_timer_stop(const MblMwTimer* timer);
/**
 * Removes the timer from the board
 * @param timer     Timer to remove
 */
METAWEAR_API void mbl_mw_timer_remove(MblMwTimer* timer);

#ifdef	__cplusplus
}
#endif
