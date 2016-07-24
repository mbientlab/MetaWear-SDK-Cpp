/**
 * @copyright MbientLab License 
 * @file datasignal.h
 * @brief Functions for controlling a MblMwDataSignal
 */
#pragma once

#include "data.h"
#include "datasignal_fwd.h"
#include "logging_fwd.h"

#include "metawear/platform/dllmarker.h"

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
 * Check if the data signal can be explicitly read
 * @param signal    Data signal to check
 * @return Zero if not readable, non-zero if it is
 */
METAWEAR_API int32_t mbl_mw_datasignal_is_readable(const MblMwDataSignal* signal);

/**
 * Reads data from sensor represented by the data signal.  Data is forwarded to the callback function 
 * assigned by the mbl_mw_datasignal_subscribe function
 * @param signal    Data signal to read from
 */
METAWEAR_API void mbl_mw_datasignal_read(const MblMwDataSignal* signal);

/**
 * Reads data from sensor represented by the data signal.  Data is forwarded to the callback function 
 * assigned by the mbl_mw_datasignal_subscribe function.  This variant is for reads that require additional parameters.  
 * @param signal        Data signal to read from
 * @param parameters    Additional parameters required for the read operation
 */
METAWEAR_API void mbl_mw_datasignal_read_with_parameters(const MblMwDataSignal* signal, const void* parameters);

/**
 * Creates an MblMwDataLogger for the signal
 * @param signal                Data signal to log
 * @param logger_ready          Callback function to be executed when the logger is created
 */
METAWEAR_API void mbl_mw_datasignal_log(MblMwDataSignal *signal, MblMwFnDataLoggerPtr logger_ready);

#ifdef	__cplusplus
}
#endif
