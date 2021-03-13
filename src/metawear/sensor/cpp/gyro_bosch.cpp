#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include "metawear/sensor/gyro_bosch.h"
#include "gyro_bosch_private.h"
#include "gyro_bosch_register.h"
#include "utils.h"

#include <cstdlib>
#include <cstring>
#include <unordered_map>

using std::forward_as_tuple;
using std::malloc;
using std::memcpy;
using std::memset;
using std::piecewise_construct;
using std::stringstream;
using std::unordered_map;
using std::vector;

#define CREATE_ROT_SIGNAL_SINGLE(offset) CREATE_ROT_SIGNAL(DataInterpreter::BMI160_ROTATION_SINGLE_AXIS, 1, offset)
#define CREATE_ROT_SIGNAL(interpreter, channels, offset) new MblMwDataSignal(GYRO_ROT_RESPONSE_HEADER, board, interpreter, \
        FirmwareConverter::BMI160_ROTATION, channels, 2, 1, offset)
#define CREATE_BMI270_ROT_SIGNAL_SINGLE(offset) CREATE_BMI270_ROT_SIGNAL(DataInterpreter::BMI160_ROTATION_SINGLE_AXIS, 1, offset)        
#define CREATE_BMI270_ROT_SIGNAL(interpreter, channels, offset) new MblMwDataSignal(GYRO_BMI270_ROT_RESPONSE_HEADER, board, interpreter, \
        FirmwareConverter::BMI160_ROTATION, channels, 2, 1, offset)

const float FSR_SCALE[5]= {16.4f, 32.8f, 65.6f, 131.2f, 262.4f};
const uint8_t PACKED_ROT_REVISION= 1;
const ResponseHeader    GYRO_ROT_RESPONSE_HEADER(MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::DATA)),
                        GYRO_PACKED_ROT_RESPONSE_HEADER(MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::PACKED_DATA)),
                        GYRO_BMI270_ROT_RESPONSE_HEADER(MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi270Register::DATA)),
                        GYRO_BMI270_PACKED_ROT_RESPONSE_HEADER(MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi270Register::PACKED_DATA));

struct GyroBmi160Config {
    uint8_t gyr_odr : 4;
    uint8_t gyr_bwp : 2;
    uint8_t:2;
    uint8_t gyr_range : 3;
    uint8_t:5;
};

struct GyroBmi160State {
    MblMwFnBoardPtrInt read_config_completed;
    void *read_config_context;
};

static unordered_map<const MblMwMetaWearBoard*, GyroBmi160State> states;

static int32_t received_config_response(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    memcpy(board->module_config.at(MBL_MW_MODULE_GYRO), response + 2, sizeof(GyroBmi160Config));

    auto callback = states[board].read_config_completed;
    auto context = states[board].read_config_context;
    states[board].read_config_completed = nullptr;
    states[board].read_config_context = nullptr;
    callback(context, board, MBL_MW_STATUS_OK);

    return MBL_MW_STATUS_OK;
}

float bosch_gyro_get_data_scale(const MblMwMetaWearBoard *board) {
    return FSR_SCALE[((GyroBmi160Config*) board->module_config.at(MBL_MW_MODULE_GYRO))->gyr_range];
}

