#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "metawear/core/metawearboard.h"
#include "metawear/core/status.h"
#include "metawear/core/logging.h"
#include "metawear/core/datasignal.h"
#include "metawear/core/types.h"

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/debug_private.h"
#include "metawear/core/cpp/event_register.h"
#include "metawear/core/cpp/logging_register.h"
#include "metawear/core/cpp/logging_private.h"
#include "metawear/core/cpp/macro_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"
#include "metawear/core/cpp/settings_register.h"
#include "metawear/core/cpp/settings_private.h"
#include "metawear/core/cpp/timer_private.h"

#include "metawear/platform/cpp/threadpool.h"

#include "metawear/processor/cpp/dataprocessor_config.h"
#include "metawear/processor/cpp/dataprocessor_register.h"
#include "metawear/processor/cpp/dataprocessor_private.h"

#include "metawear/sensor/accelerometer.h"
#include "metawear/sensor/gyro_bosch.h"
#include "metawear/sensor/sensor_fusion.h"

#include "metawear/sensor/cpp/accelerometer_private.h"
#include "metawear/sensor/cpp/ambientlight_ltr329_private.h"
#include "metawear/sensor/cpp/barometer_bosch_private.h"
#include "metawear/sensor/cpp/colordetector_tcs34725_private.h"
#include "metawear/sensor/cpp/gpio_private.h"
#include "metawear/sensor/cpp/gpio_register.h"
#include "metawear/sensor/cpp/gyro_bosch_private.h"
#include "metawear/sensor/cpp/humidity_bme280_private.h"
#include "metawear/sensor/cpp/magnetometer_bmm150_private.h"
#include "metawear/sensor/cpp/multichanneltemperature_private.h"
#include "metawear/sensor/cpp/proximity_tsl2671_private.h"
#include "metawear/sensor/cpp/serialpassthrough_private.h"
#include "metawear/sensor/cpp/serialpassthrough_register.h"
#include "metawear/sensor/cpp/sensor_fusion_private.h"
#include "metawear/sensor/cpp/switch_private.h"
#include "metawear/sensor/cpp/conductance_private.h"

using namespace std;
using namespace std::chrono;

/** Constant signifying a module is not available */
const int32_t MBL_MW_MODULE_TYPE_NA = -1;
const uint8_t CARTESIAN_FLOAT_SIZE= 6;
const uint16_t MAX_TIME_PER_RESPONSE= 4000;

#define CLEAR_READ_MODIFIERS(x) (x & 0x3f)

static MblMwDataProcessor* find_processor(MblMwDataProcessor* processor, DataProcessorType key) {
    MblMwDataProcessor* value = processor;
    while(value != nullptr) {
        if (value->type == key) {
            return value;
        }
        value = value->parent();
    }

    return nullptr;
}

static int64_t extract_accounter_epoch(MblMwDataProcessor* processor, int64_t original_epoch, const uint8_t** start, uint8_t& len, uint32_t* tick) {
    uint8_t timestampLength = get_accounter_length(processor);
    // TODO: The logger uses a hardcoded prescaler of 3, upstream we force that value
    // and assume it to be so here, eventually we will have a prescale aware timestamp
    // API that works off of the base clock and call get_accounter_prescale(processor);
    memcpy(tick, *start, timestampLength);

    (*start) += timestampLength;
    len -= timestampLength;

    if (get_accounter_type(processor) == ACCOUNTER_TIME) {
        return calculate_epoch(processor->owner, *tick);
    }

    return original_epoch;
}

static bool invoke_signal_handler(MblMwDataSignal* signal, int64_t epoch, const uint8_t* response, uint8_t len, void* extra) {
    if (signal->handler != nullptr) {
        MblMwData* data = data_response_converters.at(signal->interpreter)(false, signal, response, len);
        data->epoch = epoch;
        data->extra = extra;
        signal->handler(signal->context, data);

        free_data(signal, data);
        return true;
    }
    return false;
}

