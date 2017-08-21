.. highlight:: cpp

Logging
=======
Logging functions in the `logging.h <https://mbientlab.com/docs/metawear/cpp/latest/logging_8h.html>`_ header file control the on-board logger.  These 
functions go hand in hand with the data signal logger outlined in the :doc:`mblmwdatasignal` section.  After you have setup the signal loggers, start 
the logger by calling `mbl_mw_logging_start <https://mbientlab.com/docs/metawear/cpp/latest/logging_8h.html#acab2d6b1c4f5449a39fe3bf60205471f>`_. ::

    #include "metawear/core/logging.h"
    
    void start_logging(MblMwMetaWearBoard* board) {
        mbl_mw_logging_start(board);
    }

Downloading Data
----------------
When you are ready to retrieve the data, execute 
`mbl_mw_logging_download <https://mbientlab.com/docs/metawear/cpp/latest/logging_8h.html#a5d972af91fc37cfcb235785e20974ed3>`_.  You will need to pass 
in a `MblMwLogDownloadHandler <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwLogDownloadHandler.html>`_ struct to handle notifications 
from the logger. ::

    #include "metawear/core/logging.h"
    
    void download_log_data(MblMwMetaWearBoard* board) {
        static auto progress_update = [](uint32_t entries_left, uint32_t total_entries)-> void {
            printf("download progress= %d / %d\n", entries_left, total_entries);
            if (!entries_left) {
                printf("download complete\n");
            }
        };
        static auto unknown_entry = [](uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
            printf("received unknown log entry: id= %d\n", id);
        };
    
        static MblMwLogDownloadHandler download_handler = { progress_update, unknown_entry };
        // start a log download and send 100 progress notifications
        mbl_mw_logging_download(board, 100, &download_handler);
    }

