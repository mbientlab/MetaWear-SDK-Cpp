#pragma once

#include <stdint.h>

struct GyroBmi160Config {
    uint8_t gyr_odr : 4;
    uint8_t gyr_bwp : 2;
    uint8_t:2;
    uint8_t gyr_range : 3;
    uint8_t:5;

    float get_scale() const;
};