#include <cstdlib>
#include <cstring>
#include <exception>
#include <string>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "metawear/core/metawearboard.h"
#include "metawear/core/status.h"

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/event_register.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"
#include "metawear/core/cpp/timer_private.h"
#include "metawear/core/cpp/timer_register.h"

#include "metawear/processor/cpp/dataprocessor_register.h"
#include "metawear/processor/cpp/dataprocessor_private.h"

#include "metawear/sensor/cpp/accelerometer_bmi160_register.h"
#include "metawear/sensor/cpp/accelerometer_mma8452q_register.h"
#include "metawear/sensor/cpp/ambientlight_ltr329_register.h"
#include "metawear/sensor/cpp/barometer_bmp280_register.h"
#include "metawear/sensor/cpp/gpio_register.h"
#include "metawear/sensor/cpp/gyro_bmi160_register.h"
#include "metawear/sensor/cpp/multichanneltemperature_register.h"
#include "metawear/sensor/cpp/switch_register.h"

using std::exception;
using std::forward_as_tuple;
using std::free;
using std::memcpy;
using std::piecewise_construct;
using std::string;
using std::unordered_map;
using std::vector;

int32_t response_handler_data_no_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    try {
        ResponseHeader header(response[0], response[1]);
        MblMwData* data = data_response_converters.at(board->sensor_data_signals.at(header)->interpreter)(board, response + 2, len - 2);

        board->data_signal_handlers.at(header)(data);

        free(data->value);
        free(data);

        return MBL_MW_STATUS_OK;
    } catch (exception) {
        return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
    }
}

int32_t response_handler_data_with_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    try {
        ResponseHeader header(response[0], response[1], response[2]);
        MblMwData* data = data_response_converters.at(board->sensor_data_signals.at(header)->interpreter)(board, response + 3, len - 3);

        board->data_signal_handlers.at(header)(data);

        free(data->value);
        free(data);

        return MBL_MW_STATUS_OK;
    } catch (exception) {
        return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
    }
}

static int32_t timer_created(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto signal = board->pending_timers.front();
    signal->header.data_id = response[2];
    board->timer_signals.emplace(signal->header, signal);
    board->pending_timers.pop();

    board->timer_callbacks.front()(signal);
    board->timer_callbacks.pop();

    return MBL_MW_STATUS_OK;
}

static int32_t event_command_recorded(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    board->event_owner->event_command_ids.push_back(response[2]);

    if ((uint8_t)board->event_owner->event_command_ids.size() == board->event_owner->num_expected_cmds) {
        board->event_owner = nullptr;
        board->event_recorded_callback();
    }

    return MBL_MW_STATUS_OK;
}

static int32_t dataprocessor_created(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto next_processor = board->pending_dataprocessors.front();
    board->pending_dataprocessors.pop();
    next_processor->header.data_id = response[2];
    board->sensor_data_signals.emplace(next_processor->header, next_processor);

    auto next_callback = board->dataprocessor_callbacks.front();
    board->dataprocessor_callbacks.pop();
    next_callback(next_processor);

    return MBL_MW_STATUS_OK;
}

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
    {MBL_MW_MODULE_DEBUG, READ_INFO_REGISTER}
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

MblMwMetaWearBoard::MblMwMetaWearBoard() : data_token(nullptr) {
    sensor_data_signals[SWITCH_RESPONSE_HEADER]= new MblMwDataSignal(SWITCH_RESPONSE_HEADER, this, DataInterpreter::UINT32, 
            1, 1, 0, 0);

    responses[SWITCH_RESPONSE_HEADER]= response_handler_data_no_id;
    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_AI_ABS_REF))), 
            forward_as_tuple(response_handler_data_with_id));
    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_AI_ADC))),
        forward_as_tuple(response_handler_data_with_id));
    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_DI))),
        forward_as_tuple(response_handler_data_with_id));
    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY)),
        forward_as_tuple(response_handler_data_with_id));
    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY)),
        forward_as_tuple(response_handler_data_with_id));

    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::TIMER_ENTRY)),
        forward_as_tuple(timer_created));

    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::ENTRY)),
        forward_as_tuple(event_command_recorded));

    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::ADD)),
        forward_as_tuple(dataprocessor_created));
    responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_DATA_PROCESSOR, READ_REGISTER(ORDINAL(DataProcessorRegister::STATE))),
        forward_as_tuple(response_handler_data_with_id));
}

