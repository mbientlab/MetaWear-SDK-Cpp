#include "metawearboard_def.h"
#include "datainterpreter.h"
#include "debug_private.h"
#include "register.h"
#include "datasignal_private.h"

#include "metawear/core/debug.h"
#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/core/cpp/metawearboard_macro.h"

#include <chrono>
#include <cstring>
#include <memory>

using namespace std;
using namespace std::chrono;

enum class DebugRegister : uint8_t {
    RESET = 0x1,
    BOOTLOADER,
    NOTIFICATION_SPOOF,
    KEY_REGISTER,
    RESET_GC,
    DISCONNECT,
    POWER_SAVE,
    STACK_OVERFLOW = 0x9,
    SCHEDULE_QUEUE
};

struct DebugState {
    MblMwFnData overflow_state_handler, schedule_queue_handler;
    void *overflow_state_context;
    void *schedule_queue_context;
};

const uint8_t RES_MONITOR_REVISION= 2;

const ResponseHeader 
    DEBUG_KEY_REGISTER_RESPONSE_HEADER(MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::KEY_REGISTER)));

#define GET_DEBUG_STATE(board) static_pointer_cast<DebugState>(board->debug_state)
#define CAST_RESPONSE(type, context, handler) auto data = data_response_converters.at(type)(false, nullptr, response + 2, len - 2);\
data->epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();\
\
GET_DEBUG_STATE(board)->handler(GET_DEBUG_STATE(board)->context, data);\
free_data(nullptr, data)

static int32_t schedule_queue_status_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CAST_RESPONSE(DataInterpreter::BYTE_ARRAY, schedule_queue_context, schedule_queue_handler);
    return MBL_MW_STATUS_OK;
}

static int32_t overflow_status_received(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {    
    CAST_RESPONSE(DataInterpreter::DEBUG_OVERFLOW_STATE, overflow_state_context, overflow_state_handler);
    return MBL_MW_STATUS_OK;
}

void init_debug_module(MblMwMetaWearBoard *board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::SCHEDULE_QUEUE))),
        forward_as_tuple(schedule_queue_status_received));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::STACK_OVERFLOW))),
        forward_as_tuple(overflow_status_received));

    if (!board->module_events.count(DEBUG_KEY_REGISTER_RESPONSE_HEADER)) {
        board->module_events[DEBUG_KEY_REGISTER_RESPONSE_HEADER] = new MblMwDataSignal(DEBUG_KEY_REGISTER_RESPONSE_HEADER, board, 
            DataInterpreter::UINT32, 1, 4, 0, 0);
    }
    board->responses[DEBUG_KEY_REGISTER_RESPONSE_HEADER] = response_handler_data_no_id;

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

void mbl_mw_debug_enable_power_save(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::POWER_SAVE)};
    SEND_COMMAND;
}

void mbl_mw_debug_read_schedule_queue_usage(const MblMwMetaWearBoard *board, void *context, MblMwFnData handler) {
    if (board->module_info.at(MBL_MW_MODULE_DEBUG).revision >= RES_MONITOR_REVISION) {
        GET_DEBUG_STATE(board)->schedule_queue_context = context;
        GET_DEBUG_STATE(board)->schedule_queue_handler = handler;

        uint8_t command[2]= {MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::SCHEDULE_QUEUE))};
        SEND_COMMAND;
    } else {
        handler(context, nullptr);
    }
}

void mbl_mw_debug_set_stack_overflow_assertion(const MblMwMetaWearBoard *board, uint8_t enable) {
    if (board->module_info.at(MBL_MW_MODULE_DEBUG).revision >= RES_MONITOR_REVISION) {
        uint8_t command[3]= {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::STACK_OVERFLOW), static_cast<uint8_t>(enable == 0 ? enable : 1)};
        SEND_COMMAND;
    }
}

void mbl_mw_debug_read_stack_overflow_state(const MblMwMetaWearBoard *board, void *context, MblMwFnData handler) {
    if (board->module_info.at(MBL_MW_MODULE_DEBUG).revision >= RES_MONITOR_REVISION) {
        GET_DEBUG_STATE(board)->overflow_state_context = context;
        GET_DEBUG_STATE(board)->overflow_state_handler = handler;
        
        uint8_t command[2]= {MBL_MW_MODULE_DEBUG, READ_REGISTER(ORDINAL(DebugRegister::STACK_OVERFLOW))};
        SEND_COMMAND;
    } else {
        handler(context, nullptr);
    }
}

void mbl_mw_debug_spoof_notification(const MblMwMetaWearBoard *board, const uint8_t *value, uint8_t length) {
    vector<uint8_t> command(value, value + length);
    command.insert(command.begin(), {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::NOTIFICATION_SPOOF)});

    send_command(board, command.data(), (uint8_t) command.size());
}

void mbl_mw_debug_send_command(const MblMwMetaWearBoard *board, const uint8_t *value, uint8_t length) {
    send_command(board, value, length);
}

MblMwDataSignal* mbl_mw_debug_get_key_register_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(DEBUG_KEY_REGISTER_RESPONSE_HEADER);
}

void mbl_mw_debug_set_key_register(const MblMwMetaWearBoard *board, uint32_t value) {
    uint8_t command[6]= {MBL_MW_MODULE_DEBUG, ORDINAL(DebugRegister::KEY_REGISTER)};
    memcpy(command + 2, &value, sizeof(uint32_t));
    SEND_COMMAND;
}
