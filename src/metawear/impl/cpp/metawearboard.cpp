#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <functional>
#include <string>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "metawear/core/metawearboard.h"
#include "metawear/core/status.h"

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/event_register.h"
#include "metawear/core/cpp/logging_register.h"
#include "metawear/core/cpp/logging_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"
#include "metawear/core/cpp/settings_register.h"
#include "metawear/core/cpp/settings_private.h"
#include "metawear/core/cpp/timer_private.h"

#include "metawear/platform/cpp/threadpool.h"

#include "metawear/processor/cpp/dataprocessor_register.h"
#include "metawear/processor/cpp/dataprocessor_private.h"

#include "metawear/sensor/cpp/accelerometer_private.h"
#include "metawear/sensor/cpp/ambientlight_ltr329_private.h"
#include "metawear/sensor/cpp/barometer_bosch_private.h"
#include "metawear/sensor/cpp/colordetector_tcs34725_private.h"
#include "metawear/sensor/cpp/gpio_private.h"
#include "metawear/sensor/cpp/gpio_register.h"
#include "metawear/sensor/cpp/gyro_bmi160_private.h"
#include "metawear/sensor/cpp/humidity_bme280_private.h"
#include "metawear/sensor/cpp/magnetometer_bmm150_private.h"
#include "metawear/sensor/cpp/multichanneltemperature_private.h"
#include "metawear/sensor/cpp/proximity_tsl2671_private.h"
#include "metawear/sensor/cpp/serialpassthrough_private.h"
#include "metawear/sensor/cpp/serialpassthrough_register.h"
#include "metawear/sensor/cpp/switch_private.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::exception;
using std::forward_as_tuple;
using std::free;
using std::less;
using std::malloc;
using std::memcpy;
using std::piecewise_construct;
using std::sort;
using std::string;
using std::unordered_map;
using std::vector;

const uint8_t CARTESIAN_FLOAT_SIZE= 6;
const uint16_t MAX_TIME_PER_RESPONSE= 4000;

#define CLEAR_READ_MODIFIERS(x) (x & 0x3f)

static inline int32_t forward_response(const ResponseHeader& header, MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    try {
        auto signal = dynamic_cast<MblMwDataSignal*>(board->module_events.at(header));
        if (signal->handler == nullptr) {
            return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
        }

        MblMwData* data = data_response_converters.at(signal->interpreter)(board, response, len);
        data->epoch= duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        signal->handler(data);

        free(data->value);
        free(data);

        return MBL_MW_STATUS_OK;
    } catch (exception) {
        return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
    }
}

int32_t response_handler_data_no_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    return forward_response(ResponseHeader(response[0], response[1]), board, response + 2, len - 2);
}

int32_t response_handler_data_with_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    return forward_response(ResponseHeader(response[0], response[1], response[2]), board, response + 3, len - 3);
}

int32_t response_handler_packed_data(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    try {
        ResponseHeader header(response[0], response[1]);
        auto signal = dynamic_cast<MblMwDataSignal*>(board->module_events.at(header));
        if (signal->handler == nullptr) {
            return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
        }

        for(uint8_t i= 2; i < len; i+= CARTESIAN_FLOAT_SIZE) {
            MblMwData* data = data_response_converters.at(signal->interpreter)(board, response + i, len - i);
            data->epoch= duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

            signal->handler(data);

            free(data->value);
            free(data);
        }

        return MBL_MW_STATUS_OK;
    } catch (exception) {
        return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
    }
}

const vector<void(*)(MblMwMetaWearBoard*)> INITIALIZE_FNS = {
    init_accelerometer_module,
    init_barometer_module,
    init_gyro_module,
    init_ambient_light_module,
    init_multichannel_temp_module,
    init_logging,
    init_magnetometer_module,
    init_settings_module,
    init_colordetector_module,
    init_proximity_module,
    init_humidity_module,
    init_event_module,
    init_timer_module,
    init_dataprocessor_module,
    init_gpio_module,
    init_serialpassthrough_module,
    init_switch_module
};