static int32_t forward_response(const ResponseHeader& header, MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto it = board->module_events.find(header);
    if (it == board->module_events.end()) {
        return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
    }

    auto signal = dynamic_cast<MblMwDataSignal*>(it->second);
    bool handled= false;
    int64_t epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    
    MblMwDataProcessor* processor = dynamic_cast<MblMwDataProcessor*>(signal);
    const uint8_t* start = response;
    uint32_t extra;
    if (processor != nullptr) {
        switch(processor->type) {
            case DataProcessorType::ACCOUNTER: {
                epoch = extract_accounter_epoch(processor, epoch, &start, len, &extra);
                auto parent = find_processor(processor, DataProcessorType::PACKER);

                if (parent != nullptr) {
                    uint8_t i = 0, count = get_packer_count(parent), pack_size = get_packer_length(parent);
                    do {
                        handled|= invoke_signal_handler(signal, epoch, start, pack_size, &extra);
                        i++;
                        start+= pack_size;
                    } while(i < count);
                    return handled ? MBL_MW_STATUS_OK : MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
                }
                break;
            }
            case DataProcessorType::PACKER: {                    
                auto parent = find_processor(processor, DataProcessorType::ACCOUNTER);
                uint8_t i = 0, count = get_packer_count(processor), 
                        pack_size = get_packer_length(processor) - (parent == nullptr ? 0 : get_accounter_length(parent));
                
                do {
                    int64_t real_epoch = parent == nullptr ? epoch : extract_accounter_epoch(parent, epoch, &start, len, &extra);
                    handled|= invoke_signal_handler(signal, real_epoch, start, pack_size, &extra);
                    i++;
                    len-= pack_size;
                    start+= pack_size;
                } while(i < count);
                return handled ? MBL_MW_STATUS_OK : MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
            }
            default:
                break;
        }
    }

    handled|= invoke_signal_handler(signal, epoch, start, len, &extra);
    for(auto it: signal->components) {
        handled|= invoke_signal_handler(it, epoch, start, len, &extra);
    }

    return handled ? MBL_MW_STATUS_OK : MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
}

int32_t response_handler_data_no_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    return forward_response(ResponseHeader(response[0], response[1]), board, response + 2, len - 2);
}

int32_t response_handler_data_with_id(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    return forward_response(ResponseHeader(response[0], response[1], response[2]), board, response + 3, len - 3);
}

int32_t response_handler_packed_data(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    ResponseHeader header(response[0], response[1]);
    MblMwDataSignal* signal;
    auto it = board->module_events.find(header);

    if (it == board->module_events.end() || (signal = dynamic_cast<MblMwDataSignal*>(it->second))->handler == nullptr) {
        return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
    }

    int64_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    for(uint8_t i= 2; i < len; i+= CARTESIAN_FLOAT_SIZE) {
        MblMwData* data = data_response_converters.at(signal->interpreter)(false, signal, response + i, len - i);
        data->epoch= now;

        if (signal->handler != nullptr) {
            signal->handler(signal->context, data);
        }

        free_data(signal, data);
    }

    return MBL_MW_STATUS_OK;
}

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
    {MBL_MW_MODULE_CONDUCTANCE, READ_INFO_REGISTER},
    {MBL_MW_MODULE_SETTINGS, READ_INFO_REGISTER},
    {MBL_MW_MODULE_BAROMETER, READ_INFO_REGISTER},
    {MBL_MW_MODULE_GYRO, READ_INFO_REGISTER},
    {MBL_MW_MODULE_AMBIENT_LIGHT, READ_INFO_REGISTER},
    {MBL_MW_MODULE_MAGNETOMETER, READ_INFO_REGISTER},
    {MBL_MW_MODULE_HUMIDITY, READ_INFO_REGISTER},
    {MBL_MW_MODULE_COLOR_DETECTOR, READ_INFO_REGISTER},
    {MBL_MW_MODULE_PROXIMITY, READ_INFO_REGISTER},
    {MBL_MW_MODULE_SENSOR_FUSION, READ_INFO_REGISTER},
    {MBL_MW_MODULE_DEBUG, READ_INFO_REGISTER}
};

