#pragma once

enum class GyroBmi160Register : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    CONFIG,
    DATA = 5,
    PACKED_DATA= 0x7
};
