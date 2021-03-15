/**
 * @copyright MbientLab License 
 * @file accelerometer_bosch.h
 * @brief Interacts with the supported Bosch accelerometers, currently BMI270, BMI160 and BMA255
 * @details The BMI160 and BMA255 sensors are identical except for the output data rates.  Functions that are generic for 
 * both accelerometers have "bosch" in the name whereas functions specific to either accelerometer are contain their model 
 * i.e. "bmi160" or "bma255".
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_MODULE_ACC_TYPE_BMI160 = 1;            ///< Constant identifying the BMI160 accelerometer module type
const uint8_t MBL_MW_MODULE_ACC_TYPE_BMA255 = 3;            ///< Constant identifying the BMA255 accelerometer module type
const uint8_t MBL_MW_MODULE_ACC_TYPE_BMI270 = 4;            ///< Constant identifying the BMI270 accelerometer module type

/**
 * Available wrist gestures on the BMI270
 */
typedef enum {
    MBL_MW_ACC_BOSCH_TYPEWRIST_NONE= 0,
    MBL_MW_ACC_BOSCH_TYPEWRIST_WEARK_WAKEUP,
    MBL_MW_ACC_BOSCH_TYPEWRIST_GESTURE,
} MblMwAccBoschTypewrist;

/**
 * Available activities on the BMI270
 */
typedef enum {
    MBL_MW_ACC_BOSCH_ACTIVITY_STILL= 0,
    MBL_MW_ACC_BOSCH_ACTIVITY_WALKING,
    MBL_MW_ACC_BOSCH_ACTIVITY_RUNNING,
    MBL_MW_ACC_BOSCH_ACTIVITY_UNKNOWN
} MblMwAccBoschActivity;

/**
 * Available wrist gestures on the BMI270
 */
typedef enum {
    MBL_MW_ACC_BOSCH_GESTURE_UNKNOWN= 0,   	///< Unknown gesture
    MBL_MW_ACC_BOSCH_GESTURE_PUSH_ARM_DOWN,	///< Arm pushed down gesture
    MBL_MW_ACC_BOSCH_GESTURE_PIVOT_UP,     	///< Pivot up gesture
    MBL_MW_ACC_BOSCH_GESTURE_SHAKE,        	///< Shake/jiggle gesture
    MBL_MW_ACC_BOSCH_GESTURE_ARM_FLICK_IN, 	///< Arm flick in gesture
    MBL_MW_ACC_BOSCH_GESTURE_ARM_FLICK_OUT	///< Arm flick out gesture
} MblMwAccBoschGesture;

/**
 * Available motion detectors on the Bosch accelerometers
 */
typedef enum {
    MBL_MW_ACC_BOSCH_MOTION_SIGMOTION= 0,     ///< Significant motion detector
    MBL_MW_ACC_BOSCH_MOTION_NOMOTION,         ///< No motion detector
    MBL_MW_ACC_BOSCH_MOTION_ANYMOTION,        ///< Any motion detector
} MblMwAccBoschMotion;

/**
 * Available g-ranges on the Bosch accelerometers
 */
typedef enum {
    MBL_MW_ACC_BOSCH_RANGE_2G= 0,    ///< +/- 2g
    MBL_MW_ACC_BOSCH_RANGE_4G,       ///< +/- 4g
    MBL_MW_ACC_BOSCH_RANGE_8G,       ///< +/- 8g
    MBL_MW_ACC_BOSCH_RANGE_16G       ///< +/- 16g
} MblMwAccBoschRange;

/**
 * Available ouput data rates on the BMI160 accelerometer
 */
typedef enum {
    MBL_MW_ACC_BMI160_ODR_0_78125Hz= 0,
    MBL_MW_ACC_BMI160_ODR_1_5625Hz,
    MBL_MW_ACC_BMI160_ODR_3_125Hz,
    MBL_MW_ACC_BMI160_ODR_6_25Hz,
    MBL_MW_ACC_BMI160_ODR_12_5Hz,
    MBL_MW_ACC_BMI160_ODR_25Hz,
    MBL_MW_ACC_BMI160_ODR_50Hz,
    MBL_MW_ACC_BMI160_ODR_100Hz,
    MBL_MW_ACC_BMI160_ODR_200Hz,
    MBL_MW_ACC_BMI160_ODR_400Hz,
    MBL_MW_ACC_BMI160_ODR_800Hz,
    MBL_MW_ACC_BMI160_ODR_1600Hz
} MblMwAccBmi160Odr;

