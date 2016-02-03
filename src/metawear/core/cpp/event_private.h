#pragma once

#include <vector>

#include "metawear/core/event_fwd.h"
#include "metawear/core/metawearboard_fwd.h"
#include "responseheader.h"

struct MblMwEvent {
    MblMwEvent(const ResponseHeader& header, MblMwMetaWearBoard* owner);
    virtual ~MblMwEvent();

    ResponseHeader header;
    MblMwMetaWearBoard* const owner;
    bool remove;
    std::vector<uint8_t> event_command_ids;
    std::vector<std::vector<uint8_t>> commands;
    uint8_t num_expected_cmds;
};
