#pragma once

#include "metawear/core/timer_fwd.h"
#include "event_private.h"

struct MblMwTimer : public MblMwEvent {
    MblMwTimer(const ResponseHeader& header, MblMwMetaWearBoard *owner);
    virtual ~MblMwTimer();
};
