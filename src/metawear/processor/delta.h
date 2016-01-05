#pragma once

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_DELTA_MODE_ABSOLUTE = 0,
    MBL_MW_DELTA_MODE_DIFFERENTIAL,
    MBL_MW_DELTA_MODE_BINARY
} MblMwDeltaMode;

METAWEAR_API void mbl_mw_dataprocessor_create_delta(MblMwDataSignal *source, MblMwDeltaMode mode, float magnitude, 
        MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_delta_set_previous_value(MblMwDataProcessor *delta, float previous_value);
METAWEAR_API int32_t mbl_mw_dataprocessor_delta_modify_magnitude(MblMwDataProcessor *delta, float magnitude);

#ifdef	__cplusplus
}
#endif
