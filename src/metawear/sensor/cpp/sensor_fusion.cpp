#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include "metawear/sensor/accelerometer.h"
#include "metawear/sensor/gyro_bmi160.h"
#include "metawear/sensor/magnetometer_bmm150.h"
#include "metawear/sensor/sensor_fusion.h"
#include "sensor_fusion_private.h"
#include "sensor_fusion_register.h"
#include "utils.h"

#include <cstdlib>
#include <cstring>
#include <unordered_map>

using std::forward_as_tuple;
using std::malloc;
using std::memset;
using std::piecewise_construct;
using std::stringstream;
using std::strlen;
using std::unordered_map;

#define CORRECTED_ACC_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_CORRECTED_ACC]
#define CORRECTED_GYRO_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_CORRECTED_GYRO]
#define CORRECTED_MAG_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_CORRECTED_MAG]
#define QUATERNION_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_QUATERNION]
#define EULER_ANGLES_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_EULER_ANGLE]
#define GRAVITY_VECTOR_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_GRAVITY_VECTOR]
#define LINEAR_ACC_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_LINEAR_ACC]

const float ACC_RANGES[] = {2.f, 4.f, 8.f, 16.f};

const ResponseHeader RESPONSE_HEADERS[] {
    ResponseHeader(MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::CORRECTED_ACC)),
    ResponseHeader(MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::CORRECTED_GYRO)),
    ResponseHeader(MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::CORRECTED_MAG)),
    ResponseHeader(MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::QUATERNION)),
    ResponseHeader(MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::EULER_ANGLES)),
    ResponseHeader(MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::GRAVITY_VECTOR)),
    ResponseHeader(MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::LINEAR_ACC))
};

const ResponseHeader CALIB_STATE_RESPONSE_HEADER(MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::CALIBRATION_STATE)));
const uint8_t CALIBRATION_REVISION = 1, CALIB_DATA_REVISION = 2;

struct SensorFusionState {
    struct {
        uint8_t mode;
        uint8_t acc_range:4;
        uint8_t gyro_range:4;
    } config;
    uint8_t enable_mask;
};

struct SensorFusionTransientState {
    MblMwCalibrationData* calib_data;
    MblMwFnBoardPtrInt read_config_completed;
    MblMwFnBoardPtrCalibDataPtr read_calib_data_completed;
    void *read_config_context, *read_calib_data_context;
};

static unordered_map<const MblMwMetaWearBoard*, SensorFusionTransientState> transient_states;

static int32_t received_config_response(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state = (SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION);
    memcpy(&(state->config), response + 2, sizeof(state->config));

    auto callback = transient_states[board].read_config_completed;
    auto context = transient_states[board].read_config_context;
    transient_states[board].read_config_completed = nullptr;
    transient_states[board].read_config_context = nullptr;
    callback(context, board, MBL_MW_STATUS_OK);

    return 0;
}

static int32_t received_acc_cal_data(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    MblMwCalibrationData* data = (MblMwCalibrationData*) malloc(sizeof(MblMwCalibrationData));
    memcpy(data->acc, response + 2, 10);
    transient_states[board].calib_data = data;
    
    uint8_t command[2] = { MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::GYRO_CAL_DATA)) };
    SEND_COMMAND;

    return 0;
}

static int32_t received_gyro_cal_data(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    memcpy(transient_states[board].calib_data->gyro, response + 2, 10);
    
    uint8_t command[2] = { MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::MAG_CAL_DATA)) };
    SEND_COMMAND;

    return 0;
}

static int32_t received_mag_cal_data(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    memcpy(transient_states[board].calib_data->mag, response + 2, 10);

    transient_states[board].read_calib_data_completed(transient_states[board].read_calib_data_context, board, transient_states[board].calib_data);

    return 0;
}

