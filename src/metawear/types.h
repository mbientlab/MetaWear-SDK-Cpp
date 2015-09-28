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
} CartesianFloat;

/**
 * Tuple combining short data on a 3D cartesian coordinate system
 */
typedef struct {
    int16_t x;      ///< Data corresponding to the x-axis
    int16_t y;      ///< Data corresponding to the y-axis
    int16_t z;      ///< Data corresponding to the z-axis
} CartesianShort;
