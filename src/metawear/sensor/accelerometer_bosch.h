/**
 * @copyright MbientLab License 
 * @file accelerometer_bosch.h
 * @brief Interacts with the supported Bosch accelerometers, currenly BMI160 and BMA255
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
 * @author Eric Tsai
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
 * Wrapper class encapsulating responses from any motion detection
 */
typedef struct {
    uint8_t sign;                   ///< Slope sign of the triggering motion, 0 if negative, non-zero if positive
    uint8_t x_axis_active;          ///< Non-zero if x-axis triggered the motion interrupt
    uint8_t y_axis_active;          ///< Non-zero if y-axis triggered the motion interrupt
    uint8_t z_axis_active;          ///< Non-zero if z-axis triggered the motion interrupt
} MblMwBoschAnyMotion;

/**
 * Retrieves the data signal representing acceleration data from a Bosch accelerometer
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 acceleration data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_acceleration_data_signal(const MblMwMetaWearBoard* board);
/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_acc_bosch_get_packed_acceleration_data_signal instead.
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard* board);
/**
 * Variant of acceleration data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * @return Pointer to the data singal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_packed_acceleration_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing data from the BMI160 step counter
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 step counter data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi160_get_step_counter_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the BMI160 step detector
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 step detector data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi160_get_step_detector_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the orientation detection algorithm
 * @param board     Calling object
 * @return Pointer to Bosch's orientation detection data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_orientation_detection_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the motion detection algorithm
 * @param board     Calling object
 * @return Pointer to Bosch's motion detection data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_motion_data_signal(const MblMwMetaWearBoard* board);
/**
 * Retrieves the data signal representing data from the tap detection algorithm
 * @param board     Calling object
 * @return Pointer to Bosch's tap detection data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bosch_get_tap_data_signal(const MblMwMetaWearBoard* board);

/**
 * Sets the output data rate for the BMI160 accelerometer
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_acc_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi160Odr odr);
/**
 * Sets the output data rate for the BMA255 accelerometer
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_acc_bma255_set_odr(MblMwMetaWearBoard *board, MblMwAccBma255Odr odr);
/**
 * Sets the acceleration range
 * @param board     Pointer to the board to modify
 * @param range     Acceleration range to assign
 */
METAWEAR_API void mbl_mw_acc_bosch_set_range(MblMwMetaWearBoard *board, MblMwAccBoschRange range);
/**
 * Writes the acceleration settings to the sensor
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_write_acceleration_config(const MblMwMetaWearBoard *board);

/**
 * Sets the operational mode of the step counter
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
 * @param board     Board to write to
 */
METAWEAR_API void mbl_mw_acc_bmi160_write_step_counter_config(const MblMwMetaWearBoard* board);
/**
 * Resets the BMI160 step counter
 * @param board     Board to reset
 */
METAWEAR_API void mbl_mw_acc_bmi160_reset_step_counter(const MblMwMetaWearBoard* board);

/**
 * Enables the BMI160 step detector 
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_enable_step_detector(const MblMwMetaWearBoard *board);
/**
 * Disables the BMI160 step detector
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_disable_step_detector(const MblMwMetaWearBoard *board);

/**
 * Set the hysteresis offset for portrait/landscape detection
 * @param board         Calling object
 * @param hysteresis    New calculation mode
 */
METAWEAR_API void mbl_mw_acc_bosch_set_orientation_hysteresis(MblMwMetaWearBoard *board, float hysteresis);
/**
 * Set the orientation calculation mode
 * @param board         Calling object
 * @param mode          New calculation mode
 */
METAWEAR_API void mbl_mw_acc_bosch_set_orientation_mode(MblMwMetaWearBoard *board, MblMwAccBoschOrientationMode mode);
/**
 * Writes the orientation detection settings to the board
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_write_orientation_config(const MblMwMetaWearBoard *board);
/**
 * Enables orientation detection
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_enable_orientation_detection(const MblMwMetaWearBoard *board);
/**
 * Disables orientation detection
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_disable_orientation_detection(const MblMwMetaWearBoard *board);

/**
 * Sets the tap detector's quiet time parameter
 * @param board     Calling object
 * @param time      New quiet time
 */
METAWEAR_API void mbl_mw_acc_bosch_set_quiet_time(MblMwMetaWearBoard *board, MblMwAccBoschTapQuietTime time);
/**
 * Sets the tap detector's shock time parameter
 * @param board     Calling object
 * @param time      New shock time
 */
METAWEAR_API void mbl_mw_acc_bosch_set_shock_time(MblMwMetaWearBoard *board, MblMwAccBoschTapShockTime time);
/**
 * Sets the tap detector's double tap window
 * @param board     Calling object
 * @param window    New double tap window time
 */
METAWEAR_API void mbl_mw_acc_bosch_set_double_tap_window(MblMwMetaWearBoard *board, MblMwAccBoschDoubleTapWindow window);
/**
 * Sets the tap detector's threshold
 * @param board     Calling object
 * @param window    New threshold level
 */
METAWEAR_API void mbl_mw_acc_bosch_set_threshold(MblMwMetaWearBoard *board, float threshold);
/**
 * Sets the tap detector's double tap window
 * @param board     Calling object
 * @param window    New double tap window time
 */
METAWEAR_API void mbl_mw_acc_bosch_write_tap_config(const MblMwMetaWearBoard *board);
/**
 * Enables the tap detector
 * @param board             Calling object
 * @param enable_single     0 to ignore single tap detection, non-zero to detect
 * @param enable_double     0 to ignore double tap detection, non-zero to detect
 */
METAWEAR_API void mbl_mw_acc_bosch_enable_tap_detection(const MblMwMetaWearBoard *board, uint8_t enable_single, uint8_t enable_double);
/**
 * Disable the tap detector
 * @param board     Calling object
 * @param window    New double tap window time
 */
METAWEAR_API void mbl_mw_acc_bosch_disable_tap_detection(const MblMwMetaWearBoard *board);

/**
 * Sets the any motion detector's count parameter
 * @param board     Calling object
 * @param count     Number of consecutive slope data points that must be above the threshold
 */
METAWEAR_API void mbl_mw_acc_bosch_set_any_motion_count(MblMwMetaWearBoard *board, uint8_t count);
/**
 * Sets the any motion detector's threshold parameter
 * @param board     Calling object
 * @param count     Value that the slope data points must be above
 */
METAWEAR_API void mbl_mw_acc_bosch_set_any_motion_threshold(MblMwMetaWearBoard *board, float threshold);
/**
 * Writes the motion configuration to the remote device
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_write_motion_config(const MblMwMetaWearBoard *board);
/**
 * Enables motion detection
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_enable_motion_detection(const MblMwMetaWearBoard *board);
/**
 * Disables motion detection
 * @param board     Calling object
 */
METAWEAR_API void mbl_mw_acc_bosch_disable_motion_detection(const MblMwMetaWearBoard *board);


/**
 * Switches the accelerometer to active mode.  When in active mode, the accelerometer cannot be configured
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_start(const MblMwMetaWearBoard *board);
/**
 * Switches the accelerometer to standby mode
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_stop(const MblMwMetaWearBoard *board);

/**
 * Enables acceleration sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_enable_acceleration_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables acceleration sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bosch_disable_acceleration_sampling(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
