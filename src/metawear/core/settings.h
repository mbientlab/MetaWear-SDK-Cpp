#pragma once

#include <stdint.h>
#include "dllmarker.h"
#include "metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

METAWEAR_API void mbl_mw_settings_set_device_name(const MblMwMetaWearBoard *board, const uint8_t *device_name, uint8_t len);

METAWEAR_API void mbl_mw_settings_set_ad_interval(const MblMwMetaWearBoard *board, uint16_t interval, uint8_t timeout);

METAWEAR_API void mbl_mw_settings_set_tx_power(const MblMwMetaWearBoard *board, int8_t tx_power);

METAWEAR_API void mbl_mw_settings_start_advertising(const MblMwMetaWearBoard *board);

METAWEAR_API void mbl_mw_settings_set_scan_response(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);

METAWEAR_API void mbl_mw_settings_set_connection_parameters(const MblMwMetaWearBoard *board, float min_conn_interval, float max_conn_interval, 
        uint16_t latency, uint16_t timeout);

#ifdef	__cplusplus
}
#endif
