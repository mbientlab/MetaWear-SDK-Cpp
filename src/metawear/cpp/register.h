#pragma once

#include <stdint.h>

#define ORDINAL(x) static_cast<uint8_t>(x)

const uint8_t INFO_REGISTER= 0x0;

enum class Module : uint8_t {
    SWITCH= 1, LED, ACCELEROMETER, TEMPERATURE, GPIO, NEO_PIXEL, IBEACON, HAPTIC, DATA_PROCESSOR, EVENT, LOGGING, TIMER, I2C, 
    MACRO= 0xf, GSR, SETTINGS, BAROMETER, GYRO, AMBIENT_LIGHT,
    DEBUG= 0xfe
};

enum class AccelerometerBmi160Register : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    DATA_CONFIG,
    DATA_INTERRUPT,
    DATA_INTERRUPT_CONFIG
};

enum class AccelerometerMma8452qRegister : uint8_t {
    GLOBAL_ENABLE= 1,
    DATA_ENABLE,
    DATA_CONFIG,
    DATA_VALUE,
    MOVEMENT_ENABLE,
    MOVEMENT_CONFIG,
    MOVEMENT_VALUE,
    ORIENTATION_ENABLE,
    ORIENTATION_CONFIG,
    ORIENTATION_VALUE,
    PULSE_ENABLE,
    PULSE_CONFIG,
    PULSE_STATUS,
    SHAKE_ENABLE,
    SHAKE_CONFIG,
    SHAKE_STATUS
};

enum class AmbientLightLtr329Register : uint8_t {
    ENABLE= 1,
    CONFIG,
    OUTPUT,
};

enum class BarometerBmp280Register : uint8_t {
    PRESSURE= 1,
    ALTITUDE,
    CONFIG,
    CYCLIC
};

enum class GyroBmi160Register : uint8_t {
    POWER_MODE= 1,
    DATA_INTERRUPT_ENABLE,
    CONFIG,
    DATA= 5
};

enum class SwitchRegister : uint8_t {
    STATE= 1
};