/**
 * Available ouput data rates on the BMA255 accelerometer
 */
typedef enum {
    MBL_MW_ACC_BMA255_ODR_15_62Hz= 0,
    MBL_MW_ACC_BMA255_ODR_31_26Hz,
    MBL_MW_ACC_BMA255_ODR_62_5Hz,
    MBL_MW_ACC_BMA255_ODR_125Hz,
    MBL_MW_ACC_BMA255_ODR_250Hz,
    MBL_MW_ACC_BMA255_ODR_500Hz,
    MBL_MW_ACC_BMA255_ODR_1000Hz,
    MBL_MW_ACC_BMA255_ODR_2000Hz
} MblMwAccBma255Odr;

/**
 * Available ouput data rates on the BMI270 accelerometer
 */
typedef enum {
    MBL_MW_ACC_BMI270_ODR_0_78125Hz= 0,
    MBL_MW_ACC_BMI270_ODR_1_5625Hz,
    MBL_MW_ACC_BMI270_ODR_3_125Hz,
    MBL_MW_ACC_BMI270_ODR_6_25Hz,
    MBL_MW_ACC_BMI270_ODR_12_5Hz,
    MBL_MW_ACC_BMI270_ODR_25Hz,
    MBL_MW_ACC_BMI270_ODR_50Hz,
    MBL_MW_ACC_BMI270_ODR_100Hz,
    MBL_MW_ACC_BMI270_ODR_200Hz,
    MBL_MW_ACC_BMI270_ODR_400Hz,
    MBL_MW_ACC_BMI270_ODR_800Hz,
    MBL_MW_ACC_BMI270_ODR_1600Hz
} MblMwAccBmi270Odr;

/**
 * Operation modes for the step counter algorithm
 */
typedef enum {
    /** Recommended for most applications, well balanced between false positives and false negatives */
    MBL_MW_ACC_BMI160_STEP_COUNTER_MODE_NORMAL= 0,
    /** Recommended for light weighted persons, gives few false negatives but eventually more false positives */
    MBL_MW_ACC_BMI160_STEP_COUNTER_MODE_SENSITIVE,
    /** Gives few false positives but eventually more false negatives */
    MBL_MW_ACC_BMI160_STEP_COUNTER_MODE_ROBUST
} MblMwAccBmi160StepCounterMode;

/**
 * Calculation modes controlling the conditions that determine the sensor's orientation
 * @author Eric Tsai
 */
typedef enum {
    /** Default mode */
    MBL_MW_ACC_BOSCH_ORIENTATION_MODE_SYMMETRICAL,
    MBL_MW_ACC_BOSCH_ORIENTATION_MODE_HIGH_ASYMMETRICAL,
    MBL_MW_ACC_BOSCH_ORIENTATION_MODE_LOW_ASYMMETRICAL
} MblMwAccBoschOrientationMode;

/**
 * Available quiet times for tap detection
 * @author Eric Tsai
 */
typedef enum {
    MBL_MW_ACC_BOSCH_TAP_QUIET_TIME_30ms= 0,
    MBL_MW_ACC_BOSCH_TAP_QUIET_TIME_20ms
} MblMwAccBoschTapQuietTime;
/**
 * Available shock times for tap detection
 * @author Eric Tsai
 */
typedef enum {
    MBL_MW_ACC_BOSCH_TAP_SHOCK_TIME_50ms= 0,
    MBL_MW_ACC_BOSCH_TAP_SHOCK_TIME_75ms
} MblMwAccBoschTapShockTime;
/**
 * Available windows for double tap detection
 */
typedef enum {
    MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_50ms= 0,
    MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_100ms,
    MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_150ms,
    MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_200ms,
    MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_250ms,
    MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_375ms,
    MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_500ms,
    MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_700ms
} MblMwAccBoschDoubleTapWindow;

