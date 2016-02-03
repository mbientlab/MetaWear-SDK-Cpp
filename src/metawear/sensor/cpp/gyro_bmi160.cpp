#include "metawear/core/types.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "metawear/sensor/gyro_bmi160.h"
#include "gyro_bmi160_private.h"
#include "gyro_bmi160_register.h"

#include <cstdlib>
#include <cstring>

using std::forward_as_tuple;
using std::malloc;
using std::memcpy;
using std::memset;
using std::piecewise_construct;

static const float FSR_SCALE[5]= {16.4f, 32.8f, 65.6f, 131.2f, 262.4f};

float GyroBmi160Config::get_scale() const {
    return FSR_SCALE[gyr_range];
}

static float bmi160_to_firmware(MblMwDataSignal* source, float value) {
    return (value * ((GyroBmi160Config*)source->owner->module_config.at(MBL_MW_MODULE_GYRO))->get_scale());
}

MblMwDataSignal* mbl_mw_gyro_bmi160_get_rotation_data_signal(const MblMwMetaWearBoard *board) {
    return board->sensor_data_signals.at(GYRO_ROT_RESPONSE_HEADER);
}

void init_gyro_module(MblMwMetaWearBoard *board) {
    GyroBmi160Config *new_config = (GyroBmi160Config*)malloc(sizeof(GyroBmi160Config));

    memset(new_config, 0, sizeof(GyroBmi160Config));
    new_config->gyr_bwp = 2;
    new_config->gyr_odr = MBL_MW_GYRO_BMI160_ODR_100HZ;
    new_config->gyr_range = MBL_MW_GYRO_BMI160_FSR_2000DPS;
    board->module_config.emplace(MBL_MW_MODULE_GYRO, new_config);

    MblMwDataSignal *gyro_signal= new MblMwDataSignal(GYRO_ROT_RESPONSE_HEADER, board, DataInterpreter::BMI160_ROTATION, 
            3, 2, 1, 0);
    gyro_signal->number_to_firmware = bmi160_to_firmware;
    board->sensor_data_signals[GYRO_ROT_RESPONSE_HEADER]= gyro_signal;
    board->responses[GYRO_ROT_RESPONSE_HEADER]= response_handler_data_no_id;
}

void mbl_mw_gyro_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwGyroBmi160Odr odr) {
    ((GyroBmi160Config*) board->module_config.at(MBL_MW_MODULE_GYRO))->gyr_odr= odr;
}

void mbl_mw_gyro_bmi160_set_range(MblMwMetaWearBoard *board, MblMwGyroBmi160Range range) {
    ((GyroBmi160Config*) board->module_config.at(MBL_MW_MODULE_GYRO))->gyr_range= range;
}

void mbl_mw_gyro_bmi160_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::CONFIG)};
    memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_GYRO), sizeof(GyroBmi160Config));
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::POWER_MODE), 1};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::POWER_MODE), 0};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_enable_rotation_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::DATA_INTERRUPT_ENABLE), 0x1, 0x0};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_disable_rotation_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::DATA_INTERRUPT_ENABLE), 0x0, 0x1};
    SEND_COMMAND;
}
