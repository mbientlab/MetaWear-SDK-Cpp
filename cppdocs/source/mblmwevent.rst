.. highlight:: cpp

Events
======
An event is an asynchronous notification from the MetaWear board represented in the C++ API by the 
`MblMwEvent <https://mbientlab.com/docs/metawear/cpp/latest/event__fwd_8h.html#a569b89edd88766619bb41a2471743695>`_ struct.  

Recording Commands
------------------
The board can be programmed to execute MetaWear commands in response to an event firing.  To start recording commands, call 
`mbl_mw_event_record_commands <https://mbientlab.com/docs/metawear/cpp/latest/event_8h.html#a771158b2eedeea765163a7df5f6c51e7>`_.  While in a recording 
state, all MetaWear functions called will instead be recorded on the board and executed when the event is fired.  

To stop recording, call 
`mbl_mw_event_end_record <https://mbientlab.com/docs/metawear/cpp/latest/event_8h.html#a5d4f44a844d2ff90b9e97ed33613fca8>`_.  This function is 
asynchronous and will alert the caller when it is completed via a callback function.  If recording events was successful, 
`MBL_MW_STATUS_OK <https://mbientlab.com/docs/metawear/cpp/latest/status_8h.html#a609cbd145a50305cca56db3af8ba3000>`_ will be 
passed into the callback function, otherwise 
`MBL_MW_STATUS_ERROR_TIMEOUT <https://mbientlab.com/docs/metawear/cpp/latest/status_8h.html#a5b59ac66b86ad7c56dc904a901039931>`_ is used.  ::

    #include "metawear/core/event.h"
    #include "metawear/core/settings.h"
    
    void setup_dc_event(MblMwMetaWearBoard* board) {
        MblMwLedPattern pattern;
        mbl_mw_led_load_preset_pattern(&pattern, MBL_MW_LED_PRESET_BLINK);
        pattern.repeat_count = 10;
    
        // Setup disconnect event to blink the blue led 10x when fired
        MblMwEvent* dc_event = mbl_mw_settings_get_disconnect_event(board);
        mbl_mw_event_record_commands(dc_event);
        mbl_mw_led_write_pattern(board, &pattern, MBL_MW_LED_COLOR_BLUE);
        mbl_mw_led_play(board);
        mbl_mw_event_end_record(dc_event, [](MblMwEvent* event, int32_t status) -> void {
            if (!status) {
                cout << "Error recording commands, status= " << status << endl;
            } else {
                cout << "Commands recorded" << endl;
            }
        });
    }