/**
 * Available windows for double tap detection
 */
typedef enum {
    MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_XYZ= 0,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_YZX,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_ZXY,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_XZY,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_YXZ,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_ZYX
} MblMwAccBoschAxisXyzRemap;

/**
 * Available windows for double tap detection
 */
typedef enum {
    MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_000= 0,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_100,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_110,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_101,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_010,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_011,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_001,
    MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_111
} MblMwAccBoschAxisXyzSign;

/**
 * Wrapper class encapsulating responses from any motion detection
 */
typedef struct {
    uint8_t sign;                   ///< Slope sign of the triggering motion, 0 if negative, non-zero if positive
    uint8_t x_axis_active;          ///< Non-zero if x-axis triggered the motion interrupt
    uint8_t y_axis_active;          ///< Non-zero if y-axis triggered the motion interrupt
    uint8_t z_axis_active;          ///< Non-zero if z-axis triggered the motion interrupt
} MblMwBoschAnyMotion;

/**
 * Wrapper class encapsulating responses from any motion detection
 */
typedef struct {
    uint8_t type;                   ///< Slope sign of the triggering motion, 0 if negative, non-zero if positive
    uint8_t gesture_code;          ///< Non-zero if x-axis triggered the motion interrupt
} MblMwBoschGestureType;

/**
 * Retrieves the data signal representing acceleration data from a Bosch accelerometer
 * This signal is timestamp,x,y,z acc data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 acceleration data signal
 * MblMwCartesianFloat is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_acceleration_data_signal(const MblMwMetaWearBoard* board);
/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_acc_bosch_get_packed_acceleration_data_signal instead.
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard* board);
/**
 * Variant of acceleration data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z acc data (it packs three acc data points in one timestamp)
 * @return Pointer to the data signal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_packed_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing data from the BMI160 step counter
 * This signal represents the number of steps a user has taken
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 step counter data signal
 * UINT32 is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi160_get_step_counter_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the BMI270 step counter
 * This signal represents the number of steps a user has taken
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI270 step counter data signal
 * UINT32 is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi270_get_step_counter_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the BMI160 step detector
 * This signal simply detects a step, it does not count it
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 step detector data signal
 * No return data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi160_get_step_detector_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the BMI270 step detector
 * This signal simply detects a step, it does not count it
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI270 step detector data signal
 * No return data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi270_get_step_detector_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the wrist gesture algorithm
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI270 step detector data signal
 * UINT32 is return signal data type represented by MblMwAccGestureCode
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi270_get_wrist_detector_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the activity detection algorithm
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI270 step detector data signal
 * UINT32 is return signal data type represented by MblMwAccActivityCode
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi270_get_activity_detector_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the orientation detection algorithm
 * This signal is identical to smartphone orientation (portrait, landscape)
 * The BMI270 does not support this function.
 * @param board     Calling object
 * @return Pointer to Bosch's orientation detection data signal
 * MblMwSensorOrientation is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_orientation_detection_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the motion detection algorithm
 * This signal retrieves either "no motion", "any motion" or "significant motion" depending on user settings
 * @param board     Calling object
 * @return Pointer to Bosch's motion detection data signal
 * UINT32 is return signal data type represented by MblMwAccMotionTypes
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_motion_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the tap detection algorithm
 * This signal retrieves the number of taps the board has been subjected to based on user settings
 * The BMI270 does not support this function.
 * @param board     Calling object
 * @return Pointer to Bosch's tap detection data signal
 * MblMwBoschTap is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_tap_data_signal(const MblMwMetaWearBoard* board);
/**
 * Sets the output data rate for the BMI160 accelerometer
 * The ODR sets the output data frequency in Hz.
 * See MblMwAccBmi160Odr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_acc_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi160Odr odr);
/**
 * Sets the output data rate for the BMI160 accelerometer
 * The ODR sets the output data frequency in Hz.
 * See MblMwAccBmi160Odr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_acc_bmi270_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi270Odr odr);
/**
 * Sets the output data rate for the BMI160 accelerometer
 * The ODR sets the output data frequency in Hz.
 * See MblMwAccBmi160Odr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_acc_bma255_set_odr(MblMwMetaWearBoard *board, MblMwAccBma255Odr odr);
/**
 * Sets the acceleration range
 * The range is in units of Gs between 2 and 16gs for Bosch sensors
 * See MblMwAccBoschRange for allowed values.
 * @param board     Pointer to the board to modify
 * @param range     Acceleration range to assign
 */
