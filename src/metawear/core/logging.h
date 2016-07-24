/**
 * @copyright MbientLab License
 * @file logging.h
 * @brief Controls the onboard logging system
 */
#pragma once

#include <stdint.h>

#include "data.h"
#include "datasignal_fwd.h"
#include "logging_fwd.h"
#include "metawearboard_fwd.h"

#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Callback functions for handling notifications from the logger
 */
typedef struct {
    /**
     * Called when a progress update is received
     * @param entries_left          Number of entries left to download
     * @param total_entries         Total number of entries
     */
    void (*received_progress_update)(uint32_t entries_left, uint32_t total_entries);
    /**
     * Called when a log entry has been received but cannot be matched to a log handler
     * @param id                    Id of the received log entry
     * @param epoch                 Number of milliseconds since epoch
     * @param data                  Byte array holding the data
     * @param length                Length of the array
     */
    void (*received_unknown_entry)(uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length);
    /**
     * Called when a log entry has been received but has no MblMwFnData pointer to forward the data to
     */
    MblMwFnData received_unhandled_entry;
} MblMwLogDownloadHandler;

/**
 * Starts data logging
 * @param board                 Board to log data on
 * @param overwrite             Non-zero if older entries will be overwritten
 */
METAWEAR_API void mbl_mw_logging_start(const MblMwMetaWearBoard* board, uint8_t overwrite);
/**
 * Stops data logging
 * @param board                 Board to stop logging
 */
METAWEAR_API void mbl_mw_logging_stop(const MblMwMetaWearBoard* board);
/**
 * Clear the logger of saved entries
 * @param board                 Board to remove entries from
 */
METAWEAR_API void mbl_mw_logging_clear_entries(const MblMwMetaWearBoard* board);
/**
 * Downloads the log data
 * @param board                     Board to download the log data from
 * @param n_notifies                How many progress updates to send
 * @param handler                   Handler for processing logger responses
 */
METAWEAR_API void mbl_mw_logging_download(MblMwMetaWearBoard* board, uint8_t n_notifies, const MblMwLogDownloadHandler* handler);

/**
 * Retrieves the id value identifying the logger
 * @param logger            Logger to lookup
 * @return Numerical id of the logger
 */
METAWEAR_API uint8_t mbl_mw_logger_get_id(const MblMwDataLogger* logger);
/**
 * Retrieves the data signal the logger is recording data for
 * @param logger            Logger to lookup
 * @return Pointer to owning MblMwDataSignal object
 */
METAWEAR_API MblMwDataSignal* mbl_mw_logger_get_signal(const MblMwDataLogger* logger);
/**
 * Looks up the MblMwDataLogger object corresponding to the id
 * @param board             Board to search on
 * @param id                Numerical id to lookup
 * @return Logger object identified by the id, null if no object is found
 */
METAWEAR_API MblMwDataLogger* mbl_mw_logger_lookup_id(const MblMwMetaWearBoard* board, uint8_t id);
/**
 * Removes the logger from the board
 * @param logger                Logger to remove
 */
METAWEAR_API void mbl_mw_logger_remove(MblMwDataLogger* logger);
/**
 * Subscribes to responses from the data logger
 * @param logger                Logger to subscribe to
 * @param received_data         Callback function to handle data received from the logger
 */
METAWEAR_API void mbl_mw_logger_subscribe(MblMwDataLogger* logger, MblMwFnData received_data);

#ifdef	__cplusplus
}
#endif
