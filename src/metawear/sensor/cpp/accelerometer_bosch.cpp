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

const uint8_t BMI270_DEFAULT_CONFIG[]= {
    0xa8, 0x02,
    0x00,
    0x00,
    0x88, 0x00, 0x00,
    0x05, 0xE0, 0xAA, 0x38, 0x01,
    0x05, 0xE0, 0x90, 0x30, 0x02,
    0xFA, 0x00, 0x03,
    0x2D, 0x01, 0xD4, 0x7B, 0x3B, 0x01, 0xDB, 0x7A, 0x04, 0x00, 0x3F, 0x7B, 0xCD, 0x6C, 0xC3, 0x4C, 0x04,
    0x85, 0x09, 0xC3, 0x04, 0xEC, 0xE6, 0x0C, 0x46, 0x01, 0x00, 0x27, 0x00, 0x19, 0x00, 0x96, 0x00, 0x05,
    0xA0, 0x00, 0x01, 0x00, 0x0C, 0x00, 0xF0, 0x3C, 0x00, 0x01, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x06,
    0x00, 0x0E, 0x00, 0x00, 0x07,
    0x05, 0x00, 0xEE, 0x06, 0x04, 0x00, 0xC8, 0x00, 0x08,
    0xA8, 0x05, 0xEE, 0x06, 0x00, 0x04, 0xBC, 0x02, 0xB3, 0x00, 0x85, 0x07, 0x09,
    0x00, 0x00, 0x00, 0x00, 0x00
};
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
const uint8_t BMI270_FSR_BITMASKS[4]= {0x0, 0x1, 0x2, 0x3};
const unordered_map<uint8_t, float> FSR_SCALE= {{0x3, 16384.f}, {0x5, 8192.f}, {0x8, 4096.f}, {0xc, 2048.f}},
    BMI270_FSR_SCALE= {{0x0, 16384.f}, {0x1, 8192.f}, {0x2, 4096.f}, {0x3, 2048.f}},
    BOSCH_MOTION_THS_STEPS = {{0x3, 0.00391f}, {0x5, 0.00781f}, {0x8, 0.01563f}, {0xc, 0.03125f}},
    BOSCH_TAP_THS_STEPS = {{0x3, 0.0625f}, {0x5, 0.125f}, {0x8, 0.250f}, {0xc, 0.5f}};
const ResponseHeader BOSCH_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::DATA_INTERRUPT)),
    BOSCH_MOTION_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::MOTION_INTERRUPT)),
    BOSCH_ORIENTATION_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::ORIENT_INTERRUPT)),
    BOSCH_TAP_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::TAP_INTERRUPT)),
    BMI160_STEP_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::STEP_DETECTOR_INTERRUPT)),
    BMI160_STEP_COUNTER(MBL_MW_MODULE_ACCELEROMETER, READ_REGISTER(ORDINAL(AccelerometerBoschRegister::STEP_COUNTER_DATA))),
    BOSCH_PACKED_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::PACKED_ACC_DATA)),
    BMI270_MOTION_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::MOTION_INTERRUPT)),
    BMI270_STEP_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::STEP_COUNT_INTERRUPT)),
    BMI270_STEP_COUNTER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::STEP_COUNT_INTERRUPT)),
    BMI270_WRIST_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::WRIST_INTERRUPT)),
    BMI270_ACTIVITY_DETECTOR(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::ACTIVITY_INTERRUPT)),
    BMI270_PACKED_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::PACKED_ACC_DATA));