METAWEAR_API void mbl_mw_acc_bosch_set_range(MblMwMetaWearBoard *board, MblMwAccBoschRange range);
/**
 * Writes the acceleration settings to the sensor
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_write_acceleration_config(const MblMwMetaWearBoard *board);
/**
 * Sets the operational mode of the step counter
 * - Normal mode (default setting, recommended for most applications)
 * - Sensitive mode (can be used for light weighted, small persons)
 * - Robust mode (can be used, if many false positive detections are observed)
 * @param board     Board to modify
 * @param mode      New operation mode
 */
METAWEAR_API void mbl_mw_acc_bmi160_set_step_counter_mode(MblMwMetaWearBoard* board, MblMwAccBmi160StepCounterMode mode);
/**
 * Enables the BMI160 step counter
 * @param board     Board to modify
 */
METAWEAR_API void mbl_mw_acc_bmi160_enable_step_counter(MblMwMetaWearBoard* board);
/**
 * Disables the BMI160 step counter
 * @param board     Board to modify
 */
METAWEAR_API void mbl_mw_acc_bmi160_disable_step_counter(MblMwMetaWearBoard* board);
/**
 * Writes the step counter configuration to the sensor
 * Applies the MODE set by set_step_counter_mode()
 * @param board     Board to write to
 */
METAWEAR_API void mbl_mw_acc_bmi160_write_step_counter_config(const MblMwMetaWearBoard *board);
/**
 * Resets the BMI160 step counter (step counter = 0)
 * Does not disable the step counter
 * @param board     Board to reset
 */
METAWEAR_API void mbl_mw_acc_bmi160_reset_step_counter(const MblMwMetaWearBoard *board);
/**
 * Enables the BMI160 step detector 
 * A callback will notify the user when a step is detected but will NOT count steps
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_enable_step_detector(const MblMwMetaWearBoard *board);
/**
 * Disables the BMI160 step detector
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_disable_step_detector(const MblMwMetaWearBoard *board);
/**
 * Writes the step counter configuration to the sensor
 * Applies the MODE set by set_step_counter_mode()
 * @param board     Board to write to
 */
METAWEAR_API void mbl_mw_acc_bmi160_write_step_counter_config(const MblMwMetaWearBoard *board);
/**
 * Sets the watermark level of the step counter
 * The Step-counter will trigger output every time this number of steps are counted.
 * Holds implicitly a 20x factor, so the range is 0 to 20460, with resolution of 20 steps. 
 * If 0, the output is disabled. If 1, it will count to 20 steps. 
 * @param board     Board to modify
 * @param trigger   Number of steps
 */
METAWEAR_API void mbl_mw_acc_bmi270_set_step_counter_trigger(MblMwMetaWearBoard* board, uint16_t trigger);
/**
 * Enables the BMI270 step counter 
 * The trigger count will trigger the callback signal with the # of steps
 * @param board     Board to modify
 */
METAWEAR_API void mbl_mw_acc_bmi270_enable_step_counter(const MblMwMetaWearBoard *board);
/**
 * Disables the BMI270 step counter
 * @param board     Board to modify
 */
METAWEAR_API void mbl_mw_acc_bmi270_disable_step_counter(const MblMwMetaWearBoard *board);
/**
 * Writes the step counter configuration to the sensor
 * Applies the TRIGGER set by set_step_counter_trigger()
 * @param board     Board to write to
 */
METAWEAR_API void mbl_mw_acc_bmi270_write_step_counter_config(const MblMwMetaWearBoard *board);
/**
 * Resets the BMI270 step counter (step counter = 0)
 * Does not disable the step counter
 * @param board     Board to reset
 */
