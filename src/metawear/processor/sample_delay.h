#pragma once

#include <stdint.h>

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

METAWEAR_API void mbl_mw_dataprocessor_create_sample_delay(MblMwDataSignal *source, uint8_t bin_size, 
        MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_sample_delay_modify_bin_size(MblMwDataProcessor *sample_delay, uint8_t bin_size);

#ifdef	__cplusplus
}
#endif
