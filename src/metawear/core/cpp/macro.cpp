#include "constant.h"
#include "metawearboard_def.h"
#include "register.h"

#include "macro_private.h"
#include "macro_register.h"
#include "metawear/core/macro.h"
#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/platform/cpp/threadpool.h"

#include <stdint.h>
#include <vector>

using std::forward_as_tuple;
using std::make_shared;
using std::piecewise_construct;
using std::static_pointer_cast;
using std::vector;

#define GET_MACRO_STATE(board) static_pointer_cast<MacroState>(board->macro_state)

const size_t PARTIAL_LENGTH = 2;
const ResponseHeader MACRO_BEGIN(MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::BEGIN));

struct MacroState {
    MacroState();

    MblMwFnBoardPtrInt commands_recorded;
    void *commands_recorded_context;
    vector<vector<uint8_t>> commands;
    bool is_recording;
    uint8_t exec_on_boot;
};

MacroState::MacroState() : is_recording(false) { }

static int32_t macro_add_cmd_response(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state = GET_MACRO_STATE(board);

    for(auto it: state->commands) {
        send_command(board, it.data(), it.size());
    }

    uint8_t end_cmd[2] = {MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::END)};
    send_command(board, end_cmd, sizeof(end_cmd));

    state->commands_recorded(state->commands_recorded_context, board, response[2]);

    return MBL_MW_STATUS_OK;
}

static int32_t macro_add_cmd_response_raw(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state = GET_MACRO_STATE(board);
    state->commands_recorded(state->commands_recorded_context, board, response[2]);

    return MBL_MW_STATUS_OK;
}

void init_macro_module(MblMwMetaWearBoard *board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::BEGIN)),
        forward_as_tuple(macro_add_cmd_response));

    if (!board->macro_state) {
        board->macro_state = make_shared<MacroState>();
    }
}

void free_macro_module(void *state) {
    delete (MacroState*) state;
}

void mbl_mw_macro_record(MblMwMetaWearBoard *board, uint8_t exec_on_boot) {
    auto state = GET_MACRO_STATE(board);
    state->commands.clear();
    state->is_recording = true;
    state->exec_on_boot = exec_on_boot == 0 ? 0 : 1;
}

void mbl_mw_macro_record_raw(MblMwMetaWearBoard *board, uint8_t exec_on_boot, void *context, MblMwFnBoardPtrInt ready) {
    auto state = GET_MACRO_STATE(board);
    board->responses[MACRO_BEGIN] = macro_add_cmd_response_raw;
    state->commands_recorded_context = context;
    state->commands_recorded = ready;
    
    uint8_t command[3]= {MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::BEGIN), exec_on_boot};
    send_command(board, command, sizeof(command));
}

void mbl_mw_macro_end_record(MblMwMetaWearBoard *board, void *context, MblMwFnBoardPtrInt commands_recorded) {
    auto state = GET_MACRO_STATE(board);
    state->is_recording = false;
    state->commands_recorded_context = context;
    state->commands_recorded = commands_recorded;

    ThreadPool::schedule([state, board](void) -> void {
        uint8_t command[3]= {MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::BEGIN), state->exec_on_boot};
        send_command(board, command, sizeof(command));
    }, 2000);
}

void mbl_mw_macro_execute(MblMwMetaWearBoard *board, uint8_t id) {
    uint8_t command[3] = { MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::EXECUTE), id };
    send_command(board, command, sizeof(command));
}

void mbl_mw_macro_erase_all(MblMwMetaWearBoard *board) {
    uint8_t command[2] = { MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::ERASE_ALL) };
    send_command(board, command, sizeof(command));
}

void record_macro(const MblMwMetaWearBoard *board, const uint8_t* command, uint8_t len) {
    auto state = GET_MACRO_STATE(board);
    if (state != nullptr && state->is_recording) {
        if (len >= MW_CMD_MAX_LENGTH) {
            vector<uint8_t> macro_cmd = { MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::ADD_PARTIAL) };
            macro_cmd.insert(macro_cmd.end(), command, command + PARTIAL_LENGTH);
            state->commands.push_back(macro_cmd);

            macro_cmd = { MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::ADD_COMMAND) };
            macro_cmd.insert(macro_cmd.end(), command + PARTIAL_LENGTH, command + (len - PARTIAL_LENGTH));
            state->commands.push_back(macro_cmd);
        } else {
            vector<uint8_t> macro_cmd = { MBL_MW_MODULE_MACRO, ORDINAL(MacroRegister::ADD_COMMAND) };
            macro_cmd.insert(macro_cmd.end(), command, command + len);

            state->commands.push_back(macro_cmd);
        }
    }
}