void init_gyro_module(MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_GYRO).implementation) {
    case MBL_MW_MODULE_GYRO_TYPE_BMI160:
        if (board->module_info.count(MBL_MW_MODULE_GYRO) && board->module_info.at(MBL_MW_MODULE_GYRO).present) {
            if (!board->module_config.count(MBL_MW_MODULE_GYRO)) {
                GyroBmi160Config *new_config = (GyroBmi160Config*)malloc(sizeof(GyroBmi160Config));

                memset(new_config, 0, sizeof(GyroBmi160Config));
                new_config->gyr_bwp = 2;
                new_config->gyr_odr = MBL_MW_GYRO_BOSCH_ODR_100Hz;
                new_config->gyr_range = MBL_MW_GYRO_BOSCH_RANGE_2000dps;
                board->module_config.emplace(MBL_MW_MODULE_GYRO, new_config);
            }

            MblMwDataSignal* rotation;
            if (board->module_events.count(GYRO_ROT_RESPONSE_HEADER)) {
                rotation = dynamic_cast<MblMwDataSignal*>(board->module_events[GYRO_ROT_RESPONSE_HEADER]);
            } else {
                rotation = CREATE_ROT_SIGNAL(DataInterpreter::BMI160_ROTATION, 3, 0);
                board->module_events[GYRO_ROT_RESPONSE_HEADER] = rotation;
            }
            if (!rotation->components.size()) {
                rotation->components.push_back(CREATE_ROT_SIGNAL_SINGLE(0));
                rotation->components.push_back(CREATE_ROT_SIGNAL_SINGLE(2));
                rotation->components.push_back(CREATE_ROT_SIGNAL_SINGLE(4));
            }

            board->responses[GYRO_ROT_RESPONSE_HEADER]= response_handler_data_no_id;

            if (board->module_info.at(MBL_MW_MODULE_GYRO).revision >= PACKED_ROT_REVISION) {
                if (!board->module_events.count(GYRO_PACKED_ROT_RESPONSE_HEADER)) {
                    board->module_events[GYRO_PACKED_ROT_RESPONSE_HEADER]= new MblMwDataSignal(GYRO_PACKED_ROT_RESPONSE_HEADER, board, 
                        DataInterpreter::BMI160_ROTATION, FirmwareConverter::BMI160_ROTATION, 3, 2, 1, 0);
                }
                board->responses[GYRO_PACKED_ROT_RESPONSE_HEADER]= response_handler_packed_data;
            }

            board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GYRO, READ_REGISTER(ORDINAL(GyroBmi160Register::CONFIG))),
                    forward_as_tuple(received_config_response));

            GyroBmi160State newState = {nullptr};
            states.insert({board, newState});
        }
        break;
    case MBL_MW_MODULE_GYRO_TYPE_BMI270:
        if (board->module_info.count(MBL_MW_MODULE_GYRO) && board->module_info.at(MBL_MW_MODULE_GYRO).present) {
            if (!board->module_config.count(MBL_MW_MODULE_GYRO)) {
                GyroBmi160Config *new_config = (GyroBmi160Config*)malloc(sizeof(GyroBmi160Config));

                memset(new_config, 0, sizeof(GyroBmi160Config));
                new_config->gyr_bwp = 2;
                new_config->gyr_odr = MBL_MW_GYRO_BOSCH_ODR_100Hz;
                new_config->gyr_range = MBL_MW_GYRO_BOSCH_RANGE_2000dps;
                board->module_config.emplace(MBL_MW_MODULE_GYRO, new_config);
            }

            MblMwDataSignal* rotation;
            if (board->module_events.count(GYRO_BMI270_ROT_RESPONSE_HEADER)) {
                rotation = dynamic_cast<MblMwDataSignal*>(board->module_events[GYRO_BMI270_ROT_RESPONSE_HEADER]);
            } else {
                rotation = CREATE_BMI270_ROT_SIGNAL(DataInterpreter::BMI160_ROTATION, 3, 0);
                board->module_events[GYRO_BMI270_ROT_RESPONSE_HEADER] = rotation;
            }
            if (!rotation->components.size()) {
                rotation->components.push_back(CREATE_BMI270_ROT_SIGNAL_SINGLE(0));
                rotation->components.push_back(CREATE_BMI270_ROT_SIGNAL_SINGLE(2));
                rotation->components.push_back(CREATE_BMI270_ROT_SIGNAL_SINGLE(4));
            }

            board->responses[GYRO_BMI270_ROT_RESPONSE_HEADER]= response_handler_data_no_id;

            if (board->module_info.at(MBL_MW_MODULE_GYRO).revision >= PACKED_ROT_REVISION) {
                if (!board->module_events.count(GYRO_BMI270_PACKED_ROT_RESPONSE_HEADER)) {
                    board->module_events[GYRO_BMI270_PACKED_ROT_RESPONSE_HEADER]= new MblMwDataSignal(GYRO_BMI270_PACKED_ROT_RESPONSE_HEADER, board, 
                        DataInterpreter::BMI160_ROTATION, FirmwareConverter::BMI160_ROTATION, 3, 2, 1, 0);
                }
                board->responses[GYRO_BMI270_PACKED_ROT_RESPONSE_HEADER]= response_handler_packed_data;
            }

            board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GYRO, READ_REGISTER(ORDINAL(GyroBmi270Register::CONFIG))),
                    forward_as_tuple(received_config_response));

            GyroBmi160State newState = {nullptr};
            states.insert({board, newState});
        }
        break;    
    default:
        return;
    }
}

void free_gyro_module(MblMwMetaWearBoard *board) {
    states.erase(board);
}

