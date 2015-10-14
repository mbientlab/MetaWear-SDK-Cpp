#pragma once

#include <stdint.h>
#include <unordered_map>
#include <vector>

#include "responseheader.h"

#include "metawear/data.h"
#include "metawear/metawearboard.h"
#include "metawear/accelerometer_mma8452q.h"
#include "metawear/accelerometer_bmi160.h"
#include "metawear/ambientlight_ltr329.h"
#include "metawear/barometer_bmp280.h"
#include "metawear/gyro_bmi160.h"

struct Mma8452qConfig {
    float tap_threshold;
    uint32_t tap_latency, tap_window, tap_duration;

    float shake_threshold;
    uint32_t shake_duration;

    float movement_threshold;
    uint32_t movement_duration;

    uint32_t orientation_delay;

    uint32_t active_timeout;

//    uint16_t shake_axis:2;
    uint16_t movement_axis_x:1;
    uint16_t movement_axis_y:1;
    uint16_t movement_axis_z:1;
//    uint16_t movement_type:1;
//    uint16_t tap_axis:2;
    uint16_t tap_type_single:1;
    uint16_t tap_type_double:1;
    uint16_t odr:3;
    uint16_t accel_fsr:2;

    Mma8452qConfig();
};

struct Ltr329Config {
    uint8_t :2;
    uint8_t als_gain:3;
    uint8_t :3;
    uint8_t als_measurement_rate:3;
    uint8_t als_integration_time:3;
    uint8_t :2;

    Ltr329Config();
};

struct Bmp280Config {
    uint8_t :2;
    uint8_t pressure_oversampling:3;
    uint8_t :3;
    uint8_t :2;
    uint8_t iir_filter:3;
    uint8_t standby_time: 3;

    Bmp280Config();
};

struct GyroBmi160Config {
    uint8_t gyr_odr:4;
    uint8_t gyr_bwp:2;
    uint8_t :2;
    uint8_t gyr_range:3;
    uint8_t :5;

    GyroBmi160Config();
    float get_scale() const;
};

struct AccBmi160Config {
    uint8_t acc_odr:4;
    uint8_t acc_bwp:3;
    uint8_t acc_us:1;
    uint8_t acc_range:4;
    uint8_t :4;

    AccBmi160Config();

    void set_output_data_rate(MblMwAccBmi160Odr odr);
    void set_range(MblMwAccBmi160Range range);
    float get_scale() const;
};

struct MblMwDataSignal {
    ResponseHeader header;

    uint8_t data_type_id;
    uint8_t is_signed;
    uint8_t length;
    uint8_t offset;

    MblMwMetaWearBoard* const owner;

    MblMwDataSignal(const ResponseHeader& header, uint8_t data_type_id, 
            uint8_t is_signed, uint8_t length, uint8_t offset, MblMwMetaWearBoard *owner);
};

typedef MblMwData* (*ResponseHandler)(const MblMwMetaWearBoard *board, const uint8_t*, uint8_t);
struct MblMwMetaWearBoard {
    Mma8452qConfig mma8452q_config;
    Ltr329Config ltr329_config;
    Bmp280Config bmp280_config;
    GyroBmi160Config gyro_bmi160_config;
    AccBmi160Config acc_bmi160_config;

    std::vector<MblMwDataSignal> sensor_data_signals;
    std::unordered_map<ResponseHeader, ResponseHandler> response_processors;
    std::unordered_map<ResponseHeader, MblMwDataSignal*> active_data_signals;

    MblMwMetaWearBoard();
};
