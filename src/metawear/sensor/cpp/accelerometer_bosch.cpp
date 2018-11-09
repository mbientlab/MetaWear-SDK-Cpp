#include "metawear/sensor/accelerometer_bosch.h"
#include "accelerometer_bosch_private.h"
#include "accelerometer_bosch_register.h"
#include "utils.h"

#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/core/types.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

using std::forward_as_tuple;
using std::free;
using std::malloc;
using std::memcpy;
using std::min;
using std::piecewise_construct;
using std::stringstream;
using std::unordered_map;
using std::vector;

#define CREATE_ACC_SIGNAL_SINGLE(offset) CREATE_ACC_SIGNAL(DataInterpreter::BOSCH_ACCELERATION_SINGLE_AXIS, 1, offset)
#define CREATE_ACC_SIGNAL(interpreter, channels, offset) new MblMwDataSignal(BOSCH_ACCEL_RESPONSE_HEADER, board, interpreter, \
        FirmwareConverter::BOSCH_ACCELERATION, channels, 2, 1, offset)
#define GET_CONFIG(x) ((x*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))

const uint8_t BMI160_DEFAULT_CONFIG[]= {
    0x28, 0x03,
    0x07, 0x30, 0x81, 0x0b, 0xc0,
    0x00, 0x14, 0x14, 0x14, 
    0x04, 0x0a, 
    0x18, 0x48, 
    0x08, 0x11, 
    0x00, 0x00
};
const uint8_t BMA255_DEFAULT_CONFIG[]= {
    0x0a, 0x03, 
    0x09, 0x30, 0x81, 0x0f, 0xc0,
    0x00, 0x14, 0x14, 
    0x04, 0x0a, 
    0x18, 0x48, 
    0x08, 0x11
};

const float ORIENT_HYS_G_PER_STEP = 0.0625f;
const uint8_t FSR_BITMASKS[4]= {0x3, 0x5, 0x8, 0xc}, PACKED_ACC_REVISION= 1;
const unordered_map<uint8_t, float> FSR_SCALE= {{0x3, 16384.f}, {0x5, 8192.f}, {0x8, 4096.f}, {0xc, 2048.f}}, 
    BOSCH_MOTION_THS_STEPS = {{0x3, 0.00391f}, {0x5, 0.00781f}, {0x8, 0.01563f}, {0xc, 0.03125f}},
    BOSCH_TAP_THS_STEPS = {{0x3, 0.0625f}, {0x5, 0.125f}, {0x8, 0.250f}, {0xc, 0.5f}};
const ResponseHeader BOSCH_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::DATA_INTERRUPT)),
    BOSCH_MOTION_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::MOTION_INTERRUPT)),
    BOSCH_ORIENTATION_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::ORIENT_INTERRUPT)),
    BOSCH_TAP_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::TAP_INTERRUPT)),
    BMI160_STEP_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::STEP_DETECTOR_INTERRUPT)),
    BMI160_STEP_COUNTER(MBL_MW_MODULE_ACCELEROMETER, READ_REGISTER(ORDINAL(AccelerometerBoschRegister::STEP_COUNTER_DATA))),
    BOSCH_PACKED_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::PACKED_ACC_DATA));

