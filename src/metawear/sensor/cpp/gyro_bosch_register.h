#pragma once

enum class GyroBmi160Register : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    CONFIG,
    DATA = 5,
    PACKED_GYRO_DATA = 0x7
};

enum class GyroBmi270Register : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    CONFIG,
    DATA = 4,
    PACKED_GYRO_DATA = 0x5,
    OFFSET
};