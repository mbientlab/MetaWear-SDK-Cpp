/**
 * @copyright MbientLab License 
 * @file switch.h
 * @brief Push button switch
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Retrieves the data signal representing switch state data
 * The switch is either pushed (1) or not pushed (0)
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the switch data signal
 * UINT32 is return signal data type 
 */
METAWEAR_API MblMwDataSignal* mbl_mw_switch_get_state_data_signal(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