const unordered_map<uint8_t, void(*)(const MblMwMetaWearBoard*, vector<uint8_t>&)> CONFIG_SERIALIZATION = {
    { MBL_MW_MODULE_ACCELEROMETER, serialize_accelerometer_config },
    { MBL_MW_MODULE_BAROMETER, serialize_barometer_config },
    { MBL_MW_MODULE_GYRO, serialize_gyro_config },
    { MBL_MW_MODULE_AMBIENT_LIGHT, serialize_ambient_light_config },
    { MBL_MW_MODULE_COLOR_DETECTOR, serialize_colordetector_config },
    { MBL_MW_MODULE_PROXIMITY, serialize_proximity_config },
    { MBL_MW_MODULE_SENSOR_FUSION, serialize_sensor_fusion_config }
};
const unordered_map<uint8_t, void(*)(MblMwMetaWearBoard*, uint8_t**)> CONFIG_DESERIALIZATION = {
    { MBL_MW_MODULE_ACCELEROMETER, deserialize_accelerometer_config },
    { MBL_MW_MODULE_BAROMETER, deserialize_barometer_config },
    { MBL_MW_MODULE_GYRO, deserialize_gyro_config },
    { MBL_MW_MODULE_AMBIENT_LIGHT, deserialize_ambient_light_config },
    { MBL_MW_MODULE_COLOR_DETECTOR, deserialize_colordetector_config },
    { MBL_MW_MODULE_PROXIMITY, deserialize_proximity_config },
    { MBL_MW_MODULE_SENSOR_FUSION, deserialize_sensor_fusion_config }
};

const MblMwGattChar METAWEAR_SERVICE_NOTIFY_CHAR = { 0x326a900085cb9195, 0xd9dd464cfbbae75a, 0x326a900685cb9195, 0xd9dd464cfbbae75a };

const uint64_t DEVICE_INFO_SERVICE_UUID_HIGH = 0x0000180a00001000, 
        DEVICE_INFO_SERVICE_UUID_LOW = 0x800000805f9b34fb;

const MblMwGattChar METAWEAR_COMMAND_CHAR = { METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_high, METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_low, 
        0x326a900185cb9195, 0xd9dd464cfbbae75a };
const MblMwGattChar DEV_INFO_FIRMWARE_CHAR = { DEVICE_INFO_SERVICE_UUID_HIGH, DEVICE_INFO_SERVICE_UUID_LOW, 0x00002a2600001000, 0x800000805f9b34fb },
        DEV_INFO_MODEL_CHAR = { DEVICE_INFO_SERVICE_UUID_HIGH, DEVICE_INFO_SERVICE_UUID_LOW, 0x00002a2400001000, 0x800000805f9b34fb },
        DEV_INFO_HW_CHAR = { DEVICE_INFO_SERVICE_UUID_HIGH, DEVICE_INFO_SERVICE_UUID_LOW, 0x00002a2700001000, 0x800000805f9b34fb },
        DEV_INFO_MFT_CHAR = { DEVICE_INFO_SERVICE_UUID_HIGH, DEVICE_INFO_SERVICE_UUID_LOW, 0x00002a2900001000, 0x800000805f9b34fb },
        DEV_INFO_SERIAL_CHAR = { DEVICE_INFO_SERVICE_UUID_HIGH, DEVICE_INFO_SERVICE_UUID_LOW, 0x00002a2500001000, 0x800000805f9b34fb };
 