const uint8_t READ_INFO_REGISTER= READ_REGISTER(0x0);
const vector<vector<uint8_t>> MODULE_DISCOVERY_CMDS= {
    {MBL_MW_MODULE_SWITCH, READ_INFO_REGISTER},
    {MBL_MW_MODULE_LED, READ_INFO_REGISTER},
    {MBL_MW_MODULE_ACCELEROMETER, READ_INFO_REGISTER},
    {MBL_MW_MODULE_TEMPERATURE, READ_INFO_REGISTER},
    {MBL_MW_MODULE_GPIO, READ_INFO_REGISTER},
    {MBL_MW_MODULE_NEO_PIXEL, READ_INFO_REGISTER},
    {MBL_MW_MODULE_IBEACON, READ_INFO_REGISTER},
    {MBL_MW_MODULE_HAPTIC, READ_INFO_REGISTER},
    {MBL_MW_MODULE_DATA_PROCESSOR, READ_INFO_REGISTER},
    {MBL_MW_MODULE_EVENT, READ_INFO_REGISTER},
    {MBL_MW_MODULE_LOGGING, READ_INFO_REGISTER},
    {MBL_MW_MODULE_TIMER, READ_INFO_REGISTER},
    {MBL_MW_MODULE_I2C, READ_INFO_REGISTER},
    {MBL_MW_MODULE_MACRO, READ_INFO_REGISTER},
    {MBL_MW_MODULE_GSR, READ_INFO_REGISTER},
    {MBL_MW_MODULE_SETTINGS, READ_INFO_REGISTER},
    {MBL_MW_MODULE_BAROMETER, READ_INFO_REGISTER},
    {MBL_MW_MODULE_GYRO, READ_INFO_REGISTER},
    {MBL_MW_MODULE_AMBIENT_LIGHT, READ_INFO_REGISTER},
    {MBL_MW_MODULE_MAGNETOMETER, READ_INFO_REGISTER},
    {MBL_MW_MODULE_HUMIDITY, READ_INFO_REGISTER},
    {MBL_MW_MODULE_COLOR_DETECTOR, READ_INFO_REGISTER},
    {MBL_MW_MODULE_PROXIMITY, READ_INFO_REGISTER},
    {MBL_MW_MODULE_DEBUG, READ_INFO_REGISTER}
};

const unordered_map<uint8_t, void(*)(const MblMwMetaWearBoard*, vector<uint8_t>&)> CONFIG_SERIALIZATION = {
    { MBL_MW_MODULE_ACCELEROMETER, serialize_accelerometer_config },
    { MBL_MW_MODULE_BAROMETER, serialize_barometer_config },
    { MBL_MW_MODULE_GYRO, serialize_gyro_config },
    { MBL_MW_MODULE_AMBIENT_LIGHT, serialize_ambient_light_config },
    { MBL_MW_MODULE_COLOR_DETECTOR, serialize_colordetector_config },
    { MBL_MW_MODULE_PROXIMITY, serialize_proximity_config }
};
const unordered_map<uint8_t, void(*)(MblMwMetaWearBoard*, uint8_t**)> CONFIG_DESERIALIZATION = {
    { MBL_MW_MODULE_ACCELEROMETER, deserialize_accelerometer_config },
    { MBL_MW_MODULE_BAROMETER, deserialize_barometer_config },
    { MBL_MW_MODULE_GYRO, deserialize_gyro_config },
    { MBL_MW_MODULE_AMBIENT_LIGHT, deserialize_ambient_light_config },
    { MBL_MW_MODULE_COLOR_DETECTOR, deserialize_colordetector_config },
    { MBL_MW_MODULE_PROXIMITY, deserialize_proximity_config }
};

const uint64_t DEVICE_INFO_SERVICE_UUID_HIGH = 0x0000180a00001000;
const uint64_t DEVICE_INFO_SERVICE_UUID_LOW = 0x800000805f9b34fb;

const MblMwGattChar METAWEAR_COMMAND_CHAR = { METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_high, METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_low, 
        0x326a900185cb9195, 0xd9dd464cfbbae75a };
const MblMwGattChar DEV_INFO_FIRMWARE_CHAR = { DEVICE_INFO_SERVICE_UUID_HIGH, DEVICE_INFO_SERVICE_UUID_LOW, 0x00002a2600001000, 0x800000805f9b34fb },
        DEV_INFO_MODEL_CHAR = { DEVICE_INFO_SERVICE_UUID_HIGH, DEVICE_INFO_SERVICE_UUID_LOW, 0x00002a2400001000, 0x800000805f9b34fb };

const vector<MblMwGattChar> BOARD_DEV_INFO_CHARS = {
    DEV_INFO_FIRMWARE_CHAR,
    DEV_INFO_MODEL_CHAR
};

const uint8_t SERIALIZATION_FORMAT = 0;

