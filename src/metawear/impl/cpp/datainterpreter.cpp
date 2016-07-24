#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "metawear/core/types.h"
#include "metawear/core/cpp/datainterpreter.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/sensor/cpp/accelerometer_bosch_private.h"
#include "metawear/sensor/cpp/gyro_bmi160_private.h"

using std::calloc;
using std::malloc;
using std::memcpy;
using std::memset;
using std::min;
using std::unordered_map;

struct CartesianShort {
    int16_t x, y, z;
};

#define CAST_INT32(x) if ((response[len - 1] & 0x80) == 0x80) {\
    x= -1;\
} else {\
    x= 0;\
}\
memcpy(&x, response, min(len, (uint8_t) sizeof(x)))

#define CREATE_MESSAGE(dt_id) MblMwData *msg= (MblMwData*) malloc(sizeof(MblMwData));\
    msg->value= value;\
    msg->type_id= dt_id;\
    msg->length= sizeof(*value);\
    return msg

static const float BOSCH_BARO_SCALE= 256.f, TEMPERATURE_SCALE= 8.f, MMA8452Q_ACC_SCALE= 1000.f, BMM150_SCALE= 16.f, BME280_HUMIDITY_SCALE= 1024.f, 
        Q16_16_SCALE= 0x10000;

static MblMwData* convert_to_int32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t *value = (int32_t*)malloc(sizeof(int32_t));
    CAST_INT32(*value);

    CREATE_MESSAGE(MBL_MW_DT_ID_INT32);
}

static MblMwData* convert_to_uint32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t *value= (uint32_t*) calloc(1, sizeof(uint32_t));
    memcpy(value, response, min(len, (uint8_t) sizeof(*value)));

    CREATE_MESSAGE(MBL_MW_DT_ID_UINT32);
}

