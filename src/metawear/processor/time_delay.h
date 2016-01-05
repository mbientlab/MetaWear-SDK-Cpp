#pragma once

#include <stdint.h>

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_TIME_ABSOLUTE = 0,
    MBL_MW_TIME_DIFFERENTIAL
} MblMwTimeMode;

METAWEAR_API void mbl_mw_dataprocessor_create_time_delay(MblMwDataSignal *source, MblMwTimeMode mode, uint32_t period, 
        MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_time_delay_modify_period(MblMwDataProcessor *time_delay, uint32_t period);

#ifdef	__cplusplus
}
#endif