const vector<tuple<MblMwGattChar, void(*)(MblMwMetaWearBoard*, const uint8_t*, uint8_t), bool(*)(MblMwMetaWearBoard*)>> BOARD_DEV_INFO_CHARS = {
    make_tuple(DEV_INFO_FIRMWARE_CHAR, [](MblMwMetaWearBoard* board, const uint8_t* value, uint8_t length) {
        Version current(string(value, value + length));

        if (!(board->firmware_revision == current)) {
            board->firmware_revision = current;

            board->logger_state.reset();
            board->timer_state.reset();
            board->event_state.reset();
            board->dp_state.reset();
            board->macro_state.reset();
            board->debug_state.reset();
            free_accelerometer_module(board);
            free_gyro_module(board);
            free_sensor_fusion_module(board);
            free_settings_module(board);

            for (auto it : board->module_events) {
                it.second->remove = false;
                delete it.second;
            }
            board->module_events.clear();

            for (auto it : board->module_config) {
                free(it.second);
            }
            board->module_config.clear();

            board->module_info.clear();
        }
    }, [](MblMwMetaWearBoard* board) { return false; }),
    make_tuple(DEV_INFO_MODEL_CHAR, [](MblMwMetaWearBoard* board, const uint8_t* value, uint8_t length) { board->module_number.assign(value, value + length); }, 
            [](MblMwMetaWearBoard* board) { return !board->module_number.empty(); }
    ),
    make_tuple(DEV_INFO_HW_CHAR, [](MblMwMetaWearBoard* board, const uint8_t* value, uint8_t length) { board->hardware_revision.assign(value, value + length); }, 
            [](MblMwMetaWearBoard* board) { return !board->hardware_revision.empty(); }
    ),
    make_tuple(DEV_INFO_MFT_CHAR, [](MblMwMetaWearBoard* board, const uint8_t* value, uint8_t length) { board->manufacturer.assign(value, value + length); }, 
            [](MblMwMetaWearBoard* board) { return !board->manufacturer.empty(); }
    ),
    make_tuple(DEV_INFO_SERIAL_CHAR, [](MblMwMetaWearBoard* board, const uint8_t* value, uint8_t length) { board->serial_number.assign(value, value + length); },
            [](MblMwMetaWearBoard* board) { return !board->serial_number.empty(); }
    )
};

MblMwMetaWearBoard::MblMwMetaWearBoard() : logger_state(nullptr, [](void *ptr) -> void { tear_down_logging(ptr, false); }), 
        timer_state(nullptr, [](void *ptr) -> void { free_timer_module(ptr); }),
        event_state(nullptr, [](void *ptr) -> void { free_event_module(ptr); }), 
        dp_state(nullptr, [](void *ptr) -> void { free_dataprocessor_module(ptr); }),
        macro_state(nullptr, [](void *ptr) -> void { free_macro_module(ptr); }),
        debug_state(nullptr, [](void *ptr) -> void { free_debug_module(ptr); }),
        time_per_response(150), module_discovery_index(-1) {
}

