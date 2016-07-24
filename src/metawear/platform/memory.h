/**
 * @copyright MbientLab License
 * @file memory.h
 * @brief Memory management for memory allocated by the API
 */
#pragma once

#include "dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Frees allocated memory
 * @param ptr   Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_memory_free(void* ptr);

#ifdef	__cplusplus
}
#endif
