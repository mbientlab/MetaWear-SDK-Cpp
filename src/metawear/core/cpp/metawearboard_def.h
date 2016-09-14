#pragma once

#include <memory>
#include <stdint.h>
#include <unordered_map>

#include "moduleinfo.h"
#include "responseheader.h"
#include "version.h"

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/event_fwd.h"
#include "metawear/core/metawearboard_fwd.h"
#include "metawear/core/timer_fwd.h"
#include "metawear/platform/btle_connection.h"
#include "metawear/platform/cpp/task.h"

#define SEND_COMMAND send_command(board, command, sizeof(command))
#define SEND_COMMAND_BOARD(board) send_command(board, command, sizeof(command))

typedef int32_t (*ResponseHandler)(MblMwMetaWearBoard *board, const uint8_t*, uint8_t);

struct MblMwMetaWearBoard {
    MblMwMetaWearBoard();
    ~MblMwMetaWearBoard();

    std::unordered_map<ResponseHeader, MblMwEvent*> module_events;
    std::unordered_map<ResponseHeader, ResponseHandler> responses;
    std::unordered_map<uint8_t, ModuleInfo> module_info;
    std::unordered_map<uint8_t, void*> module_config;

    std::shared_ptr<void> logger_state, timer_state, event_state, dp_state;
    MblMwFnBoardPtrInt initialized;
    Task* initialized_timeout;
    MblMwBtleConnection btle_conn;
    Version firmware_revision;
    std::string module_number;

    int64_t time_per_response;
    int8_t module_discovery_index, dev_info_index;
};

void send_command(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len);

/** only for acc, gyro, and mag streaming */
int32_t response_handler_packed_data(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
int32_t response_handler_data_no_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
int32_t response_handler_data_with_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
