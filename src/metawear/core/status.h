/**
* @copyright MbientLab License (LICENSE.md)
* @file status.h
* @brief Status codes for the library
*/

#pragma once

#include <stdint.h>

/** Code executed normally, no errors or warnings */
const int32_t MBL_MW_STATUS_OK= 0;
/** Data unexpectedly received from a sensor.  This could happen if the library connects to an already setup board */
const int32_t MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA= 1;
const int32_t MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE= 2;
