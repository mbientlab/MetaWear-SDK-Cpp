#pragma once

#include <unordered_map>

#include "metawear/core/data.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/metawearboard_fwd.h"

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
    BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS
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

extern std::unordered_map<DataInterpreter, DataInterpreter> signed_to_unsigned;
extern std::unordered_map<DataInterpreter, DataInterpreter> unsigned_to_signed;

typedef MblMwData* (*FnBoardByteArray)(const MblMwMetaWearBoard*, const uint8_t*, uint8_t);
extern std::unordered_map<DataInterpreter, FnBoardByteArray> data_response_converters;

typedef float (*FnDataSignalFloat)(const MblMwDataSignal*, float);
extern std::unordered_map<FirmwareConverter, FnDataSignalFloat> number_to_firmware_converters;
