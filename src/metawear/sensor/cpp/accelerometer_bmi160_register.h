#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class AccelerometerBmi160Register : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    DATA_CONFIG,
    DATA_INTERRUPT,
    DATA_INTERRUPT_CONFIG
};

const ResponseHeader BMI160_ACCEL_RESPONSE_HEADER(MBL_MW_MODULE_ACCELEROMETER, ORDINAL(AccelerometerBmi160Register::DATA_INTERRUPT));