METAWEAR_API void mbl_mw_acc_bmi270_reset_step_counter(const MblMwMetaWearBoard *board);
/**
 * Reads the current step count.  The callback function will be called with:  
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param handler       Callback function that is executed when the task is finished
 */
METAWEAR_API void mbl_mw_acc_bmi270_read_step_counter(MblMwMetaWearBoard* board, void* context, MblMwFnBoardPtrInt handler);
/**
 * Sets the arm side the MetaWear is worn on for gesture recognition
 * Device in left (0 - false) or right (1 - true) arm. 
 * By default, the wearable device is assumed to be in left arm i.e. default value is 0 - false.
 * @param board     Pointer to the board to send the command to 
 * @param side      1 = Right arm, 0 = Left arm
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_gesture_armside(const MblMwMetaWearBoard *board, uint8_t side);
/**
 * Sets the tilt angle for gesture recognition
 * Sine of the minimum tilt angle in portrait down direction of the device when wrist is rolled away (roll-out) from user. 
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). 
 * Range is 1448 to 1774. Default value is 1774.
 * @param board     Pointer to the board to send the command to 
 * @param peak      Tilt Angle
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_gesture_peak(const MblMwMetaWearBoard* board, uint16_t peak);
/**
 * Value of minimum time difference between wrist roll-out and roll-in movement during flick gesture. 
 * Range is 3 to 5 samples at 50Hz (i.e. 0.06 to 0.1 seconds). Default value is 4 (i.e. 0.08 seconds).
 * @param board     Pointer to the board to send the command to 
 * @param sample      
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_gesture_samples(const MblMwMetaWearBoard* board, uint16_t samples);
/**
 * Sets the maximum time for the gesture recognition
 * Maximum time within which gesture movement has to be completed. 
 * Range is 150 to 250 samples at 50Hz (i.e. 3 to 5 seconds). Defualt value is 200 (i.e. 4 seconds).
 * @param board     Pointer to the board to send the command to 
 * @param duration      
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_gesture_duration(const MblMwMetaWearBoard* board, uint16_t duration);
/**
 * Writes the wrist gesture recognition configuration to the sensor
 * Applies the ARM SIDE, PEAK, SAMPLE and DURATION set by set_wrist_gesture_*()
 * @param board     Board to write to
 */
METAWEAR_API void mbl_mw_acc_bmi270_write_wrist_gesture_config(const MblMwMetaWearBoard* board);
/**
 * Enables the BMI270 wrist gesture recognition
 * @param board     Board to modify
 */
METAWEAR_API void mbl_mw_acc_bmi270_enable_wrist_gesture(const MblMwMetaWearBoard* board);
/**
 * Disables the BMI270 wrist gesture recognition
 * @param board     Board to modify
 */
METAWEAR_API void mbl_mw_acc_bmi270_disable_wrist_gesture(const MblMwMetaWearBoard* board);
/**
 * Sets the minimum angle change for wrist wakeup
 * Cosine of minimum expected attitude change of the device within 1 second time window when moving within focus position. 
 * The parameter is scaled by 2048 i.e. 2048 * cos(angle). Range is 1024 to 1774. Default is 1448.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Minimum angle
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_wakeup_angle_focus(const MblMwMetaWearBoard* board, uint16_t angle);
/**
 * Sets the maximum angle change for wrist wakeup
 * Cosine of minimum expected attitude change of the device within 1 second time window when moving from non-focus to focus position. 
 * The parameter is scaled by 2048 i.e. 2048 * cos(angle). Range is 1448 to 1856. Default value is 1774.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum angle
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_wakeup_angle_nonfocus(const MblMwMetaWearBoard* board, uint16_t angle);
/**
 * Sets the 
 * Sine of the maximum allowed downward tilt angle in landscape right direction of the device, when it is in focus position. 
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). Range is 700 to 1024. Default value is 1024.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum tilt angle in Landscape Right mode 
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_wakeup_tilt_lr(const MblMwMetaWearBoard* board, uint16_t angle);
/**
 * Sets the 
 * Sine of the maximum allowed downward tilt angle in landscape left direction of the device, when it is in focus position.
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). Range is 700 to 1024. Default value is 700.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum tilt angle in Landscape Left mode 
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_wakeup_tilt_ll(const MblMwMetaWearBoard* board, uint16_t angle);
/**
 * Sets the 
 * Sine of the maximum allowed backward tilt angle in portrait down direction of the device, when it is in focus position.
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). Range is 0 to179. Default value is 179.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum tilt angle in Portrait Down mode 
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_wakeup_tilt_pd(const MblMwMetaWearBoard* board, uint16_t angle);
/**
 * Sets the 
 * Sine of the maximum allowed forward tilt angle in portrait up direction of the device, when it is in focus position. 
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). Range is 1774 to 1978. Default value is 1925.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum tilt angle in Portrait Up mode 
 */
