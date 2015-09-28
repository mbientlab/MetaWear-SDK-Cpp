#include "metawear/sensor.h"

#include "metawear/ambientlight_ltr329.h"
#include "metawear/barometer_bmp280.h"
#include "metawear/dataprocessor.h"
#include "metawear/switch.h"
#include "metawear/gyro_bmi160.h"
#include "metawear/accelerometer_bmi160.h"
#include "metawear/accelerometer_mma8452q.h"

#include <cstring>

enum DataSourceId {
    DS_ID_SWITCH= 0,
    DS_ID_ACCEL_MMA8452Q,
    DS_ID_ACCEL_BMI160,
    DS_ID_BARO_BMP280_PRESSURE,
    DS_ID_BARO_BMP280_ALTITUDE,
    DS_ID_GYRO_BMI160,
    DS_ID_ALS_LTR329
};

enum DataType {
    DT_ID_UINT,
    DT_ID_INT,
    DT_ID_BMP280_PRESSURE,
    DT_ID_BMP280_ALTITUDE,
    DT_ID_BMI160_THREE_AXIS,
    DT_ID_MMA8452Q_THREE_AXIS
};

struct DataSource {
    uint8_t module_id;
    uint8_t register_id;
    uint8_t data_id;

    uint8_t data_type_id;
    uint8_t is_signed;
    uint8_t length;
    uint8_t offset;
};

enum ProcessorId {
    ACCUMULATOR= 0x2,
    COMPARISON= 0x6,
    MATH= 0x9
};

void mbl_mw_sensor_free_data_source(DataSource *data_attr) {
    delete data_attr;
}

const uint8_t NO_DATA_ID= 0xff, DISABLE= 0, ENABLE= 1;

static const DataSource data_sources[]= {
    {MBL_MW_SWITCH_MODULE, MBL_MW_SWITCH_DATA, NO_DATA_ID, DT_ID_UINT, 0, 1, 0},
    {MBL_MW_ACC_MMA8452Q_MODULE, MBL_MW_ACC_MMA8452Q_DATA, NO_DATA_ID, DT_ID_MMA8452Q_THREE_AXIS, 1, 6, 0},
    {MBL_MW_ACC_BMI160_MODULE, MBL_MW_ACC_BMI160_DATA, NO_DATA_ID, DT_ID_BMI160_THREE_AXIS, 1, 6, 0},
    {MBL_MW_BARO_BMP280_MODULE, MBL_MW_BARO_BMP280_PRESSURE_DATA, NO_DATA_ID, DT_ID_BMP280_PRESSURE, 0, 4, 0},
    {MBL_MW_BARO_BMP280_MODULE, MBL_MW_BARO_BMP280_ALTITUDE_DATA, NO_DATA_ID, DT_ID_BMP280_ALTITUDE, 1, 4, 0},
    {MBL_MW_GYRO_BMI160_MODULE, MBL_MW_GYRO_BMI160_DATA, NO_DATA_ID, DT_ID_BMI160_THREE_AXIS, 1, 6, 0},
    {MBL_MW_ALS_LTR329_MODULE, MBL_MW_ALS_LTR329_DATA, NO_DATA_ID, DT_ID_UINT, 0, 4, 0}
};

const DataSource* mbl_mw_switch_get_data_source() {
    return data_sources;
}

const DataSource* mbl_mw_baro_bmp280_get_pressure_data_source() {
    return data_sources + DS_ID_BARO_BMP280_PRESSURE;
}

const DataSource* mbl_mw_baro_bmp280_get_altitude_data_source() {
    return data_sources + DS_ID_BARO_BMP280_ALTITUDE;
}

const DataSource* mbl_mw_acc_bmi160_get_acceleration_data_source() {
    return data_sources + DS_ID_ACCEL_BMI160;
}

const DataSource* mbl_mw_acc_mma8452q_get_acceleration_data_source() {
    return data_sources + DS_ID_ACCEL_MMA8452Q;
}

const DataSource* mbl_mw_gyro_bmi160_get_rotation_data_source() {
    return data_sources + DS_ID_GYRO_BMI160;
}

const DataSource* mbl_mw_als_ltr329_get_data_source() {
    return data_sources + DS_ID_ALS_LTR329;
}

uint8_t mbl_mw_sensor_subscribe(uint8_t *command, const DataSource *source) {
    if (source->data_id == NO_DATA_ID) {
        std::memcpy(command, source, 2);
        command[2]= ENABLE;
        return 3;
    }
    return -1;
}

uint8_t mbl_mw_sensor_unsubscribe(uint8_t *command, const DataSource *source) {
    if (source->data_id == NO_DATA_ID) {
        std::memcpy(command, source, 2);
        command[2]= DISABLE;
        return 3;
    }
    return -1;
}

// data processor functions
void mbl_mw_dataprocessor_update_id(DataSource *data_attr, uint8_t new_id) {
    data_attr->data_id= new_id;
}

#define INITIALIZE_DATA_ATTR(attr)\
    command[0]= DATA_PROCESSOR_MODULE;\
    command[1]= DATA_PROCESSOR_ADD;\
    command[2]= attr->module_id;\
    command[3]= attr->register_id;\
    command[4]= attr->data_id;\
    command[5]= attr->offset | ((attr->length - 1) << 5)

#define PACK_LENGTHS(index, in, out) command[index]= (out - 1) | (in - 1) << 2

DataSource* mbl_mw_data_processor_accumulator(uint8_t command[8], const DataSource *data_attr) {
    INITIALIZE_DATA_ATTR(data_attr);
    command[6]= ACCUMULATOR;
    command[7]= (data_attr->length - 1) | ((data_attr->length - 1) << 2);

    uint8_t data_attr_values[]= {DATA_PROCESSOR_MODULE, DATA_PROCESSOR_ADD, NO_DATA_ID, data_attr->is_signed, data_attr->length, 0};
    DataSource *next_data_attr= new DataSource;
    
    std::memcpy(next_data_attr, data_attr_values, sizeof(DataSource));
    return next_data_attr;
}

DataSource* mbl_mw_data_processor_comparison(uint8_t command[14], const DataSource *data_attr, uint8_t is_signed, 
        Comparison op, int32_t reference) {
    INITIALIZE_DATA_ATTR(data_attr);
    command[6]= COMPARISON;
    command[7]= is_signed;
    command[8]= op;
    std::memcpy(command + 10, &reference, 4);

    uint8_t data_attr_values[]= {DATA_PROCESSOR_MODULE, DATA_PROCESSOR_ADD, NO_DATA_ID, is_signed, data_attr->length, 0};
    DataSource *next_data_attr= new DataSource;
    
    std::memcpy(next_data_attr, data_attr_values, sizeof(DataSource));
    return next_data_attr;
}

DataSource* mbl_mw_data_processor_math(uint8_t command[13], const DataSource *data_attr, uint8_t is_signed, 
        Arithmetic op, int32_t rhs) {
    uint8_t output_size= 4;
    INITIALIZE_DATA_ATTR(data_attr);
    command[6]= MATH;
    PACK_LENGTHS(7, data_attr->length, output_size) | (is_signed << 4);
    command[8]= op;
    std::memcpy(command + 9, &rhs, 4);

    uint8_t data_attr_values[]= {DATA_PROCESSOR_MODULE, DATA_PROCESSOR_ADD, NO_DATA_ID, is_signed, output_size, 0};
    DataSource *next_data_attr= new DataSource;
    
    std::memcpy(next_data_attr, data_attr_values, sizeof(DataSource));
    return next_data_attr;
}