MblMwMetaWearBoard::MblMwMetaWearBoard() : logger_state(nullptr, [](void *ptr) -> void { tear_down_logging(ptr, false); }), 
        timer_state(nullptr, [](void *ptr) -> void { free_timer_module(ptr); }),
        event_state(nullptr, [](void *ptr) -> void { free_event_module(ptr); }), 
        dp_state(nullptr, [](void *ptr) -> void { free_dataprocessor_module(ptr); }),
        time_per_response(150), module_discovery_index(-1) {
}

MblMwMetaWearBoard::~MblMwMetaWearBoard() {
    logger_state.reset();
    timer_state.reset();
    event_state.reset();
    dp_state.reset();

    for (auto it: module_events) {
        it.second->remove= false;
        delete it.second;
    }

    for (auto it: module_config) {
        free(it.second);
    }
}

MblMwMetaWearBoard* mbl_mw_metawearboard_create(const MblMwBtleConnection *connection) {
    auto new_board= new MblMwMetaWearBoard();
    memcpy(&new_board->btle_conn, connection, sizeof(MblMwBtleConnection));
    return new_board;
}

void mbl_mw_metawearboard_free(MblMwMetaWearBoard *board) {
    delete board;
}

void mbl_mw_metawearboard_set_time_for_response(MblMwMetaWearBoard* board, uint16_t response_time_ms) {
    board->time_per_response= response_time_ms > MAX_TIME_PER_RESPONSE ? MAX_TIME_PER_RESPONSE : response_time_ms;
}

static inline void service_discovery_completed(MblMwMetaWearBoard* board) {
    for (auto it : INITIALIZE_FNS) {
        it(board);
    }

    uint8_t command[2] = { MBL_MW_MODULE_LOGGING, READ_REGISTER(ORDINAL(LoggingRegister::TIME)) };
    SEND_COMMAND;
}

static inline void queue_next_query(MblMwMetaWearBoard *board) {
    do {
        board->module_discovery_index++;
    } while (board->module_discovery_index < (int8_t) MODULE_DISCOVERY_CMDS.size() && 
            board->module_info.count(MODULE_DISCOVERY_CMDS[board->module_discovery_index][0]));

    if (board->module_discovery_index >= (int8_t) MODULE_DISCOVERY_CMDS.size()) {
        service_discovery_completed(board);
    } else {
        send_command(board, MODULE_DISCOVERY_CMDS[board->module_discovery_index].data(),
            (uint8_t) MODULE_DISCOVERY_CMDS[board->module_discovery_index].size());
    }
}

static inline void queue_next_gatt_char(MblMwMetaWearBoard *board) {
    board->dev_info_index++;
    if (board->dev_info_index >= (int8_t) BOARD_DEV_INFO_CHARS.size()) {
        queue_next_query(board);
    } else {
        board->btle_conn.read_gatt_char(board, &BOARD_DEV_INFO_CHARS[board->dev_info_index]);
    }
}

void mbl_mw_metawearboard_initialize(MblMwMetaWearBoard *board, MblMwFnBoardPtrInt initialized) {
    board->initialized = initialized;
    board->dev_info_index = -1;

    board->initialized_timeout= ThreadPool::schedule([board](void) -> void {
        board->initialized(board, MBL_MW_STATUS_ERROR_TIMEOUT);
    }, (MODULE_DISCOVERY_CMDS.size() + BOARD_DEV_INFO_CHARS.size() + 1) * board->time_per_response);
    queue_next_gatt_char(board);
}

void mbl_mw_metawearboard_tear_down(MblMwMetaWearBoard *board) {
    vector<ResponseHeader> spawned_keys;

    tear_down_logging(board->logger_state.get(), true);

    for (auto it: board->module_events) {
        if (it.second->header.module_id == MBL_MW_MODULE_DATA_PROCESSOR || it.second->header.module_id == MBL_MW_MODULE_TIMER) {
            spawned_keys.push_back(it.first);
        } else {
            it.second->event_command_ids.clear();
        }
    }
    sort(spawned_keys.begin(), spawned_keys.end());
    for (auto it: spawned_keys) {
        auto event= board->module_events.at(it);

        if (it.module_id == MBL_MW_MODULE_TIMER) {
            dynamic_cast<MblMwTimer*>(event)->remove_from_board();
        }

        event->remove = false;
        delete event;

        board->module_events.erase(it);
    }

    uint8_t command[2]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::REMOVE_ALL) };
    SEND_COMMAND;

    command[0]= MBL_MW_MODULE_EVENT;
    command[1]= ORDINAL(EventRegister::REMOVE_ALL);
    SEND_COMMAND;

    command[0]= MBL_MW_MODULE_LOGGING;
    command[1]= ORDINAL(LoggingRegister::REMOVE_ALL);
    SEND_COMMAND;
}

