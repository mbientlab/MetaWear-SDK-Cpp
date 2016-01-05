#pragma once

#include "callback.h"
#include "dllmarker.h"
#include "event_fwd.h"
#include "metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

METAWEAR_API MblMwMetaWearBoard* mbl_mw_event_get_owner(const MblMwEvent *event);
METAWEAR_API void mbl_mw_event_record_commands(MblMwEvent *event);
METAWEAR_API void mbl_mw_event_end_record(MblMwEvent *event, MblMwFnVoid commands_recorded);
METAWEAR_API void mbl_mw_event_remove_commands(MblMwEvent *event);

#ifdef	__cplusplus
}
#endif
