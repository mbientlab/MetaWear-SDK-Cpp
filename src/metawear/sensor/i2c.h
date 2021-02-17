/**
 * @copyright MbientLab License (LICENSE.md)
 * @file i2c.h
 * @brief Communicates with sensors on the I2C bus
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Parameters required for an i2c read.  
 * This struct is to be used with mbl_mw_datasignal_read_with_parameters
 */
typedef struct {
    uint8_t device_addr;            ///< Device to communicate with
    uint8_t register_addr;          ///< Register to read from
} MblMwI2cReadParameters;

/**
 * Retrieves the data signal representing i2c data
 * The data signal is identified by the id value and if the id is not present, a new data signal will be created using the length parameter. 
 * @param board         Board the i2c bus resides on
 * @param length        Number of bytes to read
 * @param id            Numerical value identifying the data
 * @return Pointer to the i2c data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_i2c_get_data_signal(MblMwMetaWearBoard *board, uint8_t length, uint8_t id);
/**
 * Writes data via the i2c bus
 * @param board             Board the i2c bus resides on
 * @param device_addr       Device to write to
 * @param register_addr     Address of the register to write
 * @param value             Payload, as a byte array
 * @param length            Number of bytes
 */
METAWEAR_API void mbl_mw_i2c_write(const MblMwMetaWearBoard *board, uint8_t device_addr, uint8_t register_addr, const uint8_t* value, uint8_t length);

#ifdef	__cplusplus
}
#endif
