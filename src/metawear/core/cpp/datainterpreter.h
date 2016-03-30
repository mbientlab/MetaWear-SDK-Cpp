#pragma once

#include <unordered_map>

#include "metawear/core/data.h"
#include "metawear/core/metawearboard_fwd.h"

enum DataInterpreter : uint8_t {
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
    BME280_HUMIDITY
};

namespace std {
    template <>
    struct hash<DataInterpreter> {
        size_t operator()(const DataInterpreter& key) const;
    };
}

typedef MblMwData* (*DataHandler)(const MblMwMetaWearBoard *board, const uint8_t*, uint8_t);
extern std::unordered_map<DataInterpreter, DataHandler> data_response_converters;