struct AccBmi160Config {
    struct {
        uint8_t odr:4;
        uint8_t bwp:3;
        uint8_t us:1;
        uint8_t range:4;
        uint8_t:4;
    } acc;
    struct {
        uint8_t low_dur;
        uint8_t low_th;
        uint8_t low_hys:2;
        uint8_t low_mode:1;
        uint8_t :3;
        uint8_t high_hys:2;
        uint8_t high_dur;
        uint8_t high_th;
    } lowhigh;
    struct {
        uint8_t anym_dur:2;
        uint8_t slo_no_mot_dur:6;
        uint8_t anym_th;
        uint8_t slo_no_mot_th;
        uint8_t int_no_mot_sel:2;
        uint8_t sig_mot_skip:2;
        uint8_t sig_mot_proof:2;
        uint8_t :2;
    } motion;
    struct {
        uint8_t dur:3;
        uint8_t :3;
        uint8_t shock:1;
        uint8_t quiet:1;
        uint8_t th:5;
        uint8_t :3;
    } tap;
    struct {
        uint8_t mode:2;
        uint8_t blocking:2;
        uint8_t hyst:4;
        uint8_t theta:6;
        uint8_t ud_en:1;
        uint8_t axes_ex:1;
    } orientation;
    struct {
        uint8_t theta:6;
        uint8_t :2;
        uint8_t hy:3;
        uint8_t :1;
        uint8_t hold_time:2;
        uint8_t :2;
    } flat;
    struct {
        uint8_t steptime_min:3;
        uint8_t min_threshold:2;
        uint8_t alpha:3;
        uint8_t min_step_buf:3;
        uint8_t step_cnt_en:1;
        uint8_t :4;
    } step_counter;

    inline void set_output_data_rate(MblMwAccBmi160Odr odr) {
        acc.odr= odr + 1;
    }
    inline void set_range(MblMwAccBoschRange range) {
        acc.range= FSR_BITMASKS[range];
    }
};

struct AccBma255Config {
    struct {
        uint8_t bw:5;
        uint8_t :3;
        uint8_t range:4;
        uint8_t :4;
    } acc;
    struct {
        uint8_t low_dur;
        uint8_t low_th;
        uint8_t low_hy:2;
        uint8_t low_mode:1;
        uint8_t :3;
        uint8_t high_hy:2;
        uint8_t high_dur;
        uint8_t high_th;
    } lowhigh;
    struct {
        uint8_t slope_dur:2;
        uint8_t slo_no_mot_dur:6;
        uint8_t slope_th;
        uint8_t slow_no_mot_th;
    } motion;
    struct {
        uint8_t dur:3;
        uint8_t :3;
        uint8_t shock:1;
        uint8_t quiet:1;
        uint8_t th:5;
        uint8_t :1;
        uint8_t samp:2;
    } tap;
    struct {
        uint8_t mode:2;
        uint8_t blocking:2;
        uint8_t hyst:3;
        uint8_t :1;
        uint8_t theta:6;
        uint8_t ud_en:1;
        uint8_t :1;
    } orientation;
    struct {
        uint8_t theta:6;
        uint8_t :2;
        uint8_t hy:3;
        uint8_t :1;
        uint8_t hold_time:2;
        uint8_t :2;
    } flat;
    
    inline void set_output_data_rate(MblMwAccBma255Odr odr) {
        acc.bw= odr + 8;
    }
    inline void set_range(MblMwAccBoschRange range) {
        acc.range= FSR_BITMASKS[range];
    }
};

struct AccBoschState {
    MblMwFnBoardPtrInt read_config_completed;
    void *read_config_context;
    uint8_t motion_mask;
};

static unordered_map<const MblMwMetaWearBoard*, AccBoschState> states;

