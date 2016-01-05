#pragma once

#include "register.h"

enum class TimerRegister : uint8_t {
    ENABLE = 1,
    TIMER_ENTRY,
    START,
    STOP,
    REMOVE,
    NOTIFY,
    NOTIFY_ENABLE
};