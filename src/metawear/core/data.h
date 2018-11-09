/**
 * @copyright MbientLab License 
 * @file data.h
 * @brief Functions and types for data received from an MblMwDataSignal
 */
#pragma once

#include <stdint.h>

/**
 * Enumeration of sensor data types
 */
typedef enum {
    MBL_MW_DT_ID_UINT32= 0,             ///< Data is an unsigned integer
    MBL_MW_DT_ID_FLOAT,                 ///< Data is a float
    MBL_MW_DT_ID_CARTESIAN_FLOAT,       ///< Data is a CartesianFloat
    MBL_MW_DT_ID_INT32,                 ///< Data is a signed integer
    MBL_MW_DT_ID_BYTE_ARRAY,            ///< Data is a byte array
    MBL_MW_DT_ID_BATTERY_STATE,         ///< Data is a BatteryState
    MBL_MW_DT_ID_TCS34725_ADC,          ///< Data is a Tcs34725ColorAdc
    MBL_MW_DT_ID_EULER_ANGLE,
    MBL_MW_DT_ID_QUATERNION,
    MBL_MW_DT_ID_CORRECTED_CARTESIAN_FLOAT,
    MBL_MW_DT_ID_OVERFLOW_STATE,
    MBL_MW_DT_ID_SENSOR_ORIENTATION,
    MBL_MW_DT_ID_STRING,
    MBL_MW_DT_ID_LOGGING_TIME,
    MBL_MW_DT_ID_BTLE_ADDRESS,
    MBL_MW_DT_ID_BOSCH_ANY_MOTION,
    MBL_MW_DT_ID_CALIBRATION_STATE,
    MBL_MW_DT_ID_DATA_ARRAY,
    MBL_MW_DT_ID_BOSCH_TAP
} MblMwDataTypeId;

/**
 * Wrapper class encapsulating one sample of sensor data
 */
typedef struct {
    int64_t epoch;                  ///< Number of milliseconds since epoch
    void* extra;                    ///< Extra information attached to this data sample
    void* value;                    ///< Pointer to the data value 
    MblMwDataTypeId type_id;        ///< ID represnting the data type the value pointer points to
    uint8_t length;                 ///< Size of the value
} MblMwData;

/**
 * Definition for callback functions that handle data from an MblMwDataSignal
 * @param context       Pointer to the context the enclosing function was called with
 * @param data          Data returned from the signal
 */
typedef void (*MblMwFnData)(void *context, const MblMwData* data);
