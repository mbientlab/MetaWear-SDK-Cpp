/**
* @copyright MbientLab License (LICENSE.md)
* @file status.h
* @brief Status codes for the library
*/

#pragma once

#include <stdint.h>

/** Code executed normally, no errors or warnings */
extern const int32_t MBL_MW_STATUS_OK;
/** Data unexpectedly received from a sensor.  This could happen if the library connects to an already setup board */
extern const int32_t MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
/** Invalid processor passed into a dataprocessor function */
extern const int32_t MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
/** Processor not supported for the data signal */
extern const int32_t MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
/** Invalid response receieved from the MetaWear notify characteristic */
extern const int32_t MBL_MW_STATUS_WARNING_INVALID_RESPONSE;
/** Timeout occured during an asynchronous operation */
extern const int32_t MBL_MW_STATUS_ERROR_TIMEOUT;
/** Cannot restore API state given the input serialization format */
extern const int32_t MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT;
/** Failed to enable notifications */
extern const int32_t MBL_MW_STATUS_ERROR_ENABLE_NOTIFY;
