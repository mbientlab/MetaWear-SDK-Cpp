/**
 * @copyright MbientLab License 
 * @file sensor.h
 * @brief Generic functions for handling sensor data
 */
#pragma once

#include <stdint.h>
#include "dllmarker.h"
#include "sensor_defs.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Frees memory allocated for a DataSource struct
 * @param data_attr     Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_sensor_free_data_source(DataSource *data_attr);

/**
 * Constructs the command to enable data streaming from a sensor.  The command length can vary depending on the 
 * sensor, with the max possible length being 4 bytes
 * @param command   Byte array for the function to write the command to
 * @param source    Source to stream data from
 * @return Length of the command as number of bytes
 */
METAWEAR_API uint8_t mbl_mw_sensor_subscribe(uint8_t *command, const DataSource *source);
/**
 * Constructs the command to disable data streaming from a sensor.  The command length can vary depending on the 
 * sensor, with the max possible length being 4 bytes
 * @param command   Byte array for the function to write the command to
 * @param source    Data stream to cut
 * @return Length of the command as number of bytes
 */
METAWEAR_API uint8_t mbl_mw_sensor_unsubscribe(uint8_t *command, const DataSource *source);

#ifdef	__cplusplus
}
#endif
