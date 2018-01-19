/**
 * @copyright MbientLab License
 * @file anonymous_datasignal_fwd.h
 * @brief Forward declaration for the MblMwAnonymousDataSignal type
 */
#pragma once

#include "metawearboard_fwd.h"

/**
 * A variant of an MblMwDataLogger object, used to retrieve logged data from a board 
 * that was not programmed by the current host device
 */
#ifdef	__cplusplus
struct MblMwAnonymousDataSignal;
#else
typedef struct MblMwAnonymousDataSignal MblMwAnonymousDataSignal;
#endif

/**
 * Definition for callback functions that accept an MblMwMetaWearBoard pointer, MblMwAnonymousDataSignal double pointers, and an uint32 value
 * @param context                   Pointer to the context the enclosing function was called with
 * @param board                     Board pointer to be used with the function
 * @param anonymous_signals         Array of MblMwAnonymousDataSignal pointers
 * @param size                      Number of elements in the array
 */
typedef void(*MblMwFnAnonSignalArray)(void *context, MblMwMetaWearBoard* board, MblMwAnonymousDataSignal** anonymous_signals, uint32_t size);