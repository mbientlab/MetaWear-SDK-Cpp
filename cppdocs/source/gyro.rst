.. highlight:: cpp

Gyro
====
MetaWear RG, RPro, C, and CPro come with a `Bosch BMI160 <http://www.bosch-sensortec.com/bst/products/all_products/bmi160>`_ 6-axis IMU.  The gyro 
functionality of this sensor is accessed by the functions in the 
`gyro_bmi160.h <https://mbientlab.com/docs/metawear/cpp/latest/gyro__bmi160_8h.html>`_ header file.


Configuration
-------------
The gyro's data sampling rate and the range/resolution of the angular velocity are controlled by theoutput data rate and sampling range respectively.  
After selecting the desired settings in the API, call 
`mbl_mw_gyro_bmi160_write_config <https://mbientlab.com/docs/metawear/cpp/latest/gyro__bmi160_8h.html#aeede6e8a6aa6218106bffcb9f152488e>`_. to write the 
chnges to the sensor. ::

    #include "metawear/sensor/gyro_bmi160.h"
    
    void gyro_config(MblMwMetaWearBoard* board) {
        // Set ODR to 50Hz
        mbl_mw_gyro_bmi160_set_odr(board, MBL_MW_GYRO_BMI160_ODR_50HZ);
    
        // Set data range to +/125 degrees per second
        mbl_mw_gyro_bmi160_set_range(board, MBL_MW_GYRO_BMI160_FSR_125DPS);
    
        // Write the changes to the sensor
        mbl_mw_gyro_bmi160_write_config(board);
    }


Rotation Rate Sampling
----------------------
Rotate rate sampling measures the rate of change of the pitch, yaw, and roll angles, in other words, the angular velocity of the spin around the XYZ 
axes.  To enable rotation rate sampling, call 
`mbl_mw_gyro_bmi160_enable_rotation_sampling <https://mbientlab.com/docs/metawear/cpp/latest/gyro__bmi160_8h.html#a647e13739d2ebaaccd05fa93daa3ff6b>`_ 
before starting the gyro.

Angular velocity is represented by the 
`MblMwCartesianFloat <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwCartesianFloat.html>`_ struct and is in units of degrees per second.  
The ``x``, ``y``, and ``z`` fields contain the angular velocity of the spin around that axis.  ::

    #include "metawear/sensor/gyro_bmi160.h"
    
    void gyro_rot_rate_stream(MblMwMetaWearBoard* board) {
        static auto rot_handler = [](const MblMwData* data) -> void {
            // Cast value to MblMwCartesianFloat*
            auto rot_rate = (MblMwCartesianFloat*)data->value;
            printf("(%.3fdps, %.3fdps, %.3fdps)\n", rot_rate->x, rot_rate->y, rot_rate->z);
        };
        
        auto state_signal = mbl_mw_gyro_bmi160_get_rotation_data_signal(board);
        mbl_mw_datasignal_subscribe(state_signal, rot_handler);
    
        mbl_mw_gyro_bmi160_enable_rotation_sampling(board);
        mbl_mw_gyro_bmi160_start(board);
    }

High Frequency Stream
^^^^^^^^^^^^^^^^^^^^^
Firmware v1.2.3+ contains a packed mode for the hyro which combines 3 rotation data samples into 1 ble packet allowing the board to stream data at a
throughput higher than 100Hz.  This special data signal is retrieved from the 
`mbl_mw_gyro_bmi160_get_rotation_data_signal <https://mbientlab.com/docs/metawear/cpp/latest/gyro__bmi160_8h.html#a4b5db7b9449981c6405afabeb2da50d8>`_ 
function and is only for streaming; do not use it with data processing or logging.  ::

    auto high_freq_signal= mbl_mw_gyro_bmi160_get_rotation_data_signal(board);
    mbl_mw_datasignal_subscribe(high_freq_signal, [](const MblMwData* data) -> void {
        // Cast value to MblMwCartesianFloat*
        auto acceleration = (MblMwCartesianFloat*) data->value;
        printf("(%.3fdps, %.3fdps, %.3fdps)\n", acceleration->x, acceleration->y, acceleration->z);
    });

    mbl_mw_gyro_bmi160_set_odr(board, 200);
    mbl_mw_gyro_bmi160_enable_rotation_sampling(board);
    mbl_mw_gyro_bmi160_start(board);

