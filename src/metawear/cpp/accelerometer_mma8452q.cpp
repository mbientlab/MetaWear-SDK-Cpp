#include "connection_def.h"
#include "metawear/accelerometer_mma8452q.h"
#include "metawearboard_def.h"
#include "register.h"

#include <cstring>

using std::memcpy;
using std::memset;

Mma8452qConfig::Mma8452qConfig() : 
    tap_threshold(2.f), tap_latency(200), tap_window(300), tap_duration(60),
    shake_threshold(0.5f), shake_duration(50), 
    movement_threshold(1.5f), movement_duration(100), 
    orientation_delay(100),
    active_timeout(0),
//    shake_axis(MBL_MW_ACC_MMA8452Q_AXIS_X),
    movement_axis_x(1), movement_axis_y(1), movement_axis_z(1),
//    movement_type(MBL_MW_ACC_MMA8452Q_MOTION),
//    tap_axis(MBL_MW_ACC_MMA8452Q_AXIS_Z), 
    tap_type_single(1), tap_type_double(0),
    odr(MBL_MW_ACC_MMA8452Q_ODR_100HZ), accel_fsr(MBL_MW_ACC_MMA8452Q_FSR_2G) { }

struct Mma8452qAccBitField {
    uint8_t fs:2;
    uint8_t :2;
    uint8_t hpf_out:1;
    uint8_t :3;
    
    uint8_t hp_filter_cutoff;
    
    uint8_t active:1;
    uint8_t f_read:1;
    uint8_t lnoise:1;
    uint8_t dr:3;
    uint8_t aslp_rate:2;
    
    uint8_t ctrl_reg2;
    uint8_t aslp_count;
};

void mbl_mw_acc_mma8452q_set_odr(MblMwMetaWearBoard *board, MblMwAccMma8452qOdr odr) {
    board->mma8452q_config.odr= odr;
}

void mbl_mw_acc_mma8452q_set_range(MblMwMetaWearBoard *board, MblMwAccMma8452qRange range) {
    board->mma8452q_config.accel_fsr= range;
}

void mbl_mw_acc_mma8452q_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerMma8452qRegister::GLOBAL_ENABLE), 1};
    SEND_COMMAND;
}

void mbl_mw_acc_mma8452q_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerMma8452qRegister::GLOBAL_ENABLE), 0};
    SEND_COMMAND;
}

void mbl_mw_acc_mma8452q_enable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerMma8452qRegister::DATA_ENABLE), 1};
    SEND_COMMAND;
}

void mbl_mw_acc_mma8452q_disable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerMma8452qRegister::DATA_ENABLE), 0};
    SEND_COMMAND;
}

void mbl_mw_acc_mma8452q_write_acceleration_config(const MblMwMetaWearBoard *board) {
    uint8_t command[7]= {ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerMma8452qRegister::DATA_CONFIG)};

    Mma8452qAccBitField bit_field;
    memset(&bit_field, 0, sizeof(Mma8452qAccBitField));
    bit_field.dr= board->mma8452q_config.odr;
    bit_field.fs= board->mma8452q_config.accel_fsr;
    memcpy(command + 2, &bit_field, sizeof(bit_field));

    SEND_COMMAND;
}