MblMwMetaWearBoard::~MblMwMetaWearBoard() {
    logger_state.reset();
    timer_state.reset();
    event_state.reset();
    dp_state.reset();
    macro_state.reset();
    debug_state.reset();
    free_accelerometer_module(this);
    free_gyro_module(this);
    free_sensor_fusion_module(this);
    free_settings_module(this);

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

const unordered_map<uint8_t, tuple<const char*, void(*)(MblMwMetaWearBoard*)>> MODULE_ATTRS = {
    { MBL_MW_MODULE_SWITCH, make_tuple("Switch", init_switch_module) },
    { MBL_MW_MODULE_LED, make_tuple("Led", nullptr) },
    { MBL_MW_MODULE_ACCELEROMETER, make_tuple("Accelerometer", init_accelerometer_module) },
    { MBL_MW_MODULE_TEMPERATURE, make_tuple("Temperature", init_multichannel_temp_module) },
    { MBL_MW_MODULE_GPIO, make_tuple("Gpio", init_gpio_module) },
    { MBL_MW_MODULE_NEO_PIXEL, make_tuple("NeoPixel", nullptr) },
    { MBL_MW_MODULE_IBEACON, make_tuple("IBeacon", nullptr) },
    { MBL_MW_MODULE_HAPTIC, make_tuple("Haptic", nullptr) },
    { MBL_MW_MODULE_DATA_PROCESSOR, make_tuple("DataProcessor", init_dataprocessor_module) },
    { MBL_MW_MODULE_EVENT, make_tuple("Event", init_event_module) },
    { MBL_MW_MODULE_LOGGING, make_tuple("Logging", init_logging) },
    { MBL_MW_MODULE_TIMER, make_tuple("Timer", init_timer_module) },
    { MBL_MW_MODULE_I2C, make_tuple("SerialPassthrough", init_serialpassthrough_module) },
    { MBL_MW_MODULE_MACRO, make_tuple("Macro", init_macro_module) },
    { MBL_MW_MODULE_CONDUCTANCE, make_tuple("Conductance", init_conductance_module) },
    { MBL_MW_MODULE_SETTINGS, make_tuple("Settings", init_settings_module) },
    { MBL_MW_MODULE_BAROMETER, make_tuple("Barometer", init_barometer_module) },
    { MBL_MW_MODULE_GYRO, make_tuple("Gyro", init_gyro_module) },
    { MBL_MW_MODULE_AMBIENT_LIGHT, make_tuple("AmbientLight", init_ambient_light_module) },
    { MBL_MW_MODULE_MAGNETOMETER, make_tuple("Magnetometer", init_magnetometer_module) },
    { MBL_MW_MODULE_HUMIDITY, make_tuple("Humidity", init_humidity_module) },
    { MBL_MW_MODULE_COLOR_DETECTOR, make_tuple("Color", init_colordetector_module) },
    { MBL_MW_MODULE_PROXIMITY, make_tuple("Proximity", init_proximity_module) },
    { MBL_MW_MODULE_SENSOR_FUSION, make_tuple("SensorFusion", init_sensor_fusion_module) },
    { MBL_MW_MODULE_DEBUG, make_tuple("Debug", init_debug_module) }
};

static inline void service_discovery_completed(MblMwMetaWearBoard* board) {
    for (auto it : MODULE_ATTRS) {
        if (board->module_info.count(it.first) && board->module_info.at(it.first).present && get<1>(it.second) != nullptr) {
            get<1>(it.second)(board);
        }
    }

    MblMwDataSignal *signal = mbl_mw_logging_get_time_data_signal(board);
    if (signal != nullptr) {
        mbl_mw_datasignal_subscribe(signal, board, [](void *context, const MblMwData* data) {
            MblMwMetaWearBoard* board = static_cast<MblMwMetaWearBoard*>(context);
            if (board->initialized_timeout != nullptr) {
                board->initialized_timeout->cancel();
            }

            mbl_mw_datasignal_unsubscribe(mbl_mw_logging_get_time_data_signal(board));
            
            MblMwLoggingTime *time = static_cast<MblMwLoggingTime*>(data->value);
            mbl_mw_logging_set_reference_time(board, time->reset_uid, time->epoch);
            mbl_mw_logging_set_latest_reset_uid(board, time->reset_uid);

            board->initialized(board->initialized_context, board, MBL_MW_STATUS_OK);
        });
        mbl_mw_datasignal_read(signal);
    } else {
        if (board->initialized_timeout != nullptr) {
            board->initialized_timeout->cancel();
        }
        board->initialized(board->initialized_context, board, MBL_MW_STATUS_OK);
    }
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

static int32_t char_changed_handler(const void* caller, const uint8_t* value, uint8_t length) {
    MblMwMetaWearBoard* board = (MblMwMetaWearBoard*) caller;
    ResponseHeader header(value[0], value[1]);

    if (board->responses.count(header)) {
        return board->responses.at(header)(board, value, length);
    } else if (header.register_id == READ_INFO_REGISTER) {
        board->module_info.emplace(piecewise_construct, forward_as_tuple(value[0]), forward_as_tuple(value, length));
        queue_next_query(board);
        return MBL_MW_STATUS_OK;
    } else {
        return MBL_MW_STATUS_WARNING_INVALID_RESPONSE;
    }
}

static int32_t read_gatt_char_handler(const void* caller, const uint8_t* value, uint8_t length);
static void queue_next_read(MblMwMetaWearBoard* board) {
    do {
        board->dev_info_index++;
    } while (board->dev_info_index < (int8_t) BOARD_DEV_INFO_CHARS.size() && 
            get<2>(BOARD_DEV_INFO_CHARS[board->dev_info_index])(board));

    if (board->dev_info_index >= (int8_t) BOARD_DEV_INFO_CHARS.size()) {
        queue_next_query(board);
    } else {
        board->btle_conn.read_gatt_char(board->btle_conn.context, board, &get<0>(BOARD_DEV_INFO_CHARS[board->dev_info_index]), read_gatt_char_handler);
    }
}

static int32_t read_gatt_char_handler(const void* caller, const uint8_t* value, uint8_t length) {
    auto board = (MblMwMetaWearBoard*) caller;
    get<1>(BOARD_DEV_INFO_CHARS[board->dev_info_index])(board, value, length);
    queue_next_read(board);

    return MBL_MW_STATUS_OK;
}

static void enable_notify_ready(const void* caller, int32_t value) {
    auto board = (MblMwMetaWearBoard*) caller;

    if (value == MBL_MW_STATUS_OK) {
        board->dev_info_index = -1;
        board->initialized_timeout= ThreadPool::schedule([board](void) {
            board->initialized(board->initialized_context, board, MBL_MW_STATUS_ERROR_TIMEOUT);
        }, (MODULE_DISCOVERY_CMDS.size() + BOARD_DEV_INFO_CHARS.size() + 1) * board->time_per_response);
        queue_next_read(board);
    } else {
        board->initialized(board->initialized_context, board, MBL_MW_STATUS_ERROR_ENABLE_NOTIFY);
    }
}

const unordered_set<void(*)(MblMwMetaWearBoard*)> MODULE_DISCONNECT_HANDLERS = {
    disconnect_logging,
    disconnect_timer,
    disconnect_dataprocessor
};

static void disconnect_handler(const void* caller, int32_t value) {
    auto board = (MblMwMetaWearBoard*) caller;

    for(auto it: MODULE_DISCONNECT_HANDLERS) {
        it(board);
    }
}

void mbl_mw_metawearboard_initialize(MblMwMetaWearBoard *board, void *context, MblMwFnBoardPtrInt initialized) {
    board->initialized_context = context;
    board->initialized = initialized;
    board->dev_info_index = -1;
    board->module_discovery_index = -1;

    board->btle_conn.on_disconnect(board->btle_conn.context, board, disconnect_handler);
    board->btle_conn.enable_notifications(board->btle_conn.context, board, &METAWEAR_SERVICE_NOTIFY_CHAR, char_changed_handler, enable_notify_ready);
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

void send_command(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len) {
    if (!record_command(board, command, len)) {
        board->btle_conn.write_gatt_char(
            board->btle_conn.context,
            board, 
            command[0] == MBL_MW_MODULE_MACRO ? MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE : MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE,
            &METAWEAR_COMMAND_CHAR, command, len
        );
        record_macro(board, command, len);
    }
}

int32_t mbl_mw_metawearboard_is_initialized(const MblMwMetaWearBoard *board) {
    return board->module_discovery_index == (int8_t) MODULE_DISCOVERY_CMDS.size();
}

int32_t mbl_mw_metawearboard_lookup_module(const MblMwMetaWearBoard *board, MblMwModule module) {
    auto it = board->module_info.find(module);
    return it == board->module_info.end() || !it->second.present ? MBL_MW_MODULE_TYPE_NA : it->second.implementation;
}

MblMwModel mbl_mw_metawearboard_get_model(const MblMwMetaWearBoard* board) {
    if (board->module_number.empty()) {
        return MBL_MW_MODEL_NA;
    }

    if (board->module_number == "0") {
        return MBL_MW_MODEL_METAWEAR_R;
    }
    if (board->module_number == "1") {
        if (mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_BAROMETER) != MBL_MW_MODULE_TYPE_NA && 
                mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_AMBIENT_LIGHT) != MBL_MW_MODULE_TYPE_NA) {
            return MBL_MW_MODEL_METAWEAR_RPRO;
        }
        return MBL_MW_MODEL_METAWEAR_RG;
    }
    if (board->module_number == "2") {
        if (mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_MAGNETOMETER) != MBL_MW_MODULE_TYPE_NA) {
            return MBL_MW_MODEL_METAWEAR_CPRO;
        }
        if (mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_PROXIMITY) != MBL_MW_MODULE_TYPE_NA) {
            return MBL_MW_MODEL_METADETECT;
        }
        if (mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_HUMIDITY) != MBL_MW_MODULE_TYPE_NA) {
            return MBL_MW_MODEL_METAENV;
        }
        return MBL_MW_MODEL_METAWEAR_C;
    }
    if (board->module_number == "3") {
        return MBL_MW_MODEL_METAHEALTH;
    }
    if (board->module_number == "4") {
        return MBL_MW_MODEL_METATRACKER;
    }
    if (board->module_number == "5") {
        if (mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_AMBIENT_LIGHT) != MBL_MW_MODULE_TYPE_NA) {
            return MBL_MW_MODEL_METAMOTION_R;
        } else {
            return MBL_MW_MODEL_METAMOTION_RL;
        }
    }
    if (board->module_number == "6") {
        return MBL_MW_MODEL_METAMOTION_C;
    }
    if (board->module_number == "8") {
        return MBL_MW_MODEL_METAMOTION_S;
    }


    return MBL_MW_MODEL_NA;
}

