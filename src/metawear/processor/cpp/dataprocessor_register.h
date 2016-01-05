#pragma once

#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

enum class DataProcessorRegister : uint8_t {
    ADD= 2,
    NOTIFY,
    STATE,
    PARAMETER,
    REMOVE,
    NOTIFY_ENABLE,
    REMOVE_ALL
};

const ResponseHeader DATAPROCESSOR_RESPONSE_HEADER(MBL_MW_MODULE_DATA_PROCESSOR, ORDINAL(DataProcessorRegister::NOTIFY));