/**
 * @copyright MbientLab License
 * @file packer.h
 * @brief Combines multiple data values into 1 BLE packet 
 */
#pragma once

#include "processor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Create a packer for the input signal.  
 * Combines multiple data values into 1 BLE packet.
 * Can be used to combine 3 accelerometer data entries into 1 packet. Used to ultra fast streaming.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param count                 Number of inputs to pack into 1 BLE packet
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
METAWEAR_API int32_t mbl_mw_dataprocessor_packer_create(MblMwDataSignal *source, uint8_t count, void *context, MblMwFnDataProcessor processor_created);

#ifdef	__cplusplus
}
#endif