METAWEAR_API void mbl_mw_acc_bmi270_wrist_wakeup_tilt_pu(const MblMwMetaWearBoard* board, uint16_t angle);
/**
 * Writes the wrist gesture recognition configuration to the sensor
 * Applies the ARM SIDE, PEAK, SAMPLE and DURATION set by set_wrist_gesture_*()
 * @param board     Board to write to
 */
METAWEAR_API void mbl_mw_acc_bmi270_write_wrist_wakeup_config(const MblMwMetaWearBoard* board);
/**
 * Enables the BMI270 wrist wakeup recognition
 * @param board     Board to modify
 */
METAWEAR_API void mbl_mw_acc_bmi270_enable_wrist_wakeup(const MblMwMetaWearBoard* board);
/**
 * Disables the BMI270 wrist wakeup recognition
 * @param board     Board to modify
 */
METAWEAR_API void mbl_mw_acc_bmi270_disable_wrist_wakeup(const MblMwMetaWearBoard* board);
/**
 * Enables the BMI270 activity detector 
 * A callback will notify the user when an activity is detected (walking, running, still)
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi270_enable_activity_detection(const MblMwMetaWearBoard *board);
/**
 * Disables the BMI270 activity detector
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi270_disable_activity_detection(const MblMwMetaWearBoard *board);
/**
 * Enables the BMI270 step detector 
 * A callback will notify the user when a step is detected but will NOT count steps
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi270_enable_step_detector(const MblMwMetaWearBoard *board);
/**
 * Disables the BMI270 step detector
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi270_disable_step_detector(const MblMwMetaWearBoard *board);
/**
 * Downsampling for the BMI270 acc
 * See BMI270 datasheet for more details
 * @param board         Pointer to the board to send the command to
 * @param gyro_downs    Downsampling for Gyroscope (2**downs_gyro)
 * @param gyro_data     Selects filtered or unfiltered Gyroscope data for fifo
 * @param acc_downs     Downsampling for Accelerometer (2**downs_accel)
 * @param acc_data      Selects filtered or unfiltered Accelerometer data for fifo
 */
METAWEAR_API void mbl_mw_acc_bmi270_fifo_downs(const MblMwMetaWearBoard* board, uint8_t gyro_downs, uint8_t gyro_data, uint8_t acc_downs, uint8_t acc_data);
/**
 * Manual compensation for the BMI270 acc
 * The offset compensation registers have a width of 8 bit using two’s complement notation.
 * The offset resolution (LSB) is 3.9 mg and the offset range is +- 0.5 g
 * @param board     Pointer to the board to send the command to
 * @param x_offset  Offset compensation for Accelerometer X-axis
 * @param y_offset  Offset compensation for Accelerometer Y-axis
 * @param z_offset  Offset compensation for Accelerometer Z-axis
 */
METAWEAR_API void mbl_mw_acc_bmi270_acc_offsets(const MblMwMetaWearBoard* board, uint16_t x_offset, uint16_t y_offset, uint16_t z_offset);
/**
 * Remaps the BMI270 axis
 * If the coordinate system of the MetaWear differs from the sensor coordinate system described in the BMI270 datasheet,
 * the sensor axis must be remapped to use the orientation dependent features properly.
 * Axis remapping register allows the host to freely map individual axis to the coordinate system of the used platform. 
 * Individual axis can be mapped to any other defined axis. The sign value of the axis can be also configured. 
 * @param board     Pointer to the board to send the command to
 * @param map       X,Y,Z axis remap
 * @param sign      X,Y,Z axis flip/sign
 */