struct AccBmi270Config {
    struct {
        uint8_t odr:4; //bit 0-4
        uint8_t bwp:3;
        uint8_t us:1;
        uint8_t range:3; //bit 0-3
        uint8_t:5; 
    } acc; //DATA_CONFIG
    struct {
        uint8_t sig_motion:1; //bit 0
        uint8_t step_counter:1;
        uint8_t activity_out:1;
        uint8_t wrist_wakeup:1; 
        uint8_t wrist_gesture:1;
        uint8_t no_motion:1;
        uint8_t any_motion:1; 
        uint8_t step_detector:1;
    } feature_enable; //FEATURE_ENABLE
    struct {
        uint8_t sig_motion:1; //bit 0
        uint8_t step_counter:1;
        uint8_t activity_out:1;
        uint8_t wrist_wakeup:1; 
        uint8_t wrist_gesture:1;
        uint8_t no_motion:1;
        uint8_t any_motion:1; 
        uint8_t step_detector:1;
    } feature_int_enable; //FEATURE_INTERRUPT_ENABLE
    struct {
        struct {
            struct {
                uint8_t map_x_axis:2; //bit 0
                uint8_t map_x_axis_sign:1;
                uint8_t map_y_axis:2; 
                uint8_t map_y_axis_sign:1;
                uint8_t map_z_axis:2; 
                uint8_t map_z_axis_sign:1;
                uint8_t:7; 
            } bitmap;
            uint8_t index = 0;
        } axis_remap; //REMAP
        struct {
            struct {
                uint8_t duration_0; //bit 0
                uint8_t duration_1:5;
                uint8_t select_x:1;
                uint8_t select_y:1; 
                uint8_t select_z:1; 
                uint8_t threshold_0; //bit 0
                uint8_t threshold_1:3;
                uint8_t :5; 
            } bitmap;
            uint8_t index = 1;
        } any_motion; //ANYMOTION
        struct {
            struct {
                uint8_t duration_0; //bit 0
                uint8_t duration_1:5;
                uint8_t select_x:1;
                uint8_t select_y:1; 
                uint8_t select_z:1; 
                uint8_t threshold_0;
                uint8_t threshold_1:3;
                uint8_t :5; 
            } bitmap;
            uint8_t index = 2;
        } no_motion; //NOMOTION
        struct {
            struct {
                uint8_t block_size0; //bit 0
                uint8_t block_size1;
            } bitmap;
            uint8_t index = 3;
        } sig_motion; //SIGMOTION    
        struct {
            struct {
                uint8_t param_10;
                uint8_t param_11;
                uint8_t param_20;
                uint8_t param_21;
                uint8_t param_30;
                uint8_t param_31;
                uint8_t param_40;
                uint8_t param_41;
                uint8_t param_50;
                uint8_t param_51;
                uint8_t param_60;
                uint8_t param_61;
                uint8_t param_70;
                uint8_t param_71;
                uint8_t param_80;
                uint8_t param_81;
            } bitmap;
            uint8_t index = 4;
        } step_counter_0; //STEPCOUNTER0   
        struct {
            struct {
                uint8_t param_90;
                uint8_t param_91;
                uint8_t param_100;
                uint8_t param_101;
                uint8_t param_110;
                uint8_t param_111;
                uint8_t param_120;
                uint8_t param_121;
                uint8_t param_130;
                uint8_t param_131;
                uint8_t param_140;
                uint8_t param_141;
                uint8_t param_150;
                uint8_t param_151;
                uint8_t param_160;
                uint8_t param_161;
            } bitmap;
            uint8_t index = 5;
        } step_counter_1; //STEPCOUNTER1   
        struct {
            struct {
                uint8_t param_170;
                uint8_t param_171;
                uint8_t param_180;
                uint8_t param_181;
                uint8_t param_190;
                uint8_t param_191;
                uint8_t param_200;
                uint8_t param_201;
                uint8_t param_210;
                uint8_t param_211;
                uint8_t param_220;
                uint8_t param_221;
                uint8_t param_230;
                uint8_t param_231;
                uint8_t param_240;
                uint8_t param_241;
            } bitmap;
            uint8_t index = 6;
        } step_counter_2; //STEPCOUNTER2   
        struct {
            struct {
                uint8_t param_250;          //bit 0-7
                uint8_t param_251;          //bit 8-15
                uint8_t watermark_level_0;  //bit 0-7
                uint8_t watermark_level_1:2;//bit 0-1
                uint8_t reset_counter:1;    //bit 2
                uint8_t:5;                  //bit 3-7
            } bitmap;
            uint8_t index = 7;
        } step_counter_3; //STEPCOUNTER3   
        struct {
            struct {
                uint8_t:4; //out_conf //bit 0
                uint8_t wearable_arm:1; //bit 4
                uint8_t:3; //enable
                uint8_t:8;
                uint8_t min_flick_peak0;
                uint8_t min_flick_peak1;
                uint8_t min_flick_samples0;
                uint8_t min_flick_samples1;
                uint8_t max_duration0;
                uint8_t max_duration1;
            } bitmap;
            uint8_t index = 8;
        } wrist_gesture; //WRISTGESTURE 
        struct {
            struct {
                uint8_t min_angle_focus0;
                uint8_t min_angle_focus1;
                uint8_t min_angle_non_focus0;
                uint8_t min_angle_non_focus1;
                uint8_t max_tilt_lr0;
                uint8_t max_tilt_lr1;
                uint8_t max_tilt_ll0;
                uint8_t max_tilt_ll1;
                uint8_t max_tilt_pd0;
                uint8_t max_tilt_pd1;
                uint8_t max_tilt_pu0;
                uint8_t max_tilt_pu1;
            } bitmap;
            uint8_t index = 9;
        } wrist_wakeup; //WRISTWAKEUP
    } feature_config; //FEATURE_CONFIG
    struct {
        uint8_t spi_en:1; //bit 0
        uint8_t i2c_wdt_sel:1;
        uint8_t i2c_wdt_en:1;
        uint8_t acc_off_en:1; 
        uint8_t:4;
        uint8_t off_acc_x:8;
        uint8_t off_acc_y:8; 
        uint8_t off_acc_z:8;
    } nv_offset; //OFFSET
    struct {
        uint8_t gyr_fifo_downs:3; //bit 0
        uint8_t gyr_fifo_filt_data:1;
        uint8_t acc_fifo_downs:3;
        uint8_t acc_fifo_filt_data:1; 
    } fifo_downs; //FIFO_DOWNS

