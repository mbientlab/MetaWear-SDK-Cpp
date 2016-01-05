#pragma once

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_THRESHOLD_MODE_ABSOLUTE = 0,
    MBL_MW_THRESHOLD_MODE_BINARY
} MblMwThresholdMode;

METAWEAR_API void mbl_mw_dataprocessor_create_threshold(MblMwDataSignal *source, MblMwThresholdMode mode, float boundary, 
        float hysteresis, MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_threshold_modify_boundary(MblMwDataProcessor *threshold, float boundary, 
        float hysteresis);

#ifdef	__cplusplus
}
#endif
