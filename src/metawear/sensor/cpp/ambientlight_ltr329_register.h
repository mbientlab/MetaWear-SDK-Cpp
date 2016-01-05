#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class AmbientLightLtr329Register : uint8_t {
    ENABLE = 1,
    CONFIG,
    OUTPUT,
};

const ResponseHeader LTR329_ILLUMINANCE_RESPONSE_HEADER(MBL_MW_MODULE_AMBIENT_LIGHT, ORDINAL(AmbientLightLtr329Register::OUTPUT));