.. highlight:: cpp

Bluetooth LE Communication
==========================
Because the library does not implement Bluetooth LE communication, users must implement the GATT operations in their target language and pass those 
functions to the C++ API.  The required GATT operations are defined as function pointers in the 
`MblMwBtleConnection <https://mbientlab.com/docs/metawear/cpp/0/structMblMwBtleConnection.html>`_ struct.

Read Characteristic
-------------------
The first field in the struct is ``read_gatt_char`` which points to the function reading the requested GATT characteristic.  The function also 
accepts a function pointer that the characteristic value will be passed to when the read operation completes.  ::

    #include <iomanip>
    #include <iostream>

    using namespace std;

    static void read_gatt_char(const void* caller, const MblMwGattChar* characteristic, 
            MblMwFnIntVoidPtrArray handler) {
        // replace with platform specific BluetoothGatt code

        cout << setw(8) << setfill('0') << hex 
            << "Reading characteristic: {service_uuid_high: " << characteristic->service_uuid_high
            << ", service_uuid_low: " << characteristic->service_uuid_low
            << ", uuid_high: " << characteristic->uuid_high 
            << ", uuid_low: " << characteristic->uuid_low 
            << "}" << dec << endl;

        if (characteristic->uuid_high == 0x00002a2400001000 && 
                characteristic->uuid_low == 0x800000805f9b34fb) {
            // simulate response for reading model characteristic
            const uint8_t model[] = "4";
            handler(caller, model, 1);
        } else if (characteristic->uuid_high == 0x00002a2600001000 && 
                characteristic->uuid_low == 0x800000805f9b34fb) {
            // simulate response for reading firmware characteristic
            const uint8_t firmware[] = "1.2.5";
            handler(caller, firmware, 5);
        }
    }

Write Characteristic
--------------------
The next operation required by the SDK is writing GATT characteristics.  When implementing the write function, note that there are two types of 
GATT writes defined by the 
`MblMwGattCharWriteType  <https://mbientlab.com/docs/metawear/cpp/0/btle__connection_8h.html#a3cb392874fc65c2703366e9185c7122d>`_ enum.  Your function 
must appropriately distinguish between the types when writing the GATT characteristic.  ::

    static void write_gatt_char(const void* caller, MblMwGattCharWriteType write_type, 
            const MblMwGattChar* characteristic, const uint8_t* value, uint8_t length) {
        // replace with platform specific BluetoothGatt code

        cout << "{write type: " << write_type << ", value: " << 
        cout << hex << setw(2) << setfill('0') << "[0x" << (int)value[0];
        for (uint8_t i = 1; i < length; i++) {
            cout << ", 0x" << (int)value[i];
        }
        cout << "]}" << dec << endl;
    }

Enabling Notifications
----------------------
The C++ SDK uses the ``enable_char_notiy`` function pointer to request characteristic notifications.  Along with the characteristic to modify, the 
function takes in 2 function pointers: the first one is run everytime the specific characteristic has changed and the latter is will be run when the 
notify task is completed.  

The ``ready`` function pointer requires an additional int32 value that represents the status of the enable notify task.  If sucessful, use the  
``MBL_MW_STATUS_OK`` value and if it fails, use ``MBL_MW_STATUS_ERROR_ENABLE_NOTIFY`` instead.  ::

    #include "metawear/core/status.h"
    #include <unordered_map>

    using std::unordered_map;

    static unordered_map<const void*, MblMwFnIntVoidPtrArray> notify_handlers;

    static void enable_char_notify(const void* caller, const MblMwGattChar* characteristic, 
            MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready) {
        // replace with platform specific BluetoothGatt code
        notify_handlers.insert({ caller, handler });
        // call the 'ready' function pointer when the enable notification requeset has finished
        ready(caller, MBL_MW_STATUS_OK);
    }

Disconnect Handler
------------------
The SDK also needs to know when the bluetooth connection is lost.  It uses the ``on_disconnect`` field to pass a function pointer to the Bluetooth LE 
wrapper for handling disconnect events.  The handler also accepts a second int value which, for now, can be set to 0.  ::

    static unordered_map<const void*, MblMwFnVoidVoidPtrInt> dc_handlers;

    static void on_disconnect(const void* caller, MblMwFnVoidVoidPtrInt handler) {
        // call this handler everytime connection is lost, use 0 for 'value' parameter
        // handler(caller, 0)
        dc_handlers.insert({ caller, handler });
    }
