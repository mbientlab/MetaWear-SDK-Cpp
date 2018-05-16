.. highlight:: cpp

Sensor Fusion
=============

The `sensor_fusion.h <https://mbientlab.com/docs/metawear/cpp/latest/sensor__fusion_8h.html>`_ header file interfaces with the sensor fusion algorithm 
running on MetaMotion boards.  When using the sensor fusion algorithm, it is important that you do not simultaenously use the 
Accelerometer, Gyro, and Magnetometer modules; the algorithm configures those sensors internally based on the selected fusion mode.

To activate the sensor fusion algorithm, first set the fusion mode and data ranges, then subscribe to and enable the desired output data, and finally, 
call `mbl_mw_sensor_fusion_start <https://mbientlab.com/docs/metawear/cpp/latest/sensor__fusion_8h.html#a941e51e4831b5a7a2426ecf328dddddf>`_.

Mode
----
The sensor fusion algorithm has 4 
`fusion modes <https://mbientlab.com/docs/metawear/cpp/latest/sensor__fusion_8h.html#ac5064d8edcb6ffa988f25f4f66e09c48>`_, listed in the below table:

======== ==========================================================================
Mode     Description                             
======== ==========================================================================
NDoF     Calculates absolute roeintation from accelerometer, gyro, and magnetometer
IMUPlus  Calculates relative orientation in space from accelerometer and gyro data
Compass  Determines geographic direction from th Earth's magnetic field
M4G      Similar to IMUPlus except rotation is detected with the magnetometer
======== ==========================================================================

The mode is set with 
`mbl_mw_sensor_fusion_set_mode <https://mbientlab.com/docs/metawear/cpp/latest/sensor__fusion_8h.html#a138a2d52134dee3772f0df3f9a7d9098>`_ and written 
to the board by calling 
`mbl_mw_sensor_fusion_write_config <https://mbientlab.com/docs/metawear/cpp/latest/sensor__fusion_8h.html#a09bb5d96b305c0ee0cf57e2a37300295>`_.  Before 
writing the configuration, you can also set the acceleration and rotation ranges of the accelerometer and gyroscope respectively. ::

    #include "metawear/sensor/sensor_fusion.h"

    void configure_sensor_fusion(MblMwMetaWearBoard* board) {
    	// set fusion mode to ndof (n degress of freedom)
    	mbl_mw_sensor_fusion_set_mode(board, MBL_MW_SENSOR_FUSION_MODE_NDOF);
    	// set acceleration rangen to +/-8G, note accelerometer is configured here
    	mbl_mw_sensor_fusion_set_acc_range(board, MBL_MW_SENSOR_FUSION_ACC_RANGE_8G);
    	// write changes to the board
    	mbl_mw_sensor_fusion_write_config(board);
    }

Data
----
The sensor fusion algorithm provides raw acceleration, rotation, and magnetic field values along with quaternion values and Euler angles.  Furthermore, 
the source of acceleration can be separated into gravity and linear acceleration and both values are also provided.  Keep in mind that each sensor  
fusion mode has different sets of available data and produces it at different rates.

======== ===== ===== ====
Mode     Acc   Gyro  Mag                       
======== ===== ===== ====
NDoF     100Hz 100Hz 25Hz
IMUPlus  100Hz 100Hz N/A
Compass  25Hz  N/A   25Hz
M4G      50Hz  N/A   50Hz
======== ===== ===== ====

Also note that the units and type casting of the sensor fusion data is different for each type of data..

============== ======= ============================
Data           Units   Casted Data
============== ======= ============================
Acceleration    g      MblMwCorrectedCartesianFloat
Rotation       deg/s   MblMwCorrectedCartesianFloat
Magnetic Field uT      MblMwCorrectedCartesianFloat
Quaternion      None   MblMwQuaternion
Euler Angles   degrees MblMwEulerAngles
Linear Acc      g      MblMwCartesianFloat
Gravity         g      MblMwCartesianFloat
============== ======= ============================

::

    #include <cstdio>

    #include "metawear/core/datasignal.h"
    #include "metawear/core/data.h"
    #include "metawear/core/types.h"

    void stream_quaternion(MblMwMetaWearBoard* board) {
    	auto quaternion = mbl_mw_sensor_fusion_get_data_signal(board, 
                MBL_MW_SENSOR_FUSION_DATA_QUATERNION);
    	mbl_mw_datasignal_subscribe(quaternion, [](const MblMwData* data) -> void {
    		MblMwQuaternion* quaternion = (MblMwQuaternion*) data->value;
    		std::printf("{w: %.3f, x: %.3f, y: %.3f, z: %.3f}\n", 
    			quaternion->w, quaternion->x, quaternion->y, quaternion->z);
    	});

    	mbl_mw_sensor_fusion_enable_data(board, MBL_MW_SENSOR_FUSION_DATA_QUATERNION);
    	mbl_mw_sensor_fusion_start(board);
    }
