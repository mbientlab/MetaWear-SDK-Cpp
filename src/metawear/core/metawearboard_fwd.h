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
 * @param board         Board pointer to be used with the function
 * @param status        Status code passed into the function
 */
typedef void(*MblMwFnBoardPtrInt)(MblMwMetaWearBoard* board, int32_t status);
