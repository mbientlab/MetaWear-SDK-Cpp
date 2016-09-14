#include "metawear/core/event.h"
#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/platform/cpp/threadpool.h"

#include "event_register.h"
#include "event_private.h"
#include "metawearboard_def.h"
#include "register.h"

#include <vector>

using std::forward_as_tuple;
using std::make_shared;
using std::piecewise_construct;
using std::static_pointer_cast;
using std::vector;

#define GET_EVENT_STATE(board) static_pointer_cast<EventState>(board->event_state)

struct EventState {
    Task* record_cmd_task;
    MblMwFnEventPtrInt event_recorded_callback;
    MblMwEvent* event_owner;
    const EventDataParameter* data_token;
    vector<uint8_t> event_config;

    EventState();
};

EventState::EventState() : event_recorded_callback(nullptr), event_owner(nullptr), data_token(nullptr) {
}

MblMwEvent::MblMwEvent(uint8_t** state_stream, MblMwMetaWearBoard *owner) : header(state_stream), owner(owner), remove(true) {
    uint8_t n_cmds = **state_stream;
    for (uint8_t i = 0; i < n_cmds; i++) {
        event_command_ids.push_back(*(++(*state_stream)));
    }

    (*state_stream)++;
}

MblMwEvent::MblMwEvent(const ResponseHeader& header, MblMwMetaWearBoard *owner) : header(header), owner(owner), remove(true) {
}

MblMwEvent::~MblMwEvent() {
    if (remove) {
        uint8_t command[3]= {MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::REMOVE)};

        for(auto it: event_command_ids) {
            command[2]= it;
            SEND_COMMAND_BOARD(owner);
        }
    }
}

void MblMwEvent::serialize(vector<uint8_t>& state) const {
    header.serialize(state);

    state.push_back((uint8_t) event_command_ids.size());
    state.insert(state.end(), event_command_ids.begin(), event_command_ids.end());
}

static int32_t event_command_recorded(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state = GET_EVENT_STATE(board);

    state->event_owner->event_command_ids.push_back(response[2]);

    if ((uint8_t)state->event_owner->event_command_ids.size() == state->event_owner->num_expected_cmds) {
        state->record_cmd_task->cancel();

        auto caller = state->event_owner;
        state->event_owner = nullptr;
        state->event_recorded_callback(caller, MBL_MW_STATUS_OK);
    }

    return MBL_MW_STATUS_OK;
}

void init_event_module(MblMwMetaWearBoard* board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::ENTRY)),
        forward_as_tuple(event_command_recorded));

    if (!board->event_state) {
        board->event_state = make_shared<EventState>();
    }
}

void free_event_module(void *state) {
    delete (EventState*) state;
}

void mbl_mw_event_record_commands(MblMwEvent *event) {
    auto state = GET_EVENT_STATE(event->owner);

    state->event_owner= event;
    event->commands.clear();
    event->num_expected_cmds= 0;
    state->event_config.assign({event->header.module_id, event->header.register_id, event->header.data_id});
}

void mbl_mw_event_end_record(MblMwEvent *event, MblMwFnEventPtrInt commands_recorded) {
    auto state = GET_EVENT_STATE(event->owner);

    state->event_recorded_callback= commands_recorded;
    state->event_config.clear();
    state->record_cmd_task= ThreadPool::schedule([state, event](void) -> void {
        state->event_owner = nullptr;
        state->event_recorded_callback(event, MBL_MW_STATUS_ERROR_TIMEOUT);
    }, event->commands.size() * event->owner->time_per_response);

    for(auto it: event->commands) {
        send_command(event->owner, it.data(), (uint8_t) it.size());
    }
}

MblMwMetaWearBoard* mbl_mw_event_get_owner(const MblMwEvent *event) {
    return event->owner;
}

bool record_command(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len) {
    auto state = GET_EVENT_STATE(board);

    if (state != nullptr && !state->event_config.empty()) {
        state->event_owner->num_expected_cmds++;
        vector<uint8_t> event_entry = { MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::ENTRY),
            state->event_config.at(0), state->event_config.at(1), state->event_config.at(2), command[0], command[1],
            (uint8_t)(len - 2) };

        if (state->data_token != nullptr) {
            uint8_t event_data[2] = {
                (uint8_t)(0x01 | (state->data_token->data_length << 1) | (state->data_token->data_offset << 4)),
                state->data_token->dest_offset
            };
            event_entry.insert(event_entry.end(), event_data, event_data + sizeof(event_data));
        }
        state->event_owner->commands.emplace_back(event_entry);

        vector<uint8_t> event_parameters(command + 2, command + len);
        uint8_t prefix[2] = { MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::CMD_PARAMETERS) };
        event_parameters.insert(event_parameters.begin(), prefix, prefix + sizeof(prefix));
        state->event_owner->commands.emplace_back(event_parameters);

        return true;
    }
    return false;
}

void set_data_token(MblMwMetaWearBoard* board, const EventDataParameter* token) {
    GET_EVENT_STATE(board)->data_token = token;
}
void clear_data_token(MblMwMetaWearBoard* board) {
    GET_EVENT_STATE(board)->data_token = nullptr;;
}
