.. highlight:: cpp

Accelerometer
=============
All boards come with an accelerometer. An accelerometer is an electromechanical device that will measure acceleration forces. 
These forces may be static, like the constant force of gravity pulling at your feet, or they could be dynamic - caused by moving or vibrating the accelerometer.

Acceleration is measured in units of gravities (g) or units of m/s2. One g unit = 9.81 m/s2.

The specific accelerometer model varies amongst the boards, howevever the API provides accelerometer 
agnostic functions in the `accelerometer.h <https://mbientlab.com/docs/metawear/cpp/latest/accelerometer_8h.html>`_ header file that can be safely used 
with all supported accelerometers.

Users can check which accelerometer is on their board at runtime to determine the appropriate accelerometer specific functions they need to use, if 
necessary. ::

    #include "metawear/core/module.h"

    #include "metawear/sensor/accelerometer_bosch.h"
    #include "metawear/sensor/accelerometer_mma8452q.h"
     
    void check_acc_type(MblMwMetaWearBoard* board) {
        auto acc_type= mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_ACCELEROMETER);
        switch(acc_type) {
        case MBL_MW_MODULE_TYPE_NA:
            // should never reach this case statement
            printf("No accelerometer on this board\n");
            break;
        case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
            printf("Acc Type = MMA8452Q\n");
            break;
        case MBL_MW_MODULE_ACC_TYPE_BMI160:
            printf("Acc Type = BMI160\n");
            break;
        case MBL_MW_MODULE_ACC_TYPE_BMI270:
            printf("Acc Type = BMI160\n");
            break;
        case MBL_MW_MODULE_ACC_TYPE_BMA255:
            printf("Acc Type = BMA255\n");
            break;
        default:
            printf("Unknown type\n");
            break;
        }

Acceleration Sampling
---------------------
Acceleration sampling measures the current acceleration forces at periodic intervals.  To enable acceleration sampling, call 
`mbl_mw_acc_enable_acceleration_sampling <https://mbientlab.com/docs/metawear/cpp/latest/accelerometer_8h.html#a58272eea512ca22d0de2ae0db0e9f867>`_ 
before starting the accelerometer.

Linear acceleration is represented with the 
`MblMwCartesianFloat <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwCartesianFloat.html>`_ struct and the values are in units of Gs.  The 
``x``, ``y``, and ``z`` fields contain the acceleration in that direction. ::

    #include "metawear/core/datasignal.h"
    #include "metawear/core/types.h"

    #include "metawear/sensor/accelerometer.h"

    void enable_acc_sampling(MblMwMetaWearBoard* board) {
        auto data_handler = [](const MblMwData* data) -> void {
            // Cast value to MblMwCartesianFloat*
            auto acceleration = (MblMwCartesianFloat*) data->value;
            printf("(%.3fg, %.3fg, %.3fg)\n", acceleration->x, acceleration->y, acceleration->z);
        };

        auto acc_signal= mbl_mw_acc_get_acceleration_data_signal(board);    
        mbl_mw_datasignal_subscribe(acc_signal, data_handler);

        mbl_mw_acc_enable_acceleration_sampling(board);
        mbl_mw_acc_start(board);
    }

Configuration
^^^^^^^^^^^^^
Users can configure the output data rate and the sampling range; these parameters control the sampling rate and the data range and resolution 
respectively.  After configuring the settings, call 
`mbl_mw_acc_write_acceleration_config <https://mbientlab.com/docs/metawear/cpp/latest/accelerometer_8h.html#a7f3339b25871344518175f97ae7c95b7>`_ to 
write the configuration to the sensor. ::

    #include "metawear/sensor/accelerometer.h"
    
    void configure_acc(MblMwMetaWearBoard* board) {
        // Set ODR to 25Hz or closest valid frequency
        mbl_mw_acc_set_odr(board, 25.f);
    
        // Set range to +/-4g or closest valid range
        mbl_mw_acc_set_range(board, 4.f);
        
        // Write the config to the sensor
        mbl_mw_acc_write_acceleration_config(board);
    }

