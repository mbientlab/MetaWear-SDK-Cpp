/**
 * @copyright MbientLab License
 * @file timer_fwd.h
 * @brief Forward declaration of the MblMwTimer type
 */
#pragma once

/**
 * On board timer that periodically fires events.  An MblMwTimer pointer can be casted as an MblMwEvent pointer 
 * and used with any function that accepts an MblMwEvent pointer.
 */
#ifdef __cplusplus
struct MblMwTimer; 
#else
typedef struct MblMwTimer MblMwTimer;
#endif

/**
 * Definition for callback functions that accept an MblMwTimer pointer
 * @param context       Pointer to the context the enclosing function was called with
 * @param timer         Timer to be used with the function
 */
typedef void (*MblMwFnTimerPtr)(void *context, MblMwTimer* timer);
