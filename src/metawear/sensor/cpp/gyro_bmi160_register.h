#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class GyroBmi160Register : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    CONFIG,
    DATA = 5
};

const ResponseHeader GYRO_ROT_RESPONSE_HEADER(MBL_MW_MODULE_GYRO, ORDINAL(GyroBmi160Register::DATA));