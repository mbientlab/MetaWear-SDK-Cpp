#pragma once

#include "metawear/core/cpp/register.h"

enum class GpioRegister : uint8_t {
    SET_DO = 1,
    CLEAR_DO,
    PULL_UP_DI,
    PULL_DOWN_DI,
    NO_PULL_DI,
    READ_AI_ABS_REF,
    READ_AI_ADC,
    READ_DI,
    PIN_CHANGE,
    PIN_CHANGE_NOTIFY,
    PIN_CHANGE_NOTIFY_ENABLE
};