    inline void set_output_data_rate(MblMwAccBmi270Odr odr) {
        acc.odr= odr + 1;
    }
    inline void set_range(MblMwAccBoschRange range) {
        acc.range= BMI270_FSR_BITMASKS[range];
    }
};

struct AccBmi160Config {
    struct {
        uint8_t odr:4;      //bit 0-3 ACC_CONF
        uint8_t bwp:3;      //bit 4-6
        uint8_t us:1;       //bit 7
        uint8_t range:4;    //bit 0-3 ACC_RANGE
        uint8_t:4;          //bit 4-7
    } acc; //DATA_CONFIG
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
    MblMwFnBoardPtrInt read_step_counter_handler;
    void *read_step_counter_context;
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270: {
        auto config= &((AccBmi270Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc;
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

#include <iostream>

static int32_t received_step_counter_response(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t step_count = 0;
    if (len == 0x04) {
        uint16_t first = response[len-2];
        uint16_t second = response[len-1] << 8;
        uint16_t tempy = second | first;
        step_count = tempy;
    }
    
    auto step_callback = states[board].read_step_counter_handler;
    auto step_context = states[board].read_step_counter_context;
    states[board].read_step_counter_handler = nullptr;
    states[board].read_step_counter_context = nullptr;
    step_callback(step_context, board, step_count);
    
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

    AccBoschState newState = {nullptr, nullptr, 0x0, nullptr, nullptr};
    states.insert({board, newState});
}

void init_accelerometer_bmi270(MblMwMetaWearBoard *board) {
    AccBmi270Config* config = (AccBmi270Config*) malloc(sizeof(AccBmi270Config));
    memcpy(config, BMI270_DEFAULT_CONFIG, sizeof(BMI270_DEFAULT_CONFIG));

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

    if (!board->module_events.count(BMI270_MOTION_DETECTOR)) {
        board->module_events[BMI270_MOTION_DETECTOR]= new MblMwDataSignal(BMI270_MOTION_DETECTOR, board, 
                DataInterpreter::UINT32, FirmwareConverter::DEFAULT, 1, 1, 0, 0);
    }
    board->responses.emplace(BMI270_MOTION_DETECTOR, response_handler_data_no_id);

    if (!board->module_events.count(BMI270_STEP_DETECTOR)) {
        board->module_events[BMI270_STEP_DETECTOR] = new MblMwDataSignal(BMI270_STEP_DETECTOR, board, 
            DataInterpreter::UINT32, FirmwareConverter::DEFAULT, 1, 1, 0, 0);
    }
    board->responses[BMI270_STEP_DETECTOR]= response_handler_data_no_id;

    if (!board->module_events.count(BMI270_STEP_COUNTER)) {
        board->module_events[BMI270_STEP_COUNTER] = new MblMwDataSignal(BMI270_STEP_COUNTER, board, 
            DataInterpreter::UINT32, FirmwareConverter::DEFAULT, 1, 2, 0, 0);
    }
    board->responses[BMI270_STEP_COUNTER]= response_handler_data_no_id;

    if (!board->module_events.count(BMI270_WRIST_DETECTOR)) {
        board->module_events[BMI270_WRIST_DETECTOR] = new MblMwDataSignal(BMI270_WRIST_DETECTOR, board, 
            DataInterpreter::BMI270_GESTURE, FirmwareConverter::DEFAULT, 1, 1, 0, 0);
    }
    board->responses[BMI270_WRIST_DETECTOR]= response_handler_data_no_id;

    if (!board->module_events.count(BMI270_ACTIVITY_DETECTOR)) {
        board->module_events[BMI270_ACTIVITY_DETECTOR] = new MblMwDataSignal(BMI270_ACTIVITY_DETECTOR, board, 
            DataInterpreter::BMI270_ACTIVITY, FirmwareConverter::DEFAULT, 1, 1, 0, 0);
    }
    board->responses[BMI270_ACTIVITY_DETECTOR]= response_handler_data_no_id;

    if (!board->module_events.count(BMI270_PACKED_ACCEL_RESPONSE_HEADER)) {
        board->module_events[BMI270_PACKED_ACCEL_RESPONSE_HEADER]= new MblMwDataSignal(BMI270_PACKED_ACCEL_RESPONSE_HEADER, board, 
            DataInterpreter::BOSCH_ACCELERATION, FirmwareConverter::BOSCH_ACCELERATION, 3, 2, 1, 0);
    }
    board->responses[BMI270_PACKED_ACCEL_RESPONSE_HEADER]= response_handler_packed_data;

    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_ACCELEROMETER, READ_REGISTER(ORDINAL(AccelerometerBoschBmi270Register::DATA_CONFIG))),
        forward_as_tuple(received_config_response));
    
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_ACCELEROMETER, READ_REGISTER(ORDINAL(AccelerometerBoschBmi270Register::STEP_COUNT_INTERRUPT))),
        forward_as_tuple(received_step_counter_response));
    
