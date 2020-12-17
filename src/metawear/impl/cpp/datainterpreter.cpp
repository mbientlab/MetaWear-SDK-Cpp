#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <chrono>

#include "metawear/core/types.h"
#include "metawear/core/settings.h"
#include "metawear/core/cpp/datainterpreter.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/sensor/accelerometer_bosch.h"
#include "metawear/sensor/cpp/accelerometer_bosch_private.h"
#include "metawear/sensor/cpp/gyro_bmi160_private.h"
#include "metawear/core/cpp/logging_private.h"
#include "metawear/processor/cpp/dataprocessor_config.h"
#include "metawear/processor/cpp/dataprocessor_private.h"

using namespace std;
using namespace std::chrono;

struct CartesianShort {
    int16_t x, y, z;
};

#define CAST_INT32(x) uint8_t max_pos= log_data ? min(len, signal->length()) : min(len, (uint8_t) (signal->offset + signal->length()));\
    x= (response[max_pos - 1] & 0x80) == 0x80 ? -1 : 0;\
    if (log_data) {\
        memcpy(&x, response, max_pos);\
    } else {\
        memcpy(&x, response + signal->offset, max_pos - signal->offset);\
    }

#define CAST_UINT32(x) if (log_data) {\
        memcpy(&x, response, sizeof(uint32_t));\
    } else {\
        uint8_t max_pos= min(len, (uint8_t) (signal->offset + signal->length()));\
        memcpy(&x, response + signal->offset, max_pos - signal->offset);\
    }

#define CREATE_MESSAGE(dt_id) MblMwData *msg= (MblMwData*) malloc(sizeof(MblMwData));\
    msg->value= value;\
    msg->type_id= dt_id;\
    msg->length= sizeof(*value);\
    return msg

#define CONVERT_TO_FLOAT(name, raw_type, scale) static MblMwData* name(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {\
    raw_type unscaled;\
    CAST_INT32(unscaled)\
\
    float *value= (float*) malloc(sizeof(float));\
    *value= unscaled / scale;\
\
    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);\
}\

#define RAW_CONVERT(name, type, type_id)  static MblMwData* name(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {\
    type* value = (type*) malloc(sizeof(type));\
    memcpy(value, response, sizeof(type));\
    CREATE_MESSAGE(type_id);\
}\

const float BOSCH_BARO_SCALE= 256.f, TEMPERATURE_SCALE= 8.f, MMA8452Q_ACC_SCALE= 1000.f, BMM150_SCALE= 16.f, BME280_HUMIDITY_SCALE= 1024.f, 
        Q16_16_SCALE= 0x10000, SENSOR_FUSION_ACC_SCALE = 1000.f, MSS_TO_G_SCALE = 9.80665f;

static MblMwData* convert_to_int32(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    int32_t *value = (int32_t*) malloc(sizeof(int32_t));

    CAST_INT32(*value)
    CREATE_MESSAGE(MBL_MW_DT_ID_INT32);
}

static MblMwData* convert_to_uint32(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    uint32_t *value= (uint32_t*) calloc(1, sizeof(uint32_t));

    CAST_UINT32(*value);
    CREATE_MESSAGE(MBL_MW_DT_ID_UINT32);
}