static MblMwData* convert_to_temperature(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    CAST_INT32(unscaled);

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / TEMPERATURE_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmp280_pressure(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / BOSCH_BARO_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmp280_altitude(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / BOSCH_BARO_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_mma8452q_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort milliG;
    memcpy(&milliG, response, sizeof(milliG));

    MblMwCartesianFloat *value= (MblMwCartesianFloat*) malloc(sizeof(MblMwCartesianFloat));
    value->x= milliG.x / MMA8452Q_ACC_SCALE;
    value->y= milliG.y / MMA8452Q_ACC_SCALE;
    value->z= milliG.z / MMA8452Q_ACC_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_mma8452q_acceleration_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    CAST_INT32(unscaled);

    float *value = (float*) malloc(sizeof(float));
    *value = unscaled / MMA8452Q_ACC_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_mma8452q_acceleration_unsigned_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t unscaled= 0;
    memcpy(&unscaled, response, min(len, (uint8_t) sizeof(unscaled)));

    float *value= (float*) malloc(sizeof(float));
    *value = unscaled / MMA8452Q_ACC_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bosch_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float scale = bosch_get_data_scale(board);

    MblMwCartesianFloat *value = (MblMwCartesianFloat*)malloc(sizeof(MblMwCartesianFloat));
    value->x = unscaled.x / scale;
    value->y = unscaled.y / scale;
    value->z = unscaled.z / scale;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_bosch_acceleration_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    CAST_INT32(unscaled);

    float *value = (float*)malloc(sizeof(float));
    *value = unscaled / bosch_get_data_scale(board);

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bosch_acceleration_unsigned_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t unscaled= 0;
    memcpy(&unscaled, response, min(len, (uint8_t) sizeof(unscaled)));

    float *value= (float*) malloc(sizeof(float));
    *value = unscaled / bosch_get_data_scale(board);

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmi160_rotation(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float scale = bosch_gyro_get_data_scale(board);

    MblMwCartesianFloat *value = (MblMwCartesianFloat*)malloc(sizeof(MblMwCartesianFloat));
    value->x = unscaled.x / scale;
    value->y = unscaled.y / scale;
    value->z = unscaled.z / scale;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_bmi160_rotation_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    CAST_INT32(unscaled);

    float *value = (float*)malloc(sizeof(float));
    *value = unscaled / bosch_gyro_get_data_scale(board);

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmi160_rotation_unsigned_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t unscaled= 0;
    memcpy(&unscaled, response, min(len, (uint8_t) sizeof(unscaled)));

    float *value= (float*) malloc(sizeof(float));
    *value = unscaled / bosch_gyro_get_data_scale(board);

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_byte_array(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    MblMwData *msg= (MblMwData*) malloc(sizeof(MblMwData));
    msg->type_id= MBL_MW_DT_ID_BYTE_ARRAY;
    msg->length= len;

    msg->value= malloc(len);
    memcpy(msg->value, response, len);

    return msg;
}

static MblMwData* convert_to_bmm150_b_field(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    MblMwCartesianFloat *value = (MblMwCartesianFloat*)malloc(sizeof(MblMwCartesianFloat));
    value->x = unscaled.x / BMM150_SCALE;
    value->y = unscaled.y / BMM150_SCALE;
    value->z = unscaled.z / BMM150_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_bmm150_b_field_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    CAST_INT32(unscaled);

    float *value = (float*) malloc(sizeof(float));
    *value = unscaled / BMM150_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmm150_b_field_unsigned_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t unscaled= 0;
    memcpy(&unscaled, response, min(len, (uint8_t) sizeof(unscaled)));

    float *value= (float*) malloc(sizeof(float));
    *value = unscaled / BMM150_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_battery_state(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    MblMwBatteryState *value= (MblMwBatteryState*) malloc(sizeof(MblMwBatteryState));
    memcpy(&value->voltage, response + 1, 2);
    value->charge= response[0];

    CREATE_MESSAGE(MBL_MW_DT_ID_BATTERY_STATE);
}

static MblMwData* convert_to_tcs34725_adc(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    MblMwTcs34725ColorAdc *value= (MblMwTcs34725ColorAdc*) malloc(sizeof(MblMwTcs34725ColorAdc));
    memcpy(value, response, sizeof(MblMwTcs34725ColorAdc));

    CREATE_MESSAGE(MBL_MW_DT_ID_TCS34725_ADC);
}

static MblMwData* convert_to_bme280_humidity(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / BME280_HUMIDITY_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_q16_16_fixed_point(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value = unscaled / Q16_16_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

unordered_map<DataInterpreter, DataInterpreter> signed_to_unsigned= {
    { DataInterpreter::INT32, DataInterpreter::UINT32 },
    { DataInterpreter::BMI160_ROTATION_SINGLE_AXIS, DataInterpreter::BMI160_ROTATION_UNSIGNED_SINGLE_AXIS },
    { DataInterpreter::BOSCH_ACCELERATION_SINGLE_AXIS, DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS },
    { DataInterpreter::MMA8452Q_ACCELERATION_SINGLE_AXIS, DataInterpreter::MMA8452Q_ACCELERATION_UNSIGNED_SINGLE_AXIS },
    { DataInterpreter::BOSCH_ACCELERATION_SINGLE_AXIS, DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS },
    { DataInterpreter::BMM150_B_FIELD_SINGLE_AXIS, DataInterpreter::BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS }
};

unordered_map<DataInterpreter, DataInterpreter> unsigned_to_signed= {
    { DataInterpreter::UINT32, DataInterpreter::INT32 },
    { DataInterpreter::BMI160_ROTATION_UNSIGNED_SINGLE_AXIS, DataInterpreter::BMI160_ROTATION_SINGLE_AXIS },
    { DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS, DataInterpreter::BOSCH_ACCELERATION_SINGLE_AXIS },
    { DataInterpreter::MMA8452Q_ACCELERATION_UNSIGNED_SINGLE_AXIS, DataInterpreter::MMA8452Q_ACCELERATION_SINGLE_AXIS },
    { DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS, DataInterpreter::BOSCH_ACCELERATION_SINGLE_AXIS },
    { DataInterpreter::BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS, DataInterpreter::BMM150_B_FIELD_SINGLE_AXIS }
};

unordered_map<DataInterpreter, FnBoardByteArray> data_response_converters = {
    { DataInterpreter::INT32 , convert_to_int32 },
    { DataInterpreter::UINT32 , convert_to_uint32 },
    { DataInterpreter::TEMPERATURE , convert_to_temperature },
    { DataInterpreter::BOSCH_PRESSURE , convert_to_bmp280_pressure },
    { DataInterpreter::BOSCH_ALTITUDE , convert_to_bmp280_altitude },
    { DataInterpreter::BMI160_ROTATION , convert_to_bmi160_rotation },
    { DataInterpreter::BMI160_ROTATION_SINGLE_AXIS , convert_to_bmi160_rotation_single_axis },
    { DataInterpreter::BMI160_ROTATION_UNSIGNED_SINGLE_AXIS , convert_to_bmi160_rotation_unsigned_single_axis },
    { DataInterpreter::BOSCH_ACCELERATION , convert_to_bosch_acceleration },
    { DataInterpreter::BOSCH_ACCELERATION_SINGLE_AXIS , convert_to_bosch_acceleration_single_axis },
    { DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS , convert_to_bosch_acceleration_unsigned_single_axis },
    { DataInterpreter::MMA8452Q_ACCELERATION , convert_to_mma8452q_acceleration },
    { DataInterpreter::MMA8452Q_ACCELERATION_SINGLE_AXIS , convert_to_mma8452q_acceleration_single_axis },
    { DataInterpreter::MMA8452Q_ACCELERATION_UNSIGNED_SINGLE_AXIS , convert_to_mma8452q_acceleration_unsigned_single_axis },
    { DataInterpreter::BYTE_ARRAY , convert_to_byte_array },
    { DataInterpreter::BMM150_B_FIELD , convert_to_bmm150_b_field },
    { DataInterpreter::BMM150_B_FIELD_SINGLE_AXIS , convert_to_bmm150_b_field_single_axis },
    { DataInterpreter::BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS , convert_to_bmm150_b_field_unsigned_single_axis },
    { DataInterpreter::SETTINGS_BATTERY_STATE , convert_to_battery_state },
    { DataInterpreter::TCS34725_COLOR_ADC , convert_to_tcs34725_adc },
    { DataInterpreter::BME280_HUMIDITY , convert_to_bme280_humidity },
    { DataInterpreter::Q16_16_FIXED_POINT , convert_to_q16_16_fixed_point }
};

static float bosch_acc_to_firmware(const MblMwDataSignal* signal, float value) {
    return value * bosch_get_data_scale(signal->owner);
}

static float mma8452q_to_firmware(const MblMwDataSignal* signal, float value) {
    return value * MMA8452Q_ACC_SCALE;
}

static float bosch_baro_to_firmware(const MblMwDataSignal* signal, float value) {
    return value * BOSCH_BARO_SCALE;
}

static float bosch_gyro_to_firmware(const MblMwDataSignal* signal, float value) {
    return value * bosch_gyro_get_data_scale(signal->owner);
}

static float number_to_firmware_default(const MblMwDataSignal* signal, float value) {
    return value;
}

static float bme280_humidity_to_firmware(const MblMwDataSignal* signal, float value) {
    return value * BME280_HUMIDITY_SCALE;
}

static float temp_to_firmware(const MblMwDataSignal* signal, float value) {
    return value * TEMPERATURE_SCALE;
}

static float q16_16_fixed_point_to_firmware(const MblMwDataSignal* signal, float value) {
    return value * Q16_16_SCALE;
}

static float bosch_magnetometer_to_firmware(const MblMwDataSignal* signal, float value) {
    return value * BMM150_SCALE;
}

unordered_map<FirmwareConverter, FnDataSignalFloat> number_to_firmware_converters = {
    { FirmwareConverter::DEFAULT , number_to_firmware_default },
    { FirmwareConverter::BME280_HUMIDITY , bme280_humidity_to_firmware },
    { FirmwareConverter::BOSCH_ACCELERATION , bosch_acc_to_firmware },
    { FirmwareConverter::BOSCH_BAROMETER , bosch_baro_to_firmware },
    { FirmwareConverter::BMI160_ROTATION , bosch_gyro_to_firmware },
    { FirmwareConverter::MMA8452Q_ACCELERATION , mma8452q_to_firmware},
    { FirmwareConverter::TEMPERATURE , temp_to_firmware},
    { FirmwareConverter::Q16_16_FIXED_POINT , q16_16_fixed_point_to_firmware},
    { FirmwareConverter::BOSCH_MAGNETOMETER , bosch_magnetometer_to_firmware},
};

namespace std {

size_t hash<DataInterpreter>::operator()(const DataInterpreter& key) const {
    return static_cast<uint8_t>(key);
}

size_t hash<FirmwareConverter>::operator()(const FirmwareConverter& key) const {
    return static_cast<uint8_t>(key);
}

}
