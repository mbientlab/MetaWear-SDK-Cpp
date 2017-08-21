.. highlight:: cpp

Settings
========
The Settings module configures Bluetooth LE parameters and reports diagnostic information about the board.  Functions communicating with this module are 
defined in the `settings.h <https://mbientlab.com/docs/metawear/cpp/latest/settings_8h.html>`_ header file.

Battery State
-------------
The battery data provided by the Settings module reports both charge percetange and voltage, encapsulated by the 
`MblMwBatteryState <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwBatteryState.html>`_ struct.  Unlike the battery gatt servive, this 
battery data can be utilized with MetaWear features such as the logger. ::

    #include "metawear/core/datasignal.h"
    #include "metawear/core/settings.h"
    #include "metawear/core/types.h"
    
    void battery_stream(MblMwMetaWearBoard* board) {
        auto battery_signal = mbl_mw_settings_get_battery_state_data_signal(board);

        mbl_mw_datasignal_subscribe(battery_signal, [](const MblMwData* data) -> void {
            auto state = (MblMwBatteryState*) data->value;
            printf("{voltage: %dmV, charge: %d}\n", state->voltage, state->charge);
        });
        mbl_mw_datasignal_read(battery_signal);
    }

Handling Disconnects
--------------------
The disconnect event is a type of `MblMwEvent <https://mbientlab.com/docs/metawear/cpp/latest/event__fwd_8h.html#a569b89edd88766619bb41a2471743695>`_ 
that is fired when the Bluetooth connection is terminated. ::

    #include "metawear/core/event.h"
    #include "metawear/core/settings.h"
    
    #include "metawear/sensor/accelerometer.h"
    
    void handle_disconnect(MblMwMetaWearBoard* board) {
        static auto cmds_recorded = [](void) -> void {
            printf("commands recorded\n");
        };
    
        auto dc_event = mbl_mw_settings_get_disconnect_event(board);
        mbl_mw_event_record_commands(dc_event);
        // stop the accelerometer when connection is lost
        mbl_mw_acc_stop(board);
        mbl_mw_event_end_record(dc_event, cmds_recorded);
    }

Advertising Parameters
----------------------
Advertising parameters control how the Bluetooth radio sends its advertising data.  You can modify the device name, timeout, tx power, and scan 
response.  If you have set an timeout, you can manually begin another advertisement cycle by calling 
`mbl_mw_settings_start_advertising <https://mbientlab.com/docs/metawear/cpp/latest/settings_8h.html#aad3d9f431b6e2178dbb5a409ce14cbce>`_. ::

    #include "metawear/core/settings.h"
    
    void modify_ad_params(MblMwMetaWearBoard* board) {
        // change the device name to 'metaware'
        const uint8_t* name = "metaware";
        mbl_mw_settings_set_device_name(board, name, 8);
    }

Connection Parameters
---------------------
Connection parameters control how BTLE devices communicate with each other.  Modifying the connection parameters are all handled at the same time by 
calling 
`mbl_mw_settings_set_connection_parameters <https://mbientlab.com/docs/metawear/cpp/latest/settings_8h.html#a1cf3cae052fe7981c26124340a41d66d>`_.  
A more detailed explanation of about BTLE connection parameters can be found on this 
`post <https://devzone.nordicsemi.com/question/60/what-is-connection-parameters/>`_ from the Nordic Developer Zone forums. ::

    #include "metawear/core/settings.h"
    
    void modify_conn_params(MblMwMetaWearBoard* board) {
        mbl_mw_settings_set_connection_parameters(board, 10.f, 1024.f, 0, 6000);
    }

