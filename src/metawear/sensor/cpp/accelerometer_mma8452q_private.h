#pragma once

#include <stdint.h>

#include "metawear/sensor/accelerometer_mma8452q.h"

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
    uint16_t odr : 3;
    uint16_t accel_fsr : 2;
};

void init_accelerometer_mma8452q(MblMwMetaWearBoard *board);