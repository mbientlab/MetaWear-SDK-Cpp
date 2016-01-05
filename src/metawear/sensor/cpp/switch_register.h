#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class SwitchRegister : uint8_t {
    STATE = 1
};

const ResponseHeader SWITCH_RESPONSE_HEADER(MBL_MW_MODULE_SWITCH, ORDINAL(SwitchRegister::STATE));