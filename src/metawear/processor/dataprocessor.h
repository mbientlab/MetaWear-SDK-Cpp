/**
 * @copyright MbientLab License 
 * @file dataprocessor.h
 * @brief Interacts with an onboard data processor
 */
#pragma once

#include "dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/metawearboard_fwd.h"
#include "metawear/platform/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Get the data signal representing a processor's internal state.  
 * Processors that have an internal state are: accumulator, buffer, counter, delta, and passthrough.
 * @param processor         Processor to access
 * @return Pointer to the data signal, null if the processor does not have an internal state
 */
METAWEAR_API MblMwDataSignal* mbl_mw_dataprocessor_get_state_data_signal(const MblMwDataProcessor* processor);
/**
 * Removes a data processor and its consumers from the board
 * @param processor         Processor to remove
 */
METAWEAR_API void mbl_mw_dataprocessor_remove(MblMwDataProcessor *processor);
/**
 * Retrieves the id value identifying the processor
 * @param processor         Processor to lookup
 * @return Numerical id of the processor
 */
METAWEAR_API uint8_t mbl_mw_dataprocessor_get_id(const MblMwDataProcessor* processor);
/**
 * Looks up the MblMwDataProcessor object corresponding to the id
 * @param board             Board to search on
 * @param id                Numerical id to lookup
 * @return Data processor object identified by the id, null if no object is found
 */
METAWEAR_API MblMwDataProcessor* mbl_mw_dataprocessor_lookup_id(const MblMwMetaWearBoard* board, uint8_t id);

#ifdef	__cplusplus
}
#endif