int32_t mbl_mw_connection_notify_char_changed(MblMwMetaWearBoard *board, const uint8_t *value, uint8_t len) {
    return mbl_mw_metawearboard_notify_char_changed(board, value, len);
}

int32_t mbl_mw_metawearboard_notify_char_changed(MblMwMetaWearBoard *board, const uint8_t *value, uint8_t len) {
    ResponseHeader header(value[0], value[1]);

    if (board->responses.count(header)) {
        return board->responses.at(header)(board, value, len);
    } else if (header.register_id == READ_INFO_REGISTER) {
        board->module_info.emplace(piecewise_construct, forward_as_tuple(value[0]), forward_as_tuple(value, len));
        queue_next_query(board);
        return MBL_MW_STATUS_OK;
    } else {
        return MBL_MW_STATUS_WARNING_INVALID_RESPONSE;
    }
}

void mbl_mw_connection_char_read(MblMwMetaWearBoard *board, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length) {
    mbl_mw_metawearboard_char_read(board, characteristic, value, length);
}

void mbl_mw_metawearboard_char_read(MblMwMetaWearBoard *board, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length) {
    if (characteristic->uuid_high == DEV_INFO_FIRMWARE_CHAR.uuid_high && characteristic->uuid_low == DEV_INFO_FIRMWARE_CHAR.uuid_low) {
        if (board->firmware_revision.empty()) {
            board->firmware_revision.assign(string(value, value + length));
            board->module_discovery_index= -1;
        } else {
            Version current;
            current.assign(string(value, value + length));

            if (board->firmware_revision == current) {
                if (mbl_mw_metawearboard_is_initialized(board)) {
                    board->initialized_timeout->cancel();
                    board->initialized(board, MBL_MW_STATUS_OK);
                } else {
                    board->module_discovery_index = MODULE_DISCOVERY_CMDS.size();
                    service_discovery_completed(board);
                }
                return;
            } else {
                board->logger_state.reset();
                board->timer_state.reset();
                board->event_state.reset();
                board->dp_state.reset();

                for (auto it : board->module_events) {
                    it.second->remove = false;
                    delete it.second;
                }
                board->module_events.clear();

                for (auto it: board->module_config) {
                    free(it.second);
                }
                board->module_config.clear();

                board->module_info.clear();
                board->module_discovery_index= -1;
            }
        }
        queue_next_gatt_char(board);
    } else if (characteristic->uuid_high == DEV_INFO_MODEL_CHAR.uuid_high && characteristic->uuid_low == DEV_INFO_MODEL_CHAR.uuid_low) {
        board->module_number.assign(value, value + length);
        queue_next_gatt_char(board);
    }
}

void send_command(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len) {
    if (!record_command(board, command, len)) {
        board->btle_conn.write_gatt_char(board, &METAWEAR_COMMAND_CHAR, command, len);
    }
}

int32_t mbl_mw_metawearboard_is_initialized(const MblMwMetaWearBoard *board) {
    return board->module_discovery_index == (int8_t) MODULE_DISCOVERY_CMDS.size();
}

int32_t mbl_mw_metawearboard_lookup_module(const MblMwMetaWearBoard *board, MblMwModule module) {
    try {
        if (board->module_info.at(module).present) {
            return board->module_info.at(module).implementation;
        }
        return MBL_MW_MODULE_TYPE_NA;
    } catch (exception) {
        return MBL_MW_MODULE_TYPE_NA;
    }
}

uint8_t* mbl_mw_metawearboard_serialize(const MblMwMetaWearBoard* board, uint32_t* size) {
    vector<uint8_t> serialized_state;

    serialized_state.push_back(SERIALIZATION_FORMAT);
    
    board->firmware_revision.serialize(serialized_state);

    serialized_state.push_back((uint8_t)board->module_number.size());
    serialized_state.insert(serialized_state.end(), board->module_number.begin(), board->module_number.end());

    {
        vector<uint8_t> sorted_keys;
        for (auto it : board->module_info) {
            sorted_keys.push_back(it.first);
        }
        sort(sorted_keys.begin(), sorted_keys.end());

        serialized_state.push_back((uint8_t)board->module_info.size());
        for (auto it : sorted_keys) {
            board->module_info.at(it).serialize(serialized_state);
        }
    }

    {
        vector<ResponseHeader> sorted_keys;
        for (auto it : board->module_events) {
            sorted_keys.emplace_back(it.first.module_id, it.first.register_id, it.first.data_id);
        }
        sort(sorted_keys.begin(), sorted_keys.end());

        serialized_state.push_back((uint8_t)board->module_events.size());
        for (auto it : sorted_keys) {
            board->module_events.at(it)->serialize(serialized_state);
        }
    }

    {
        vector<uint8_t> sorted_keys;
        for (auto it : board->module_config) {
            sorted_keys.push_back(it.first);
        }
        sort(sorted_keys.begin(), sorted_keys.end());

        serialized_state.push_back((uint8_t)board->module_config.size());
        for (auto it : sorted_keys) {
            serialized_state.push_back(it);
            CONFIG_SERIALIZATION.at(it)(board, serialized_state);
        }
    }
    
    serialize_logging(board, serialized_state);
    
    *size = (uint32_t) serialized_state.size();
    uint8_t* state_bytes = (uint8_t*)malloc(sizeof(uint8_t) * (*size));
    memcpy(state_bytes, serialized_state.data(), *size);
    return state_bytes;
}