MblMwDataSignal* mbl_mw_gyro_bmi160_get_rotation_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(GYRO_ROT_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_gyro_bmi270_get_rotation_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(GYRO_BMI270_ROT_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal(const MblMwMetaWearBoard *board) {
    return mbl_mw_gyro_bmi160_get_packed_rotation_data_signal(board);
}

MblMwDataSignal* mbl_mw_gyro_bmi160_get_packed_rotation_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(GYRO_PACKED_ROT_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_gyro_bmi270_get_packed_rotation_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(GYRO_BMI270_PACKED_ROT_RESPONSE_HEADER);
}

void serialize_gyro_config(const MblMwMetaWearBoard *board, vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(GyroBmi160Config, MBL_MW_MODULE_GYRO);
}

void deserialize_gyro_config(MblMwMetaWearBoard *board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(GyroBmi160Config, MBL_MW_MODULE_GYRO);
}

void mbl_mw_gyro_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwGyroBoschOdr odr) {
    ((GyroBmi160Config*) board->module_config.at(MBL_MW_MODULE_GYRO))->gyr_odr= odr;
}

void mbl_mw_gyro_bmi160_set_range(MblMwMetaWearBoard *board, MblMwGyroBoschRange range) {
    ((GyroBmi160Config*) board->module_config.at(MBL_MW_MODULE_GYRO))->gyr_range= range;
}

void mbl_mw_gyro_bmi160_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::CONFIG)};
    memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_GYRO), sizeof(GyroBmi160Config));
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi270_set_odr(MblMwMetaWearBoard *board, MblMwGyroBoschOdr odr) {
    ((GyroBmi160Config*) board->module_config.at(MBL_MW_MODULE_GYRO))->gyr_odr= odr;
}

void mbl_mw_gyro_bmi270_set_range(MblMwMetaWearBoard *board, MblMwGyroBoschRange range) {
    ((GyroBmi160Config*) board->module_config.at(MBL_MW_MODULE_GYRO))->gyr_range= range;
}

void mbl_mw_gyro_bmi270_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi270Register::CONFIG)};
    memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_GYRO), sizeof(GyroBmi160Config));
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi160_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed) {
    states[board].read_config_context = context;
    states[board].read_config_completed = completed;

    uint8_t command[2]= {MBL_MW_MODULE_GYRO, READ_REGISTER(ORDINAL(GyroBmi270Register::CONFIG))};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi270_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed) {
    states[board].read_config_context = context;
    states[board].read_config_completed = completed;

    uint8_t command[2]= {MBL_MW_MODULE_GYRO, READ_REGISTER(ORDINAL(GyroBmi270Register::CONFIG))};
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

void mbl_mw_gyro_bmi270_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi270Register::POWER_MODE), 1};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi270_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi270Register::POWER_MODE), 0};
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

void mbl_mw_gyro_bmi270_enable_rotation_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi270Register::DATA_INTERRUPT_ENABLE), 0x1, 0x0};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi270_disable_rotation_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi270Register::DATA_INTERRUPT_ENABLE), 0x0, 0x1};
    SEND_COMMAND;
}

void mbl_mw_gyro_bmi270_acc_offsets(const MblMwMetaWearBoard* board, uint8_t x_offset, uint8_t y_offset, uint8_t z_offset) {
    uint8_t gyr_usr_off_x_7_0 = x_offset;
    uint8_t gyr_usr_off_y_7_0 = y_offset;
    uint8_t gyr_usr_off_z_7_0 = z_offset;

    uint8_t command[5]= {MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi270Register::OFFSET), gyr_usr_off_x_7_0, gyr_usr_off_y_7_0, gyr_usr_off_z_7_0};
    SEND_COMMAND;
}

void create_gyro_uri(const MblMwDataSignal* signal, std::stringstream& uri) {
    switch(signal->owner->module_info.at(MBL_MW_MODULE_GYRO).implementation) {
    case MBL_MW_MODULE_GYRO_TYPE_BMI160:
        switch(CLEAR_READ(signal->header.register_id)) {
        case ORDINAL(GyroBmi160Register::DATA):
            uri << "angular-velocity";
            if (signal->length() <= 2) {
                uri << "[" << (int) (signal->offset >> 1) << "]";
            }
        }
        break;
    case MBL_MW_MODULE_GYRO_TYPE_BMI270:
        switch(CLEAR_READ(signal->header.register_id)) {
        case ORDINAL(GyroBmi270Register::DATA):
            uri << "angular-velocity";
            if (signal->length() <= 2) {
                uri << "[" << (int) (signal->offset >> 1) << "]";
            }
        }
        break;
    
    }
}
