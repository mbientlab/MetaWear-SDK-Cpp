#include "connection_def.h"
#include "metawear/accelerometer_bmi160.h"
#include "metawearboard_def.h"
#include "register.h"

#include <cstring>
#include <unordered_map>

using std::unordered_map;
using std::memcpy;

static const uint8_t FSR_BITMASKS[4]= {0x3, 0x5, 0x8, 0xc};
static const unordered_map<uint8_t, float> FSR_SCALE= {{0x3, 16384.f}, {0x5, 8192.f}, {0x8, 4096.f}, {0xc, 2048.f}};

AccBmi160Config::AccBmi160Config() {
    std::memset(this, 0, sizeof(AccBmi160Config));

    acc_bwp= 2;
    set_output_data_rate(MBL_MW_ACC_BMI160_ODR_100HZ);
    set_range(MBL_MW_ACC_BMI160_FSR_2G);
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

void mbl_mw_acc_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi160Odr odr) {
    board->acc_bmi160_config.set_output_data_rate(odr);
}

void mbl_mw_acc_bmi160_set_range(MblMwMetaWearBoard *board, MblMwAccBmi160Range range) {
    board->acc_bmi160_config.set_range(range);
}

void mbl_mw_acc_bmi160_write_acceleration_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerBmi160Register::DATA_CONFIG)};
    std::memcpy(command + 2, &board->acc_bmi160_config, sizeof(AccBmi160Config));
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerBmi160Register::POWER_MODE), 1};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerBmi160Register::POWER_MODE), 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_enable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerBmi160Register::DATA_INTERRUPT_ENABLE), 1, 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_disable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerBmi160Register::DATA_INTERRUPT_ENABLE), 0, 1};
    SEND_COMMAND;
}
