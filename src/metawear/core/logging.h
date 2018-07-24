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
     * Pointer to additional data for the callback functions
     */
    void *context;
    /**
     * Called when a progress update is received
     * @param context               Pointer to the <code>context</code> field
     * @param entries_left          Number of entries left to download
     * @param total_entries         Total number of entries
     */
    void (*received_progress_update)(void *context, uint32_t entries_left, uint32_t total_entries);
    /**
     * Called when a log entry has been received but cannot be matched to a log handler
     * @param context               Pointer to the <code>context</code> field
     * @param id                    Id of the received log entry
     * @param epoch                 Number of milliseconds since epoch
     * @param data                  Byte array holding the data
     * @param length                Length of the array
     */
    void (*received_unknown_entry)(void *context, uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length);
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
 * @param logger                logger to remove
 */
METAWEAR_API void mbl_mw_logger_remove(MblMwDataLogger* logger);
/**
 * Subscribes to responses from the data logger
 * @param logger                Logger to subscribe to
 * @param context               Pointer to additional data for the callback function
 * @param received_data         Callback function to handle data received from the logger
 */
METAWEAR_API void mbl_mw_logger_subscribe(MblMwDataLogger* logger, void *context, MblMwFnData received_data);
/**
 * Generates a string identifying the data chain the logger is receiving data from.  This string is matched with the 
 * output of mbl_mw_anonymous_datasignal_get_identifier.
 * The memory allocated by the function must be freed by calling mbl_mw_memory_free.
 * @param logger                Calling object
 */
METAWEAR_API const char* mbl_mw_logger_generate_identifier(const MblMwDataLogger* logger);
/**
 * Retrieves a data signal representing the length of the log, including timestamps
 * @param board                 Board to get reset_uid from
 */
METAWEAR_API MblMwDataSignal* mbl_mw_logging_get_length_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves a data signal representing the current logger time state.  This includes the
 * reset_uid and time of boot.
 * @param board                 Board to get time from
 */
METAWEAR_API MblMwDataSignal* mbl_mw_logging_get_time_data_signal(const MblMwMetaWearBoard *board);
/**
 * Get the latest reset_uid read duing connection
 * @param board                 Board to get reset_uid from
 * @return value of reset_uid
 */
METAWEAR_API uint8_t mbl_mw_logging_get_latest_reset_uid(const MblMwMetaWearBoard* board);
/**
 * Get the device boot time for a given reset_uid.  This reference time
 * is automatically calulated at connection time.
 * @param board                 Board to use
 * @param reset_uid             Reset id
 * @return Number of milliseconds since epoch that the given reset_uid occured
 */
METAWEAR_API int64_t mbl_mw_logging_get_reference_time(const MblMwMetaWearBoard *board, uint8_t reset_uid);
/**
 * Set the device boot time for a given reset_uid.  This reference time
 * is used to calcuated real timestamps from logged data.
 * @param board                 Board to use
 * @param reset_uid             Reset id
 * @param reference_epoch       New reference epoch (in milliseconds) to use
 */
METAWEAR_API void mbl_mw_logging_set_reference_time(const MblMwMetaWearBoard *board, uint8_t reset_uid, int64_t reference_epoch);


/**
 * Callback functions for handling notifications from the logger
 */
typedef struct {
    /**
     * Pointer to additional data for the callback functions
     */
    void *context;
    /**
     * Called when a log entry is received
     * @param context               Pointer to the <code>context</code> field
     * @param entry_id              Logger id of the entry
     * @param reset_uid             Reset id when this entry was logged
     * @param entry_tick            Raw time when this entry was logged
     * @param data                  Raw data
     */
    void (*received_entry)(void *context, uint8_t entry_id, uint8_t reset_uid, uint32_t entry_tick, uint32_t data);
    /**
     * Called when a progress update is received
     * @param context               Pointer to the <code>context</code> field
     * @param entries_left          Number of entries left to download
     * @param total_entries         Total number of entries
     */
    void (*received_progress_update)(void *context, uint32_t entries_left, uint32_t total_entries);
    /**
     * Called when a log page is complete, use the ready function to indicate when data has been saved
     * and you're ready to receive the next page
     * @param context               Pointer to the <code>context</code> field
     * @param caller                Object using this function pointer
     * @param ready                 Callback function to handle when ready for the next page
     */
    void (*logging_page_completed)(void *context, const MblMwMetaWearBoard* caller, MblMwFnBoardPtr ready);
} MblMwRawLogDownloadHandler;

/**
 * Downloads the raw log data
 * @param board                     Board to download the log data from
 * @param n_notifies                How many progress updates to send
 * @param handler                   Handler for processing logger responses
 */
METAWEAR_API void mbl_mw_logging_raw_download(MblMwMetaWearBoard* board, uint8_t n_notifies, const MblMwRawLogDownloadHandler* handler);

    
#ifdef	__cplusplus
}
#endif
