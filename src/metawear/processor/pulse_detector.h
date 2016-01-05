#pragma once

#include <stdint.h>

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_PULSE_OUTPUT_WIDTH = 0,
    MBL_MW_PULSE_OUTPUT_AREA,
    MBL_MW_PULSE_OUTPUT_PEAK
} MblMwPulseOutput;

METAWEAR_API void mbl_mw_dataprocessor_create_pulse_detector(MblMwDataSignal *source, MblMwPulseOutput output, 
            float threshold, uint16_t width, MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_pulse_detector_modify(MblMwDataProcessor *pulse, float threshold, uint16_t width);

#ifdef	__cplusplus
}
#endif
