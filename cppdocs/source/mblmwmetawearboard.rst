.. highlight:: cpp

MblMwMetaWearBoard
==================
Like the ``MetaWearBoard`` (Android) and ``MBLMetaWear`` (iOS) classes, the 
`MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html>`__ struct is the C++ 
API's representation of a MetaWear board and is the central type for using the API.  The 
`metawearboard.h <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html>`_ header file provides functions for instantiating and 
retrieving information about a board.

Instantiation
-------------
Before you can instantiate a 
`MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html>`__ struct, you 
must first create a `MblMwBtleConnection <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwBtleConnection.html>`__ as outlined in the 
:doc:`btlecommunication` section.  

Once you have your struct properly setup, call 
`mbl_mw_metawearboard_create <https://mbientlab.com/documents/metawear/cpp/latest/metawearboard_8h.html#ae3adb5c7cfbdc6133796843db339a75e>`__ to instantiate 
a `MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html>`_ struct. ::

    #include "metawear/core/connection.h"
    #include "metawear/core/metawearboard.h"
    
    int main(int argc, char **argv) {
        MblMwBtleConnection btle_conn = { write_gatt_char, read_gatt_char, enable_char_notify };
        MblMwMetaWearBoard* board = mbl_mw_metawearboard_create(&btle_conn);
    }

Initialize
----------
After you have instantiated a 
`MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html#a2c238febd06fcaaa403e937489a12652>`__, you then need 
to initialize its internal state by calling 
`mbl_mw_metawearboard_initialize <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#a78afe422e5279121d8ae859a4d8dbe70>`_.  
Initializing can take a few seconds and will asynchronously alert the caller when it is finished.  If initialization succeeded, ``MBL_MW_STATUS_OK`` 
will be passed to the callback function, otherwise a non-zero value will be returned instead signalling the task failed.

You can check if a `MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html>`__ 
has been initialized by calling 
`mbl_mw_metawearboard_is_initialized <https://mbientlab.com/documents/metawear/cpp/latest/metawearboard_8h.html#af2cdb8a6af5f20af661a6c6fd05ff45a>`_. 

::

    mbl_mw_metawearboard_initialize(board, [](MblMwMetaWearBoard* board, int32_t status) -> void {
        if (!status) {
            printf("Error initializing board: %d\n", status);
        } else {
            printf("Board initialized\n");
        }
    );

Initialization must be done everytime you connect to a board.

Model
-----
Despite the name, the ``MetaWearBoard`` interface communicates with all MetaSensor boards, not just MetaWear boards.  Because of this, the header file  
provides the `mbl_mw_metawearboard_get_model  <https://mbientlab.com/documents/metawear/cpp/latest/metawearboard_8h.html#a3026e5b8b32b77560056978f3bcd6dc9>`_ 
method that determines exactly which board the ``MblMwMetaWearBoard`` is currently communicating with.

::

    cout << "model = " << mbl_mw_metawearboard_get_model(board) << endl;
    
This will return model # '9' for a MetaMotionR v0.3.
    
::

    cout << "model = " << mbl_mw_metawearboard_get_model_name(board) << endl;
    
This will return the board name 'MetaMotionR'.

Tear Down
---------
Calling  
`mbl_mw_metawearboard_tear_down <https://mbientlab.com/documents/metawear/cpp/latest/metawearboard_8h.html#ae84720bcc733aa6be7243b23465871aa>`_ will remove 
all data processors, loggers, timers, and recorded events from both the board and the struct's internal state.  It does not reset the board so any 
configuration changes will be preserved.

Reset
------
Calling  
`mbl_mw_debug_reset <https://mbientlab.com/documents/metawear/cpp/latest/debug_8h.html#a7c08136c14e58170fe5c1cd8d43a26f1>`_ will reset the board. This is a soft reset and macros and loggers will be preserved.

Full Reset
-----------
If you want to do a full reset, it is recommended to run the following:

::

    MetaWear.mbl_mw_logging_stop(board);
    MetaWear.mbl_mw_logging_clear_entries(board);
    MetaWear.mbl_mw_macro_erase_all(board);
    MetaWear.mbl_mw_debug_reset_after_gc(board);

Modules
-------
Modules are sensors or features supported by the MetaWear firmware.  While the C++ API does not partition the module functions in the same way the 
Android and iOS APIs do, it does provide the 
`mbl_mw_metawearboard_lookup_module <https://mbientlab.com/documents/metawear/cpp/latest/metawearboard_8h.html#a6a86f10210179a6e82c63e9f6667a9b6>`_ function.  
The output is used to determine what kind of module is present, i.e. which accelerometer is on the board, or if the module is even supported on the 
board e.g. barometers are not available on all boards.  

::

    cout << "present = " << mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_GYRO) << endl;

If the result is -1, the gyroscope would not be present. Most of the time it will return 0 or 1.

In the accelerometer case, a non -1 return can signify which accelerometer is present. The MMA8452Q returns 0; the BMI160 returns 1; and the BMA255 returns 3 (these are different accelerometer ICs we have included in our boards in the past).

Constants identifying the modules are defined in the `module.h <https://mbientlab.com/docs/metawear/cpp/latest/module_8h.html>`_ header file.

Constants identifying the specific sensor used in the module are found in the individual sensor file itself. For the accelerometer it would be found `here <https://mbientlab.com/documents/metawear/cpp/latest/accelerometer__bosch_8h.html#a622c5b211655ab1a1f0febff0cbf1c09>`_

Freeing Memory
--------------
When you are done using the struct, call 
`mbl_mw_metawearboard_free <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#a78afe422e5279121d8ae859a4d8dbe70>`_.  This function 
does not affect the state of the MetaWear board, it only frees the memory allocated by the 
`mbl_mw_metawearboard_create <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#ae3adb5c7cfbdc6133796843db339a75e>`_ function.
