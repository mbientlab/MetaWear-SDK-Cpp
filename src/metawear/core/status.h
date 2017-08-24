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
/** Invalid processor passed into a dataprocessor function */
const int32_t MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE= 2;
/** Processor not supported for the data signal */
const int32_t MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR = 4;
/** Invalid response receieved from the MetaWear notify characteristic */
const int32_t MBL_MW_STATUS_WARNING_INVALID_RESPONSE = 8;
/** Timeout occured during an asynchronous operation */
const int32_t MBL_MW_STATUS_ERROR_TIMEOUT = 16;
/** Cannot restore API state given the input serialization format */
const int32_t MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT = 32;
/** Failed to enable notifications */
const int32_t MBL_MW_STATUS_ERROR_ENABLE_NOTIFY = 64;
