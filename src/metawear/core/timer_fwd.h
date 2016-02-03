/**
 * @copyright MbientLab License
 * @file timer_fwd.h
 * @brief Forward declaration of the MblMwTimer type
 */
#pragma once

#ifdef __cplusplus
struct MblMwTimer; 
#else
/**
 * On board timer that periodically fires events.  An MblMwTimer pointer can be casted as an MblMwEvent pointer 
 * and used with any function that accepts an MblMwEvent pointer.
 */
typedef struct MblMwTimer MblMwTimer;
#endif

/**
 * Definition for callback functions that accept an MblMwTimer pointer
 * @param timer         Timer to be used with the function
 */
typedef void (*MblMwTimerPtr)(MblMwTimer* timer);