High Frequency Stream
^^^^^^^^^^^^^^^^^^^^^
Firmware v1.2.3+ contains a packed mode for the accelerometer which combines 3 acceleration data samples into 1 ble packet allowing the board to 
stream data at a throughput higher than 100Hz.  This special data signal is retrieved from the 
`mbl_mw_acc_get_high_freq_acceleration_data_signal <https://mbientlab.com/docs/metawear/cpp/latest/accelerometer_8h.html#a9203ed5a20d63f6c37ae173aabaaa287>`_ function 
and is only for streaming; do not use it with data processing or logging.  ::

    auto high_freq_signal= mbl_mw_acc_get_high_freq_acceleration_data_signal(board);
    mbl_mw_datasignal_subscribe(high_freq_signal, [](const MblMwData* data) -> void {
        // Cast value to MblMwCartesianFloat*
        auto acceleration = (MblMwCartesianFloat*) data->value;
        printf("(%.3fg, %.3fg, %.3fg)\n", acceleration->x, acceleration->y, acceleration->z);
    });

    mbl_mw_acc_set_odr(board, 200.f);
    mbl_mw_acc_enable_acceleration_sampling(board);
    mbl_mw_acc_start(board);


Wrist Wear Gestures
---------------------
The BMI270 is designed for Wear OS by GoogleTM7 and features wrist gestures such as flick in/out, push arm down/pivot up, wrist jiggle/shake that help navigate the smartwatch.

See https://support.google.com/wearos/answer/6312406?hl=en