MblMwMetaWearBoard::~MblMwMetaWearBoard() {
    for (auto it: sensor_data_signals) {
        it.second->remove= false;
        delete it.second;
    }

    for (auto it: timer_signals) {
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

static inline void queue_next_query(MblMwMetaWearBoard *board) {
    board->module_discovery_index++;
    send_command(board, MODULE_DISCOVERY_CMDS[board->module_discovery_index].data(), 
            (uint8_t) MODULE_DISCOVERY_CMDS[board->module_discovery_index].size());
}

static inline void queue_next_gatt_char(MblMwMetaWearBoard *board) {
    board->dev_info_index++;
    board->btle_conn.read_gatt_char(&BOARD_DEV_INFO_CHARS[board->dev_info_index]);
}

void mbl_mw_metawearboard_initialize(MblMwMetaWearBoard *board, MblMwFnVoid initialized) {
    board->initialized = initialized;
    board->module_discovery_index= -1;
    board->dev_info_index = -1;
    queue_next_gatt_char(board);
}

void mbl_mw_metawearboard_tear_down(MblMwMetaWearBoard *board) {
    vector<ResponseHeader> dp_keys;

    for (auto it: board->sensor_data_signals) {
        if (typeid(*(it.second)) == typeid(MblMwDataProcessor)) {
            dp_keys.push_back(it.first);
            it.second->remove= false;
            delete it.second;
        }
    }
    for (auto it: dp_keys) {
        board->sensor_data_signals.erase(it);
    }

    for (auto it: board->timer_signals) {
        it.second->remove= false;
        delete it.second;
    }
    board->timer_signals.clear();

    uint8_t command[2]= { MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::REMOVE_ALL) };
    SEND_COMMAND;

    command[0]= MBL_MW_MODULE_EVENT;
    command[1]= ORDINAL(EventRegister::REMOVE_ALL);
    SEND_COMMAND;
}

int32_t mbl_mw_connection_notify_char_changed(MblMwMetaWearBoard *board, const uint8_t *value, uint8_t length) {
    ResponseHeader header(value[0], value[1]);

    if (board->responses.count(header) != 0) {
        return board->responses.at(header)(board, value, length);
    } else if (header.register_id == READ_INFO_REGISTER) {
        board->module_info.emplace(piecewise_construct, forward_as_tuple(value[0]), forward_as_tuple(value, length));

        if (board->module_discovery_index == (uint8_t)(MODULE_DISCOVERY_CMDS.size() - 1)) {
            init_accelerometer_module(board);
            init_barometer_module(board);
            init_gyro_module(board);
            init_ambient_light_module(board);
            init_multichannel_temp_module(board);

            board->initialized();
        } else {
            queue_next_query(board);
        }

        return MBL_MW_STATUS_OK;
    } else {
        return MBL_MW_STATUS_WARNING_INVALID_RESPONSE;
    }
}

void mbl_mw_connection_char_read(MblMwMetaWearBoard *board, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length) {
    bool valid = false;

    if (characteristic->uuid_high == DEV_INFO_FIRMWARE_CHAR.uuid_high && characteristic->uuid_low == DEV_INFO_FIRMWARE_CHAR.uuid_low) {
        if (board->firmware_revision.empty()) {
            board->firmware_revision.assign(string(value, value + length));
        } else {
            Version current;
            current.assign(string(value, value + length));

            if (board->firmware_revision == current) {
                board->initialized();
                return;
            } else {
                for (auto it: board->sensor_data_signals) {
                    it.second->remove= false;
                    delete it.second;
                }
                for (auto it: board->timer_signals) {
                    it.second->remove= false;
                    delete it.second;
                }
                for (auto it: board->module_config) {
                    free(it.second);
                }

                board->sensor_data_signals.clear();
                board->timer_signals.clear();
                board->module_config.clear();
                board->module_info.clear();
            }
        }
        valid = true;
    } else if (characteristic->uuid_high == DEV_INFO_MODEL_CHAR.uuid_high && characteristic->uuid_low == DEV_INFO_MODEL_CHAR.uuid_low) {
        board->module_number.assign(value, value + length);
        valid = true;
    }

    if (valid) {
        if (board->dev_info_index == (uint8_t)(BOARD_DEV_INFO_CHARS.size() - 1)) {
            queue_next_query(board);
        } else {
            queue_next_gatt_char(board);
        }
    }
}

void send_command(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len) {
    if (!board->event_config.empty()) {
        board->event_owner->num_expected_cmds++;
        vector<uint8_t> event_entry = { MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::ENTRY),
            board->event_config.at(0), board->event_config.at(1), board->event_config.at(2), command[0], command[1],
            (uint8_t)(len - 2) };

        if (board->data_token != nullptr) {
            uint8_t event_data[2] = {
                (uint8_t)(0x01 | (board->data_token->data_length << 1) | (board->data_token->data_offset << 4)),
                board->data_token->dest_offset
            };
            event_entry.insert(event_entry.end(), event_data, event_data + sizeof(event_data));
        }
        board->event_owner->commands.emplace_back(event_entry);

        vector<uint8_t> event_parameters(command + 2, command + len);
        uint8_t prefix[2] = { MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::CMD_PARAMETERS) };
        event_parameters.insert(event_parameters.begin(), prefix, prefix + sizeof(prefix));
        board->event_owner->commands.emplace_back(event_parameters);
    } else {
        board->btle_conn.write_gatt_char(&METAWEAR_COMMAND_CHAR, command, len);
    }
}

int32_t mbl_mw_metawearboard_is_initialized(const MblMwMetaWearBoard *board) {
    return board->module_discovery_index == (uint8_t)(MODULE_DISCOVERY_CMDS.size() - 1);
}

int32_t mbl_mw_metawearboard_lookup_module(const MblMwMetaWearBoard *board, uint8_t module) {
    try {
        if (board->module_info.at(module).present) {
            return board->module_info.at(module).implementation;
        }
        return MBL_MW_MODULE_TYPE_NA;
    } catch (exception) {
        return MBL_MW_MODULE_TYPE_NA;
    }
}
