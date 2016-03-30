#include "accelerometer_bosch_private.h"
#include "accelerometer_bosch_register.h"

#include "metawear/core/types.h"
#include "metawear/core/cpp/constant.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

using std::forward_as_tuple;
using std::calloc;
using std::memcpy;
using std::min;
using std::piecewise_construct;
using std::unordered_map;

static const uint8_t FSR_BITMASKS[4]= {0x3, 0x5, 0x8, 0xc};
static const unordered_map<uint8_t, float> FSR_SCALE= {{0x3, 16384.f}, {0x5, 8192.f}, {0x8, 4096.f}, {0xc, 2048.f}};

static float bosch_to_firmware(MblMwDataSignal* source, float value) {
    return value * bosch_get_data_scale(source->owner);
}

static inline void init_accelerometer_bosch(MblMwMetaWearBoard *board, void *config) {
    MblMwDataSignal *acc_signal= new MblMwDataSignal(BOSCH_ACCEL_RESPONSE_HEADER, board, DataInterpreter::BOSCH_ACCELERATION, 3, 2, 1, 0);
    acc_signal->number_to_firmware = bosch_to_firmware;
    board->sensor_data_signals[BOSCH_ACCEL_RESPONSE_HEADER]= acc_signal;
    board->module_config[MBL_MW_MODULE_ACCELEROMETER]= config;
    board->responses[BOSCH_ACCEL_RESPONSE_HEADER]= response_handler_data_no_id;
}

struct AccBmi160Config {
    uint8_t acc_odr:4;
    uint8_t acc_bwp:3;
    uint8_t acc_us:1;
    uint8_t acc_range:4;
    uint8_t:4;

    inline void set_output_data_rate(MblMwAccBmi160Odr odr) {
        acc_odr= odr + 1;
    }
    inline void set_range(MblMwAccBoschRange range) {
        acc_range= FSR_BITMASKS[range];
    }
};

struct AccBma255Config {
    uint8_t acc_bw:5;
    uint8_t :3;
    uint8_t acc_range:4;
    uint8_t :4;

    inline void set_output_data_rate(MblMwAccBma255Odr odr) {
        acc_bw= odr + 8;
    }
    inline void set_range(MblMwAccBoschRange range) {
        acc_range= FSR_BITMASKS[range];
    }
};

void init_accelerometer_bmi160(MblMwMetaWearBoard *board) {
    AccBmi160Config* new_config = (AccBmi160Config*) calloc(1, sizeof(AccBmi160Config));

    new_config->acc_bwp = 2;
    new_config->set_output_data_rate(MBL_MW_ACC_BMI160_ODR_100HZ);
    new_config->set_range(MBL_MW_ACC_BOSCH_FSR_2G);

    init_accelerometer_bosch(board, new_config);
}

void init_accelerometer_bma255(MblMwMetaWearBoard *board) {
    AccBma255Config* new_config = (AccBma255Config*) calloc(1, sizeof(AccBma255Config));

    new_config->set_output_data_rate(MBL_MW_ACC_BMA255_ODR_62_5HZ);
    new_config->set_range(MBL_MW_ACC_BOSCH_FSR_2G);

    init_accelerometer_bosch(board, new_config);
}

float bosch_get_data_scale(const MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        return FSR_SCALE.at(((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc_range);
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        return FSR_SCALE.at(((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc_range);
    default:
        return 1.f;
    }
}

MblMwDataSignal* mbl_mw_acc_bosch_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    auto implementation= board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation;
    if (implementation != MBL_MW_MODULE_ACC_TYPE_BMI160 && implementation != MBL_MW_MODULE_ACC_TYPE_BMA255) {
        return NULL;
    }
    return board->sensor_data_signals.at(BOSCH_ACCEL_RESPONSE_HEADER);
}

void mbl_mw_acc_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi160Odr odr) {
    auto config= (AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER);

    config->set_output_data_rate(odr);
    if (odr < MBL_MW_ACC_BMI160_ODR_12_5HZ) {
        config->acc_us= 1;
        config->acc_bwp= 0;
    } else {
        config->acc_us= 0;
        config->acc_bwp= 2;
    }
}

void mbl_mw_acc_bma255_set_odr(MblMwMetaWearBoard *board, MblMwAccBma255Odr odr) {
    ((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->set_output_data_rate(odr);
}

void mbl_mw_acc_bosch_set_range(MblMwMetaWearBoard *board, MblMwAccBoschRange range) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->set_range(range);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        ((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->set_range(range);
        break;
    }
}

void mbl_mw_acc_bosch_write_acceleration_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::DATA_CONFIG)};
    size_t size;

    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        size= sizeof(AccBmi160Config);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        size= sizeof(AccBma255Config);
        break;
    default:
        return;
    }
    std::memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_ACCELEROMETER), size);
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::POWER_MODE), 1};
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::POWER_MODE), 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_enable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::DATA_INTERRUPT_ENABLE), 1, 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_disable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::DATA_INTERRUPT_ENABLE), 0, 1};
    SEND_COMMAND;
}