void init_sensor_fusion_module(MblMwMetaWearBoard* board) {
    if (board->module_info.count(MBL_MW_MODULE_SENSOR_FUSION) && board->module_info.at(MBL_MW_MODULE_SENSOR_FUSION).present) {
        if (!board->module_config.count(MBL_MW_MODULE_SENSOR_FUSION)) {
            SensorFusionState* new_state = (SensorFusionState*) malloc(sizeof(SensorFusionState));
            new_state->config.mode = MBL_MW_SENSOR_FUSION_MODE_SLEEP;
            new_state->config.acc_range = MBL_MW_SENSOR_FUSION_ACC_RANGE_16G;
            // The +1 here is because the sensor fusion library defines a 2048 at 
            // location zero which our hardware doesn't acutally support, so we
            // essentially shift the MBL_MW_SENSOR_FUSION_GYRO_RANGE enum by 1
            new_state->config.gyro_range = MBL_MW_SENSOR_FUSION_GYRO_RANGE_2000DPS + 1;
            new_state->enable_mask = 0;
            board->module_config.emplace(MBL_MW_MODULE_SENSOR_FUSION, new_state);
        }

        for(auto& it: RESPONSE_HEADERS) {
            board->responses[it] = response_handler_data_no_id;
        }

        if (!board->module_events.count(CORRECTED_ACC_RESPONSE_HEADER)) {
            board->module_events[CORRECTED_ACC_RESPONSE_HEADER] = new MblMwDataSignal(CORRECTED_ACC_RESPONSE_HEADER, board, 
                    DataInterpreter::SENSOR_FUSION_CORRECTED_ACC, FirmwareConverter::DEFAULT, 1, 13, 1, 0);
        }

        if (!board->module_events.count(CORRECTED_GYRO_RESPONSE_HEADER)) {
            board->module_events[CORRECTED_GYRO_RESPONSE_HEADER] = new MblMwDataSignal(CORRECTED_GYRO_RESPONSE_HEADER, board, 
                    DataInterpreter::SENSOR_FUSION_CORRECTED_FLOAT_VECTOR3, FirmwareConverter::DEFAULT, 1, 13, 1, 0);
        }

        if (!board->module_events.count(CORRECTED_MAG_RESPONSE_HEADER)) {
            board->module_events[CORRECTED_MAG_RESPONSE_HEADER] = new MblMwDataSignal(CORRECTED_MAG_RESPONSE_HEADER, board, 
                    DataInterpreter::SENSOR_FUSION_CORRECTED_FLOAT_VECTOR3, FirmwareConverter::DEFAULT, 1, 13, 1, 0);
        }

        if (!board->module_events.count(QUATERNION_RESPONSE_HEADER)) {
            board->module_events[QUATERNION_RESPONSE_HEADER] = new MblMwDataSignal(QUATERNION_RESPONSE_HEADER, board, 
                    DataInterpreter::SENSOR_FUSION_QUATERNION, FirmwareConverter::DEFAULT, 4, 4, 1, 0);
        }

        if (!board->module_events.count(EULER_ANGLES_RESPONSE_HEADER)) {
            board->module_events[EULER_ANGLES_RESPONSE_HEADER] = new MblMwDataSignal(EULER_ANGLES_RESPONSE_HEADER, board, 
                    DataInterpreter::SENSOR_FUSION_EULER_ANGLE, FirmwareConverter::DEFAULT, 4, 4, 1, 0);
        }

        if (!board->module_events.count(GRAVITY_VECTOR_RESPONSE_HEADER)) {
            board->module_events[GRAVITY_VECTOR_RESPONSE_HEADER] = new MblMwDataSignal(GRAVITY_VECTOR_RESPONSE_HEADER, board, 
                    DataInterpreter::SENSOR_FUSION_FLOAT_VECTOR3, FirmwareConverter::DEFAULT, 3, 4, 1, 0);
        }

        if (!board->module_events.count(LINEAR_ACC_RESPONSE_HEADER)) {
            board->module_events[LINEAR_ACC_RESPONSE_HEADER] = new MblMwDataSignal(LINEAR_ACC_RESPONSE_HEADER, board, 
                    DataInterpreter::SENSOR_FUSION_FLOAT_VECTOR3, FirmwareConverter::DEFAULT, 3, 4, 1, 0);
        }

        if (!board->module_events.count(CALIB_STATE_RESPONSE_HEADER)) {
            board->module_events[CALIB_STATE_RESPONSE_HEADER] = new MblMwDataSignal(CALIB_STATE_RESPONSE_HEADER, board, 
                    DataInterpreter::SENSOR_FUSION_CALIB_STATE, FirmwareConverter::DEFAULT, 3, 1, 0, 0);
        }

        board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::MODE))),
                forward_as_tuple(received_config_response));
        board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::CALIBRATION_STATE))),
                forward_as_tuple(response_handler_data_no_id));
        board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::ACC_CAL_DATA))),
                forward_as_tuple(received_acc_cal_data));
        board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::GYRO_CAL_DATA))),
                forward_as_tuple(received_gyro_cal_data));
        board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::MAG_CAL_DATA))),
                forward_as_tuple(received_mag_cal_data));

        SensorFusionTransientState newState = {nullptr};
        transient_states.insert({board, newState});
    }
}