static int32_t received_config_response(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        auto config= &((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc;
        memcpy(config, response + 2, sizeof(*config));
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        auto config= &((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc;
        memcpy(config, response + 2, sizeof(*config));
        break;
    }
    default:
        return MBL_MW_STATUS_OK;
    }

    auto callback = states[board].read_config_completed;
    auto context = states[board].read_config_context;
    states[board].read_config_completed = nullptr;
    states[board].read_config_context = nullptr;
    callback(context, board, MBL_MW_STATUS_OK);

    return MBL_MW_STATUS_OK;
}

static void init_accelerometer_bosch(MblMwMetaWearBoard *board, void *config) {
    if (!board->module_config.count(MBL_MW_MODULE_ACCELEROMETER)) {
        board->module_config[MBL_MW_MODULE_ACCELEROMETER] = config;
    } else {
        free(config);
    }

    MblMwDataSignal* acc;
    if (board->module_events.count(BOSCH_ACCEL_RESPONSE_HEADER)) { 
        acc = dynamic_cast<MblMwDataSignal*>(board->module_events[BOSCH_ACCEL_RESPONSE_HEADER]);
    } else {
        acc = CREATE_ACC_SIGNAL(DataInterpreter::BOSCH_ACCELERATION, 3, 0);
        board->module_events[BOSCH_ACCEL_RESPONSE_HEADER] = acc;
    }
    if (!acc->components.size()) {
        acc->components.push_back(CREATE_ACC_SIGNAL_SINGLE(0));
        acc->components.push_back(CREATE_ACC_SIGNAL_SINGLE(2));
        acc->components.push_back(CREATE_ACC_SIGNAL_SINGLE(4));
    }

    board->responses[BOSCH_ACCEL_RESPONSE_HEADER]= response_handler_data_no_id;

    if (!board->module_events.count(BOSCH_MOTION_DETECTOR)) {
        board->module_events[BOSCH_MOTION_DETECTOR]= new MblMwDataSignal(BOSCH_MOTION_DETECTOR, board, 
                DataInterpreter::BOSCH_ANY_MOTION, FirmwareConverter::DEFAULT, 1, 1, 0, 0);
    }
    if (!board->module_events.count(BOSCH_ORIENTATION_DETECTOR)) {
        board->module_events[BOSCH_ORIENTATION_DETECTOR]= new MblMwDataSignal(BOSCH_ORIENTATION_DETECTOR, board, 
                DataInterpreter::SENSOR_ORIENTATION, FirmwareConverter::DEFAULT, 1, 1, 0, 0);
    }
    if (!board->module_events.count(BOSCH_TAP_DETECTOR)) {
        board->module_events[BOSCH_TAP_DETECTOR]= new MblMwDataSignal(BOSCH_TAP_DETECTOR, board, 
                DataInterpreter::BOSCH_TAP, FirmwareConverter::DEFAULT, 1, 1, 0, 0);
    }

    if (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).revision >= PACKED_ACC_REVISION) {
        if (!board->module_events.count(BOSCH_PACKED_ACCEL_RESPONSE_HEADER)) {
            board->module_events[BOSCH_PACKED_ACCEL_RESPONSE_HEADER]= new MblMwDataSignal(BOSCH_PACKED_ACCEL_RESPONSE_HEADER, board, 
                DataInterpreter::BOSCH_ACCELERATION, FirmwareConverter::BOSCH_ACCELERATION, 3, 2, 1, 0);
        }
        board->responses[BOSCH_PACKED_ACCEL_RESPONSE_HEADER]= response_handler_packed_data;
    }

    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_ACCELEROMETER, READ_REGISTER(ORDINAL(AccelerometerBoschRegister::DATA_CONFIG))),
        forward_as_tuple(received_config_response));
    board->responses.emplace(BOSCH_MOTION_DETECTOR, response_handler_data_no_id);
    board->responses.emplace(BOSCH_TAP_DETECTOR, response_handler_data_no_id);
    board->responses.emplace(BOSCH_ORIENTATION_DETECTOR, response_handler_data_no_id);

    AccBoschState newState = {nullptr, nullptr, 0x0};
    states.insert({board, newState});
}

