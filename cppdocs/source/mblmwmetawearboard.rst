.. highlight:: cpp

MblMwMetaWearBoard
==================
Like the ``MetaWearBoard`` (Android) and ``MBLMetaWear`` (iOS) classes, the 
`MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html#a2c238febd06fcaaa403e937489a12652>`_ struct is the C++ 
API's representation of a MetaWear board and is the central type for using the API.  The 
`metawearboard.h <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html>`_ header file provides functions for instantiating and 
retrieving information about a board.

Instantiation
-------------
Before you can instantiate a 
`MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html#a2c238febd06fcaaa403e937489a12652>`_ struct, you 
must first create a `MblMwBtleConnection <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwBtleConnection.html>`_ as outlined in the 
:doc:`btlecommunication` section.  Once you have your struct properly setup, call 
`mbl_mw_metawearboard_create <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#ae3adb5c7cfbdc6133796843db339a75e>`_ to instantiate 
a `MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html#a2c238febd06fcaaa403e937489a12652>`_ struct. ::

    #include "metawear/core/connection.h"
    #include "metawear/core/metawearboard.h"
    
    int main(int argc, char **argv) {
        MblMwBtleConnection btle_conn = { write_gatt_char, read_gatt_char, enable_char_notify };
        MblMwMetaWearBoard* board = mbl_mw_metawearboard_create(&btle_conn);
    }

Initialize
----------
After you have instantiated a 
`MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html#a2c238febd06fcaaa403e937489a12652>`_, you then need 
to initialize its internal state by calling 
`mbl_mw_metawearboard_initialize <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#a78afe422e5279121d8ae859a4d8dbe70>`_.  
Initializing can take a few seconds and will asynchronously alert the caller when it is finished.  If initialization succeeded, ``MBL_MW_STATUS_OK`` 
will be passed to the callback function, otherwise a non-zero value will be returned instead signalling the task failed.

You can check if a `MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html#a2c238febd06fcaaa403e937489a12652>`_ 
has been initialized by calling 
`mbl_mw_metawearboard_is_initialized <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#af2cdb8a6af5f20af661a6c6fd05ff45a>`_. ::

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
provides the `mbl_mw_metawearboard_get_model  <https://mbientlab.com/docs/metawear/cpp/0/metawearboard_8h.html#a3026e5b8b32b77560056978f3bcd6dc9>`_ 
method that determines exactly which board the ``MblMwMetaWearBoard`` is currently communicating with.

::

    cout << "model = " << mbl_mw_metawearboard_get_model(board) << endl;

Tear Down
---------
Calling  
`mbl_mw_metawearboard_tear_down <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#ae84720bcc733aa6be7243b23465871aa>`_ will remove 
all data processors, loggers, timers, and recorded events from both the board and the struct's internal state.  It does not reset the board so any 
configuration changes will be preserved.

Modules
-------
Modules are sensors or features supported by the MetaWear firmware.  While the C++ API does not partition the module functions in the same way the 
Android and iOS APIs do, it does provide the 
`mbl_mw_metawearboard_lookup_module <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#ad9c7e7f60f77fc1e929ac48c6a3ffb9b>`_ function.  
The output is used to determine what kind of module is present, i.e. which accelerometer is on the board, or if the module is even supported on the 
board e.g. barometers are not available on all boards.  

Constants identifying the modules are defined in the `module.h <https://mbientlab.com/docs/metawear/cpp/latest/module_8h.html>`_ header file.

Freeing Memory
--------------
When you are done using the struct, call 
`mbl_mw_metawearboard_free <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#a78afe422e5279121d8ae859a4d8dbe70>`_.  This function 
does not affect the state of the MetaWear board, it only frees the memory allocated by the 
`mbl_mw_metawearboard_create <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#ae3adb5c7cfbdc6133796843db339a75e>`_ function.
