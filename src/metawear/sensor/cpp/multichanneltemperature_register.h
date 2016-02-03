#pragma once

#include "metawear/core/cpp/register.h"

enum class MultiChannelTempRegister : uint8_t {
    TEMPERATURE = 1,
    MODE
};

const ResponseHeader TEMPERATURE_RESPONSE_HEADER(MBL_MW_MODULE_TEMPERATURE, READ_REGISTER(ORDINAL(MultiChannelTempRegister::TEMPERATURE)));