const char * MODEL_NAMES[] = {
    "Unknown",
    "MetaWear R",
    "MetaWear RG",
    "MetaWear RPro",
    "MetaWear C",
    "MetaWear CPro",
    "MetaEnvironment",
    "MetaDetector",
    "MetaHealth",
    "MetaTracker",
    "MetaMotion R",
    "MetaMotion RL",
    "MetaMotion C",
    "MetaMotion S"
};
const char* mbl_mw_metawearboard_get_model_name(const MblMwMetaWearBoard* board) {
    return MODEL_NAMES[mbl_mw_metawearboard_get_model(board) + 1];
}

const MblMwDeviceInformation* mbl_mw_metawearboard_get_device_information(const MblMwMetaWearBoard* board) {
    MblMwDeviceInformation* dev_info = (MblMwDeviceInformation*) malloc(sizeof(MblMwDeviceInformation));
    dev_info->manufacturer = board->manufacturer.c_str();
    dev_info->model_number = board->module_number.c_str();
    dev_info->serial_number = board->serial_number.c_str();
    dev_info->firmware_revision = board->firmware_revision.sem_ver.c_str();
    dev_info->hardware_revision = board->hardware_revision.c_str();
    return dev_info;
}

MblMwModuleInfo* mbl_mw_metawearboard_get_module_info(const MblMwMetaWearBoard* board, uint32_t* size) {
    *size = static_cast<uint32_t>(board->module_info.size());
    MblMwModuleInfo* info = (MblMwModuleInfo*) malloc(sizeof(MblMwModuleInfo) * (*size));

    vector<uint8_t> sorted_keys;
    for (auto it : board->module_info) {
        sorted_keys.push_back(it.first);
    }
    sort(sorted_keys.begin(), sorted_keys.end());

    auto temp = info;
    for(auto it: sorted_keys) {
        auto value = &board->module_info.at(it);
        temp->name = get<0>(MODULE_ATTRS.at(it));
        temp->extra = value->extra.data();
        temp->extra_len = value->extra.size();
        temp->present = value->present ? 1 : 0;
        temp->implementation = value->implementation;
        temp->revision = value->revision;
        temp++;
    }

    return info;
}

