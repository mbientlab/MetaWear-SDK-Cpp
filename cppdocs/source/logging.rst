.. highlight:: cpp

Logging
=======
Logging functions in the `logging.h <https://mbientlab.com/docs/metawear/cpp/latest/logging_8h.html>`_ header file control the on-board logger.  

These functions go hand in hand with the data signal logger outlined in the :doc:`mblmwdatasignal` section.  

Once a logger is created; logging functions can be used. After you have setup the signal loggers, start 
the logger by calling `mbl_mw_logging_start <https://mbientlab.com/docs/metawear/cpp/latest/logging_8h.html#acab2d6b1c4f5449a39fe3bf60205471f>`_. ::

    #include "metawear/core/logging.h"
    
    void start_logging(MblMwMetaWearBoard* board) {
        mbl_mw_logging_start(board);
    }

Once we are done logging, simply call: ::

    mbl_mw_logging_stop(board);

Note for the MMS
----------------
The MMS (MetaMotionS) board uses NAND flash memory to store data on the device itself. The NAND memory stores data in pages that are 512 entries large. When data is retrieved, it is retrieved in page sized chunks.

Before doing a full download of the log memory on the MMS, the final set of data needs to be written to the NAND flash before it can be downloaded as a page. To do this, you must call the function: ::

   mbl_mw_logging_flush_page(board);

This should not be called if you are still logging data.

Downloading Data
----------------
When you are ready to retrieve the data, execute 
`mbl_mw_logging_download <https://mbientlab.com/docs/metawear/cpp/latest/logging_8h.html#a5d972af91fc37cfcb235785e20974ed3>`_.  

You will need to pass in a `MblMwLogDownloadHandler <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwLogDownloadHandler.html>`_ struct to handle notifications 
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

Typical setup
----------------
Here is the skeleton code for a typical scenario logging and downloading the accelerometer data: ::

    function startLogging() {
        mbl_mw_acc_set_odr(board, 50.0);
        mbl_mw_acc_set_range(board, 16.0);
        mbl_mw_acc_write_acceleration_config(board);
        
        auto accSignal = mbl_mw_acc_get_acceleration_data_signal(board);
        mbl_mw_datasignal_log(accSignal, [](MblMwDataLogger* logger) -> void {
            acc_logger = logger;
            printf("logger created\n");
        });
    
        mbl_mw_logging_start(board, 0);
        
        mbl_mw_acc_enable_acceleration_sampling(board);
        mbl_mw_acc_start(board);
    }

    function downloadLog(acc_logger) {
        static auto progress_update = [](uint32_t entries_left, uint32_t total_entries)-> void {
            printf("download progress= %d / %d\n", entries_left, total_entries);
            if (!entries_left) {
                printf("download complete\n");
            }
        };
        static auto unknown_entry = [](uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
            printf("received unknown log entry: id= %d\n", id);
        };
    
        mbl_mw_acc_stop(board);
        mbl_mw_acc_disable_acceleration_sampling(board);
        
        mbl_mw_logging_stop(board);
        
        mbl_mw_logger_subscribe(acc_logger, = [](const MblMwData* data) -> void {
            auto acceleration = (MblMwCartesianFloat*) data->value;
            printf("(%.3fg, %.3fg, %.3fg)\n", acceleration->x, acceleration->y, acceleration->z);
        });
        
        static MblMwLogDownloadHandler downloadHandler = { progress_update, unknown_entry };
        mbl_mw_logging_download(board, 20, downloadHandler);
    }  



