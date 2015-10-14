#include <cstring>

#include "connection_def.h"
#include "metawear/gyro_bmi160.h"
#include "metawearboard_def.h"
#include "register.h"

using std::memcpy;
using std::memset;

static const float FSR_SCALE[5]= {16.4f, 32.8f, 65.6f, 131.2f, 262.4f};

GyroBmi160Config::GyroBmi160Config() {
    memset(this, 0, sizeof(GyroBmi160Config));

    gyr_bwp= 2;
    gyr_odr= MBL_MW_GYRO_BMI160_ODR_100HZ;
    gyr_range= MBL_MW_GYRO_BMI160_FSR_2000DPS;
}

float GyroBmi160Config::get_scale() const {
    return FSR_SCALE[gyr_range];
}

void mbl_mw_gyro_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwGyroBmi160Odr odr) {
    board->gyro_bmi160_config.gyr_odr= odr;
}

void mbl_mw_gyro_bmi160_set_range(MblMwMetaWearBoard *board, MblMwGyroBmi160Range range) {
    board->gyro_bmi160_config.gyr_range= range;
}

void mbl_mw_gyro_bmi160_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::GYRO), ORDINAL(GyroBmi160Register::CONFIG)};
    memcpy(command + 2, &board->gyro_bmi160_config, sizeof(GyroBmi160Config));
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::GYRO), ORDINAL(GyroBmi160Register::POWER_MODE), 1};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::GYRO), ORDINAL(GyroBmi160Register::POWER_MODE), 0};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_enable_rotation_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::GYRO), ORDINAL(GyroBmi160Register::DATA_INTERRUPT_ENABLE), 0x1, 0x0};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_disable_rotation_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::GYRO), ORDINAL(GyroBmi160Register::DATA_INTERRUPT_ENABLE), 0x0, 0x1};
    SEND_COMMAND;
}
