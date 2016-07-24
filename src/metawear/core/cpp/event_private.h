#pragma once

#include <vector>

#include "metawear/core/event_fwd.h"
#include "metawear/core/metawearboard_fwd.h"
#include "responseheader.h"

struct EventDataParameter {
    uint8_t data_length;
    uint8_t data_offset;
    uint8_t dest_offset;
};

struct MblMwEvent {
    MblMwEvent(uint8_t** state_stream, MblMwMetaWearBoard *owner);
    MblMwEvent(const ResponseHeader& header, MblMwMetaWearBoard* owner);
    virtual ~MblMwEvent();

    virtual void serialize(std::vector<uint8_t>& state) const;

    std::vector<uint8_t> event_command_ids;
    ResponseHeader header;

    MblMwMetaWearBoard* const owner;
    std::vector<std::vector<uint8_t>> commands;
    uint8_t num_expected_cmds;
    bool remove;
};

void init_event_module(MblMwMetaWearBoard* board);
void free_event_module(void *state);
bool record_command(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len);
void set_data_token(MblMwMetaWearBoard* board, const EventDataParameter* token);
void clear_data_token(MblMwMetaWearBoard* board);