void free_sensor_fusion_module(MblMwMetaWearBoard* board) {
    transient_states.erase(board);
}

void serialize_sensor_fusion_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(SensorFusionState, MBL_MW_MODULE_SENSOR_FUSION);
}

void deserialize_sensor_fusion_config(MblMwMetaWearBoard *board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(SensorFusionState, MBL_MW_MODULE_SENSOR_FUSION);
}

MblMwDataSignal* mbl_mw_sensor_fusion_get_data_signal(const MblMwMetaWearBoard* board, MblMwSensorFusionData data) {
    GET_DATA_SIGNAL(RESPONSE_HEADERS[data]);
}

MblMwDataSignal* mbl_mw_sensor_fusion_calibration_state_data_signal(const MblMwMetaWearBoard* board) {
    if (board->module_info.at(MBL_MW_MODULE_SENSOR_FUSION).revision < CALIBRATION_REVISION) {
        return nullptr;
    }
    GET_DATA_SIGNAL(CALIB_STATE_RESPONSE_HEADER);
}

void mbl_mw_sensor_fusion_set_mode(MblMwMetaWearBoard* board, MblMwSensorFusionMode mode) {
    ((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->config.mode = mode;
}
void mbl_mw_sensor_fusion_set_acc_range(MblMwMetaWearBoard* board, MblMwSensorFusionAccRange range) {
    ((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->config.acc_range = range;
}
void mbl_mw_sensor_fusion_set_gyro_range(MblMwMetaWearBoard* board, MblMwSensorFusionGyroRange range) {
    ((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->config.gyro_range = (range + 1);
}
void mbl_mw_sensor_fusion_write_config(MblMwMetaWearBoard* board) {
    auto state = (SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION);

    uint8_t command[4] = {MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::MODE)};
    memcpy(command + 2, &(state->config), sizeof(state->config));
    SEND_COMMAND;

    switch(state->config.mode) {
    case MBL_MW_SENSOR_FUSION_MODE_SLEEP:
        break;
    case MBL_MW_SENSOR_FUSION_MODE_NDOF:
        mbl_mw_acc_set_range(board, ACC_RANGES[state->config.acc_range]);
        mbl_mw_acc_set_odr(board, 100.f);
        mbl_mw_acc_write_acceleration_config(board);

        mbl_mw_gyro_bmi160_set_range(board, (MblMwGyroBmi160Range) (state->config.gyro_range - 1));
        mbl_mw_gyro_bmi160_set_odr(board, MBL_MW_GYRO_BMI160_ODR_100Hz);
        mbl_mw_gyro_bmi160_write_config(board);

        mbl_mw_mag_bmm150_configure(board, 9, 15, MBL_MW_MAG_BMM150_ODR_25Hz);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS:
        mbl_mw_acc_set_range(board, ACC_RANGES[state->config.acc_range]);
        mbl_mw_acc_set_odr(board, 100.f);
        mbl_mw_acc_write_acceleration_config(board);

        mbl_mw_gyro_bmi160_set_range(board, (MblMwGyroBmi160Range) (state->config.gyro_range - 1));
        mbl_mw_gyro_bmi160_set_odr(board, MBL_MW_GYRO_BMI160_ODR_100Hz);
        mbl_mw_gyro_bmi160_write_config(board);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_COMPASS:
        mbl_mw_acc_set_range(board, ACC_RANGES[state->config.acc_range]);
        mbl_mw_acc_set_odr(board, 25.f);
        mbl_mw_acc_write_acceleration_config(board);

        mbl_mw_mag_bmm150_configure(board, 9, 15, MBL_MW_MAG_BMM150_ODR_25Hz);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_M4G:
        mbl_mw_acc_set_range(board, ACC_RANGES[state->config.acc_range]);
        mbl_mw_acc_set_odr(board, 50.f);
        mbl_mw_acc_write_acceleration_config(board);

        mbl_mw_mag_bmm150_configure(board, 9, 15, MBL_MW_MAG_BMM150_ODR_25Hz);
    }
}

void mbl_mw_sensor_fusion_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed) {
    transient_states[board].read_config_context = context;
    transient_states[board].read_config_completed = completed;
    uint8_t command[2] = { MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::MODE)) };
    SEND_COMMAND;
}

void mbl_mw_sensor_fusion_enable_data(MblMwMetaWearBoard* board, MblMwSensorFusionData data) {
    ((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->enable_mask |= (0x1 << (int) data);
}
void mbl_mw_sensor_fusion_clear_enabled_mask(MblMwMetaWearBoard* board) {
    ((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->enable_mask = 0x0;
}

void mbl_mw_sensor_fusion_start(const MblMwMetaWearBoard* board) {
    switch(((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->config.mode) {
    case MBL_MW_SENSOR_FUSION_MODE_NDOF:
        mbl_mw_acc_enable_acceleration_sampling(board);
        mbl_mw_gyro_bmi160_enable_rotation_sampling(board);
        mbl_mw_mag_bmm150_enable_b_field_sampling(board);
        mbl_mw_acc_start(board);
        mbl_mw_gyro_bmi160_start(board);
        mbl_mw_mag_bmm150_start(board);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS:
        mbl_mw_acc_enable_acceleration_sampling(board);
        mbl_mw_gyro_bmi160_enable_rotation_sampling(board);
        mbl_mw_acc_start(board);
        mbl_mw_gyro_bmi160_start(board);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_COMPASS:
        mbl_mw_acc_enable_acceleration_sampling(board);
        mbl_mw_mag_bmm150_enable_b_field_sampling(board);
        mbl_mw_acc_start(board);
        mbl_mw_mag_bmm150_start(board);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_M4G:
        mbl_mw_acc_enable_acceleration_sampling(board);
        mbl_mw_mag_bmm150_enable_b_field_sampling(board);
        mbl_mw_acc_start(board);
        mbl_mw_mag_bmm150_start(board);
        break;
    }

    uint8_t enable_cmd[4] = {MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::OUTPUT_ENABLE), 
            ((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->enable_mask, 0x0};
    send_command(board, enable_cmd, sizeof(enable_cmd));

    uint8_t start_cmd[3] = {MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::ENABLE), 0x1};
    send_command(board, start_cmd, sizeof(start_cmd));
}

void mbl_mw_sensor_fusion_stop(const MblMwMetaWearBoard* board) {
    uint8_t stop_cmd[3] = {MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::ENABLE), 0x0};
    send_command(board, stop_cmd, sizeof(stop_cmd));

    uint8_t disable_cmd[4] = {MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::OUTPUT_ENABLE), 0x0, 0x7f};
    send_command(board, disable_cmd, sizeof(disable_cmd));

    switch(((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->config.mode) {
    case MBL_MW_SENSOR_FUSION_MODE_NDOF:
        mbl_mw_acc_stop(board);
        mbl_mw_gyro_bmi160_stop(board);
        mbl_mw_mag_bmm150_stop(board);
        mbl_mw_acc_disable_acceleration_sampling(board);
        mbl_mw_gyro_bmi160_disable_rotation_sampling(board);
        mbl_mw_mag_bmm150_disable_b_field_sampling(board);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS:
        mbl_mw_acc_stop(board);
        mbl_mw_gyro_bmi160_stop(board);
        mbl_mw_acc_disable_acceleration_sampling(board);
        mbl_mw_gyro_bmi160_disable_rotation_sampling(board);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_COMPASS:
        mbl_mw_acc_stop(board);
        mbl_mw_mag_bmm150_stop(board);
        mbl_mw_acc_disable_acceleration_sampling(board);
        mbl_mw_mag_bmm150_disable_b_field_sampling(board);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_M4G:
        mbl_mw_acc_stop(board);
        mbl_mw_mag_bmm150_stop(board);
        mbl_mw_acc_disable_acceleration_sampling(board);
        mbl_mw_mag_bmm150_disable_b_field_sampling(board);
        break;
    }
}

void mbl_mw_sensor_fusion_read_calibration_data(MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrCalibDataPtr completed) {
    if (board->module_info.at(MBL_MW_MODULE_SENSOR_FUSION).revision < CALIB_DATA_REVISION) {
        completed(context, board, nullptr);
    } else {
        transient_states[board].read_calib_data_context = context;
        transient_states[board].read_calib_data_completed = completed;

        uint8_t command[2] = { MBL_MW_MODULE_SENSOR_FUSION, READ_REGISTER(ORDINAL(SensorFusionRegister::ACC_CAL_DATA)) };
        SEND_COMMAND;
    }
}

void mbl_mw_sensor_fusion_write_calibration_data(const MblMwMetaWearBoard* board, const MblMwCalibrationData* data) {
    if (board->module_info.at(MBL_MW_MODULE_SENSOR_FUSION).revision >= CALIB_DATA_REVISION) {
        uint8_t command[12] = { MBL_MW_MODULE_SENSOR_FUSION, ORDINAL(SensorFusionRegister::ACC_CAL_DATA) };
        memcpy(command + 2, data->acc, sizeof(command) - 2);
        SEND_COMMAND;

        auto mode = ((SensorFusionState*) board->module_config.at(MBL_MW_MODULE_SENSOR_FUSION))->config.mode;
        if (mode == MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS || mode == MBL_MW_SENSOR_FUSION_MODE_NDOF) {
            command[1] = ORDINAL(SensorFusionRegister::GYRO_CAL_DATA);
            memcpy(command + 2, data->gyro, sizeof(command) - 2);
            SEND_COMMAND;
        }

        if (mode == MBL_MW_SENSOR_FUSION_MODE_M4G || mode == MBL_MW_SENSOR_FUSION_MODE_NDOF || mode == MBL_MW_SENSOR_FUSION_MODE_COMPASS) {
            command[1] = ORDINAL(SensorFusionRegister::MAG_CAL_DATA);
            memcpy(command + 2, data->mag, sizeof(command) - 2);
            SEND_COMMAND;
        }
    }
}

void create_sensor_fusion_uri(const MblMwDataSignal* signal, stringstream& uri) {
    switch(CLEAR_READ(signal->header.register_id)) {
    case ORDINAL(SensorFusionRegister::CORRECTED_ACC):
        uri << "corrected-acceleration";
        break;
    case ORDINAL(SensorFusionRegister::CORRECTED_GYRO):
        uri << "corrected-angular-velocity";
        break;
    case ORDINAL(SensorFusionRegister::CORRECTED_MAG):
        uri << "corrected-magnetic-field";
        break;
    case ORDINAL(SensorFusionRegister::QUATERNION):
        uri << "quaternion";
        break;
    case ORDINAL(SensorFusionRegister::EULER_ANGLES):
        uri << "euler-angles";
        break;
    case ORDINAL(SensorFusionRegister::GRAVITY_VECTOR):
        uri << "gravity";
        break;
    case ORDINAL(SensorFusionRegister::LINEAR_ACC):
        uri << "linear-acceleration";
        break;
    }
}
