.. highlight:: cpp

Timer
=====
A MetaWear timer can be thought of as an event that is fired at fixed intervals.  

These timers are represented by the 
`MblMwTimer <https://mbientlab.com/docs/metawear/cpp/latest/timer__fwd_8h.html#ac32a834c8b7bc7230ce6947425f43926>`_ struct and can be safely typcased to a 
`MblMwEvent <https://mbientlab.com/docs/metawear/cpp/latest/event__fwd_8h.html#a569b89edd88766619bb41a2471743695>`_ struct.  

Timers can be used to schedule periodic tasks or setup a delayed task execution. For example, you can use the timer to record temperature samples are extremely low frequencies such as once per day or once per hour.

ID
--
MblMwTimer objects are identified by a numerical id; you can retrieve the id by calling 
`mbl_mw_timer_get_id <https://mbientlab.com/docs/metawear/cpp/latest/timer_8h.html#a695e95e035825b626b78416b5df5611e>`_.  

The id is used to retrieve existing timers from the API with the 
`mbl_mw_timer_lookup_id <https://mbientlab.com/docs/metawear/cpp/latest/timer_8h.html#a84d84562f66090e61061b67321c22961>`_ function.

As with previous sections, you may want to keep the id handy so that you can retrieve a timer at a later time.

Task Scheduling
---------------
Before you can schedule tasks, you first need to create a timer, by calling either 
`mbl_mw_timer_create <https://mbientlab.com/docs/metawear/cpp/latest/timer_8h.html#a749457dc6c8a181990367d8b1f92284c>`_ or 
`mbl_mw_timer_create_indefinite <https://mbientlab.com/docs/metawear/cpp/latest/timer_8h.html#ae6a58f97ba8e443aec84769a9cc84453>`_.  These functions are asynchronous and 
will pass a pointer to the caller when the timer is created.  

When you have a valid `MblMwTimer <https://mbientlab.com/docs/metawear/cpp/latest/timer__fwd_8h.html#ac32a834c8b7bc7230ce6947425f43926>`_, you can use the command recording system outlined in 
:doc:`mblmwevent` section to program the board to respond to the periodic events.  

Upon recording timer task commands, call 
`mbl_mw_timer_start <https://mbientlab.com/docs/metawear/cpp/latest/timer_8h.html#a90455d9e29548c1332ef7ad9db46c50e>`_ to start the timer.

When you are done using a timer, you can remove it with 
`mbl_mw_timer_remove <https://mbientlab.com/docs/metawear/cpp/latest/timer_8h.html#a96d102b4f39a46ccbaf8ee5a37a2a55e>`_. ::

    #include "metawear/core/event.h"
    #include "metawear/core/timer.h"
    
    #include "metawear/sensor/gpio.h"
    
    void timer_setup(MblMwMetaWearBoard* board) {
        static auto cmds_recorded = [](void) -> void {
            printf("timer task setup\n");
        };
        static auto timer_created = [](MblMwTimer* timer) -> void {
            auto owner = mbl_mw_event_get_owner((MblMwEvent*) timer);
    
            auto adc_signal= mbl_mw_gpio_get_analog_input_data_signal(board, 0, 
                    MBL_MW_GPIO_ANALOG_READ_MODE_ADC);
            // read gpio adc data every time the timer fires an event
            mbl_mw_event_record_commands((MblMwEvent*) timer);
            mbl_mw_datasignal_read(adc_signal);
            mbl_mw_event_end_record((MblMwEvent*) timer, cmds_recorded);
    
            mbl_mw_timer_start(timer);
        };
    
        // create a timer that indefinitely fires events every 500ms
        mbl_mw_timer_create_indefinite(board, 500, 0, timer_created);
    }

A good example is the one mentioned above. Because the temperature sensor is an analog sensor (not the one in the BMP or BME), it cannot be sampled at specific intervals unless a timer is used:

::

    function startLogging(device, callback) {
        static auto timer_created = [](MblMwTimer* timer) -> void {
            auto tempTimer = timer;
            auto tempSignal = mbl_mw_multi_chnl_temp_get_temperature_data_signal(board, 1);

            mbl_mw_event_record_commands(tempTimer);
            mbl_mw_datasignal_read(tempSignal);
            mbl_mw_event_end_record(tempTimer, cmds_recorded) 
       
            mbl_mw_timer_start(tempTimer);
        
            mbl_mw_datasignal_log(tempSignal, [](MblMwDataLogger* logger) -> void {
                if (logger != nullptr) {
                    printf("logger ready\n");
                } else {
                    printf("Failed to create the logger\n");
                }
            });
        
            mbl_mw_logging_start(board, 0);
        };
        
        mbl_mw_timer_create_indefinite(board, 1000, 0, timer_created) 
    }

    function downloadLog(tempLogger, tempTimer) {
        static auto progress_update = [](uint32_t entries_left, uint32_t total_entries)-> void {
            printf("download progress= %d / %d\n", entries_left, total_entries);
            if (!entries_left) {
                printf("download complete\n");
            }
        };
        static auto unknown_entry = [](uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
            printf("received unknown log entry: id= %d\n", id);
        };
    
        mbl_mw_timer_remove(tempTimer);
        mbl_mw_logging_stop(board);
        
        mbl_mw_logger_subscribe(tempLogger, [](const MblMwData* data) -> void {
            printf(data);
        });
        
        mbl_mw_logging_download(board, 20, downloadHandler);
            
        static MblMwLogDownloadHandler download_handler = { progress_update, unknown_entry };
        mbl_mw_logging_download(board, 100, &download_handler);
    }


