/**
 * @copyright MbientLab License
 * @file event_fwd.h
 * @brief Forward declaration of the MblMwEvent type
 */
#pragma once

#include <stdint.h>

/**
 * Represents an event fired from the MetaWear board.
 */
#ifdef	__cplusplus
struct MblMwEvent;
#else
typedef struct MblMwEvent MblMwEvent;
#endif

/**
 * Definition for callback functions that accept an MblMwEvent pointer and an int32
 * @param context       Pointer to the context the enclosing function was called with
 * @param event         Event to be used with the function
 * @param status        Status code passed into the function
 */
typedef void(*MblMwFnEventPtrInt)(void *context, MblMwEvent* event, int32_t status);
