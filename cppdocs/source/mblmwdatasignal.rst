.. highlight:: cpp

Data Signal
===========
Data signals are an abstract representation of data producers.  The API treats them as an event that contains data and represents 
them with the `MblMwDataSignal <https://mbientlab.com/docs/metawear/cpp/latest/datasignal__fwd_8h.html#a1ce49f0af124dfa7984a59074c11e789>`_ struct.  
They can be safely typecasted to an `MblMwEvent <https://mbientlab.com/docs/metawear/cpp/latest/event__fwd_8h.html#a569b89edd88766619bb41a2471743695>`_

Data Handling
-------------
Signal data is encapsulated by the `MblMwData <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwData.html>`_ struct.  The struct contains a: 

* Timestamp of when the data was created
* Pointer to the data
* Data type id indicating how to cast the pointer

An enumeration of data types is defined by the 
`MblMwDataTypeId <https://mbientlab.com/docs/metawear/cpp/latest/data_8h.html#a8335412069204db23669001bcaed532e>`_ enum and structs wrapping non basic 
data types are defined in the `types.h <https://mbientlab.com/docs/metawear/cpp/latest/types_8h.html>`_ header file. ::

    #include "metawear/core/data.h"
    
    #include <chrono>
    #include <iomanip>
    
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::system_clock;
    using std::chrono::time_point;
    
    void data_printer(const MblMwData* data) {
        // Print data as 2 digit hex values
        uint8_t* data_bytes = (uint8_t*) data->value;
        string bytes_str("[");
        char buffer[5];
        for (uint8_t i = 0; i < data->length; i++) {
            if (i) {
                bytes_str += ", ";
            }
            sprintf(buffer, "0x%02x", data_bytes[i]);
            bytes_str += buffer;
        }
        bytes_str += "]";
    
        // Format time as YYYYMMDD-HH:MM:SS.LLL
        time_point<system_clock> then(milliseconds(data->epoch));
        auto time_c = system_clock::to_time_t(then);
        auto rem_ms= data->epoch % 1000;
    
        cout << "{timestamp: " << put_time(localtime(&time_c), "%Y%m%d-%T") << "." << rem_ms << ", "
            << "type_id: " << data->type_id << ", "
            << "bytes: " << bytes_str.c_str() << "}"
            << endl;
    }

Streaming
---------
To stream data live to your device, call 
`mbl_mw_datasignal_subscribe <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#ab2708a821b8cca7c0d67cf61acec42c3>`_  with the 
desired data signal and a callback function for handling the received data.  Terminating the live stream is done by calling 
`mbl_mw_datasignal_unsubscribe <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#ab2708a821b8cca7c0d67cf61acec42c3>`_. ::

    #include "metawear/core/datasignal.h"
    
    #include "metawear/sensor/switch.h"
    
    void subscribe_switch(MblMwMetaWearBoard* board) {
        static auto data_handler = [](const MblMwData* data) -> void {
            if (*((uint32_t*)data->value)) {
                cout << "Switch Pressed" << endl;
            } else {
                cout << "Switch Released" << endl;
            }
        };
    
        auto switch_signal = mbl_mw_switch_get_state_data_signal(board);
        mbl_mw_datasignal_subscribe(switch_signal, data_handler);
    }

Logging
-------
Alternatively, data can be logged and retrieved at a later time.  See the :doc:`mblmwlogger` section for more details.

Readable Signals
----------------
Some sensors will only send data when they receive a command to do so.  Data signals that represent this type of data source are called readable signals.  
You can check if a data signal is readable by calling 
`mbl_mw_datasignal_is_readable <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a9633497a3785ba2369f57b939bd156c2>`_.  

The read command is issued by calling 
`mbl_mw_datasignal_read <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a0a456ad1b6d7e7abb157bdf2fc98f179>`_ or 
`mbl_mw_datasignal_read_with_parameters <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a71391d5862eb18327ce2aaaac4a12159>`_.  Most 
readable data signals will use the former function which does not require any additional parameters.  The latter function is for reads that require 
additional parameters which are bundled into one struct.

When using readable signals, you must decide up front if the data will be streamed or logged before interacting with it.  That is, you should either 
have subscribed to or setup a logger for a readable signal before reading it.

Data Processing
---------------
Data signals can be fed through the on-board data processors to filter and/or transform the data in the firmware.  By performing computations on the 
MetaWear side, you can reduce the amount of data that is sent over the radio and the amount of postprocessing that is done on your mobile device.  Data 
processors can also be chained together to perform more complex tasks, such as using the rss, average, and threshold processors to determine if the 
board is in freefall based on the XYZ acceleration data. 

See the :doc:`mblmwdataprocessor` section for more details on the data processing system. ::

    #include "metawear/core/dataprocessor_fwd.h"
    #include "metawear/processor/rss.h"
    #include "metawear/sensor/accelerometer.h"
    
    void rss_accelerometer(MblMwMetaWearBoard* board) {
        static auto data_handler = [](const MblMwData* data) -> void {
            printf("acc rss= %.3fg\n", (float*) data->value);
        };
        static auto rss_ready = [](MblMwDataProcessor* processor) -> void {
            // subscribe to the rss processor
            mbl_mw_datasignal_subscribe((MblMwDataSignal*) processor, data_handler);
        };
    
        // Create an rss processor to transform the XYZ values into vector magnitude
        // Do not need to compute rss on your device and less data is transmitted
        auto acc_signal = mbl_mw_acc_get_acceleration_data_signal(board);
        mbl_mw_dataprocessor_rss_create(acc_signal, rss_ready);
    }

Component Signals
-----------------
Some signals, such as the acceleration datasignal, are composed of multiple values.  While you can interact with them as a whole, somtimes it is more 
convenient to only use individual values.  To access the component values, call 
`mbl_mw_datasignal_get_component <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#abf5eaa69c5f5978cb7bdd9ea04a910e0>`_ with the signal 
and an index represnting which component to retrieve.  If a signal is single valued, the function will return null. ::

    #include "metawear/sensor/accelerometer.h"

    void component_demo(MblMwMetaWearBoard* board) {
        auto acc_root = mbl_mw_acc_get_acceleration_data_signal(board);
        // get z axis signal
        auto acc_z = mbl_mw_datasignal_get_component(acc_root, MBL_MW_ACC_ACCEL_Z_AXIS_INDEX);

        mbl_mw_datasignal_subscribe(acc_z, [](MblMwData* data) -> void {
            //combined xyz data is MblMwCartesianFloat, individual axis is float
            printf("z-axis: %.3f\n", *((float*) data->value));
        });
    }

