#pragma once

#include <stdint.h>

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

METAWEAR_API void mbl_mw_dataprocessor_create_accumulator(MblMwDataSignal *source, MblMwFnDataProcessor processor_created);
METAWEAR_API void mbl_mw_dataprocessor_create_accumulator_size(MblMwDataSignal *source, uint8_t output_size, 
        MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_set_accumulator_state(MblMwDataProcessor *accumulator, float new_running_sum);

METAWEAR_API void mbl_mw_dataprocessor_create_counter(MblMwDataSignal *source, MblMwFnDataProcessor processor_created);
METAWEAR_API void mbl_mw_dataprocessor_create_counter_size(MblMwDataSignal *source, uint8_t size, 
        MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_set_counter_state(MblMwDataProcessor *counter, uint32_t new_count);

#ifdef	__cplusplus
}
#endif
