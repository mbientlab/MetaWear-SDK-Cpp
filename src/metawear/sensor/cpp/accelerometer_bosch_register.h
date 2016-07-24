#pragma once

enum class AccelerometerBoschRegister : uint8_t {
    POWER_MODE = 1,
    DATA_INTERRUPT_ENABLE,
    DATA_CONFIG,
    DATA_INTERRUPT,
    DATA_INTERRUPT_CONFIG,

    ///<step counter/detector BMI160 only
    STEP_DETECTOR_INTERRUPT_EN= 0x17,
    STEP_DETECTOR_CONFIG,
    STEP_DETECTOR_INTERRUPT,
    STEP_COUNTER_DATA,
    STEP_COUNTER_RESET,
    PACKED_ACC_DATA
};
