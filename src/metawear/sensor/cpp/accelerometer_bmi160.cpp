#include "accelerometer_bmi160_private.h"
#include "accelerometer_bmi160_register.h"

#include "metawear/core/types.h"
#include "metawear/core/cpp/constant.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

using std::forward_as_tuple;
using std::malloc;
using std::memcpy;
using std::memset;
using std::min;
using std::piecewise_construct;
using std::unordered_map;

static const uint8_t FSR_BITMASKS[4]= {0x3, 0x5, 0x8, 0xc};
static const unordered_map<uint8_t, float> FSR_SCALE= {{0x3, 16384.f}, {0x5, 8192.f}, {0x8, 4096.f}, {0xc, 2048.f}};

static float bmi160_to_firmware(MblMwDataSignal* source, float value) {
    return (value * ((AccBmi160Config*)source->owner->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->get_scale());
}

static void* create_acc_bmi160_config() {
    AccBmi160Config* new_config = (AccBmi160Config*)malloc(sizeof(AccBmi160Config));

    memset(new_config, 0, sizeof(AccBmi160Config));
    new_config->acc_bwp = 2;
    new_config->set_output_data_rate(MBL_MW_ACC_BMI160_ODR_100HZ);
    new_config->set_range(MBL_MW_ACC_BMI160_FSR_2G);

    return new_config;
}

void init_accelerometer_bmi160(MblMwMetaWearBoard *board) {
    MblMwDataSignal *acc_signal= new MblMwDataSignal(BMI160_ACCEL_RESPONSE_HEADER, board, 
            DataInterpreter::BMI160_ACCELERATION, 3, 2, 1, 0);
    acc_signal->number_to_firmware = bmi160_to_firmware;
    board->sensor_data_signals[BMI160_ACCEL_RESPONSE_HEADER]= acc_signal;
    board->module_config[MBL_MW_MODULE_ACCELEROMETER]= create_acc_bmi160_config();
    board->responses[BMI160_ACCEL_RESPONSE_HEADER]= response_handler_data_no_id;
}

void AccBmi160Config::set_output_data_rate(MblMwAccBmi160Odr odr) {
    acc_odr= odr + 1;
}

void AccBmi160Config::set_range(MblMwAccBmi160Range range) {
    acc_range= FSR_BITMASKS[range];
}

float AccBmi160Config::get_scale() const {
    return FSR_SCALE.at(acc_range);
}

MblMwDataSignal* mbl_mw_acc_bmi160_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    if (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation != MBL_MW_MODULE_ACC_TYPE_BMI160) {
        return NULL;
    }
    return board->sensor_data_signals.at(BMI160_ACCEL_RESPONSE_HEADER);
}

void mbl_mw_acc_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi160Odr odr) {
    ((AccBmi160Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->set_output_data_rate(odr);
}

void mbl_mw_acc_bmi160_set_range(MblMwMetaWearBoard *board, MblMwAccBmi160Range range) {
    ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->set_range(range);
}

void mbl_mw_acc_bmi160_write_acceleration_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBmi160Register::DATA_CONFIG)};
    std::memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_ACCELEROMETER), sizeof(AccBmi160Config));
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBmi160Register::POWER_MODE), 1};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBmi160Register::POWER_MODE), 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_enable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBmi160Register::DATA_INTERRUPT_ENABLE), 1, 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_disable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBmi160Register::DATA_INTERRUPT_ENABLE), 0, 1};
    SEND_COMMAND;
}
