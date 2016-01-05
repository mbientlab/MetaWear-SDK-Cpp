#pragma once

#include <stdint.h>

#include "metawear/sensor/accelerometer_bmi160.h"

struct AccBmi160Config {
    uint8_t acc_odr:4;
    uint8_t acc_bwp:3;
    uint8_t acc_us:1;
    uint8_t acc_range:4;
    uint8_t:4;

    void set_output_data_rate(MblMwAccBmi160Odr odr);
    void set_range(MblMwAccBmi160Range range);
    float get_scale() const;
};

void init_accelerometer_bmi160(MblMwMetaWearBoard *board);