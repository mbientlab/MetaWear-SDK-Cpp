#pragma once

#include "responseheader.h"

enum class LoggingRegister : uint8_t {
    ENABLE = 1,
    TRIGGER,
    REMOVE,
    TIME,
    LENGTH,
    READOUT,
    READOUT_NOTIFY,
    READOUT_PROGRESS,
    REMOVE_ENTRIES,
    REMOVE_ALL,
    CIRCULAR_BUFFER,
    READOUT_PAGE_COMPLETED = 0xd,
    READOUT_PAGE_CONFIRM,
    PAGE_FLUSH = 0x10,
};