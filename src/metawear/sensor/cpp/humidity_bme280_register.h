#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class HumidityBme280Register : uint8_t {
    HUMIDITY= 1,
    MODE
};

const ResponseHeader HUMIDITY_BME280_ADC_RESPONSE_HEADER(MBL_MW_MODULE_HUMIDITY, READ_REGISTER(ORDINAL(HumidityBme280Register::HUMIDITY)));