METAWEAR_API void mbl_mw_acc_bmi270_axis_remap(const MblMwMetaWearBoard *board, MblMwAccBoschAxisXyzRemap map, MblMwAccBoschAxisXyzSign sign);
/**
 * Set the hysteresis offset (degrees) for portrait/landscape detection
 * Upside/downside recognition hysteresis is not configurable.
 * See the BMI160 datasheet for more information. 
 * Not supported by the BMI270.
 * @param board         Calling object
 * @param hysteresis    New calculation mode
 */
METAWEAR_API void mbl_mw_acc_bosch_set_orientation_hysteresis(MblMwMetaWearBoard *board, float hysteresis);
/**
 * Set the orientation calculation mode
 * Options include: Symmetrical, High asymmetrical, Low asymmetrical
 * See the BMI160 datasheet for more information. 
 * Not supported by the BMI270.
 * @param board         Calling object
 * @param mode          New calculation mode
 */
METAWEAR_API void mbl_mw_acc_bosch_set_orientation_mode(MblMwMetaWearBoard *board, MblMwAccBoschOrientationMode mode);
/**
 * Writes the orientation detection settings to the board
 * Applies MODE and HYSTERESIS from set_orientation_hysteresis() and set_orientation_mode()
 * Not supported by the BMI270.
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_write_orientation_config(const MblMwMetaWearBoard *board);
/**
 * Enables orientation detection
 * Not supported by the BMI270.
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_enable_orientation_detection(const MblMwMetaWearBoard *board);
/**
 * Disables orientation detection
 * Not supported by the BMI270.
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_disable_orientation_detection(const MblMwMetaWearBoard *board);
/**
 * Sets the tap detector's quiet time parameter
 * Selects a tap quiet duration of ‘0’->30 ms, ‘1’->20 ms (i.e how long to wait for another tap)
 * The criteria for a double-tap are fulfilled if the second tap occurs after the time quiet time and within the tap window. 
 * If during the quiet time period (30/20ms) a tap occurs, it will be considered as a new tap.
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param time      New quiet time
 */
METAWEAR_API void mbl_mw_acc_bosch_set_quiet_time(MblMwMetaWearBoard *board, MblMwAccBoschTapQuietTime time);
/**
 * Sets the tap detector's shock time parameter
 * Selects a tap shock duration of ‘0’->50ms, ‘1’->75ms (i.e how long a tap can be)
 * This is the time it take the tap to "settle".
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param time      New shock time
 */
METAWEAR_API void mbl_mw_acc_bosch_set_shock_time(MblMwMetaWearBoard *board, MblMwAccBoschTapShockTime time);
/**
 * Sets the tap detector's double tap window
 * The time window between two taps is between 12.5ms and 500ms. 
 * The criteria for a double-tap are fulfilled if the second tap occurs after the quiet time and within the tap window.
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param window    New double tap window time
 */
METAWEAR_API void mbl_mw_acc_bosch_set_double_tap_window(MblMwMetaWearBoard *board, MblMwAccBoschDoubleTapWindow window);
/**
 * Sets the tap detector's threshold
 * Threshold of the single/double-tap interrupt corresponding to an acceleration difference.
 * The threshold is typically between 0.7g and 1.5g in 2g measurement range.
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param window    New threshold level
 */
METAWEAR_API void mbl_mw_acc_bosch_set_threshold(MblMwMetaWearBoard *board, float threshold);
/**
 * Sets the tap detector's double tap window
 * Applies the duration, quiet window, shock, and threshold from set_threshold(), set_double_tap_window(), set_shock_time(), set_quiet_time().
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param window    New double tap window time
 */
METAWEAR_API void mbl_mw_acc_bosch_write_tap_config(const MblMwMetaWearBoard *board);
/**
 * Enables the tap detector
 * Not supported by the BMI270.
 * @param board             Calling object
 * @param enable_single     0 to ignore single tap detection, non-zero to detect
 * @param enable_double     0 to ignore double tap detection, non-zero to detect
 */