static MblMwData* convert_to_mma8452q_acceleration(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    CartesianShort milliG;
    memcpy(&milliG, response, sizeof(milliG));

    MblMwCartesianFloat *value= (MblMwCartesianFloat*) malloc(sizeof(MblMwCartesianFloat));
    value->x= milliG.x / MMA8452Q_ACC_SCALE;
    value->y= milliG.y / MMA8452Q_ACC_SCALE;
    value->z= milliG.z / MMA8452Q_ACC_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_bosch_acceleration(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float scale = bosch_get_data_scale(signal->owner);

    MblMwCartesianFloat *value = (MblMwCartesianFloat*)malloc(sizeof(MblMwCartesianFloat));
    value->x = unscaled.x / scale;
    value->y = unscaled.y / scale;
    value->z = unscaled.z / scale;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_bosch_acceleration_single_axis(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    CAST_INT32(unscaled)

    float *value = (float*) malloc(sizeof(float));
    *value = unscaled / bosch_get_data_scale(signal->owner);

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bosch_acceleration_unsigned_single_axis(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    uint32_t unscaled= 0;
    CAST_UINT32(unscaled)

    float *value= (float*) malloc(sizeof(float));
    *value = unscaled / bosch_get_data_scale(signal->owner);

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmi160_rotation(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float scale = bosch_gyro_get_data_scale(signal->owner);

    MblMwCartesianFloat *value = (MblMwCartesianFloat*)malloc(sizeof(MblMwCartesianFloat));
    value->x = unscaled.x / scale;
    value->y = unscaled.y / scale;
    value->z = unscaled.z / scale;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_bmi160_rotation_single_axis(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    CAST_INT32(unscaled)

    float *value = (float*) malloc(sizeof(float));
    *value = unscaled / bosch_gyro_get_data_scale(signal->owner);

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_bmi160_rotation_unsigned_single_axis(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    uint32_t unscaled= 0;
    CAST_UINT32(unscaled)

    float *value= (float*) malloc(sizeof(float));
    *value = unscaled / bosch_gyro_get_data_scale(signal->owner);

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

static MblMwData* convert_to_byte_array(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwData *msg= (MblMwData*) malloc(sizeof(MblMwData));
    msg->type_id= MBL_MW_DT_ID_BYTE_ARRAY;
    msg->length= len;

    msg->value= malloc(len);
    memcpy(msg->value, response, len);

    return msg;
}

static MblMwData* convert_to_bmm150_b_field(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    MblMwCartesianFloat *value = (MblMwCartesianFloat*)malloc(sizeof(MblMwCartesianFloat));
    value->x = unscaled.x / BMM150_SCALE;
    value->y = unscaled.y / BMM150_SCALE;
    value->z = unscaled.z / BMM150_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_battery_state(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwBatteryState *value= (MblMwBatteryState*) malloc(sizeof(MblMwBatteryState));
    memcpy(&value->voltage, response + 1, 2);
    value->charge= response[0];

    CREATE_MESSAGE(MBL_MW_DT_ID_BATTERY_STATE);
}

static MblMwData* convert_to_bosch_any_motion(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwBoschAnyMotion *value= (MblMwBoschAnyMotion*) malloc(sizeof(MblMwBoschAnyMotion));
    auto detected = [response](uint8_t& field, uint8_t axis) {
        auto mask = 0x1 << (axis + 3);
        field = (mask & response[0]) == mask;
    };

    value->sign = (response[0] & 0x40) != 0x40;
    detected(value->x_axis_active, 0);
    detected(value->y_axis_active, 1);
    detected(value->z_axis_active, 2);
    
    CREATE_MESSAGE(MBL_MW_DT_ID_BOSCH_ANY_MOTION);
}

CONVERT_TO_FLOAT(convert_to_temperature, int32_t, TEMPERATURE_SCALE)

CONVERT_TO_FLOAT(convert_to_mma8452q_acceleration_single_axis, int32_t, MMA8452Q_ACC_SCALE)
CONVERT_TO_FLOAT(convert_to_mma8452q_acceleration_unsigned_single_axis, uint32_t, MMA8452Q_ACC_SCALE)

CONVERT_TO_FLOAT(convert_to_bmp280_pressure, uint32_t, BOSCH_BARO_SCALE)
CONVERT_TO_FLOAT(convert_to_bmp280_altitude, int32_t, BOSCH_BARO_SCALE)

CONVERT_TO_FLOAT(convert_to_bmm150_b_field_single_axis, int32_t, BMM150_SCALE)
CONVERT_TO_FLOAT(convert_to_bmm150_b_field_unsigned_single_axis, uint32_t, BMM150_SCALE)

CONVERT_TO_FLOAT(convert_to_bme280_humidity, uint32_t, BME280_HUMIDITY_SCALE)
CONVERT_TO_FLOAT(convert_to_q16_16_fixed_point, int32_t, Q16_16_SCALE)

RAW_CONVERT(convert_to_tcs34725_adc, MblMwTcs34725ColorAdc, MBL_MW_DT_ID_TCS34725_ADC)
RAW_CONVERT(convert_to_quaternion, MblMwQuaternion, MBL_MW_DT_ID_QUATERNION)
RAW_CONVERT(convert_to_euler_angles, MblMwEulerAngles, MBL_MW_DT_ID_EULER_ANGLE)
RAW_CONVERT(convert_to_corrected_vector3, MblMwCorrectedCartesianFloat, MBL_MW_DT_ID_CORRECTED_CARTESIAN_FLOAT)

static MblMwData* convert_to_vector3(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwCartesianFloat unscaled, *value = (MblMwCartesianFloat*) malloc(sizeof(MblMwCartesianFloat));
    memcpy(&unscaled, response, sizeof(MblMwCartesianFloat));

    value->x = unscaled.x / MSS_TO_G_SCALE;
    value->y = unscaled.y / MSS_TO_G_SCALE;
    value->z = unscaled.z / MSS_TO_G_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_corrected_acc(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwCorrectedCartesianFloat unscaled, *value = (MblMwCorrectedCartesianFloat*) malloc(sizeof(MblMwCorrectedCartesianFloat));
    memcpy(&unscaled, response, sizeof(float) * 3);

    value->x = unscaled.x / SENSOR_FUSION_ACC_SCALE;
    value->y = unscaled.y / SENSOR_FUSION_ACC_SCALE;
    value->z = unscaled.z / SENSOR_FUSION_ACC_SCALE;
    value->accuracy = response[12];

    CREATE_MESSAGE(MBL_MW_DT_ID_CORRECTED_CARTESIAN_FLOAT);
}

static MblMwData* convert_to_overflow_state(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwOverflowState *value= (MblMwOverflowState*) malloc(sizeof(MblMwOverflowState));
    memcpy(&value->length, response + 1, 2);
    value->assert_en= response[0];

    CREATE_MESSAGE(MBL_MW_DT_ID_OVERFLOW_STATE);
}

static MblMwData* convert_to_sensor_orientation(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {    
    MblMwSensorOrientation *value= (MblMwSensorOrientation*) malloc(sizeof(MblMwSensorOrientation));
    *value = (MblMwSensorOrientation) (((response[0] & 0x6) >> 1) + 4 * ((response[0] & 0x8) >> 3));

    CREATE_MESSAGE(MBL_MW_DT_ID_SENSOR_ORIENTATION);
}

static MblMwData* convert_to_mac_address(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {    
    const int strSize = 17 + 1;
    const uint8_t offset = len == 7 ? 1 : 0;
    char *value = (char *)malloc(strSize);
    sprintf(value, "%02X:%02X:%02X:%02X:%02X:%02X", response[5 + offset], response[4 + offset], 
        response[3 + offset], response[2 + offset], response[1 + offset], response[0 + offset]);
    value[strSize - 1] = '\0';

    MblMwData *msg = (MblMwData*)malloc(sizeof(MblMwData));
    msg->value = value;
    msg->type_id = MBL_MW_DT_ID_STRING;
    msg->length = strSize;
    return msg;
}

static MblMwData* convert_to_sensor_orientation_mma8452q(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {    
    MblMwSensorOrientation *value= (MblMwSensorOrientation*) malloc(sizeof(MblMwSensorOrientation));
    uint32_t offset = (response[0] & 0x06) >> 1;
    *value = (MblMwSensorOrientation) (4 * (response[0] & 0x01) + ((offset == 2 || offset == 3) ? offset ^ 0x1 : offset));

    CREATE_MESSAGE(MBL_MW_DT_ID_SENSOR_ORIENTATION);
}

static MblMwData* convert_to_logging_time(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwLoggingTime *value = (MblMwLoggingTime*)malloc(sizeof(MblMwLoggingTime));

    // We need to convert the local MetaWear time in 'real' Earth time
    value->epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    // Get the current reading of the MetaWear clock
    uint32_t tick;
    memcpy(&tick, response, 4);
    // By going backwards the number of miliseconds the current MetaWear time 
    // represents, we find the real world time the MetaWear booted
    value->epoch -= static_cast<int64_t>(round((double)tick * TICK_TIME_STEP));
    // Each MetaWear reset event has a corresponding reset_uid
    value->reset_uid = response[4];
    
    CREATE_MESSAGE(MBL_MW_DT_ID_LOGGING_TIME);
}

static MblMwData* convert_to_btle_address(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwBtleAddress *value = (MblMwBtleAddress*)malloc(sizeof(MblMwBtleAddress));
    memcpy(value, response, sizeof(MblMwBtleAddress));

    CREATE_MESSAGE(MBL_MW_DT_ID_BTLE_ADDRESS);
}

static MblMwData* convert_to_calibration_state(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    MblMwCalibrationState *value = (MblMwCalibrationState*)malloc(sizeof(MblMwCalibrationState));
    memcpy(value, response, sizeof(MblMwCalibrationState));

    CREATE_MESSAGE(MBL_MW_DT_ID_CALIBRATION_STATE);
}

static MblMwData* convert_to_fused(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {
    auto processor = dynamic_cast<const MblMwDataProcessor*>(signal);
    if (processor->type != DataProcessorType::FUSER) {
        return convert_to_fused(log_data, processor->input, response, len);
    }
    
    auto fused_config = (FuseConfig*) processor->config;
    MblMwData** value = (MblMwData**) calloc(fused_config->count + 1, sizeof(MblMwData*));
    uint8_t offset = 0;
    MblMwData* partialMsg;
    
    partialMsg = data_response_converters.at(processor->input->interpreter)(log_data, signal, response, len);
    value[0] = partialMsg;
    offset+= processor->input->length();

    for(uint8_t i = 0; i < fused_config->count; i++, offset+= processor->length()) {
        processor = lookup_processor(signal->owner, fused_config->references[i]);
        partialMsg = data_response_converters.at(processor->interpreter)(log_data, processor, response + offset, len - offset);
        value[i + 1] = partialMsg;
    }

    MblMwData *msg= (MblMwData*) malloc(sizeof(MblMwData));
    msg->value= value;
    msg->type_id= MBL_MW_DT_ID_DATA_ARRAY;
    msg->length= sizeof(MblMwData*) * (fused_config->count + 1);

    return msg;
}

static MblMwData* convert_to_bosch_tap(bool log_data, const MblMwDataSignal* signal, const uint8_t *response, uint8_t len) {    
    MblMwBoschTap *value = (MblMwBoschTap*) malloc(sizeof(MblMwBoschTap));

    if ((response[0] & 0x1) == 0x1) {
        value->type = 0x1;
    } else if ((response[0] & 0x2) == 0x2) {
        value->type = 0x2;
    } else {
        value->type = 0;
    }
    value->sign = (response[0] & 0x20) == 0x20;

    CREATE_MESSAGE(MBL_MW_DT_ID_BOSCH_TAP);
}

unordered_map<DataInterpreter, FnBoolDataSignalByteArray> data_response_converters = {
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
    { DataInterpreter::Q16_16_FIXED_POINT , convert_to_q16_16_fixed_point },
    { DataInterpreter::SENSOR_FUSION_QUATERNION , convert_to_quaternion },
    { DataInterpreter::SENSOR_FUSION_EULER_ANGLE , convert_to_euler_angles },
    { DataInterpreter::SENSOR_FUSION_CORRECTED_FLOAT_VECTOR3 , convert_to_corrected_vector3 },
    { DataInterpreter::SENSOR_FUSION_FLOAT_VECTOR3 , convert_to_vector3 },
    { DataInterpreter::SENSOR_FUSION_CORRECTED_ACC , convert_to_corrected_acc },
    { DataInterpreter::DEBUG_OVERFLOW_STATE , convert_to_overflow_state },
    { DataInterpreter::SENSOR_ORIENTATION, convert_to_sensor_orientation },
    { DataInterpreter::MAC_ADDRESS, convert_to_mac_address },
    { DataInterpreter::SENSOR_ORIENTATION_MMA8452Q, convert_to_sensor_orientation_mma8452q },
    { DataInterpreter::LOGGING_TIME, convert_to_logging_time },
    { DataInterpreter::BTLE_ADDRESS, convert_to_btle_address },
    { DataInterpreter::BOSCH_ANY_MOTION, convert_to_bosch_any_motion },
    { DataInterpreter::SENSOR_FUSION_CALIB_STATE, convert_to_calibration_state },
    { DataInterpreter::FUSED_DATA, convert_to_fused },
    { DataInterpreter::BOSCH_TAP, convert_to_bosch_tap }
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

void free_data(const MblMwDataSignal* signal, MblMwData* data) {
    if (data->type_id == MBL_MW_DT_ID_DATA_ARRAY) {
        auto processor = dynamic_cast<const MblMwDataProcessor*>(signal);
        auto fused_config = (FuseConfig*) processor->config;
        auto casted = (MblMwData**) data->value;

        for(uint8_t i = 0; i < fused_config->count; i++) {
            free(casted[i]);
        }
    }
    free(data->value);
    free(data);
}