void init_accelerometer_bmi160(MblMwMetaWearBoard *board) {
    AccBmi160Config* new_config = (AccBmi160Config*) malloc(sizeof(AccBmi160Config));
    memcpy(new_config, BMI160_DEFAULT_CONFIG, sizeof(BMI160_DEFAULT_CONFIG));

    if (!board->module_events.count(BMI160_STEP_DETECTOR)) {
        board->module_events[BMI160_STEP_DETECTOR] = new MblMwDataSignal(BMI160_STEP_DETECTOR, board, 
            DataInterpreter::UINT32, FirmwareConverter::DEFAULT, 1, 1, 0, 0);
    }
    board->responses[BMI160_STEP_DETECTOR]= response_handler_data_no_id;

    if (!board->module_events.count(BMI160_STEP_COUNTER)) {
        board->module_events[BMI160_STEP_COUNTER] = new MblMwDataSignal(BMI160_STEP_COUNTER, board, 
            DataInterpreter::UINT32, FirmwareConverter::DEFAULT, 1, 2, 0, 0);
    }
    board->responses[BMI160_STEP_COUNTER]= response_handler_data_no_id;

    init_accelerometer_bosch(board, new_config);
}

void init_accelerometer_bma255(MblMwMetaWearBoard *board) {
    AccBma255Config* new_config = (AccBma255Config*) malloc(sizeof(AccBma255Config));
    memcpy(new_config, BMA255_DEFAULT_CONFIG, sizeof(BMA255_DEFAULT_CONFIG));

    init_accelerometer_bosch(board, new_config);
}

void free_accelerometer_bosch(MblMwMetaWearBoard *board) {
    states.erase(board);
}

void serialize_accelerometer_bmi160_config(const MblMwMetaWearBoard* board, vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(AccBmi160Config, MBL_MW_MODULE_ACCELEROMETER);
}

void serialize_accelerometer_bma255_config(const MblMwMetaWearBoard* board, vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(AccBma255Config, MBL_MW_MODULE_ACCELEROMETER);
}

void deserialize_accelerometer_bmi160_config(MblMwMetaWearBoard* board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(AccBmi160Config, MBL_MW_MODULE_ACCELEROMETER);
}

void deserialize_accelerometer_bma255_config(MblMwMetaWearBoard* board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(AccBma255Config, MBL_MW_MODULE_ACCELEROMETER);
}

void read_accelerometer_bosch_acceleration_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed) {
    states[board].read_config_context = context;
    states[board].read_config_completed = completed;

    uint8_t command[2]= {MBL_MW_MODULE_ACCELEROMETER, READ_REGISTER(ORDINAL(AccelerometerBoschRegister::DATA_CONFIG))};
    SEND_COMMAND;
}

float bosch_get_data_scale(const MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        return FSR_SCALE.at(((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc.range);
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        return FSR_SCALE.at(((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc.range);
    default:
        return 1.f;
    }
}

MblMwDataSignal* mbl_mw_acc_bosch_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    auto implementation= board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation;
    if (implementation != MBL_MW_MODULE_ACC_TYPE_BMI160 && implementation != MBL_MW_MODULE_ACC_TYPE_BMA255) {
        return nullptr;
    }
    GET_DATA_SIGNAL(BOSCH_ACCEL_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard* board) {
    return mbl_mw_acc_bosch_get_packed_acceleration_data_signal(board);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_packed_acceleration_data_signal(const MblMwMetaWearBoard* board) {
    auto implementation= board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation;
    if (implementation != MBL_MW_MODULE_ACC_TYPE_BMI160 && implementation != MBL_MW_MODULE_ACC_TYPE_BMA255) {
        return nullptr;
    }
    GET_DATA_SIGNAL(BOSCH_PACKED_ACCEL_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_acc_bmi160_get_step_counter_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BMI160_STEP_COUNTER);
}

MblMwDataSignal* mbl_mw_acc_bmi160_get_step_detector_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BMI160_STEP_DETECTOR);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_orientation_detection_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BOSCH_ORIENTATION_DETECTOR);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_motion_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BOSCH_MOTION_DETECTOR);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_tap_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BOSCH_TAP_DETECTOR);
}

