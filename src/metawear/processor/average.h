#pragma once

#include <stdint.h>

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

METAWEAR_API void mbl_mw_dataprocessor_create_average(MblMwDataSignal *source, uint8_t size, 
        MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_reset_average(MblMwDataProcessor *average);
METAWEAR_API int32_t mbl_mw_dataprocessor_average_modify_size(MblMwDataProcessor *average, uint8_t size);

#ifdef	__cplusplus
}
#endif
