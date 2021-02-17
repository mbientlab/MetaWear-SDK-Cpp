/**
 * @copyright MbientLab License
 * @file sensor_fusion.h
 * @brief Performs sensor fusion using accelerometer, gyro, and magnetometer data
 * @details When using the sensor fusion module, do not use the accelerometer, gyro, and magnetometer functions.  
 * The api will automatically configure the sensors based on the selected fusion mode.
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_SENSOR_FUSION_CALIBRATION_ACCURACY_UNRELIABLE = 0,
        MBL_MW_SENSOR_FUSION_CALIBRATION_ACCURACY_LOW = 1,
        MBL_MW_SENSOR_FUSION_CALIBRATION_ACCURACY_MEDIUM = 2,
        MBL_MW_SENSOR_FUSION_CALIBRATION_ACCURACY_HIGH = 3;

/**
 * Fusion modes supported by the algorithm
 */
typedef enum {
    MBL_MW_SENSOR_FUSION_MODE_SLEEP = 0,
    MBL_MW_SENSOR_FUSION_MODE_NDOF,
    MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS,
    MBL_MW_SENSOR_FUSION_MODE_COMPASS,
    MBL_MW_SENSOR_FUSION_MODE_M4G
} MblMwSensorFusionMode;

/**
 * Supported acceleration ranges
 */
typedef enum {
    MBL_MW_SENSOR_FUSION_ACC_RANGE_2G = 0,
    MBL_MW_SENSOR_FUSION_ACC_RANGE_4G,
    MBL_MW_SENSOR_FUSION_ACC_RANGE_8G,
    MBL_MW_SENSOR_FUSION_ACC_RANGE_16G
} MblMwSensorFusionAccRange;

/**
 * Supported rotation ranges
 */
typedef enum {
    MBL_MW_SENSOR_FUSION_GYRO_RANGE_2000DPS = 0,
    MBL_MW_SENSOR_FUSION_GYRO_RANGE_1000DPS,
    MBL_MW_SENSOR_FUSION_GYRO_RANGE_500DPS,
    MBL_MW_SENSOR_FUSION_GYRO_RANGE_250DPS
} MblMwSensorFusionGyroRange;

/**
 * Data computed by the algorithm
 */
typedef enum {
    MBL_MW_SENSOR_FUSION_DATA_CORRECTED_ACC = 0,
    MBL_MW_SENSOR_FUSION_DATA_CORRECTED_GYRO,
    MBL_MW_SENSOR_FUSION_DATA_CORRECTED_MAG,
    MBL_MW_SENSOR_FUSION_DATA_QUATERNION,
    MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE,
    MBL_MW_SENSOR_FUSION_DATA_GRAVITY_VECTOR,
    MBL_MW_SENSOR_FUSION_DATA_LINEAR_ACC
} MblMwSensorFusionData;

/**
 * Container class holding the IMU calibration data
 */
typedef struct {
    uint8_t acc[10];
    uint8_t gyro[10];
    uint8_t mag[10];
} MblMwCalibrationData;

/**
 * Definition for callback functions that accept an MblMwMetaWearBoard and MblMwCalibrationData pointer
 * @param context       Additional parameters for the callback function
 * @param board         Calling object
 * @param data          Pointer to calibration data
 */
typedef void(*MblMwFnBoardPtrCalibDataPtr)(void *context, MblMwMetaWearBoard* board, const MblMwCalibrationData* data);

/**
 * Get the data signal object representing data from the sensor fusion algorithm
 * The sensor fusion algo is a kalman filter that combines acc, gyro, and mag data into outputs such as correct acceleration, euler angles or quaternions
 * @param board         Calling object
 * @param data          Desired sensor fusion data
 * @return Data signal object  
 * Return type can be MblMwCorrectedCartesianFloat, MblMwQuaternion, MblMwEulerAngl, MblMwCartesianFloat
 */
