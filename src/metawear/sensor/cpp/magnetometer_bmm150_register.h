#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class MagnetometerBmm150Register : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    DATA_RATE,
    DATA_REPETITIONS,
    MAG_DATA
};

const ResponseHeader BMM150_MAG_DATA_RESPONSE_HEADER(MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::MAG_DATA));
