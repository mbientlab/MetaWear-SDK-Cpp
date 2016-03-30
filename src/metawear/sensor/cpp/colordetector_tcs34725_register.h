#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class ColorDetectorTcs34725Register : uint8_t {
    RGB_COLOR= 1,
    MODE
};

const ResponseHeader CD_TCS34725_ADC_RESPONSE_HEADER(MBL_MW_MODULE_COLOR_DETECTOR, READ_REGISTER(ORDINAL(ColorDetectorTcs34725Register::RGB_COLOR)));
