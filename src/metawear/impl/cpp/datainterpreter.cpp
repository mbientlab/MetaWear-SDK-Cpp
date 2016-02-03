#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "metawear/core/types.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/datainterpreter.h"
#include "metawear/sensor/cpp/accelerometer_bmi160_private.h"
#include "metawear/sensor/cpp/gyro_bmi160_private.h"

using std::calloc;
using std::malloc;
using std::memcpy;
using std::memset;
using std::min;

#define CREATE_MESSAGE(dt_id) MblMwData *msg= (MblMwData*) malloc(sizeof(MblMwData));\
    msg->value= value;\
    msg->type_id= dt_id;\
    return msg

static const float BMP280_SCALE= 256.f, TEMPERATURE_SCALE= 8.f, MMA8452Q_ACC_SCALE= 1000.f;

static MblMwData* convert_to_int32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t *value = (int32_t*)malloc(sizeof(int32_t));

    if ((response[len - 1] & 0x80) == 0x80) {
        memset(value, 0xff, sizeof(int32_t));
    } else {
        memset(value, 0x0, sizeof(int32_t));
    }
    memcpy(value, response, min(len, (uint8_t) sizeof(value)));

    CREATE_MESSAGE(MBL_MW_DT_ID_INT32);
}

static MblMwData* convert_to_uint32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t *value= (uint32_t*) calloc(1, sizeof(uint32_t));
    memcpy(value, response, min(len, (uint8_t) sizeof(value)));

    CREATE_MESSAGE(MBL_MW_DT_ID_UINT32);
}

static MblMwData* convert_to_temperature(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int16_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / TEMPERATURE_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmp280_pressure(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / BMP280_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmp280_altitude(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / BMP280_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_mma8452q_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort milliG;
    memcpy(&milliG, response, sizeof(milliG));

    CartesianFloat *value= (CartesianFloat*) malloc(sizeof(CartesianFloat));
    value->x= milliG.x / MMA8452Q_ACC_SCALE;
    value->y= milliG.y / MMA8452Q_ACC_SCALE;
    value->z= milliG.z / MMA8452Q_ACC_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_mma8452q_acceleration_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    if ((response[len - 1] & 0x80) == 0x80) {
        unscaled= -1;
    } else {
        unscaled= 0;
    }
    memcpy(&unscaled, response, min(len, (uint8_t) sizeof(unscaled)));

    float *value = (float*) malloc(sizeof(float));
    *value = unscaled / MMA8452Q_ACC_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmi160_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float scale = ((AccBmi160Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->get_scale();

    CartesianFloat *value = (CartesianFloat*)malloc(sizeof(CartesianFloat));
    value->x = unscaled.x / scale;
    value->y = unscaled.y / scale;
    value->z = unscaled.z / scale;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_bmi160_acceleration_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    if ((response[len - 1] & 0x80) == 0x80) {
        unscaled = -1;
    } else {
        unscaled = 0;
    }
    memcpy(&unscaled, response, min(len, (uint8_t) sizeof(unscaled)));

    float *value = (float*)malloc(sizeof(float));
    *value = unscaled / ((AccBmi160Config*)board->module_config.at(MBL_MW_MODULE_ACCELEROMETER))->get_scale();

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}


static MblMwData* convert_to_bmi160_rotation(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float scale = ((GyroBmi160Config*)board->module_config.at(MBL_MW_MODULE_GYRO))->get_scale();

    CartesianFloat *value = (CartesianFloat*)malloc(sizeof(CartesianFloat));
    value->x = unscaled.x / scale;
    value->y = unscaled.y / scale;
    value->z = unscaled.z / scale;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_bmi160_rotation_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int16_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value = (float*)malloc(sizeof(float));
    *value = unscaled / ((GyroBmi160Config*)board->module_config.at(MBL_MW_MODULE_GYRO))->get_scale();

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

std::unordered_map<DataInterpreter, DataHandler> data_response_converters = {
    { DataInterpreter::INT32 , convert_to_int32 },
    { DataInterpreter::UINT32 , convert_to_uint32 },
    { DataInterpreter::TEMPERATURE , convert_to_temperature },
    { DataInterpreter::BMP280_PRESSURE , convert_to_bmp280_pressure },
    { DataInterpreter::BMP280_ALTITUDE , convert_to_bmp280_altitude },
    { DataInterpreter::BMI160_ROTATION , convert_to_bmi160_rotation },
    { DataInterpreter::BMI160_ROTATION_SINGLE_AXIS , convert_to_bmi160_rotation_single_axis },
    { DataInterpreter::BMI160_ACCELERATION , convert_to_bmi160_acceleration },
    { DataInterpreter::BMI160_ACCELERATION_SINGLE_AXIS , convert_to_bmi160_acceleration_single_axis },
    { DataInterpreter::MMA8452Q_ACCELERATION , convert_to_mma8452q_acceleration },
    { DataInterpreter::MMA8452Q_ACCELERATION_SINGLE_AXIS , convert_to_mma8452q_acceleration_single_axis }
};

namespace std {

size_t hash<DataInterpreter>::operator()(const DataInterpreter& key) const {
    return static_cast<uint8_t>(key);
}

}
