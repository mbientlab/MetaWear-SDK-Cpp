#include "accelerometer_mma8452q_private.h"
#include "accelerometer_mma8452q_register.h"

#include "metawear/core/cpp/constant.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <cstdlib>
#include <cstring>

using std::forward_as_tuple;
using std::calloc;
using std::memcpy;
using std::memset;
using std::piecewise_construct;

static void* create_mma8452q_config() {
    Mma8452qConfig* new_config = (Mma8452qConfig*)calloc(1, sizeof(Mma8452qConfig));

    new_config->tap_threshold = 2.f;
    new_config->tap_latency = 200;
    new_config->tap_window = 300;
    new_config->tap_duration = 60;
    new_config->shake_threshold = 0.5f;
    new_config->shake_duration = 50;
    new_config->movement_threshold = 1.5f;
    new_config->movement_duration = 100;
    new_config->orientation_delay = 100;
    new_config->active_timeout = 0;
    //    shake_axis(MBL_MW_ACC_MMA8452Q_AXIS_X),
    new_config->movement_axis_x = 1;
    new_config->movement_axis_y = 1;
    new_config->movement_axis_z = 1;
    //    movement_type(MBL_MW_ACC_MMA8452Q_MOTION),
    //    tap_axis(MBL_MW_ACC_MMA8452Q_AXIS_Z), 
    new_config->tap_type_single = 1;
    new_config->tap_type_double = 0;
    new_config->odr = MBL_MW_ACC_MMA8452Q_ODR_100HZ;
    new_config->accel_fsr = MBL_MW_ACC_MMA8452Q_FSR_2G;
    return new_config;
}

static float mma8452q_to_firmware(MblMwDataSignal* source, float value) {
    return (value * 1000.f);
}

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

void init_accelerometer_mma8452q(MblMwMetaWearBoard *board) {
    MblMwDataSignal *acc_signal= new MblMwDataSignal(MMA8452Q_ACCEL_RESPONSE_HEADER, board, 
            DataInterpreter::MMA8452Q_ACCELERATION, 3, 2, 1, 0);
    acc_signal->number_to_firmware = mma8452q_to_firmware;
    board->sensor_data_signals[MMA8452Q_ACCEL_RESPONSE_HEADER]= acc_signal;
    board->module_config[MBL_MW_MODULE_ACCELEROMETER]= create_mma8452q_config();
    board->responses[MMA8452Q_ACCEL_RESPONSE_HEADER]= response_handler_data_no_id;
}

MblMwDataSignal* mbl_mw_acc_mma8452q_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    if (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation != MBL_MW_MODULE_ACC_TYPE_MMA8452Q) {
        return NULL;
    }
    return board->sensor_data_signals.at(MMA8452Q_ACCEL_RESPONSE_HEADER);
}

void mbl_mw_acc_mma8452q_set_odr(MblMwMetaWearBoard *board, MblMwAccMma8452qOdr odr) {
    ((Mma8452qConfig*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->odr= odr;
}

void mbl_mw_acc_mma8452q_set_range(MblMwMetaWearBoard *board, MblMwAccMma8452qRange range) {
    ((Mma8452qConfig*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->accel_fsr= range;
}

void mbl_mw_acc_mma8452q_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerMma8452qRegister::GLOBAL_ENABLE), 1};
    SEND_COMMAND;
}

void mbl_mw_acc_mma8452q_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerMma8452qRegister::GLOBAL_ENABLE), 0};
    SEND_COMMAND;
}

void mbl_mw_acc_mma8452q_enable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerMma8452qRegister::DATA_ENABLE), 1};
    SEND_COMMAND;
}

void mbl_mw_acc_mma8452q_disable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerMma8452qRegister::DATA_ENABLE), 0};
    SEND_COMMAND;
}

void mbl_mw_acc_mma8452q_write_acceleration_config(const MblMwMetaWearBoard *board) {
    uint8_t command[7]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerMma8452qRegister::DATA_CONFIG)};

    auto config = (Mma8452qConfig*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER);
    Mma8452qAccBitField bit_field;

    memset(&bit_field, 0, sizeof(Mma8452qAccBitField));
    bit_field.dr= config->odr;
    bit_field.fs= config->accel_fsr;
    memcpy(command + 2, &bit_field, sizeof(bit_field));

    SEND_COMMAND;
}
