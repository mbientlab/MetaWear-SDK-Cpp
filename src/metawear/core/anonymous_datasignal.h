/**
 * @copyright MbientLab License
 * @file anonymous_datasignal.h
 * @brief Functions operating on the MblMwAnonymousDataSignal object
 */
#pragma once

#include "metawear/platform/dllmarker.h"

#include "anonymous_datasignal_fwd.h"
#include "data.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Subscribe to the data produced by the signal
 * @param signal            Calling object
 * @param data_handler      Callback function to handle data received from the signal
 */
METAWEAR_API void mbl_mw_anonymous_datasignal_subscribe(MblMwAnonymousDataSignal* signal, MblMwFnData data_handler);
/**
 * Generates a string identifying the data chain the anonymous data signal is receiving data from.  
 * The memory allocated by the function must be freed by calling mbl_mw_memory_free.
 */
METAWEAR_API char* mbl_mw_anonymous_datasignal_get_identifier(const MblMwAnonymousDataSignal* signal);

#ifdef __cplusplus
}
#endif