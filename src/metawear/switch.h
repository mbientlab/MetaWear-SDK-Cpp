/**
 * @copyright MbientLab License 
 * @file switch.h
 * @brief Push button switch
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "sensor_defs.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_SWITCH_MODULE= 0x1;        ///< Module id for the switch
const uint8_t MBL_MW_SWITCH_DATA= 0x1;          ///< Register id for switch state data

/**
 * Retrieves the attributes for switch state data 
 * @return Pointer to the switch data source
 */
METAWEAR_API const DataSource* mbl_mw_switch_get_data_source();
/**
 * Extracts the the switch state from a response sent by the switch
 * @param data      Pointer to where the function should write the switch state to
 * @param response  Response received from the switch
 * @return #STATUS_OK if the response contains valid switch data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_switch_get_data(uint8_t *data, uint8_t response[3]);

#ifdef	__cplusplus
}
#endif