uint8_t* mbl_mw_metawearboard_serialize(const MblMwMetaWearBoard* board, uint32_t* size) {
    vector<uint8_t> serialized_state;

    serialized_state.push_back(ORDINAL(SerializationFormat::TIME_REFERENCE));
    
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
        uint8_t n_events= 0;
        vector<uint8_t> event_states;
        vector<ResponseHeader> sorted_keys;
        for (auto it : board->module_events) {
            sorted_keys.emplace_back(it.first.module_id, it.first.register_id, it.first.data_id);
        }
        sort(sorted_keys.begin(), sorted_keys.end());

        for (auto it : sorted_keys) {
            board->module_events.at(it)->serialize(event_states);
            n_events++;

            // serialize component signals after the main signal
            // rely on this ordering to restore the components vector
            if (MblMwDataSignal* signal= dynamic_cast<MblMwDataSignal*>(board->module_events.at(it))) {
                for(auto component: signal->components) {
                    component->serialize(event_states);
                    n_events++;
                }
            }
        }

        serialized_state.push_back(n_events);
        serialized_state.insert(serialized_state.end(), event_states.begin(), event_states.end());
    }

    {
        vector<uint8_t> sorted_keys;
        for (auto it : board->module_config) {
            sorted_keys.push_back(it.first);
        }
        sort(sorted_keys.begin(), sorted_keys.end());

        serialized_state.push_back((uint8_t)board->module_config.size());
        for (auto it : sorted_keys) {
            if (CONFIG_SERIALIZATION.count(it)) {
                serialized_state.push_back(it);
                CONFIG_SERIALIZATION.at(it)(board, serialized_state);
            }
        }
    }
    
    serialize_logging(board, serialized_state);
    
    *size = (uint32_t) serialized_state.size();
    uint8_t* state_bytes = (uint8_t*)malloc(sizeof(uint8_t) * (*size));
    memcpy(state_bytes, serialized_state.data(), *size);
    return state_bytes;
}

