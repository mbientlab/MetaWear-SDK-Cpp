#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

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
using std::vector;

const float FSR_SCALE[5]= {16.4f, 32.8f, 65.6f, 131.2f, 262.4f}, PACKED_ROT_REVISION= 1;
const ResponseHeader GYRO_ROT_RESPONSE_HEADER(MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::DATA)),
    GYRO_PACKED_ROT_RESPONSE_HEADER(MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::PACKED_DATA));

struct GyroBmi160Config {
    uint8_t gyr_odr : 4;
    uint8_t gyr_bwp : 2;
    uint8_t:2;
    uint8_t gyr_range : 3;
    uint8_t:5;
};

float bosch_gyro_get_data_scale(const MblMwMetaWearBoard *board) {
    return FSR_SCALE[((GyroBmi160Config*) board->module_config.at(MBL_MW_MODULE_GYRO))->gyr_range];
}

void init_gyro_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_GYRO) && board->module_info.at(MBL_MW_MODULE_GYRO).present) {
        if (!board->module_config.count(MBL_MW_MODULE_GYRO)) {
            GyroBmi160Config *new_config = (GyroBmi160Config*)malloc(sizeof(GyroBmi160Config));

            memset(new_config, 0, sizeof(GyroBmi160Config));
            new_config->gyr_bwp = 2;
            new_config->gyr_odr = MBL_MW_GYRO_BMI160_ODR_100HZ;
            new_config->gyr_range = MBL_MW_GYRO_BMI160_FSR_2000DPS;
            board->module_config.emplace(MBL_MW_MODULE_GYRO, new_config);
        }

        if (!board->module_events.count(GYRO_ROT_RESPONSE_HEADER)) {
            board->module_events[GYRO_ROT_RESPONSE_HEADER] = new MblMwDataSignal(GYRO_ROT_RESPONSE_HEADER, board,
                DataInterpreter::BMI160_ROTATION, FirmwareConverter::BMI160_ROTATION, 3, 2, 1, 0);
        }
        board->responses[GYRO_ROT_RESPONSE_HEADER]= response_handler_data_no_id;

        if (board->module_info.at(MBL_MW_MODULE_GYRO).revision >= PACKED_ROT_REVISION) {
            if (!board->module_events.count(GYRO_PACKED_ROT_RESPONSE_HEADER)) {
                board->module_events[GYRO_PACKED_ROT_RESPONSE_HEADER]= new MblMwDataSignal(GYRO_PACKED_ROT_RESPONSE_HEADER, board, 
                    DataInterpreter::BMI160_ROTATION, FirmwareConverter::BMI160_ROTATION, 3, 2, 1, 0);
            }
            board->responses[GYRO_PACKED_ROT_RESPONSE_HEADER]= response_handler_packed_data;
        }
    }
}

MblMwDataSignal* mbl_mw_gyro_bmi160_get_rotation_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(GYRO_ROT_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(GYRO_PACKED_ROT_RESPONSE_HEADER);
}

void serialize_gyro_config(const MblMwMetaWearBoard *board, vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(GyroBmi160Config, MBL_MW_MODULE_GYRO);
}

void deserialize_gyro_config(MblMwMetaWearBoard *board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(GyroBmi160Config, MBL_MW_MODULE_GYRO);
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