METAWEAR_API void mbl_mw_acc_bosch_enable_tap_detection(const MblMwMetaWearBoard *board, uint8_t enable_single, uint8_t enable_double);
/**
 * Disable the tap detector
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param window    New double tap window time
 */
METAWEAR_API void mbl_mw_acc_bosch_disable_tap_detection(const MblMwMetaWearBoard *board);
/**
 * Sets the any motion detector's count parameter
 * Any-motion generates an interrupt when the absolute value of the acceleration exceeds a "threshold"
 * for a certain "count" of consecutive data points.
 * For the BMI160, it is a simple counter.
 * For the BMI270, it is expressed in 50 Hz samples (20 ms). Range is 0 to 163sec. Default value is 5=100ms.
 * @param board     Calling object
 * @param count     Number of consecutive slope data points that must be above the threshold
 */
METAWEAR_API void mbl_mw_acc_bosch_set_any_motion_count(MblMwMetaWearBoard *board, uint8_t count);
/**
 * Sets the any motion detector's threshold parameter
 * Any-motion generates an interrupt when the absolute value of the acceleration exceeds a "threshold"
 * for a certain "count" of consecutive data points.
 * For the BMI160, it is range-dependent (see datasheet). Default value is 0x14 = 5.11mg in 2g mode.
 * For the BMI270, range is 0 to 1g. Default value is 0xAA = 83mg.
 * @param board     Calling object
 * @param threshold Value that the slope data points must be above
 */
METAWEAR_API void mbl_mw_acc_bosch_set_any_motion_threshold(MblMwMetaWearBoard *board, float threshold);
/**
 * Sets the no motion detector's count parameter
 * @param board     Calling object
 * @param count     Number of consecutive slope data points that must be above the threshold
 */
METAWEAR_API void mbl_mw_acc_bosch_set_no_motion_count(MblMwMetaWearBoard *board, uint8_t count);
/**
 * Sets the no motion detector's threshold parameter
 * @param board     Calling object
 * @param threshold Value that the slope data points must be above
 */
METAWEAR_API void mbl_mw_acc_bosch_set_no_motion_threshold(MblMwMetaWearBoard *board, float threshold);
/**
 * Sets the sig motion detector's blocksize parameter
 * @param board     Calling object
 * @param threshold Value that the slope data points must be above
 */
METAWEAR_API void mbl_mw_acc_bosch_set_sig_motion_blocksize(MblMwMetaWearBoard *board, uint16_t blocksize);
/**
 * Writes the motion configuration to the remote device
 * Applies the threshold and count parameter or blocksize depending on motion type and settings from set_*_*()
 * For the BMI270, types include no motion, significant motion, and any motion. 
 * Ignored for other Bosch sensors.
 * @param board     Calling object
 * @param type      Type of motion requested
 */
METAWEAR_API void mbl_mw_acc_bosch_write_motion_config(const MblMwMetaWearBoard *board, MblMwAccBoschMotion type);
/**
 * Enables motion detection
 * The signal will callback if any motion is sensed based on the motion config
 * For the BMI270, signals include no motion, significant motion, and any motion. 
 * Ignored for other Bosch sensors.
 * @param board     Calling object
 * @param type      Type of motion requested
 */
METAWEAR_API void mbl_mw_acc_bosch_enable_motion_detection(const MblMwMetaWearBoard *board, MblMwAccBoschMotion type);
/**
 * Disables motion detection
 * @param board     Calling object
 * @param type      Type of motion requested
 */
METAWEAR_API void mbl_mw_acc_bosch_disable_motion_detection(const MblMwMetaWearBoard *board, MblMwAccBoschMotion type);
/**
 * Switches the accelerometer to active mode
 * When in active mode, the accelerometer cannot be configured.
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_start(const MblMwMetaWearBoard *board);
/**
 * Switches the accelerometer to standby mode (low power state)
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_stop(const MblMwMetaWearBoard *board);
/**
 * Enables acceleration sampling
 * The board will start gathering data from the accelerometer
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_enable_acceleration_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables acceleration sampling
 * The board will stop gathering data from the accelerometer
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_disable_acceleration_sampling(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
