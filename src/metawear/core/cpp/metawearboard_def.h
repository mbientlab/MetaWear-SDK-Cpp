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
#include "metawear/dfu/cpp/dfu_operations.h"
#include "metawear/platform/btle_connection.h"
#include "metawear/platform/cpp/task.h"

#define SEND_COMMAND send_command(board, command, sizeof(command))
#define SEND_COMMAND_BOARD(board) send_command(board, command, sizeof(command))

enum class SerializationFormat : uint8_t {
    INIT = 0,
    SIGNAL_COMPONENT,
    TIME_REFERENCE
};

typedef int32_t (*ResponseHandler)(MblMwMetaWearBoard *board, const uint8_t*, uint8_t);
/** UUIDs for the MetaWear DFU characteristic */
const MblMwGattChar DFU_PACKET_CHAR = { 0x000015301212EFDE, 0x1523785FEABCD123, 0x000015321212EFDE, 0x1523785FEABCD123 };
const MblMwGattChar DFU_CONTROL_POINT_CHAR = { 0x000015301212EFDE, 0x1523785FEABCD123, 0x000015311212EFDE, 0x1523785FEABCD123 };

struct MblMwMetaWearBoard {
    MblMwMetaWearBoard();
    ~MblMwMetaWearBoard();

    std::unordered_map<ResponseHeader, MblMwEvent*> module_events;
    std::unordered_map<ResponseHeader, ResponseHandler> responses;
    std::unordered_map<uint8_t, ModuleInfo> module_info;
    std::unordered_map<uint8_t, void*> module_config;

    std::shared_ptr<void> logger_state, timer_state, event_state, dp_state, macro_state, debug_state;
    MblMwFnBoardPtrInt initialized;
    void *initialized_context;
    MblMwFnAnonSignalArray anon_signals_created;
    void *anon_signals_context;
    std::shared_ptr<Task> initialized_timeout;
    MblMwBtleConnection btle_conn;
    Version firmware_revision;
    std::string module_number, hardware_revision, serial_number, manufacturer;
    std::unique_ptr<DfuOperations> operations;
    const char* filename;

    int64_t time_per_response;
    int8_t module_discovery_index, dev_info_index;

    inline void write_gatt_char(const MblMwGattChar* gatt_char, MblMwGattCharWriteType type, const uint8_t* value, uint8_t len) const {
        btle_conn.write_gatt_char(btle_conn.context, this, type, gatt_char, value, len);
    }
};

void send_command(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len);

/** only for acc, gyro, and mag streaming */
int32_t response_handler_packed_data(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
int32_t response_handler_data_no_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
int32_t response_handler_data_with_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
