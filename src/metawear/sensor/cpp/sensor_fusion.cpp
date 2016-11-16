#include "metawear/core/module.h"
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

#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memset;

#define CORRECTED_ACC_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_CORRECTED_ACC]
#define CORRECTED_GYRO_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_CORRECTED_GYRO]
#define CORRECTED_MAG_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_CORRECTED_MAG]
#define QUATERNION_RESPONSE_HEADER RESPONSE_HEADERS[MBL_MW_SENSOR_FUSION_DATA_QUATERION]
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

struct SensorFusionState {
    struct {
        uint8_t mode;
        uint8_t acc_range:4;
        uint8_t gyro_range:4;
    } config;
    uint8_t enable_mask;
};

void init_sensor_fusion_module(MblMwMetaWearBoard* board) {
    if (board->module_info.count(MBL_MW_MODULE_SENSOR_FUSION) && board->module_info.at(MBL_MW_MODULE_SENSOR_FUSION).present) {
        SensorFusionState* new_state = (SensorFusionState*) malloc(sizeof(SensorFusionState));
        new_state->config.mode = MBL_MW_SENSOR_FUSION_MODE_SLEEP;
        new_state->config.acc_range = MBL_MW_SENSOR_FUSION_ACC_RANGE_16G;
        new_state->config.gyro_range = MBL_MW_SENSOR_FUSION_GYRO_RANGE_2000DPS;
        new_state->enable_mask = 0;
        board->module_config.emplace(MBL_MW_MODULE_SENSOR_FUSION, new_state);

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
                    DataInterpreter::SENSOR_FUSION_QUATERION, FirmwareConverter::DEFAULT, 4, 4, 1, 0);
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
    }
}

MblMwDataSignal* mbl_mw_sensor_fusion_get_data_signal(const MblMwMetaWearBoard* board, MblMwSensorFusionData data) {
    GET_DATA_SIGNAL(RESPONSE_HEADERS[data]);
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
        mbl_mw_gyro_bmi160_set_odr(board, MBL_MW_GYRO_BMI160_ODR_100HZ);
        mbl_mw_gyro_bmi160_write_config(board);

        mbl_mw_mag_bmm150_configure(board, 9, 15, MBL_MW_MAG_BMM150_ODR_25_HZ);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS:
        mbl_mw_acc_set_range(board, ACC_RANGES[state->config.acc_range]);
        mbl_mw_acc_set_odr(board, 100.f);
        mbl_mw_acc_write_acceleration_config(board);

        mbl_mw_gyro_bmi160_set_range(board, (MblMwGyroBmi160Range) (state->config.gyro_range - 1));
        mbl_mw_gyro_bmi160_set_odr(board, MBL_MW_GYRO_BMI160_ODR_100HZ);
        mbl_mw_gyro_bmi160_write_config(board);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_COMPASS:
        mbl_mw_acc_set_range(board, ACC_RANGES[state->config.acc_range]);
        mbl_mw_acc_set_odr(board, 25.f);
        mbl_mw_acc_write_acceleration_config(board);

        mbl_mw_mag_bmm150_configure(board, 9, 15, MBL_MW_MAG_BMM150_ODR_25_HZ);
        break;
    case MBL_MW_SENSOR_FUSION_MODE_M4G:
        mbl_mw_acc_set_range(board, ACC_RANGES[state->config.acc_range]);
        mbl_mw_acc_set_odr(board, 50.f);
        mbl_mw_acc_write_acceleration_config(board);

        mbl_mw_mag_bmm150_configure(board, 9, 15, MBL_MW_MAG_BMM150_ODR_25_HZ);
    }
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
