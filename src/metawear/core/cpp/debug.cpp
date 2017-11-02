#include "metawearboard_def.h"
#include "datainterpreter.h"
#include "debug_private.h"
#include "register.h"

#include "metawear/core/debug.h"
#include "metawear/core/module.h"
#include "metawear/core/status.h"

#include <chrono>
#include <memory>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::forward_as_tuple;
using std::make_shared;
using std::piecewise_construct;
using std::static_pointer_cast;

enum class DebugRegister : uint8_t {
    RESET = 0x1,
    BOOTLOADER,
    RESET_GC = 0x5,
    DISCONNECT,
    POWER_SAVE,
    STACK_OVERFLOW = 0x9,
    SCHEDULE_QUEUE
};

struct DebugState {
    MblMwFnData overflow_state_handler, schedule_queue_handler;
};

const uint8_t RES_MONITOR_REVISION= 2;

#define GET_DEBUG_STATE(board) static_pointer_cast<DebugState>(board->debug_state)
#define CAST_RESPONSE(type, handler) auto data = data_response_converters.at(type)(false, nullptr, response + 2, len - 2);\
data->epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();\
\
GET_DEBUG_STATE(board)->handler(data);\
free(data->value);\
free(data);

static int32_t schedule_queue_status_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CAST_RESPONSE(DataInterpreter::BYTE_ARRAY, schedule_queue_handler);
    return MBL_MW_STATUS_OK;
}

static int32_t overflow_status_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {    
    CAST_RESPONSE(DataInterpreter::DEBUG_OVERFLOW_STATE, overflow_state_handler);
    return MBL_MW_STATUS_OK;
}

void init_debug_module(MblMwMetaWearBoard *board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::SCHEDULE_QUEUE))),
        forward_as_tuple(schedule_queue_status_received));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::STACK_OVERFLOW))),
        forward_as_tuple(overflow_status_received));

    if (!board->debug_state) {
        board->debug_state = make_shared<DebugState>();
    }
}

void free_debug_module(void *state) {
    delete (DebugState*) state;
}

void mbl_mw_debug_reset(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::RESET)};
    SEND_COMMAND;
}

void mbl_mw_debug_jump_to_bootloader(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::BOOTLOADER)};
    SEND_COMMAND;
}

void mbl_mw_debug_disconnect(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::DISCONNECT)};
    SEND_COMMAND;
}

void mbl_mw_debug_reset_after_gc(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::RESET_GC)};
    SEND_COMMAND;
}

void mbl_mw_debug_read_schedule_queue_usage(const MblMwMetaWearBoard *board, MblMwFnData handler) {
    if (board->module_info.at(MBL_MW_MODULE_DEBUG).revision >= RES_MONITOR_REVISION) {
        GET_DEBUG_STATE(board)->schedule_queue_handler = handler;

        uint8_t command[2]= {MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::SCHEDULE_QUEUE))};
        SEND_COMMAND;
    } else {
        handler(nullptr);
    }
}

void mbl_mw_debug_set_stack_overflow_assertion(const MblMwMetaWearBoard *board, uint8_t enable) {
    if (board->module_info.at(MBL_MW_MODULE_DEBUG).revision >= RES_MONITOR_REVISION) {
        uint8_t command[3]= {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::STACK_OVERFLOW), static_cast<uint8_t>(enable == 0 ? enable : 1)};
        SEND_COMMAND;
    }
}

void mbl_mw_debug_read_stack_overflow_state(const MblMwMetaWearBoard *board, MblMwFnData handler) {
    if (board->module_info.at(MBL_MW_MODULE_DEBUG).revision >= RES_MONITOR_REVISION) {
        GET_DEBUG_STATE(board)->overflow_state_handler = handler;
        
        uint8_t command[2]= {MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::STACK_OVERFLOW))};
        SEND_COMMAND;
    } else {
        handler(nullptr);
    }
}