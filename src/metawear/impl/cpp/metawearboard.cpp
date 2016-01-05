#include <cstdlib>
#include <exception>
#include <tuple>
#include <unordered_map>

#include "metawear/core/metawearboard.h"
#include "metawear/core/status.h"

#include "metawear/core/cpp/connection_def.h"
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
using std::piecewise_construct;
using std::unordered_map;
using std::vector;

static const vector<vector<uint8_t>> MODULE_DISCOVERY_CMDS= {
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

MblMwMetaWearBoard::MblMwMetaWearBoard() : data_token(nullptr) {
    sensor_data_signals[SWITCH_RESPONSE_HEADER]= new MblMwDataSignal(SWITCH_RESPONSE_HEADER, this, ResponseConvertor::UINT32, 
            1, 1, 0, 0);
}

MblMwMetaWearBoard::~MblMwMetaWearBoard() {
    for (auto it= sensor_data_signals.begin(); it != sensor_data_signals.end(); it++) {
        delete (it->second);
    }

    for (auto it= timer_signals.begin(); it != timer_signals.end(); it++) {
        delete (it->second);
    }

    for (auto it = module_config.begin(); it != module_config.end(); it++) {
        free(it->second);
    }

    for (auto it = data_processors.begin(); it != data_processors.end(); it++) {
        delete (it->second);
    }
}

MblMwMetaWearBoard* mbl_mw_create_metawear_board(MblMwFnByteArray send_command) {
    auto new_board= new MblMwMetaWearBoard;
    new_board->send_command = send_command;
    return new_board;
}

void mbl_mw_free_metawear_board(MblMwMetaWearBoard *board) {
    delete board;
}

static inline void queue_next_query(MblMwMetaWearBoard *board) {
    board->module_discovery_index++;
    send_command_wrapper(board, MODULE_DISCOVERY_CMDS[board->module_discovery_index].data(), 
            (uint8_t) MODULE_DISCOVERY_CMDS[board->module_discovery_index].size());
}

void mbl_mw_metawearboard_initialize(MblMwMetaWearBoard *board, MblMwFnVoid initialized) {
    board->initialized = initialized;
    board->module_discovery_index= -1;
    queue_next_query(board);
}

int32_t mbl_mw_metawearboard_handle_response(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint8_t resp_register_id= response[1] & 0x7f;

    if (resp_register_id == INFO_REGISTER) {
        board->module_info.emplace(piecewise_construct, forward_as_tuple(response[0]), forward_as_tuple(response, len));

        if (board->module_discovery_index == (uint8_t) (MODULE_DISCOVERY_CMDS.size() - 1)) {            
            init_accelerometer_module(board);
            init_barometer_module(board);
            init_gyro_module(board);
            init_ambient_light_module(board);
            init_multichannel_temp_module(board);

            board->initialized();
        } else {
            queue_next_query(board);
        }
    } else if ((response[0] == MBL_MW_MODULE_SWITCH && resp_register_id == ORDINAL(SwitchRegister::STATE)) || 
            (response[0] == MBL_MW_MODULE_ACCELEROMETER && resp_register_id == ORDINAL(AccelerometerBmi160Register::DATA_INTERRUPT)) ||
            (response[0] == MBL_MW_MODULE_ACCELEROMETER && resp_register_id == ORDINAL(AccelerometerMma8452qRegister::DATA_VALUE)) ||
            (response[0] == MBL_MW_MODULE_AMBIENT_LIGHT && resp_register_id == ORDINAL(AmbientLightLtr329Register::OUTPUT)) || 
            (response[0] == MBL_MW_MODULE_BAROMETER && resp_register_id == ORDINAL(BarometerBmp280Register::PRESSURE)) || 
            (response[0] == MBL_MW_MODULE_BAROMETER && resp_register_id == ORDINAL(BarometerBmp280Register::ALTITUDE)) ||
            (response[0] == MBL_MW_MODULE_GYRO && resp_register_id == ORDINAL(GyroBmi160Register::DATA))) {

        try {
            ResponseHeader header(response[0], resp_register_id);
            MblMwData* data= board->response_processors.at(header)(board, response + 2, len - 2);

            board->data_signal_handlers.at(header)(data);

            free(data->value);
            free(data);
        } catch (exception) {
            return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
        }
    } else if ((response[0] == MBL_MW_MODULE_TEMPERATURE && resp_register_id == ORDINAL(MultiChannelTempRegister::TEMPERATURE)) ||
            (response[0] == MBL_MW_MODULE_GPIO && resp_register_id == ORDINAL(GpioRegister::READ_AI_ABS_REF)) || 
            (response[0] == MBL_MW_MODULE_GPIO && resp_register_id == ORDINAL(GpioRegister::READ_AI_ADC)) || 
            (response[0] == MBL_MW_MODULE_GPIO && resp_register_id == ORDINAL(GpioRegister::READ_DI)) || 
            (response[0] == MBL_MW_MODULE_GPIO && resp_register_id == ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY)) || 
            (response[0] == MBL_MW_MODULE_DATA_PROCESSOR && resp_register_id == ORDINAL(DataProcessorRegister::NOTIFY))) {
        try {
            ResponseHeader header(response[0], resp_register_id, response[2]);
            MblMwData* data= board->response_processors.at(header)(board, response + 3, len - 3);

            board->data_signal_handlers.at(header)(data);

            free(data->value);
            free(data);
        } catch (exception) {
            return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
        }
    } else if ((response[0] == MBL_MW_MODULE_TIMER && resp_register_id == ORDINAL(TimerRegister::TIMER_ENTRY))) {
        auto signal= board->pending_timers.front();
        signal->header.data_id= response[2];
        board->timer_signals.emplace(signal->header, signal);
        board->pending_timers.pop();

        board->timer_callbacks.front()(signal);
        board->timer_callbacks.pop();
    } else if (response[0] == MBL_MW_MODULE_EVENT && response[1] == ORDINAL(EventRegister::ENTRY)) {
        auto top_event_owner= board->event_owner.front();
        top_event_owner->event_command_ids.push_back(response[2]);
        if ((uint8_t) top_event_owner->event_command_ids.size() == top_event_owner->num_expected_cmds) {
            board->event_owner.pop();

            auto top_event_callback = board->event_recorded_callbacks.front();
            board->event_recorded_callbacks.pop();
            top_event_callback();

            if (!board->event_owner.empty()) {
                for(auto it: board->event_owner.front()->commands) {
                    send_command_wrapper(board, it.data(), (uint8_t) it.size());
                }
            }
        }
    } else if (response[0] == MBL_MW_MODULE_DATA_PROCESSOR && response[1] == ORDINAL(DataProcessorRegister::ADD)) {
        auto next_processor = board->pending_dataprocessors.front();
        board->pending_dataprocessors.pop();
        next_processor->header.data_id = response[2];
        board->data_processors.emplace(next_processor->header, next_processor);

        auto next_callback = board->dataprocessor_callbacks.front();
        board->dataprocessor_callbacks.pop();
        next_callback(next_processor);
    }

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_metawearboard_is_initialized(const MblMwMetaWearBoard *board) {
    return board->module_discovery_index == (uint8_t)(MODULE_DISCOVERY_CMDS.size() - 1);
}

int32_t mbl_mw_metawearboard_get_module_type(const MblMwMetaWearBoard *board, uint8_t module) {
    try {
        if (board->module_info.at(module).present) {
            return board->module_info.at(module).implementation;
        }
        return MBL_MW_MODULE_TYPE_NA;
    } catch (exception) {
        return MBL_MW_MODULE_TYPE_NA;
    }
}
