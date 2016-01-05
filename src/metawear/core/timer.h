#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "metawearboard_fwd.h"
#include "timer_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

METAWEAR_API void mbl_mw_timer_create(MblMwMetaWearBoard *board, uint32_t period, uint16_t repetitions, uint8_t delay, 
        MblMwTimerPtr received_timer);
METAWEAR_API void mbl_mw_timer_create_indefinite(MblMwMetaWearBoard *board, uint32_t period, uint8_t delay,
        MblMwTimerPtr received_timer);

METAWEAR_API void mbl_mw_timer_start(const MblMwTimer* timer);
METAWEAR_API void mbl_mw_timer_stop(const MblMwTimer* timer);
METAWEAR_API void mbl_mw_timer_remove(MblMwTimer* timer);

#ifdef	__cplusplus
}
#endif
