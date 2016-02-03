/**
 * @copyright MbientLab License 
 * @file data.h
 * @brief Functions and types for data received from an MblMwDataSignal
 */
#pragma once

/**
 * Enumeration of sensor data types
 */
typedef enum {
    MBL_MW_DT_ID_UINT32= 0,             ///< Data is an unsigned integer
    MBL_MW_DT_ID_FLOAT,                 ///< Data is a float
    MBL_MW_DT_ID_CARTESIAN_FLOAT,       ///< Data is a CartesianFloat
    MBL_MW_DT_ID_INT32                  ///< Data is a signed integer
} MblMwDataTypeId;

/**
 * Wrapper class encapsulating one sample of sensor data
 */
typedef struct {
    void* value;                    ///< Pointer to the data value 
    MblMwDataTypeId type_id;        ///< ID represnting the data type the value pointer points to
} MblMwData;

/**
 * Definition for callback functions that handle data from an MblMwDataSignal
 * @param data          Data returned from the signal
 */
typedef void (*MblMwFnData)(const MblMwData* data);
