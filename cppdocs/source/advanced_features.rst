.. highlight:: cpp

Advanced Features 
=================

High Frequency Streaming
------------------------
Some developers may want to stream data from multiple motion sensors simultaneously or individually at frequencies higher than 100Hz.  To accommodate 
this use case, acceleration, angular velocity, and magnetic field data have a packed output mode that combines 3 data samples into 1 ble packett 
increasing the data throughput by 3x.

::

    mbl_mw_acc_set_odr(board, 200.0f);
    mbl_mw_acc_set_range(board, 4.0f);
    mbl_mw_acc_write_acceleration_config(board);

    auto packed = mbl_mw_acc_get_packed_acceleration_data_signal(board);
    mbl_mw_datasignal_subscribe(packed, [](const MblMwData* data) {
        auto value = (MblMwCartesianFloat*) data->value;
        printf("{x: %.3f, y: %.3f, z: %.3f}\n", value->x, value->y, value->z);
    });

    mbl_mw_acc_enable_acceleration_sampling(board);
    mbl_mw_acc_start(board);

In addition to using packed output, developers will also need to reduce the max connection interval to 7.5ms.  Reducing the max connection interval can 
also be used to speed up log downloads.  ::

    mbl_mw_settings_set_connection_parameters(board, 7.5f, 7.5f, 0, 6000);

Serialization
-------------
The internal state of the 
`MblMwMetaWearBoard <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard__fwd_8h.html#a2c238febd06fcaaa403e937489a12652>`_ object can be 
converted into a byte array, which can then be saved to the disk.  You will need to free the allocated memory after you are done using the byte array.  ::

    uint32_t size;
    uint8_t* state = mbl_mw_metawearboard_serialize(board, &size);
    
    for (uint32_t i = 0; i < size; i++) {
        // write content to a stream
    }

    mbl_mw_memory_free(state);

To restore the board state, pass the byte array into mbl_mw_metawearboard_deserialize.  You must still call 
`mbl_mw_metawearboard_initialize <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#a079fea07f792de97a34c481a31e43101>`_ after 
deserializing the state.  ::

    uint8_t* state;
    uint32_t state_size;

    // assign state and state_size

    mbl_mw_metawearboard_deserialize(board, state, sizeof(state));
    mbl_mw_metawearboard_initialize(board, [](MblMwMetaWearBoard* board, int32_t status) -> void {
        
    });