void mbl_mw_metawearboard_deserialize(MblMwMetaWearBoard* board, uint8_t* state, uint32_t size) {
    uint8_t* current_addr = state;

    if (*current_addr != SERIALIZATION_FORMAT) {
        return;
    }

    board->firmware_revision.deserialize(&(++current_addr));

    uint8_t module_str_size = *current_addr;
    current_addr++;
    board->module_number.assign(current_addr, current_addr + module_str_size);
    current_addr += module_str_size;

    board->module_info.clear();
    uint8_t module_info_size = *current_addr;
    current_addr++;
    for (uint8_t i = 0; i < module_info_size; i++) {
        board->module_info.emplace(*current_addr, &current_addr);
    }
    
    for (auto it : board->module_events) {
        delete it.second;
    }
    board->module_events.clear();

    uint8_t n_events = *current_addr;
    current_addr++;
    for (uint8_t i = 0; i < n_events; i++) {
        MblMwEvent* saved_event;

        switch (*current_addr) {
        case MBL_MW_MODULE_GPIO:
            switch(static_cast<GpioRegister>(CLEAR_READ_MODIFIERS(*(current_addr + 1)))) {
            case GpioRegister::READ_AI_ABS_REF:
            case GpioRegister::READ_AI_ADC:
                saved_event= new MblMwGpioAnalogSignal(&current_addr, board);
                break;
            case GpioRegister::PIN_CHANGE_NOTIFY:
                saved_event= new MblMwGpioPinNotifySignal(&current_addr, board);
                break;
            default:
                saved_event = new MblMwDataSignal(&current_addr, board);
                break;
            }
            break;
        case MBL_MW_MODULE_DATA_PROCESSOR:
            saved_event = static_cast<DataProcessorRegister>(CLEAR_READ_MODIFIERS(*(current_addr + 1))) == DataProcessorRegister::STATE ?
                new MblMwDataSignal(&current_addr, board) :
                new MblMwDataProcessor(&current_addr, board);
            break;
        case MBL_MW_MODULE_TIMER:
            saved_event = new MblMwTimer(&current_addr, board);
            break;
        case MBL_MW_MODULE_I2C:
            switch(static_cast<SerialPassthroughRegister>(CLEAR_READ_MODIFIERS(*(current_addr + 1)))) {
            case SerialPassthroughRegister::I2C_READ_WRITE:
                saved_event= new MblMwI2cSignal(&current_addr, board);
                break;
            case SerialPassthroughRegister::SPI_READ_WRITE:
                saved_event= new MblMwSpiSignal(&current_addr, board);
                break;
            default:
                saved_event= new MblMwDataSignal(&current_addr, board);
                break;
            }
            break;
        case MBL_MW_MODULE_SETTINGS:
            saved_event = static_cast<SettingsRegister>(*(current_addr + 1)) == SettingsRegister::DISCONNECT_EVENT ?
                new MblMwEvent(&current_addr, board) :
                new MblMwDataSignal(&current_addr, board);
            break;
        default:
            saved_event = new MblMwDataSignal(&current_addr, board);
            break;
        }

        ResponseHeader header_copy(saved_event->header);
        header_copy.disable_silent();
        board->module_events.emplace(header_copy, saved_event);
    }

    uint8_t module_config_size= *current_addr;
    current_addr++;
    for (uint8_t i= 0; i < module_config_size; i++) {
        auto fn= CONFIG_DESERIALIZATION.at(*current_addr);
        current_addr++;
        fn(board, &current_addr);
    }

    deserialize_logging(board, &current_addr);
}
