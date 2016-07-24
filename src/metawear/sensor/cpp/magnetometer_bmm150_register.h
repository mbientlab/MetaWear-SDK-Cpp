#pragma once

enum class MagnetometerBmm150Register : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    DATA_RATE,
    DATA_REPETITIONS,
    MAG_DATA,
    PACKED_MAG_DATA= 0x09
};
