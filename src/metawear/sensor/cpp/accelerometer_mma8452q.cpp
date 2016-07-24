#include "metawear/sensor/accelerometer_mma8452q.h"
#include "accelerometer_mma8452q_private.h"
#include "accelerometer_mma8452q_register.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include <cstdlib>
#include <cstring>

using std::forward_as_tuple;
using std::calloc;
using std::memcpy;
using std::memset;
using std::piecewise_construct;

uint8_t MMA8452Q_DEFAULT_CONFIG[]= {
    0x00, 0x00, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x44, 0x84,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
}, PACKED_ACC_REVISION= 1;

struct Mma8452qConfig {
    struct {
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
        uint8_t mods:2;
        uint8_t slpe:1;
        uint8_t smods:2;
        uint8_t :1;
        uint8_t rst:1;
        uint8_t st:1;
        uint8_t aslp_count;
    } acc;
    struct {
        uint8_t :3;
        uint8_t xefe:1;
        uint8_t yefe:1;
        uint8_t zefe:1;
        uint8_t oae:1;
        uint8_t ele:1;
        uint8_t :8;
        uint8_t ths:7;
        uint8_t dbcntm:1;
        uint8_t count;
    } ff_mt;
    struct {
        uint8_t :8;
        uint8_t :6;
        uint8_t pl_en:1;
        uint8_t dbcntm:1;
        uint8_t pl_count;
        uint8_t zlock:3;
        uint8_t :3;
        uint8_t bkfr:2;
        uint8_t hys:3;
        uint8_t pl_ths:5;
    } orientation;
    struct {
        uint8_t xspefe:1;
        uint8_t xdpefe:1;
        uint8_t yspefe:1;
        uint8_t ydpefe:1;
        uint8_t zspefe:1;
        uint8_t zdpefe:1;
        uint8_t ele:1;
        uint8_t dpa:1;
        uint8_t :8;
        uint8_t thsx, thsy, thsz, tmlt, ltcy, wind;
    } tap;
    struct {
        uint8_t hpf_byp:1;
        uint8_t xtefe:1;
        uint8_t ytefe:1;
        uint8_t ztefe:1;
        uint8_t ele:1;
        uint8_t :3;
        uint8_t :8;
        uint8_t ths:7;
        uint8_t dbcntm:1;
        uint8_t transient_count;
    } shake;
};

const ResponseHeader MMA8452Q_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerMma8452qRegister::DATA_VALUE)),
    MMA8452Q_PACKED_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerMma8452qRegister::PACKED_ACC_DATA));

void init_accelerometer_mma8452q(MblMwMetaWearBoard *board) {
    if (!board->module_events.count(MMA8452Q_ACCEL_RESPONSE_HEADER)) {
        board->module_events[MMA8452Q_ACCEL_RESPONSE_HEADER] = new MblMwDataSignal(MMA8452Q_ACCEL_RESPONSE_HEADER, board, 
            DataInterpreter::MMA8452Q_ACCELERATION, FirmwareConverter::MMA8452Q_ACCELERATION, 3, 2, 1, 0);
    }

    if (!board->module_config.count(MBL_MW_MODULE_ACCELEROMETER)) {
        Mma8452qConfig* config= (Mma8452qConfig*) malloc(sizeof(Mma8452qConfig));
        memcpy(config, MMA8452Q_DEFAULT_CONFIG, sizeof(MMA8452Q_DEFAULT_CONFIG));

        board->module_config[MBL_MW_MODULE_ACCELEROMETER] = config;
    }
    board->responses[MMA8452Q_ACCEL_RESPONSE_HEADER]= response_handler_data_no_id;

    if (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).revision >= PACKED_ACC_REVISION) {
        if (!board->module_events.count(MMA8452Q_PACKED_ACCEL_RESPONSE_HEADER)) {
            board->module_events[MMA8452Q_PACKED_ACCEL_RESPONSE_HEADER]= new MblMwDataSignal(MMA8452Q_PACKED_ACCEL_RESPONSE_HEADER, board, 
                DataInterpreter::MMA8452Q_ACCELERATION, FirmwareConverter::MMA8452Q_ACCELERATION, 3, 2, 1, 0);
        }
        board->responses[MMA8452Q_PACKED_ACCEL_RESPONSE_HEADER]= response_handler_packed_data;
    }    
}

void serialize_accelerometer_mma8452q_config(const MblMwMetaWearBoard* board, std::vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(Mma8452qConfig, MBL_MW_MODULE_ACCELEROMETER);
}

void deserialize_accelerometer_mma8452q_config(MblMwMetaWearBoard* board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(Mma8452qConfig, MBL_MW_MODULE_ACCELEROMETER);
}

MblMwDataSignal* mbl_mw_acc_mma8452q_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    if (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation != MBL_MW_MODULE_ACC_TYPE_MMA8452Q) {
        return nullptr;
    }
    GET_DATA_SIGNAL(MMA8452Q_ACCEL_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    if (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation != MBL_MW_MODULE_ACC_TYPE_MMA8452Q) {
        return nullptr;
    }
    GET_DATA_SIGNAL(MMA8452Q_PACKED_ACCEL_RESPONSE_HEADER);
}

void mbl_mw_acc_mma8452q_set_odr(MblMwMetaWearBoard *board, MblMwAccMma8452qOdr odr) {
    ((Mma8452qConfig*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc.dr= odr;
}

void mbl_mw_acc_mma8452q_set_range(MblMwMetaWearBoard *board, MblMwAccMma8452qRange range) {
    ((Mma8452qConfig*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc.fs= range;
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

    auto config = ((Mma8452qConfig*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc;
    memcpy(command + 2, &config, sizeof(config));

    SEND_COMMAND;
}
