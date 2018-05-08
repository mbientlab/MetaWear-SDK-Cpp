#pragma once

#include <unordered_map>

#include "metawear/core/data.h"
#include "metawear/core/datasignal_fwd.h"

enum class DataInterpreter : uint8_t {
    INT32,
    UINT32,
    TEMPERATURE,
    BOSCH_PRESSURE,
    BOSCH_ALTITUDE,
    BMI160_ROTATION,
    BMI160_ROTATION_SINGLE_AXIS,
    BOSCH_ACCELERATION,
    BOSCH_ACCELERATION_SINGLE_AXIS,
    MMA8452Q_ACCELERATION,
    MMA8452Q_ACCELERATION_SINGLE_AXIS,
    BYTE_ARRAY,
    BMM150_B_FIELD,
    BMM150_B_FIELD_SINGLE_AXIS,
    SETTINGS_BATTERY_STATE,
    TCS34725_COLOR_ADC,
    BME280_HUMIDITY,
    Q16_16_FIXED_POINT,
    BMI160_ROTATION_UNSIGNED_SINGLE_AXIS,
    BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS,
    MMA8452Q_ACCELERATION_UNSIGNED_SINGLE_AXIS,
    BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS,
    SENSOR_FUSION_QUATERION,
    SENSOR_FUSION_EULER_ANGLE,
    SENSOR_FUSION_CORRECTED_FLOAT_VECTOR3,
    SENSOR_FUSION_FLOAT_VECTOR3,
    SENSOR_FUSION_CORRECTED_ACC,
    DEBUG_OVERFLOW_STATE,
    SENSOR_ORIENTATION,
    MAC_ADDRESS,
    SENSOR_ORIENTATION_MMA8452Q,
    LOGGING_TIME,
    BTLE_ADDRESS
};

enum class FirmwareConverter : uint8_t {
    DEFAULT,
    BME280_HUMIDITY,
    BOSCH_ACCELERATION,
    BOSCH_BAROMETER,
    BMI160_ROTATION,
    MMA8452Q_ACCELERATION,
    TEMPERATURE,
    Q16_16_FIXED_POINT,
    BOSCH_MAGNETOMETER
};

namespace std {
    template <>
    struct hash<DataInterpreter> {
        size_t operator()(const DataInterpreter& key) const;
    };

    template <>
    struct hash<FirmwareConverter> {
        size_t operator()(const FirmwareConverter& key) const;
    };
}

typedef MblMwData* (*FnBoolDataSignalByteArray)(bool log_data, const MblMwDataSignal*, const uint8_t*, uint8_t);
extern std::unordered_map<DataInterpreter, FnBoolDataSignalByteArray> data_response_converters;

typedef float (*FnDataSignalFloat)(const MblMwDataSignal*, float);
extern std::unordered_map<FirmwareConverter, FnDataSignalFloat> number_to_firmware_converters;
