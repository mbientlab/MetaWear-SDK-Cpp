#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class I2cRegister : uint8_t {
    READ_WRITE= 1
};

const ResponseHeader I2C_READ_RESPONSE_HEADER(MBL_MW_MODULE_I2C, READ_REGISTER(ORDINAL(I2cRegister::READ_WRITE)));
