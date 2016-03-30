/**
 * @copyright MbientLab License 
 * @file datasignal.h
 * @brief Functions for controlling a MblMwDataSignal
 */
#pragma once

#include "callback.h"
#include "data.h"
#include "datasignal_fwd.h"
#include "dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Subscribes to a data stream, processing messages with the given handler
 * @param signal                Data signal to subscribe to
 * @param received_data         Callback function to handle data received from the signal
 */
METAWEAR_API void mbl_mw_datasignal_subscribe(MblMwDataSignal *signal, MblMwFnData received_data);
/**
 * Unsubscribes from a data stream
 * @param signal    Data signal to unsubscribe from
 */
METAWEAR_API void mbl_mw_datasignal_unsubscribe(MblMwDataSignal *signal);

/**
 * Logs the data signal, processing messages with the given handler
 * @param signal                Data signal to log
 * @param received_data         Callback function to handle the logged data
 * @param logger_ready          Callback function to be executed when the signal logger is ready
 */
METAWEAR_API void mbl_mw_datasignal_log(MblMwDataSignal *signal, MblMwFnData received_data, MblMwFnVoid logger_ready);
/**
 * Removes the logger attached to the signal
 * @param signal                Data signal to stop logging
 */
METAWEAR_API void mbl_mw_datasignal_remove_logger(MblMwDataSignal *signal);

#ifdef	__cplusplus
}
#endif
