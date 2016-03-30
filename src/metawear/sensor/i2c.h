/**
 * @copyright MbientLab License (LICENSE.md)
 * @file i2c.h
 * @brief Communicates with the I2C bus
 */
#pragma once

#include <stdint.h>

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"
#include "metawear/core/metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Retrieves the data signal representing the i2c data.  The length and id parameters must match their respective values in the 
 * mbl_mw_i2c_read function.
 * @param board         Board the i2c bus resides on
 * @param length        Data length, in number of bytes
 * @param id            Numerical value identifying the data
 * @return Pointer to the i2c data signal
 * @see mbl_mw_i2c_read
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
/**
 * Reads data via the i2c bus
 * @param board             Board the i2c bus resides on
 * @param device_addr       Device to read from
 * @param register_addr     Address of the register to read
 * @param length            Number of bytes to read
 * @param id                Numerical value identifying the data
 */
METAWEAR_API void mbl_mw_i2c_read(const MblMwMetaWearBoard *board, uint8_t device_addr, uint8_t register_addr, uint8_t length, uint8_t id);

#ifdef	__cplusplus
}
#endif
