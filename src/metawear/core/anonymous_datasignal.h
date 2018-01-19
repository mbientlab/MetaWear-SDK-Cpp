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
 * @param signal                Calling object
 * @param context               Pointer to additional data for the callback function
 * @param data_handler          Callback function to handle data received from the signal
 */
METAWEAR_API void mbl_mw_anonymous_datasignal_subscribe(MblMwAnonymousDataSignal* signal, void *context, MblMwFnData data_handler);
/**
 * Generates a string identifying the data chain the anonymous data signal is receiving data from.
 * @return String identifying the data chain
 */
METAWEAR_API const char* mbl_mw_anonymous_datasignal_get_identifier(const MblMwAnonymousDataSignal* signal);

#ifdef __cplusplus
}
#endif