    AccBoschState newState = {nullptr, nullptr, 0x0, nullptr, nullptr};
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

void serialize_accelerometer_bmi270_config(const MblMwMetaWearBoard* board, vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(AccBmi270Config, MBL_MW_MODULE_ACCELEROMETER);
}

void serialize_accelerometer_bmi160_config(const MblMwMetaWearBoard* board, vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(AccBmi160Config, MBL_MW_MODULE_ACCELEROMETER);
}

void serialize_accelerometer_bma255_config(const MblMwMetaWearBoard* board, vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(AccBma255Config, MBL_MW_MODULE_ACCELEROMETER);
}

void deserialize_accelerometer_bmi270_config(MblMwMetaWearBoard* board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(AccBmi270Config, MBL_MW_MODULE_ACCELEROMETER);
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        return BMI270_FSR_SCALE.at(((AccBmi270Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc.range);
    default:
        return 1.f;
    }
}

MblMwDataSignal* mbl_mw_acc_bosch_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    auto implementation= board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation;
    if (implementation != MBL_MW_MODULE_ACC_TYPE_BMI160 && implementation != MBL_MW_MODULE_ACC_TYPE_BMA255 && implementation != MBL_MW_MODULE_ACC_TYPE_BMI270) {
        return nullptr;
    }
    GET_DATA_SIGNAL(BOSCH_ACCEL_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard* board) {
    return mbl_mw_acc_bosch_get_packed_acceleration_data_signal(board);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_packed_acceleration_data_signal(const MblMwMetaWearBoard* board) {
    auto implementation= board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation;
    if (implementation == MBL_MW_MODULE_ACC_TYPE_BMI160) {
        GET_DATA_SIGNAL(BOSCH_PACKED_ACCEL_RESPONSE_HEADER);
    } else if (implementation == MBL_MW_MODULE_ACC_TYPE_BMA255) {
        GET_DATA_SIGNAL(BOSCH_PACKED_ACCEL_RESPONSE_HEADER);
    } else if (implementation == MBL_MW_MODULE_ACC_TYPE_BMI270) {
	GET_DATA_SIGNAL(BMI270_PACKED_ACCEL_RESPONSE_HEADER);
    } else {
	return nullptr;
    }
    
}

MblMwDataSignal* mbl_mw_acc_bmi160_get_step_counter_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BMI160_STEP_COUNTER);
}

MblMwDataSignal* mbl_mw_acc_bmi160_get_step_detector_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BMI160_STEP_DETECTOR);
}