int32_t mbl_mw_metawearboard_deserialize(MblMwMetaWearBoard* board, uint8_t* state, uint32_t size) {
    uint8_t *current_addr = state, format = *current_addr;

    if (format > ORDINAL(SerializationFormat::TIME_REFERENCE)) {
        return MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT;
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

        ResponseHeader live_header(saved_event->header);
        live_header.disable_silent();

        if (board->module_events.count(live_header)) {
            auto signal = dynamic_cast<MblMwDataSignal*>(board->module_events[live_header]);
            signal->components.push_back(dynamic_cast<MblMwDataSignal*>(saved_event));
        } else {
            board->module_events.emplace(live_header, saved_event);
        }
    }
    
    uint8_t module_config_size= *current_addr;
    current_addr++;
    for (uint8_t i= 0; i < module_config_size; i++) {
        if (CONFIG_DESERIALIZATION.count(*current_addr)) {
            auto fn = CONFIG_DESERIALIZATION.at(*current_addr);
            current_addr++;
            fn(board, &current_addr);
        }
    }

    deserialize_logging(board, format, &current_addr);

    return MBL_MW_STATUS_OK;
}

static bool has_suffix(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static int32_t dfu_char_changed_handler(const void* caller, const uint8_t* value, uint8_t length) {
    ((MblMwMetaWearBoard*) caller)->operations->processDFUResponse(value, length);
    return MBL_MW_STATUS_OK;
}

static void dfu_enable_notify_ready(const void* caller, int32_t value) {
    auto board = (MblMwMetaWearBoard*) caller;

    if (value == MBL_MW_STATUS_OK) {
        if (has_suffix(board->filename, "zip")) {
            board->operations->perfromDFUOnZipFile(board->filename);
        } else {
            board->operations->performOldDFUOnFile(board->filename);
        }
    } else {
        board->operations->cancelDFU();
    }
}

void mbl_mw_metawearboard_perform_dfu(MblMwMetaWearBoard *board, const MblMwDfuDelegate *delegate, const char *filename) {
    board->operations.reset(new DfuOperations(board, delegate));
    board->filename = filename;
    board->btle_conn.enable_notifications(board->btle_conn.context, board, &DFU_CONTROL_POINT_CHAR, dfu_char_changed_handler, dfu_enable_notify_ready);
}

static void read_sensor_fusion_config_completed(void *context, MblMwMetaWearBoard* board, int32_t value) {
    query_active_loggers(board);
}

static void read_gyro_config_completed(void *context, MblMwMetaWearBoard* board, int32_t value) {
    if (mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_SENSOR_FUSION) != MBL_MW_MODULE_TYPE_NA) {
        mbl_mw_sensor_fusion_read_config(board, context, read_sensor_fusion_config_completed);
    } else {
        read_sensor_fusion_config_completed(context, board, MBL_MW_STATUS_OK);
    }
}

static void read_acc_config_completed(void *context, MblMwMetaWearBoard* board, int32_t value) {
    if (mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_GYRO) != MBL_MW_MODULE_TYPE_NA) {
        mbl_mw_gyro_bmi160_read_config(board, context, read_gyro_config_completed);
    } else {
        read_gyro_config_completed(context, board, MBL_MW_STATUS_OK);
    }
}

void mbl_mw_metawearboard_create_anonymous_datasignals(MblMwMetaWearBoard* board, void *context, MblMwFnAnonSignalArray created) {
    board->anon_signals_context = context;
    board->anon_signals_created = created;
    mbl_mw_acc_read_config(board, context, read_acc_config_completed);
}