For flick-in detection, the user must slowly turn the wrist away from the body (i.e. roll-out shown with a light-grey arrow in) and then quickly bring it back (i.e. roll-in shown with a darker-black arrow in to its original position.

For flick-out detection, the user must quickly turn the wrist away from the body (i.e. roll-out shown with a darker-black arrow in above picture) and then slowly bring it back (i.e. roll-in shown with a light-grey arrow in above picture) to its original position.

The speed of the roll-out and roll-in movements determine if the user performed a flick-in or a flick-out movement. ::

    // Start the accelerometer
    mbl_mw_acc_start(board)
    // Configure
    mbl_mw_acc_bmi270_wrist_gesture_armside(board, 0) // left arm
    mbl_mw_acc_bmi270_write_wrist_gesture_config(board)
    // Get gesture signal
    auto gesture_signal = mbl_mw_acc_bmi270_get_wrist_detector_data_signal(board)
    mbl_mw_datasignal_subscribe(gesture_sig, [](const MblMwData* data) -> void {
        printf("wakeup= %d\n", *((MblMwBoschGestureType*) data->value));
    });   
    // Start detecting motion and turn on acc
    mbl_mw_acc_bmi270_enable_wrist_gesture(board)
    
There are config functions for the wrist wear feature: ::

    mbl_mw_acc_bmi270_wrist_gesture_peak()
    mbl_mw_acc_bmi270_wrist_gesture_samples()
    mbl_mw_acc_bmi270_wrist_gesture_duration()

Activity Detector
------------------
The BMI270 can detect simple user activities (unknown, still, walking, running) and can send an interrupt if those are changed, e.g. from walking to running or vice versus. ::

    // Start the accelerometer
    mbl_mw_acc_start(board)
    // Get activity signal
    auto activity_sig= mbl_mw_acc_bmi270_get_activity_detector_data_signal(board);
    mbl_mw_datasignal_subscribe(activity_sig, [](const MblMwData* data) -> void {
       printf("activity detected= %d\n", *((uint32_t*) data->value));
    });
    // Start detecting motion and turn on acc
    mbl_mw_acc_bmi270_enable_activity_detection(board)

Wrist Wear Wakeup 
----------------------
The BMI270 has a wrist wear wakeup feature that is designed to detect any natural way of user moving the hand to see the watch dial when wearing a classical wrist watch. 

The feature is intended to be used as wakeup gesture (i.e. for triggering screen-on or screen-off) in wrist wearable devices.

This feature has dependency on the device orientation in the user system. Implementation of the feature to detect gesture assumes that the sensor co-ordinate frame is aligned with the device/system co- ordinate frame. The assumed default device/system co-ordinate frame is depicted below. 

Please refer to `this section <https://mbientlab.com/documents/metawear/cpp/latest/accelerometer__bosch_8h.html#aca2fa97988a33550e20b02c816c6b91f>`_ regarding axis remapping. ::

    // Start the accelerometer
    mbl_mw_acc_start(device.board)
    // Get gesture signal
    auto wrist_sig= mbl_mw_acc_bmi270_get_wrist_detector_data_signal(board);
    mbl_mw_datasignal_subscribe(wrist_sig, [](const MblMwData* data) -> void {
        printf("wakeup= %d\n", *((MblMwBoschGestureType*) data->value));
    });
    // Start detecting motion and turn on acc
    mbl_mw_acc_bmi270_enable_wrist_wakeup(board)
    
There are config functions for the wrist wear feature: ::

    mbl_mw_acc_bmi270_wrist_wakeup_angle_focus()
    mbl_mw_acc_bmi270_wrist_wakeup_angle_nonfocus()
    mbl_mw_acc_bmi270_wrist_wakeup_tilt_lr()
    mbl_mw_acc_bmi270_wrist_wakeup_tilt_ll()
    mbl_mw_acc_bmi270_wrist_wakeup_tilt_pd()
    mbl_mw_acc_bmi270_wrist_wakeup_tilt_pu()

Motion Detector
----------------
The BMI270 can detect significant motion (android motion), any motion (high acc motion) or no motion. The accelerometer must be at least running at 25Hz.

Detect Any Motion
^^^^^^^^^^^^^^^^^^^
The anymotion detection uses the slope between two acceleration signals to detect changes in motion. ::

    // Start the accelerometer
    mbl_mw_acc_start(board)
    // Set any motion config - acc must be on for this
    mbl_mw_acc_bosch_set_any_motion_count(board, 5)
    mbl_mw_acc_bosch_set_any_motion_threshold(board, 170.0)
    mbl_mw_acc_bosch_write_motion_config(board, MBL_MW_ACC_BOSCH_MOTION_ANYMOTION)
    // Get any motion signal
    auto any_motion= mbl_mw_acc_bosch_get_motion_data_signal(board);
    mbl_mw_datasignal_subscribe(any_motion, [](const MblMwData* data) -> void {
        printf("motion detected= %d\n", *((uint32_t*) data->value));
    });
    // Start detecting motion
    mbl_mw_acc_bosch_enable_motion_detection(board, MBL_MW_ACC_BOSCH_MOTION_ANYMOTION)
    
Detect No Motion
^^^^^^^^^^^^^^^^^^^
The nomotion detection can detect when there is no motion for a certain amount of time. ::

    // Start the accelerometer
    mbl_mw_acc_start(board)
    // Set any motion config - acc must be on for this
    mbl_mw_acc_bosch_set_no_motion_count(board, 5)
    mbl_mw_acc_bosch_set_no_motion_threshold(board, 144.0)
    mbl_mw_acc_bosch_write_motion_config(board, MBL_MW_ACC_BOSCH_MOTION_NOMOTION)
    // Get any motion signal
    auto no_motion = mbl_mw_acc_bosch_get_motion_data_signal(board)
    mbl_mw_datasignal_subscribe(no_motion, [](const MblMwData* data) -> void {
        printf("no motion detected= %d\n", *((uint32_t*) data->value));
    });
    // Start detecting motion and turn on acc
    mbl_mw_acc_bosch_enable_motion_detection(board, MBL_MW_ACC_BOSCH_MOTION_NOMOTION)

Detect Significant Motion
^^^^^^^^^^^^^^^^^^^^^^^^^^
The significant motion interrupt implements the interrupt required for motion detection in Android 4.3 and greater: https://source.android.com/devices/sensors/sensor-types.html#significant_motion.
A significant motion is a motion due to a change in the user location.

Examples of such significant motions are walking or biking, sitting in a moving car, coach or train, etc. 
Examples of situations that does typically not trigger significant motion include phone in pocket and person is stationary or phone is at rest on a table which is in normal office use. . ::
    
    // Start the accelerometer
    mbl_mw_acc_start(board)
    // Set any motion config - acc must be on for this
    mbl_mw_acc_bosch_set_sig_motion_blocksize(board, 250)
    mbl_mw_acc_bosch_write_motion_config(board, MBL_MW_ACC_BOSCH_MOTION_SIGMOTION)
    // Get any motion signal
    auto sig_motion = mbl_mw_acc_bosch_get_motion_data_signal(board)
    mbl_mw_datasignal_subscribe(sig_motion, [](const MblMwData* data) -> void {
        printf("no motion detected= %d\n", *((uint32_t*) data->value));
    });
    // Start detecting motion and turn on acc
    mbl_mw_acc_bosch_enable_motion_detection(board, MBL_MW_ACC_BOSCH_MOTION_SIGMOTION)

Step Counter
------------
The BMI160 accelerometer comes with a built in step counter.  It has three operation modes that configure the sensitivity and robustness of the counter:

=========  ==============================================================================================
Mode       Description
=========  ==============================================================================================
Normal     Balanced between false positives and false negatives, recommended for most applications
Sensitive  Few false negatives but eventually more false positives, recommended for light weighted people
Robust     Few false positives but eventually more false negatives
=========  ==============================================================================================

When you have set the operation mode, call 
`mbl_mw_acc_bmi160_write_step_counter_config <https://mbientlab.com/docs/metawear/cpp/latest/accelerometer__bosch_8h.html#ab4fa1b742920e8aefca8bf5e59237f8e>`_ to save the configuration to the board. ::

    #include "metawear/sensor/accelerometer_bosch.h"

    mbl_mw_acc_bmi160_set_step_counter_mode(board, MBL_MW_ACC_BMI160_STEP_COUNTER_MODE_SENSITIVE);
    mbl_mw_acc_bmi160_write_step_counter_config(board);

The BMI270 accelerometer does not support step counter modes.

Reading The Counter
^^^^^^^^^^^^^^^^^^^
One way to retrieve step counts is to periodcally read the step counter.  To read the step counter, call 
`mbl_mw_datasignal_read <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a0a456ad1b6d7e7abb157bdf2fc98f179>`_ with the step counter data signal.

The counter is not enabled by default so you will need enable it by calling 
`mbl_mw_acc_bmi160_enable_step_counter <https://mbientlab.com/docs/metawear/cpp/latest/accelerometer__bosch_8h.html#ad4ef124ad3ef8ef51667e738331333b8>`_ when configuring the board. ::

    // enable the counter
    mbl_mw_acc_bmi160_enable_step_counter(board);
    // mbl_mw_acc_bmi270_enable_step_counter(board);
    mbl_mw_acc_bmi160_write_step_counter_config(board);
    // mbl_mw_acc_bmi270_write_step_counter_config(board);
    mbl_mw_acc_start(board);
    // read step counter
    auto step_counter= mbl_mw_acc_bmi160_get_step_counter_data_signal(board);
    // auto step_counter= mbl_mw_acc_bmi270_get_step_counter_data_signal(board);
    mbl_mw_datasignal_subscribe(step_counter, [](const MblMwData* data) -> void {
        printf("steps= %d\n", *((uint32_t*) data->value));
    });
    mbl_mw_datasignal_read(adc_signal);

For the BMI270, you can call `mbl_mw_acc_bmi270_enable_step_counter <https://mbientlab.com/documents/metawear/cpp/latest/accelerometer__bosch_8h.html#a48e850d6bdb4b7084c735885465fc1c7>`_ when configuring the board.

Using The Detector
^^^^^^^^^^^^^^^^^^
Alternatively, you can receive notifications for each step detected by calling 
`mbl_mw_acc_bmi160_enable_step_detector <https://mbientlab.com/docs/metawear/cpp/latest/accelerometer__bosch_8h.html#a3f1b82cb1d70334eeb7b604431e15f20>`_ instead. ::

    auto detector= mbl_mw_acc_bmi160_get_step_detector_data_signal(board);
    mbl_mw_datasignal_subscribe(detector, [](const MblMwData* data) -> void {
        printf("step detected\n");
    });
    mbl_mw_acc_bmi160_enable_step_detector(board);
    // mbl_mw_acc_bmi270_enable_step_detector(board);
    mbl_mw_acc_bosch_start(board);

For the BMI270, the detector will not send notifications every step but instead every 20*X steps: ::

    mbl_mw_acc_start(board)
    // Write the trigger for the step counter
    mbl_mw_acc_bmi270_set_step_counter_trigger(board, 1) //every 20 steps
    mbl_mw_acc_bmi270_write_step_counter_config(board)
    // Reset the counter
    mbl_mw_acc_bmi270_reset_step_counter(board)
    // Get the step signal
    auto detector= mbl_mw_acc_bmi270_get_step_detector_data_signal(board);
    mbl_mw_datasignal_subscribe(detector, [](const MblMwData* data) -> void {
        printf("another 20 steps detected\n");
    });
    // Start detecting motion and turn on acc
    mbl_mw_acc_bmi270_enable_step_counter(board)

Orientation Detection
---------------------
The orientation detector alerts you when the sensor's orientation changes between portrait/landscape and front/back.  Data is represented as an 
`MblMwSensorOrientation <https://mbientlab.com/docs/metawear/cpp/0/types_8h.html#a2e83167b55d36e1d48d100f342ad529c>`_ enum.

This feature is currently only supported on devices using the BMI160 or BMA255 accelerometers. It is not supported on the BMI270.

::

    #include "metawear/core/datasignal.h"
    #include "metawear/core/types.h"
    #include "metawear/sensor/accelerometer_bosch.h"

    auto signal = mbl_mw_acc_bosch_get_orientation_detection_data_signal(board);
    mbl_mw_datasignal_subscribe(signal, [](const MblMwData* data) {
        cout << "orientation = " << *((MblMwSensorOrientation*)data->value) << endl;
    });
    mbl_mw_acc_bosch_enable_orientation_detection(board);
    mbl_mw_acc_bosch_start(board);

