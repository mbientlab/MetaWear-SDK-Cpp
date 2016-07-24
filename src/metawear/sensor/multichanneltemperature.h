/**
 * @copyright MbientLab License
 * @file multichanneltemperature.h
 * @brief Interacts with various temperature sources.
 * @details The functions in the file only work for boards running firmware v1.0.4 or higher
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Enumeration of the available temperature sourcs.  Note that not all sources are present 
 * on all boards
 */
typedef enum {
    MBL_MW_TEMP_SOURCE_INVALID= -1,         ///< Invalid temperature source
    MBL_MW_TEMP_SOURCE_NRF_DIE= 0,          ///< Temperature from the nRF chip
    MBL_MW_TEMP_SOURCE_EXT_THERM,           ///< Tempertaure from an external thermistor
    MBL_MW_TEMP_SOURCE_BMP280,              ///< Temperature from the BMP280 sensor
    MBL_MW_TEMP_SOURCE_PRESET_THERM         ///< Temperature from an internal thremistor
} MblMwTemperatureSource;

/**
 * Channel IDs for the temperature sources on the MetaWear R board
 */
typedef enum {
    MBL_MW_METAWEAR_R_CHANNEL_ON_DIE= 0,            ///< Temperature from the nRF chip
    MBL_MW_METAWEAR_R_CHANNEL_EXT_THERMISTOR        ///< Temperature from an external thermistor
} MblMwMetaWearRChannel;

/**
 * CHannel IDs for the temperature sources on the MetaWear RPro board
 */
typedef enum {
    MBL_MW_METAWEAR_RPRO_CHANNEL_ON_DIE = 0,                ///< Temperature from the nRF chip
    MBL_MW_METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR,       ///< Temperature from the on board thermistor
    MBL_MW_METAWEAR_RPRO_CHANNEL_EXT_THERMISTOR,            ///< Temperature from an external thermistor
    MBL_MW_METAWEAR_RPRO_CHANNEL_BMP280                     ///< Temperature from the BMP280 sensor
} MblMwMetaWearRProChannel;

/**
 * Retrieves the data signal representing a temperature source
 * @param board     Board to retrieve the signal from
 * @param channel   Channel ID of the temperature source
 */
METAWEAR_API MblMwDataSignal* mbl_mw_multi_chnl_temp_get_temperature_data_signal(const MblMwMetaWearBoard *board, uint8_t channel);

/**
 * Configure the external thermistor
 * @param board             Board the external thermistor is attached to
 * @param channel           Channel ID of the external thermistor 
 * @param data_pin          GPIO pin reading the data
 * @param pulldown_pin      GPIO pin the pulldown resistor is connected to
 * @param active_high       Zero if the pulldown pin is not active high, non-zero if active high
 */
METAWEAR_API void mbl_mw_multi_chnl_temp_configure_ext_thermistor(const MblMwMetaWearBoard *board, uint8_t channel, uint8_t data_pin, 
        uint8_t pulldown_pin, uint8_t active_high);

/**
 * Retrieve the temperature source type corresponding to a channel ID
 * @param board         Board to lookup the temperature source on
 * @param channel       Channel ID to lookup
 * @return Source type of the channel ID, MBL_MW_TEMP_SOURCE_INVALID if channel ID is out of range
 */
METAWEAR_API MblMwTemperatureSource mbl_mw_multi_chnl_temp_get_source(const MblMwMetaWearBoard *board, uint8_t channel);
/**
 * Retrieve the number of available channels
 * @return Number of channel IDs
 */
METAWEAR_API uint8_t mbl_mw_multi_chnl_temp_get_num_channels(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
