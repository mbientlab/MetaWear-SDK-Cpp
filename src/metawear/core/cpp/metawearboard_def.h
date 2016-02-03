#pragma once

#include <queue>
#include <stdint.h>
#include <unordered_map>
#include <vector>

#include "moduleinfo.h"
#include "responseheader.h"
#include "version.h"

#include "metawear/core/callback.h"
#include "metawear/core/connection.h"
#include "metawear/core/data.h"
#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/event_fwd.h"
#include "metawear/core/metawearboard_fwd.h"
#include "metawear/core/timer_fwd.h"

#define SEND_COMMAND send_command(board, command, sizeof(command))
#define SEND_COMMAND_BOARD(board) send_command(board, command, sizeof(command))

struct EventDataParameter {
    uint8_t data_length;
    uint8_t data_offset;
    uint8_t dest_offset;
};

typedef int32_t (*ResponseHandler)(MblMwMetaWearBoard *board, const uint8_t*, uint8_t);

struct MblMwMetaWearBoard {
    MblMwMetaWearBoard();
    ~MblMwMetaWearBoard();

    std::vector<uint8_t> event_config;
    std::queue<MblMwTimer*> pending_timers;
    std::queue<MblMwTimerPtr> timer_callbacks;
    std::queue<MblMwFnDataProcessor> dataprocessor_callbacks;
    std::queue<MblMwDataProcessor*> pending_dataprocessors;
    std::unordered_map<uint8_t, ModuleInfo> module_info;
    std::unordered_map<ResponseHeader, MblMwDataSignal*> sensor_data_signals;
    std::unordered_map<ResponseHeader, ResponseHandler> responses;
    std::unordered_map<ResponseHeader, MblMwTimer*> timer_signals;
    std::unordered_map<ResponseHeader, MblMwFnData> data_signal_handlers;
    std::unordered_map<uint8_t, void*> module_config;

    MblMwFnVoid initialized, event_recorded_callback;
    MblMwEvent* event_owner;
    EventDataParameter* data_token;
    MblMwBtleConnection btle_conn;
    Version firmware_revision;
    std::string module_number;

    int8_t module_discovery_index, dev_info_index;
};

void send_command(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len);

int32_t response_handler_data_no_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
int32_t response_handler_data_with_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);

void init_accelerometer_module(MblMwMetaWearBoard *board);
void init_barometer_module(MblMwMetaWearBoard *board);
void init_gyro_module(MblMwMetaWearBoard *board);
void init_ambient_light_module(MblMwMetaWearBoard *board);
void init_multichannel_temp_module(MblMwMetaWearBoard *board);
