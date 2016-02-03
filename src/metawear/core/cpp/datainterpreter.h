#pragma once

#include <unordered_map>

#include "metawear/core/data.h"
#include "metawear/core/metawearboard_fwd.h"

enum DataInterpreter : uint8_t {
    INT32,
    UINT32,
    TEMPERATURE,
    BMP280_PRESSURE,
    BMP280_ALTITUDE,
    BMI160_ROTATION,
    BMI160_ROTATION_SINGLE_AXIS,
    BMI160_ACCELERATION,
    BMI160_ACCELERATION_SINGLE_AXIS,
    MMA8452Q_ACCELERATION,
    MMA8452Q_ACCELERATION_SINGLE_AXIS
};

namespace std {
    template <>
    struct hash<DataInterpreter> {
        size_t operator()(const DataInterpreter& key) const;
    };
}

typedef MblMwData* (*DataHandler)(const MblMwMetaWearBoard *board, const uint8_t*, uint8_t);
extern std::unordered_map<DataInterpreter, DataHandler> data_response_converters;