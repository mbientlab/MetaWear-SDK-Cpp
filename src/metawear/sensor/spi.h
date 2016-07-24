/**
 * @copyright MbientLab License (LICENSE.md)
 * @file spi.h
 * @brief Communicates with sensors on the SPI bus
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Available SPI clock frequencies
 */
typedef enum {
    MBL_MW_SPI_FREQUENCY_125_KHZ = 0,
    MBL_MW_SPI_FREQUENCY_250_KHZ,
    MBL_MW_SPI_FREQUENCY_500_KHZ,
    MBL_MW_SPI_FREQUENCY_1_MHZ,
    MBL_MW_SPI_FREQUENCY_2_MHZ,
    MBL_MW_SPI_FREQUENCY_4_MHZ,
    MBL_MW_SPI_FREQUENCY_8_MHZ
} MBL_MW_SPI_FREQUENCY;

/**
 * Available SPI modes, see <a href="https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus#Mode_numbers">SPI Wiki page</a> 
 * for further details
 */
typedef enum {
    MBL_MW_SPI_MODE_0= 0,           ///< clock polarity 0, clock phase 0
    MBL_MW_SPI_MODE_1,              ///< clock polarity 0, clock phase 1
    MBL_MW_SPI_MODE_2,              ///< clock polarity 1, clock phase 0
    MBL_MW_SPI_MODE_3               ///< clock polarity 1, clock phase 1
} MBL_MW_SPI_MODE;

/**
 * Parameters required for an SPI operation
 */
typedef struct {
    MBL_MW_SPI_MODE mode;               ///< spi mode
    MBL_MW_SPI_FREQUENCY frequency;     ///< spi frequency
    uint8_t *data;                      ///< Data to write through the bus, if set with a read, will write the data before performing the read
    uint8_t data_length;                ///< Length of the data array
    uint8_t slave_select_pin;           ///< pin for slave select
    uint8_t clock_pin;                  ///< pin for spi clock
    uint8_t mosi_pin;                   ///< pin for master out, slave in data
    uint8_t miso_pin;                   ///< pin for master in, slave out data
    uint8_t lsb_first;                  ///< Set to 0 if MSB should be sent first, non-zero to send LSB first
    uint8_t use_nrf_pins;               ///< Set to 0 to use MetaWear GPIO pin mapping, non-zero to use nRF pin mapping
} MblMwSpiParameters;

/**
 * Retrieves the data signal representing spi data.  The data signal is identified by the id value and if the id is not present, 
 * a new data signal will be created using the length parameter. 
 * @param board         Board to communicate with
 * @param length        Number of bytes to read
 * @param id            Numerical id identifying the data
 * @return Pointer to the spi data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_spi_get_data_signal(MblMwMetaWearBoard* board, uint8_t length, uint8_t id);
/**
 * Writes data via the spi bus
 * @param board             Board to communicate with
 * @param parameters        Parameters configuring the read
 */
METAWEAR_API void mbl_mw_spi_write(const MblMwMetaWearBoard* board, const MblMwSpiParameters* parameters);

#ifdef	__cplusplus
}
#endif
