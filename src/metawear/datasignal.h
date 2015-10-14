/**
 * @copyright MbientLab License 
 * @file datasignal.h
 * @brief Generic functions for handling sensor data
 */
#pragma once

#include "datasignal_fwd.h"
#include "dllmarker.h"
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Subscribes to a data stream, processing messages with the given handler
 * @param signal    Data signal to subscribe to
 */
METAWEAR_API void mbl_mw_datasignal_subscribe(MblMwDataSignal *signal);

/**
 * Unsubscribes from a data stream
 * @param signal    Data signal to unsubscribe from
 */
METAWEAR_API void mbl_mw_datasignal_unsubscribe(MblMwDataSignal *signal);

#ifdef	__cplusplus
}
#endif
