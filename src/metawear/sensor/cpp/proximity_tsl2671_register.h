#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class ProximityTsl2671Register : uint8_t {
    PROXIMITY= 1,
    MODE
};

const ResponseHeader PROXIMITY_TSL2671_ADC_RESPONSE_HEADER(MBL_MW_MODULE_PROXIMITY, READ_REGISTER(ORDINAL(ProximityTsl2671Register::PROXIMITY)));
