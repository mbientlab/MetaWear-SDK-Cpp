/**
 * @copyright MbientLab License (LICENSE.md)
 * @file types.h
 * @brief Non basic types encapsulating sensor data
 */
#pragma once

#include <stdint.h>

/**
 * Tuple combining float data on a 3D cartesian coordinate system 
 */
typedef struct {
    float x;        ///< Data corresponding to the x-axis
    float y;        ///< Data corresponding to the y-axis
    float z;        ///< Data corresponding to the z-axis
} MblMwCartesianFloat;

/**
 * Tuple holding battery state information
 */
typedef struct {
    uint16_t voltage;       ///< Voltage level, in mV
    uint8_t charge;         ///< Percent charged, between [0, 100] percent
} MblMwBatteryState;

/**
 * Tuple wrapping ADC data from the TCS34725 color detector
 */
typedef struct {
    uint16_t clear;         ///< ADC value from an unfiltered diode
    uint16_t red;           ///< ADC value from a red filtered diode
    uint16_t green;         ///< ADC value from a green filtered diode
    uint16_t blue;          ///< ADC value from a blue filtered diode
} MblMwTcs34725ColorAdc;

/**
 * 4-element float vector holding Euler angles, all values are in degrees
 */
typedef struct {
    float heading;
    float pitch;
    float roll;
    float yaw;
} MblMwEulerAngles;

/**
 * 4-element float vector containing a normalized quaternion value
 */
typedef struct {
    float w;
    float x;
    float y;
    float z;
} MblMwQuaternion;

/**
 * Variant of the MblMwCartesianFloat struct that also reports data accuracy
 */
typedef struct {
    float x;
    float y;
    float z;
    uint8_t accuracy;
} MblMwCorrectedCartesianFloat;

/**
 * Internal stack overflow state
 */
typedef struct {
    uint16_t length;
    uint8_t assert_en;
} MblMwOverflowState;

/**
 * Enumeration of sensor orientations
 * @author Eric Tsai
 */
typedef enum {
    MBL_MW_SENSOR_ORIENTATION_FACE_UP_PORTRAIT_UPRIGHT,
    MBL_MW_SENSOR_ORIENTATION_FACE_UP_PORTRAIT_UPSIDE_DOWN,
    MBL_MW_SENSOR_ORIENTATION_FACE_UP_LANDSCAPE_LEFT,
    MBL_MW_SENSOR_ORIENTATION_FACE_UP_LANDSCAPE_RIGHT,
    MBL_MW_SENSOR_ORIENTATION_FACE_DOWN_PORTRAIT_UPRIGHT,
    MBL_MW_SENSOR_ORIENTATION_FACE_DOWN_PORTRAIT_UPSIDE_DOWN,
    MBL_MW_SENSOR_ORIENTATION_FACE_DOWN_LANDSCAPE_LEFT,
    MBL_MW_SENSOR_ORIENTATION_FACE_DOWN_LANDSCAPE_RIGHT
} MblMwSensorOrientation;

/**
 * Internal clock for logger timestamps
 */
typedef struct {
    int64_t epoch;
    uint8_t reset_uid;
} MblMwLoggingTime;

/**
 * Sensor fusion calibration state
 */
typedef struct {
    uint8_t accelrometer;
    uint8_t gyroscope;
    uint8_t magnetometer;
} MblMwCalibrationState;

/**
 * Wrapper class encapsulating responses from the Bosch tap detector
 */
typedef struct {
    uint8_t type;   ///< 1 if double tap, 2 if single tap
    uint8_t sign;   ///< 0 if positive, 1 if negative
} MblMwBoschTap;