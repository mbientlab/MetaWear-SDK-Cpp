#pragma once

enum class BarometerBmp280Register : uint8_t {
    PRESSURE = 1,
    ALTITUDE,
    CONFIG,
    CYCLIC
};
