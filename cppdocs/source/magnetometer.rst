.. highlight:: cpp

Magnetometer
============
MetaWear CPro boards have a `BMM150 <https://www.bosch-sensortec.com/bst/products/all_products/bmm150>`_ geomagnetic sensor for sensing magnetic fields.  
Functions communicating with the magnetometer are defined in the 
`magnetometer_bmm150.h <https://mbientlab.com/docs/metawear/cpp/latest/magnetometer__bmm150_8h.html>`_.


Power Modes
-----------
The `BMM150 <https://www.bosch-sensortec.com/bst/products/all_products/bmm150>`_ magnetometer has 4 recommended power modes:

================ ==== =============== ===============================
Setting          ODR  Average Current Noise 
================ ==== =============== ===============================
LOW_POWER        10Hz 170µA           1.0µT (xy axis), 1.4µT (z axis)
REGULAR          10Hz 0.5mA           0.6µT
ENHANCED_REGULAR 10Hz 0.8mA           0.5µT
HIGH_ACCURACY    20Hz 4.9mA           0.3µT 
================ ==== =============== ===============================

Constants identifying these power modes are defined by the  
`MblMwMagBmm150Preset <https://mbientlab.com/docs/metawear/cpp/latest/magnetometer__bmm150_8h.html#ab08170453efe163de38cf9f78d60ae2a>`_ enum.  
For most use cases, ``MWL_MW_MAG_BMM_150_PP_LOW_POWER`` is the recommended power mode. ::

    #include "metawear/sensor/magnetometer_bmm150.h"
    
    void setup_magnetometer(MblMwMetaWearBoard* board) {
        // Use low power mode
        mbl_mw_mag_bmm150_set_power_preset(board, MWL_MW_MAG_BMM_150_PP_LOW_POWER);
    }

B Field Sampling
----------------
B field sampling measures the magnetic field strength for the XYZ directions.  To enable **B** field sampling, call 
`mbl_mw_mag_bmm150_enable_b_field_sampling <https://mbientlab.com/docs/metawear/cpp/latest/magnetometer__bmm150_8h.html#ac0c27f990e0e9eab6a46afa2d24df428>`_ 
before starting the magnetometer.

Field strength is represented by the 
`MblMwCartesianFloat <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwCartesianFloat.html>`_ struct and is in units of micro Teslas (µT).  The 
``x``, ``y``, and ``z`` fields contain the **B** field strength in that direction. ::

    #include "metawear/core/datasignal.h"
    #include "metawear/core/types.h"
    
    #include "metawear/sensor/magnetometer_bmm150.h"
    
    void bfield_stream(MblMwMetaWearBoard* board) {
        static auto data_handler = [](const MblMwData* data) -> void {
            // Cast value to MblMwCartesianFloat*
            auto bfield = (MblMwCartesianFloat*) data->value;
            printf("(%.3f, %.3f, %.3f)uT\n", bfield ->x, bfield->y, bfield->z);
        };
    
        auto bfield_signal = mbl_mw_mag_bmm150_get_b_field_data_signal(board);
        mbl_mw_datasignal_subscribe(bfield_signal, data_handler);
    
        mbl_mw_mag_bmm150_enable_b_field_sampling(board);
        mbl_mw_mag_bmm150_start(board);
    }


