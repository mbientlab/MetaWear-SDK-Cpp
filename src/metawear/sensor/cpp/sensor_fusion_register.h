#pragma once

enum class SensorFusionRegister : uint8_t {
    ENABLE = 1,
    MODE,
    OUTPUT_ENABLE,
    CORRECTED_ACC,
    CORRECTED_GYRO,
    CORRECTED_MAG,
    QUATERNION,
    EULER_ANGLES,
    GRAVITY_VECTOR,
    LINEAR_ACC,
    CALIBRATION_STATE,
    ACC_CAL_DATA,
    GYRO_CAL_DATA,
    MAG_CAL_DATA
};
