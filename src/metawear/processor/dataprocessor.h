/**
 * @copyright MbientLab License 
 * @file dataprocessor.h
 * @brief 
 */
#pragma once

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Get the data signal representing a processor's internal state.  Processors that have an internal state are: accumulator, buffer, 
 * counter, delta, and passthrough.
 * @param processor         Processor to access
 * @return Pointer to the data signal, null if the processor does not have an internal state
 */
METAWEAR_API const MblMwDataSignal* mbl_mw_dataprocessor_get_state_data_signal(MblMwDataProcessor *processor);
/**
 * Reads the processor's internal state
 * @param processor         Processor to read
 */
METAWEAR_API void mbl_mw_dataprocessor_read_state(MblMwDataProcessor *processor);
/**
 * Removes a data processor and its consumers from the board
 * @param processor         Processor to remove
 */
METAWEAR_API void mbl_mw_dataprocessor_remove(MblMwDataProcessor *processor);

#ifdef	__cplusplus
}
#endif
