/**
 * @copyright MbientLab License
 * @file logging.h
 * @brief Controls the onboard logging system
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "metawearboard_fwd.h"

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

#ifdef	__cplusplus
}
#endif
