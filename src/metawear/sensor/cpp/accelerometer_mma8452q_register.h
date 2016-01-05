#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class AccelerometerMma8452qRegister : uint8_t {
    GLOBAL_ENABLE = 1,
    DATA_ENABLE,
    DATA_CONFIG,
    DATA_VALUE,
    MOVEMENT_ENABLE,
    MOVEMENT_CONFIG,
    MOVEMENT_VALUE,
    ORIENTATION_ENABLE,
    ORIENTATION_CONFIG,
    ORIENTATION_VALUE,
    PULSE_ENABLE,
    PULSE_CONFIG,
    PULSE_STATUS,
    SHAKE_ENABLE,
    SHAKE_CONFIG,
    SHAKE_STATUS
};

const ResponseHeader MMA8452Q_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerMma8452qRegister::DATA_VALUE));