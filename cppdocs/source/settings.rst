.. highlight:: cpp

Settings
========
The Settings module configures Bluetooth LE parameters and reports diagnostic information about the board.  

Functions communicating with this module are defined in the `settings.h <https://mbientlab.com/docs/metawear/cpp/latest/settings_8h.html>`_ header file.

Battery State
-------------
The battery data provided by the Settings module reports both charge percentange and voltage, encapsulated by the 
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

Power and Charge
-----------------
There are a few options to find out if the device is charging.

`mbl_mw_settings_read_current_charge_status <https://mbientlab.com/documents/metawear/cpp/latest/settings_8h.html#aa885aa09ce4b5c4de03157e4a585ba17>`_ reads the current charge status of the board.  ::

    mbl_mw_settings_read_current_charge_status(board, callback); //CALLBACK RETURNS INT

`mbl_mw_settings_get_charge_status_data_signal <https://mbientlab.com/documents/metawear/cpp/latest/settings_8h.html#a22653a0a40d3f5d12e5294aebeb166c1>`_ retrieves the data signal representing the charge status.  ::

    signal = mbl_mw_settings_get_charge_status_data_signal(board);

`mbl_mw_settings_read_current_power_status <https://mbientlab.com/documents/metawear/cpp/latest/settings_8h.html#a481133ec7b8456386e97bb748755f4e7>`_ reads the current power status if available.  ::

    mbl_mw_settings_read_current_power_status(board, callback);

`mbl_mw_settings_get_power_status_data_signal <https://mbientlab.com/documents/metawear/cpp/latest/settings_8h.html#a3469c6609c3418b7f17dd21882ab8cb4>`_ retrieves the data signal representing the power status.  ::

    signal = mbl_mw_settings_get_power_status_data_signal(board);

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

Transmit Power
--------------------
The default transmission power is 0db on the MetaWear. This is the Bluetooth radio signal strength when transmitting and can be changed with `mbl_mw_settings_set_tx_power <https://mbientlab.com/documents/metawear/cpp/latest/settings_8h.html#a335f712d5fc0587eff9671b8b105d3ed>`_.

Valid values are: 4, 0, -4, -8, -12, -16, -20, -30:  ::

    mbl_mw_settings_set_tx_power(board, -20)

Advertising Parameters
----------------------
Advertising parameters control how the Bluetooth radio sends its advertising data.  You can modify the device name, timeout, tx power, and scan 
response.  If you have set an timeout, you can manually begin another advertisement cycle by calling 
`mbl_mw_settings_start_advertising <https://mbientlab.com/docs/metawear/cpp/latest/settings_8h.html#aad3d9f431b6e2178dbb5a409ce14cbce>`_. ::

    #include "metawear/core/settings.h"
    
    void modify_ad_params(MblMwMetaWearBoard* board) {
        mbl_mw_settings_set_ad_interval(board, 417, 180)
        mbl_mw_settings_start_advertising(board)
    }
    
Device Name
-----------------
The advertised name of the device (default = MetaWear) can be changed with `mbl_mw_settings_set_device_name <https://mbientlab.com/documents/metawear/cpp/latest/settings_8h.html#a7b2e5239dfb56137b86cfaddb5d10333>`_.  ::

    #include "metawear/core/settings.h"
    
    void modify_ad_params(MblMwMetaWearBoard* board) {
        // change the device name to 'metaware'
        const uint8_t* name = "metawoot";
        mbl_mw_settings_set_device_name(board, name, 8);
    }

Connection Parameters
---------------------
Connection parameters control how BTLE devices communicate with each other.  Modifying the connection parameters are all handled at the same time by 
calling 
`mbl_mw_settings_set_connection_parameters <https://mbientlab.com/docs/metawear/cpp/latest/settings_8h.html#a1cf3cae052fe7981c26124340a41d66d>`_.  

 - Connection interval = how often devices talk - min is 7.5ms, it increases in steps of 1.25ms - recommend setting min and max to same @ 7.5ms for performance.
 - Slave latency = metawear can choose not to answer when central asks for an update (i.e metawear can sleep longer - doesn't affect transfer speeds).
 - Connection supervision timeout = determines timeout from last data exchange (tells central how long to wait to attempt to reconnect to a lost conn - if your metawear goes in and out of range often, it is better to have a short timeout)

Changing connection parameters is not guaranteed to work, a central can reject peripheral and vice-versa. Settings are also hardware and OS dependent; Apple only accepts 15ms for example and will often default to 30ms.
	
A more detailed explanation of about BTLE connection parameters can be found on this 
`post <https://devzone.nordicsemi.com/question/60/what-is-connection-parameters/>`_ from the Nordic Developer Zone forums. ::

    #include "metawear/core/settings.h"
    
    void modify_conn_params(MblMwMetaWearBoard* board) {
        mbl_mw_settings_set_connection_parameters(board, 10.f, 1024.f, 0, 6000);
    }

MMS 3V Regulator
---------------------
The MMS (MetaMotion) board has a 3V regulator that can be turned on and off for IOs.

It is automatically turned on to power the coin vibration motor (if there is one attached), the ambient light sensor, and the LED.

However, if you have an external peripheral on the IOs that needs 3V power (such as a buzzer or UV sensor), you can use this function to turn on the power: ::

    mbl_mw_settings_enable_3V_regulator(board, 1);

And to turn it off: ::

    mbl_mw_settings_enable_3V_regulator(board, 0);