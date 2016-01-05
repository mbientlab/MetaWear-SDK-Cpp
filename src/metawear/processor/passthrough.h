#pragma once

#include <stdint.h>

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_PASSTHROUGH_ALL = 0,
    MBL_MW_PASSTHROUGH_CONDITIONAL,
    MBL_MW_PASSTHROUGH_COUNT
} MblMwPassthroughMode;

METAWEAR_API void mbl_mw_dataprocessor_create_passthrough(MblMwDataSignal *source, MblMwPassthroughMode mode, uint16_t count, 
        MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_passthrough_set_count(MblMwDataProcessor *passthrough, uint16_t new_count);
METAWEAR_API int32_t mbl_mw_dataprocessor_passthrough_modify(MblMwDataProcessor *passthrough, MblMwPassthroughMode mode, 
        uint16_t count);

#ifdef	__cplusplus
}
#endif
