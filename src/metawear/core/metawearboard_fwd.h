/**
 * @copyright MbientLab License
 * @file metawearboard_fwd.h
 * @brief Forward declarations for the MblMwMetaWearBoard type 
 */

#pragma once

#include <stdint.h>

/**
 * Software representation of a physical MetaWear board
 */
#ifdef __cplusplus
struct MblMwMetaWearBoard; 
#else
typedef struct MblMwMetaWearBoard MblMwMetaWearBoard;
#endif

/**
 * Definition for callback functions that accept an MblMwMetaWearBoard pointer and an int32
 * @param context       Pointer to the context the enclosing function was called with
 * @param board         Board pointer to be used with the function
 * @param value         Additional value passed to the function for context specific callbacks
 */
typedef void(*MblMwFnBoardPtrInt)(void *context, MblMwMetaWearBoard* board, int32_t value);

/**
 * Definition for callback functions that accept an MblMwMetaWearBoard pointer
 * @param board         Board pointer to be used with the function
 */
typedef void(*MblMwFnBoardPtr)(const MblMwMetaWearBoard* board);