MblMwDataSignal* mbl_mw_acc_bmi270_get_step_counter_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BMI270_STEP_COUNTER);
}

MblMwDataSignal* mbl_mw_acc_bmi270_get_step_detector_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BMI270_STEP_DETECTOR);
}

MblMwDataSignal* mbl_mw_acc_bmi270_get_wrist_detector_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BMI270_WRIST_DETECTOR);
}

MblMwDataSignal* mbl_mw_acc_bmi270_get_activity_detector_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BMI270_ACTIVITY_DETECTOR);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_orientation_detection_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BOSCH_ORIENTATION_DETECTOR);
}

MblMwDataSignal* mbl_mw_acc_bosch_get_motion_data_signal(const MblMwMetaWearBoard* board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI270: 
        GET_DATA_SIGNAL(BMI270_MOTION_DETECTOR);
        break;
    default:
        GET_DATA_SIGNAL(BOSCH_MOTION_DETECTOR);
    }
}

MblMwDataSignal* mbl_mw_acc_bosch_get_tap_data_signal(const MblMwMetaWearBoard* board) {
    GET_DATA_SIGNAL(BOSCH_TAP_DETECTOR);
}

void mbl_mw_acc_bmi270_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi270Odr odr) {
    auto config= (AccBmi270Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER);

    config->set_output_data_rate(odr);
    config->acc.us= 1;
    config->acc.bwp= 2;
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        ((AccBmi270Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->set_range(range);
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270: {
        auto config= ((AccBmi270Config*) board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->acc;
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

void mbl_mw_acc_bmi160_enable_step_detector(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::STEP_DETECTOR_INTERRUPT_EN), 1, 0};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi160_disable_step_detector(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::STEP_DETECTOR_INTERRUPT_EN), 0, 1};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi270_set_step_counter_trigger(MblMwMetaWearBoard* board, uint16_t trigger) {
    if (trigger >= 1 && trigger <= 1023) {
	auto lower_8 = trigger & 0x00ff;
        auto higher_2 = (trigger & 0x0300) >> 8;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.bitmap.watermark_level_0 = lower_8;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.bitmap.watermark_level_1 = higher_2;
    } else {
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.bitmap.watermark_level_0 = 0;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.bitmap.watermark_level_1 = 0;
    }
}

void mbl_mw_acc_bmi270_enable_step_counter(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x02, 0x00};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x02, 0x00};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_disable_step_counter(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x00, 0x02};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x00, 0x02};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_write_step_counter_config(const MblMwMetaWearBoard* board) {
    uint8_t command[7]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_CONFIG), ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.index};
    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.bitmap;
    memcpy(command + 3, &config, sizeof(config));
    SEND_COMMAND;
}

void mbl_mw_acc_bmi270_reset_step_counter(const MblMwMetaWearBoard* board) {
    uint8_t command[7]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_CONFIG), ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.index};
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.bitmap.reset_counter=1;
    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.bitmap;
    memcpy(command + 3, &config, sizeof(config));
    SEND_COMMAND;
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.step_counter_3.bitmap.reset_counter=0;
}

void mbl_mw_acc_bmi270_read_step_counter(MblMwMetaWearBoard* board, void* context, MblMwFnBoardPtrInt handler) {
    states[board].read_step_counter_handler = handler;
    states[board].read_step_counter_context = context;
        
    uint8_t command[2]= {MBL_MW_MODULE_ACCELEROMETER, READ_REGISTER(ORDINAL(AccelerometerBoschBmi270Register::STEP_COUNT_INTERRUPT))};
    SEND_COMMAND;
}

