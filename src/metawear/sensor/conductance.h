/**
 * @copyright MbientLab License
 * @file conductance.h
 * @brief Interacts with conductance measurement sources.
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Enumeration of the available conductance measurment ranges
 */
typedef enum {
    MBL_MW_CONDUCTANCE_RANGE_50uS= 0,
    MBL_MW_CONDUCTANCE_RANGE_100uS,
    MBL_MW_CONDUCTANCE_RANGE_150uS,
    MBL_MW_CONDUCTANCE_RANGE_200uS,
} MblMwConductanceRange;


/**
 * Retrieves the data signal representing a conductance source
 * @param board     Board to retrieve the signal from
 * @param channel   Channel ID of the conductance source
 */
METAWEAR_API MblMwDataSignal* mbl_mw_conductance_get_data_signal(const MblMwMetaWearBoard *board, uint8_t channel);

/**
 * Retrieve the number of available channels
 * @return Number of channel IDs
 */
METAWEAR_API uint8_t mbl_mw_conductance_get_num_channels(const MblMwMetaWearBoard *board);

/**
 * Trigger the automatic calibration routine.  Fully self contained and requires no parameters.
 * This should be called at least once after boot
 */
METAWEAR_API void mbl_mw_conductance_calibrate(const MblMwMetaWearBoard *board);

/**
 * Sets the conductance measurment mode
 * @param board     Pointer to the board to modify
 * @param range     Range of the conductance values
 */
METAWEAR_API void mbl_mw_conductance_set_range(MblMwMetaWearBoard *board, MblMwConductanceRange range);

#ifdef	__cplusplus
}
#endif