void mbl_mw_acc_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi160Odr odr) {
    auto config= (AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER);

    config->set_output_data_rate(odr);
    if (odr < MBL_MW_ACC_BMI160_ODR_12_5Hz) {
        config->acc.us= 1;
        config->acc.bwp= 0;
    } else {
        config->acc.us= 0;
        config->acc.bwp= 2;
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

    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        auto config= ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc;
        memcpy(command + 2, &config, sizeof(config));
        SEND_COMMAND;
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        auto config= ((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc;
        memcpy(command + 2, &config, sizeof(config));
        SEND_COMMAND;
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bmi160_set_step_counter_mode(MblMwMetaWearBoard* board, MblMwAccBmi160StepCounterMode mode) {
    auto config= (uint16_t*) &((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->step_counter;

    switch (mode) {
    case MBL_MW_ACC_BMI160_STEP_COUNTER_MODE_NORMAL:
        *config= 0x0315;
        break;
    case MBL_MW_ACC_BMI160_STEP_COUNTER_MODE_SENSITIVE:
        *config= 0x002d;
        break;
    case MBL_MW_ACC_BMI160_STEP_COUNTER_MODE_ROBUST:
        *config= 0x071d;
        break;
    }
}

void mbl_mw_acc_bmi160_enable_step_counter(MblMwMetaWearBoard* board) {
    ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->step_counter.step_cnt_en= 1;
}

void mbl_mw_acc_bmi160_disable_step_counter(MblMwMetaWearBoard* board) {
    ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->step_counter.step_cnt_en= 0;
}

void mbl_mw_acc_bmi160_write_step_counter_config(const MblMwMetaWearBoard* board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::STEP_DETECTOR_CONFIG)};
    auto config= ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->step_counter;

    memcpy(command + 2, &config, sizeof(config));
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_reset_step_counter(const MblMwMetaWearBoard* board) {
    uint8_t command[2]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::STEP_COUNTER_RESET)};
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_set_orientation_hysteresis(MblMwMetaWearBoard *board, float hysteresis) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->orientation.hyst = 
                min((uint8_t) 0xf, (uint8_t) (hysteresis / ORIENT_HYS_G_PER_STEP));
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        ((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->orientation.hyst = 
                min((uint8_t) 0x7, (uint8_t) (hysteresis / ORIENT_HYS_G_PER_STEP));
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_set_orientation_mode(MblMwMetaWearBoard *board, MblMwAccBoschOrientationMode mode) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->orientation.mode = (uint8_t) mode;
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        ((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->orientation.mode = (uint8_t) mode;
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_write_orientation_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::ORIENT_CONFIG)};
    
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        auto config= ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->orientation;
        memcpy(command + 2, &config, sizeof(config));
        SEND_COMMAND;
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        auto config= ((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->orientation;
        memcpy(command + 2, &config, sizeof(config));
        SEND_COMMAND;
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_set_quiet_time(MblMwMetaWearBoard *board, MblMwAccBoschTapQuietTime time) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        GET_CONFIG(AccBmi160Config)->tap.quiet = time;
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        GET_CONFIG(AccBma255Config)->tap.quiet = time;
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_set_shock_time(MblMwMetaWearBoard *board, MblMwAccBoschTapShockTime time) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        GET_CONFIG(AccBmi160Config)->tap.shock = time;
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        GET_CONFIG(AccBma255Config)->tap.shock = time;
        break;
    }
    default:
        return;
    }
}
void mbl_mw_acc_bosch_set_double_tap_window(MblMwMetaWearBoard *board, MblMwAccBoschDoubleTapWindow window) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        GET_CONFIG(AccBmi160Config)->tap.dur = window;
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        GET_CONFIG(AccBma255Config)->tap.dur = window;
        break;
    }
    default:
        return;
    }
}
void mbl_mw_acc_bosch_set_threshold(MblMwMetaWearBoard *board, float threshold) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        auto config = GET_CONFIG(AccBmi160Config);
        config->tap.th = threshold / BOSCH_TAP_THS_STEPS.at(config->acc.range);
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        auto config = GET_CONFIG(AccBma255Config);
        config->tap.th = threshold / BOSCH_TAP_THS_STEPS.at(config->acc.range);
        break;
    }
    default:
        return;
    }
}
void mbl_mw_acc_bosch_write_tap_config(const MblMwMetaWearBoard *board) {
    vector<uint8_t> command = {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::TAP_CONFIG)};
    
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        auto config= GET_CONFIG(AccBmi160Config)->tap;
        command.insert(command.end(), (uint8_t*) &config, (uint8_t*) (&config + 1));
        send_command(board, command.data(), (uint8_t) command.size());
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        auto config= GET_CONFIG(AccBma255Config)->tap;
        command.insert(command.end(), (uint8_t*) &config, (uint8_t*) (&config + 1));
        send_command(board, command.data(), (uint8_t) command.size());
        break;
    }
    default:
        return;
    }
}
void mbl_mw_acc_bosch_enable_tap_detection(const MblMwMetaWearBoard *board, uint8_t enable_single, uint8_t enable_double) {
    uint8_t mask = 0;
    if (enable_single) {
        mask|= 0x2;
    }
    if (enable_double) {
        mask|= 0x1;
    }

    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::TAP_INTERRUPT_ENABLE), mask, 0};
    SEND_COMMAND;
}
void mbl_mw_acc_bosch_disable_tap_detection(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::TAP_INTERRUPT_ENABLE), 0x00, 0x03};
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_set_any_motion_count(MblMwMetaWearBoard *board, uint8_t count) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->motion.anym_dur = (count - 1);
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        ((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->motion.slope_dur = (count - 1);
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_set_any_motion_threshold(MblMwMetaWearBoard *board, float threshold) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        auto config = (AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER);
        config->motion.anym_th = threshold / BOSCH_MOTION_THS_STEPS.at(config->acc.range);
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        auto config = (AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER);
        config->motion.slope_th = threshold / BOSCH_MOTION_THS_STEPS.at(config->acc.range);
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_write_motion_config(const MblMwMetaWearBoard *board) {
    vector<uint8_t> command = {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::MOTION_CONFIG)};
    
    states.at(board).motion_mask = 0x7;
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160: {
        auto config= ((AccBmi160Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->motion;
        command.insert(command.end(), (uint8_t*) &config, (uint8_t*) (&config + 1));
        send_command(board, command.data(), (uint8_t) command.size());
        break;
    }
    case MBL_MW_MODULE_ACC_TYPE_BMA255: {
        auto config= ((AccBma255Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->motion;
        command.insert(command.end(), (uint8_t*) &config, (uint8_t*) (&config + 1));
        send_command(board, command.data(), (uint8_t) command.size());
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_enable_motion_detection(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::MOTION_INTERRUPT_ENABLE), states.at(board).motion_mask, 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_disable_motion_detection(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::MOTION_INTERRUPT_ENABLE), 0, 0x7f};
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_enable_orientation_detection(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::ORIENT_INTERRUPT_ENABLE), 1, 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bosch_disable_orientation_detection(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::ORIENT_INTERRUPT_ENABLE), 0, 1};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_enable_step_detector(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::STEP_DETECTOR_INTERRUPT_EN), 1, 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_disable_step_detector(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::STEP_DETECTOR_INTERRUPT_EN), 0, 1};
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

void create_acc_bosch_uri(const MblMwDataSignal* signal, stringstream& uri) {
    switch(CLEAR_READ(signal->header.register_id)) {
    case ORDINAL(AccelerometerBoschRegister::DATA_INTERRUPT):
        uri << "acceleration";
        if (signal->length() <= 2) {
            uri << "[" << (int) (signal->offset >> 1) << "]";
        }
        break;
    case ORDINAL(AccelerometerBoschRegister::ORIENT_INTERRUPT):
        uri << "orientation";
        break;
    case ORDINAL(AccelerometerBoschRegister::MOTION_INTERRUPT):
        uri << "bosch-motion";
        break;
    }
}