void mbl_mw_acc_bmi270_enable_step_detector(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x80, 0x00};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x80, 0x00};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_disable_step_detector(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x00, 0x80};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x00, 0x80};
    send_command(board, command2, sizeof(command2));
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270: {
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.any_motion.bitmap.duration_0 = count;
        // HARDCODED TO 0
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.any_motion.bitmap.duration_1 = 0;
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270: {
        auto lower_8 = (uint16_t)threshold & 0x00ff;
        auto higher_3 = ((uint16_t)threshold & 0x0700) >> 8;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.any_motion.bitmap.threshold_0 = lower_8;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.any_motion.bitmap.threshold_1 = higher_3;
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_set_sig_motion_blocksize(MblMwMetaWearBoard *board, uint16_t blocksize) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI270: {
        auto lower_8 = blocksize & 0x00ff;
        auto higher_8 = (blocksize & 0xff00) >> 8;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.sig_motion.bitmap.block_size0 = lower_8;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.sig_motion.bitmap.block_size1 = higher_8;
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_set_no_motion_count(MblMwMetaWearBoard *board, uint8_t count) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI270: {
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.no_motion.bitmap.duration_0 = count;
        // HARDCODED TO 0
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.no_motion.bitmap.duration_1 = 0;
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_set_no_motion_threshold(MblMwMetaWearBoard *board, float threshold) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI270: {
        auto lower_8 = (uint16_t)threshold & 0x00ff;
        auto higher_3 = ((uint16_t)threshold & 0x0700) >> 8;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.no_motion.bitmap.threshold_0 = lower_8;
        ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.no_motion.bitmap.threshold_1 = higher_3;
        break;
    }
    default:
        return;
    }
}

void mbl_mw_acc_bosch_write_motion_config(const MblMwMetaWearBoard *board, MblMwAccBoschMotion type) {
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
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            switch(type) {
                case MBL_MW_ACC_BOSCH_MOTION_SIGMOTION: {
                    uint8_t command[5]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_CONFIG), ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.sig_motion.index};
                    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.sig_motion.bitmap;
                    memcpy(command + 3, &config, sizeof(config));
                    SEND_COMMAND;
                    break;
                }
                case MBL_MW_ACC_BOSCH_MOTION_NOMOTION: {
                    uint8_t command[7]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_CONFIG), ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.no_motion.index};
                    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.no_motion.bitmap;
                    memcpy(command + 3, &config, sizeof(config));
                    SEND_COMMAND;
                    break;
                }
                case MBL_MW_ACC_BOSCH_MOTION_ANYMOTION: {
                    uint8_t command[7]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_CONFIG), ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.any_motion.index};
                    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.any_motion.bitmap;
                    memcpy(command + 3, &config, sizeof(config));
                    SEND_COMMAND;
                    break;
                }
            }
            break;
        }
        default:
            return;
    }
}

void mbl_mw_acc_bosch_enable_motion_detection(const MblMwMetaWearBoard *board, MblMwAccBoschMotion type) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI160:
        case MBL_MW_MODULE_ACC_TYPE_BMA255: {
            uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::MOTION_INTERRUPT_ENABLE), states.at(board).motion_mask, 0};
            SEND_COMMAND;
            break;
        }
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            switch(type) {
                case MBL_MW_ACC_BOSCH_MOTION_SIGMOTION: {
                    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x01, 0x00};
                    send_command(board, command1, sizeof(command1));
                    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x01, 0x00};
                    send_command(board, command2, sizeof(command2));
                    break;
                }
                case MBL_MW_ACC_BOSCH_MOTION_NOMOTION: {
                    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x20, 0x00};
                    send_command(board, command1, sizeof(command1));
                    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x20, 0x00};
                    send_command(board, command2, sizeof(command2));
                    break;
                }
                case MBL_MW_ACC_BOSCH_MOTION_ANYMOTION: {
                    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x40, 0x00};
                    send_command(board, command1, sizeof(command1));
                    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x40, 0x00};
                    send_command(board, command2, sizeof(command2));
                    break;
                }
            }
            break;
        }
    }
}

