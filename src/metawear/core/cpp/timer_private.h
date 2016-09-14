#pragma once

#include "metawear/core/timer_fwd.h"
#include "event_private.h"

struct MblMwTimer : public MblMwEvent {
    MblMwTimer(uint8_t** state_stream, MblMwMetaWearBoard *owner);
    MblMwTimer(const ResponseHeader& header, MblMwMetaWearBoard *owner);
    virtual ~MblMwTimer();

    void remove_from_board();
};

void init_timer_module(MblMwMetaWearBoard *board);
void free_timer_module(void *state);
