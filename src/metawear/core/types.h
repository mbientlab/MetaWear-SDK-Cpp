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