void mbl_mw_acc_bosch_disable_motion_detection(const MblMwMetaWearBoard *board, MblMwAccBoschMotion type) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI160:
        case MBL_MW_MODULE_ACC_TYPE_BMA255: {
            uint8_t command[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschRegister::MOTION_INTERRUPT_ENABLE), 0, 0x7f};
            SEND_COMMAND;
            break;
            }
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            switch(type) {
                case MBL_MW_ACC_BOSCH_MOTION_SIGMOTION: {
                    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x00, 0x01};
                    send_command(board, command1, sizeof(command1));
                    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x00, 0x01};
                    send_command(board, command2, sizeof(command2));
                    break;
                }
                case MBL_MW_ACC_BOSCH_MOTION_NOMOTION: {
                    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x00, 0x20};
                    send_command(board, command1, sizeof(command1));
                    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x00, 0x20};
                    send_command(board, command2, sizeof(command2));
                    break;
                }
                case MBL_MW_ACC_BOSCH_MOTION_ANYMOTION: {
                    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x00, 0x40};
                    send_command(board, command1, sizeof(command1));
                    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x00, 0x40};
                    send_command(board, command2, sizeof(command2));
                    break;
                }
            }
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_gesture_armside(const MblMwMetaWearBoard *board, uint8_t side) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            if (side == 1) {
                ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap.wearable_arm = 1;
            } else if (side == 0) {
                ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap.wearable_arm = 0;
            }
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_gesture_peak(const MblMwMetaWearBoard *board, uint16_t peak) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = peak & 0x00ff;
            auto higher_8 = (peak & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap.min_flick_peak0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap.min_flick_peak1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_gesture_samples(const MblMwMetaWearBoard *board, uint16_t samples) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = samples & 0x00ff;
            auto higher_8 = (samples & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap.min_flick_samples0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap.min_flick_samples1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_gesture_duration(const MblMwMetaWearBoard *board, uint16_t side) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = side & 0x00ff;
            auto higher_8 = (side & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap.max_duration0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap.max_duration1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_write_wrist_gesture_config(const MblMwMetaWearBoard* board) {
    uint8_t command[11]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_CONFIG), ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.index};
    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_gesture.bitmap;
    memcpy(command + 3, &config, sizeof(config));
    SEND_COMMAND;
}

void mbl_mw_acc_bmi270_enable_wrist_gesture(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x10, 0x00};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x10, 0x00};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_disable_wrist_gesture(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x00, 0x10};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x00, 0x10};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_wrist_wakeup_angle_focus(const MblMwMetaWearBoard *board, uint16_t angle) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = angle & 0x00ff;
            auto higher_8 = (angle & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.min_angle_focus0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.min_angle_focus1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_wakeup_angle_nonfocus(const MblMwMetaWearBoard *board, uint16_t angle) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = angle & 0x00ff;
            auto higher_8 = (angle & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.min_angle_non_focus0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.min_angle_non_focus1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_wakeup_tilt_lr(const MblMwMetaWearBoard *board, uint16_t angle) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = angle & 0x00ff;
            auto higher_8 = (angle & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.max_tilt_lr0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.max_tilt_lr1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_wakeup_tilt_ll(const MblMwMetaWearBoard *board, uint16_t angle) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = angle & 0x00ff;
            auto higher_8 = (angle & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.max_tilt_ll0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.max_tilt_ll1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_wakeup_tilt_pd(const MblMwMetaWearBoard *board, uint16_t angle) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = angle & 0x00ff;
            auto higher_8 = (angle & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.max_tilt_pd0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.max_tilt_pd1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_wrist_wakeup_tilt_pu(const MblMwMetaWearBoard *board, uint16_t angle) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
        case MBL_MW_MODULE_ACC_TYPE_BMI270: {
            auto lower_8 = angle & 0x00ff;
            auto higher_8 = (angle & 0xff00) >> 8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.max_tilt_pu0 = lower_8;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap.max_tilt_pu1 = higher_8;
            break;
        }
    }
}

void mbl_mw_acc_bmi270_write_wrist_wakeup_config(const MblMwMetaWearBoard* board) {
    uint8_t command[15]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_CONFIG), ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.index};
    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.wrist_wakeup.bitmap;
    memcpy(command + 3, &config, sizeof(config));
    SEND_COMMAND;
}

void mbl_mw_acc_bmi270_enable_wrist_wakeup(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x08, 0x00};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x08, 0x00};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_disable_wrist_wakeup(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x00, 0x08};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x00, 0x08};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_enable_activity_detection(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x04, 0x00};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x04, 0x00};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_disable_activity_detection(const MblMwMetaWearBoard *board) {
    uint8_t command1[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_INTERRUPT_ENABLE), 0x00, 0x04};
    send_command(board, command1, sizeof(command1));
    uint8_t command2[4]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_ENABLE), 0x00, 0x04};
    send_command(board, command2, sizeof(command2));
}

