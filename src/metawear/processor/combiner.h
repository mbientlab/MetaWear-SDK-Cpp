#pragma once

#include <stdint.h>

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

METAWEAR_API void mbl_mw_dataprocessor_create_rms(MblMwDataSignal *source, MblMwFnDataProcessor processor_created);
METAWEAR_API void mbl_mw_dataprocessor_create_rss(MblMwDataSignal *source, MblMwFnDataProcessor processor_created);

#ifdef	__cplusplus
}
#endif
