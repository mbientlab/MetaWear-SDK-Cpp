#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class BarometerBmp280Register : uint8_t {
    PRESSURE = 1,
    ALTITUDE,
    CONFIG,
    CYCLIC
};

const ResponseHeader BARO_PRESSURE_RESPONSE_HEADER(MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::PRESSURE));
const ResponseHeader BARO_ALTITUDE_RESPONSE_HEADER(MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::ALTITUDE));