void mbl_mw_acc_bmi270_axis_remap(const MblMwMetaWearBoard* board, MblMwAccBoschAxisXyzRemap map, MblMwAccBoschAxisXyzSign sign) {
    switch(map) {
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_XYZ://[0,1,2]
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis = 2;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_YZX:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis = 2;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis = 0;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_ZXY:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis = 2;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis = 1;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_XZY:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis = 2;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis = 1;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_YXZ:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis = 2;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_REMAP_ZYX:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis = 2;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis = 0;
            break;
    }
    switch(sign) {
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_000:    
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis_sign = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis_sign = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis_sign = 0;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_100:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis_sign = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis_sign = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis_sign = 0;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_110:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis_sign = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis_sign = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis_sign = 0;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_101:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis_sign = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis_sign = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis_sign = 1;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_010:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis_sign = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis_sign = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis_sign = 0;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_011:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis_sign = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis_sign = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis_sign = 1;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_001:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis_sign = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis_sign = 0;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis_sign = 1;
            break;
        case MBL_MW_ACC_BOSCH_AXIS_XYZ_SIGN_111:
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_x_axis_sign = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_y_axis_sign = 1;
            ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap.map_z_axis_sign = 1;
            break;
    }
    uint8_t command[5]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::FEATURE_CONFIG), ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.index};
    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->feature_config.axis_remap.bitmap;
    memcpy(command + 3, &config, sizeof(config));
    SEND_COMMAND;
}

void mbl_mw_acc_bmi270_acc_offsets(const MblMwMetaWearBoard* board, uint16_t x_offset, uint16_t y_offset, uint16_t z_offset) {
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->nv_offset.acc_off_en = 1;

    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->nv_offset.off_acc_x = x_offset;
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->nv_offset.off_acc_y = y_offset;
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->nv_offset.off_acc_z = z_offset;

    uint8_t command[6]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::OFFSET)};
    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->nv_offset;
    memcpy(command + 2, &config, sizeof(config));
    SEND_COMMAND;
}

void mbl_mw_acc_bmi270_fifo_downs(const MblMwMetaWearBoard* board, uint8_t gyro_downs, uint8_t gyro_data, uint8_t acc_downs, uint8_t acc_data) {
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->fifo_downs.gyr_fifo_downs = gyro_downs;
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->fifo_downs.gyr_fifo_filt_data = gyro_data;
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->fifo_downs.acc_fifo_downs = acc_downs;
    ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->fifo_downs.acc_fifo_filt_data = acc_data;

    uint8_t command[3]= {MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBoschBmi270Register::DOWNSAMPLING)};
    auto config= ((AccBmi270Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->fifo_downs;
    memcpy(command + 2, &config, sizeof(config));
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

void create_acc_bmi270_uri(const MblMwDataSignal* signal, stringstream& uri) {
    switch(CLEAR_READ(signal->header.register_id)) {
    case ORDINAL(AccelerometerBoschBmi270Register::DATA_INTERRUPT):
        uri << "acceleration";
        if (signal->length() <= 2) {
            uri << "[" << (int) (signal->offset >> 1) << "]";
        }
        break;
    case ORDINAL(AccelerometerBoschBmi270Register::MOTION_INTERRUPT):
        uri << "bosch-motion";
        break;
    }
}
