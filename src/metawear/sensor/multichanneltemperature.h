#pragma once

#include <stdint.h>

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"
#include "metawear/core/metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_TEMP_SOURCE_INVALID= -1,
    MBL_MW_TEMP_SOURCE_NRF_DIE= 0,
    MBL_MW_TEMP_SOURCE_EXT_THERM,
    MBL_MW_TEMP_SOURCE_BMP280,
    MBL_MW_TEMP_SOURCE_PRESET_THERM
} MblMwTemperatureSource;

const uint8_t MBL_MW_METAWEAR_R_CHANNEL_ON_DIE = 0,
        MBL_MW_METAWEAR_R_CHANNEL_EXT_THERMISTOR = 1;

const uint8_t MBL_MW_METAWEAR_RPRO_CHANNEL_ON_DIE = 0,
        MBL_MW_METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR = 1,
        MBL_MW_METAWEAR_RPRO_CHANNEL_EXT_THERMISTOR = 2,
        MBL_MW_METAWEAR_RPRO_CHANNEL_BMP280 = 3;

METAWEAR_API const MblMwDataSignal* mbl_mw_multi_chnl_temp_get_temperature_data_signal(const MblMwMetaWearBoard *board, uint8_t channel);

METAWEAR_API void mbl_mw_multi_chnl_temp_configure_ext_thermistor(const MblMwMetaWearBoard *board, uint8_t channel, uint8_t data_pin, 
        uint8_t pulldown_pin, uint8_t active_high);

METAWEAR_API void mbl_mw_multi_chnl_temp_read_temperature(const MblMwMetaWearBoard *board, uint8_t channel);

METAWEAR_API MblMwTemperatureSource mbl_mw_multi_chnl_temp_get_source(const MblMwMetaWearBoard *board, uint8_t channel);

METAWEAR_API uint8_t mbl_mw_multi_chnl_temp_get_num_channels(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