METAWEAR_API MblMwDataSignal* mbl_mw_sensor_fusion_get_data_signal(const MblMwMetaWearBoard* board, MblMwSensorFusionData data);
/**
 * Get the data signal object representing thecalibration state.  
 * This signal can only be used while the sensor fusion algorithm is running
 * @param board         Calling object
 * @return Data signal object  
 */
METAWEAR_API MblMwDataSignal* mbl_mw_sensor_fusion_calibration_state_data_signal(const MblMwMetaWearBoard* board);
/**
 * Set the operation mode
 * See MblMwSensorFusionMode for allowed values
 * @param board         Calling object
 * @param mode          New operation mode
 */
METAWEAR_API void mbl_mw_sensor_fusion_set_mode(MblMwMetaWearBoard* board, MblMwSensorFusionMode mode);
/**
 * Set the accelerometer data range
 * Sets the range of the acc in Gs, see MblMwSensorFusionAccRange for allowed values
 * @param board         Calling object
 * @param range         New data range of the accelerometer
 */
METAWEAR_API void mbl_mw_sensor_fusion_set_acc_range(MblMwMetaWearBoard* board, MblMwSensorFusionAccRange range);
/**
 * Set the gyroscope data range
 * Sets the range of the gyro in DPS, see MblMwSensorFusionGyroRange for allowed values
 * @param board         Calling object
 * @param range         New data range of the gyroscope
 */
METAWEAR_API void mbl_mw_sensor_fusion_set_gyro_range(MblMwMetaWearBoard* board, MblMwSensorFusionGyroRange range);
/**
 * Reset the default orientation of the board. 
 * Works while sensor fusion is running or off. Works for NDOF and IMUPLUS only. 
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_sensor_fusion_reset_orientation(MblMwMetaWearBoard* board);
/**
 * Write the module configuration to the board
 * Applies the MODE and RANGE values set in set_*().
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_sensor_fusion_write_config(MblMwMetaWearBoard* board);
/**
 * Pulls the current sensor fusion configuration from the board
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
METAWEAR_API void mbl_mw_sensor_fusion_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed);
/**
 * Retrieve IMU calibration data; free the memory allocated for the MblMwCalibrationData pointer with mbl_mw_memory_free.
 * Only call this function when the calibration state of the IMUs is at high accuracy.  
 * This function can only be used with firmware v1.4.3+.  
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
METAWEAR_API void mbl_mw_sensor_fusion_read_calibration_data(MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrCalibDataPtr completed);
/**
 * Write IMU calibration data.  The data will be reloaded everytime the mode changes.
 * This function can only be used with firmware v1.4.3+  
 * @param board         Calling object
 * @param data          Calibration data to load
 */
METAWEAR_API void mbl_mw_sensor_fusion_write_calibration_data(const MblMwMetaWearBoard* board, const MblMwCalibrationData* data);
/**
 * Set a data enable bit
 * Turns on the Kalman filter (sensor fusion)
 * @param board         Calling object
 * @param data          Sensor fuson data to enable
 */
METAWEAR_API void mbl_mw_sensor_fusion_enable_data(MblMwMetaWearBoard* board, MblMwSensorFusionData data);
/**
 * Clear all data enable bits
 * Turns off the Kalman filter (sensor fusion)
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_sensor_fusion_clear_enabled_mask(MblMwMetaWearBoard* board);
/**
 * Start sensor fusion
 * The board will start gathering data from the gyroscope, accelerometer, and gyroscope and run the sensor fusion
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_sensor_fusion_start(const MblMwMetaWearBoard* board);
/**
 * Stop sensor fusion
 * The board will stop gathering data from the gyroscope, accelerometer, and gyroscope and stop the sensor fusion
 * @param board         Calling object
 */
METAWEAR_API void mbl_mw_sensor_fusion_stop(const MblMwMetaWearBoard* board);

#ifdef	__cplusplus
}
#endif
