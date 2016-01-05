#pragma once

#include "metawear/core/data.h"
#include "metawear/core/metawearboard_fwd.h"

#define CREATE_MESSAGE(dt_id) MblMwData *msg= (MblMwData*) malloc(sizeof(MblMwData));\
    msg->value= value;\
    msg->type_id= dt_id;\
    return msg

enum ResponseConvertor : uint8_t {
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

MblMwData* convert_to_int32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_uint32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_temperature(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmp280_pressure(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmp280_altitude(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmi160_rotation(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmi160_rotation_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmi160_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmi160_acceleration_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_mma8452q_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_mma8452q_acceleration_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);