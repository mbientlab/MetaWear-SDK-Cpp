#include "metawear/accelerometer_bmi160.h"

#include <cstring>

static const uint8_t FSR_BITMASKS[4]= {0x3, 0x5, 0x8, 0xc};
static const float FSR_SCALE[4]= {16384.f, 8192.f, 4096.f, 2048.f};

static const uint8_t MBL_MW_ACC_BMI160_POWER_MODE= 1, MBL_MW_ACC_BMI160_DATA_ENABLE= 0x2, MBL_MW_ACC_BMI160_DATA_CONFIG= 0x3;

struct Bmi160AccBitField {
    uint8_t acc_odr:4;
    uint8_t acc_bwp:3;
    uint8_t acc_us:1;
    uint8_t acc_range:4;
    uint8_t :4;
};

struct MblMwAccBmi160Config {
    Bmi160AccBitField bit_field;
    float scale;
};

MblMwAccBmi160Config* mbl_mw_acc_bmi160_create_config() {
    MblMwAccBmi160Config *config= new MblMwAccBmi160Config;

    std::memset(config, 0, sizeof(MblMwAccBmi160Config));
    config->bit_field.acc_bwp= 2;
    mbl_mw_acc_bmi160_set_odr(config, MBL_MW_ACC_BMI160_ODR_100HZ);
    mbl_mw_acc_bmi160_set_range(config, MBL_MW_ACC_BMI160_FSR_2G);
    return config;
}

void mbl_mw_acc_bmi160_free_config(MblMwAccBmi160Config *config) {
    delete config;
}

void mbl_mw_acc_bmi160_set_odr(MblMwAccBmi160Config *config, MblMwAccBmi160Odr odr) {
    config->bit_field.acc_odr= odr + 1;
}

void mbl_mw_acc_bmi160_set_range(MblMwAccBmi160Config *config, MblMwAccBmi160Range range) {
    config->bit_field.acc_range= FSR_BITMASKS[range];
    config->scale= FSR_SCALE[range];
}

void mbl_mw_acc_bmi160_write_acceleration_config(uint8_t command[4], const MblMwAccBmi160Config *config) {
    command[0]= MBL_MW_ACC_BMI160_MODULE;
    command[1]= MBL_MW_ACC_BMI160_DATA_CONFIG;
    std::memcpy(command + 2, &(config->bit_field), sizeof(Bmi160AccBitField));
}

void mbl_mw_acc_bmi160_start(uint8_t command[3]) {
    command[0]= MBL_MW_ACC_BMI160_MODULE;
    command[1]= MBL_MW_ACC_BMI160_POWER_MODE;
    command[2]= 1;
}

void mbl_mw_acc_bmi160_stop(uint8_t command[3]) {
    command[0]= MBL_MW_ACC_BMI160_MODULE;
    command[1]= MBL_MW_ACC_BMI160_POWER_MODE;
    command[2]= 0;
}

void mbl_mw_acc_bmi160_enable_acceleration_sampling(uint8_t command[4]) {
    command[0]= MBL_MW_ACC_BMI160_MODULE;
    command[1]= MBL_MW_ACC_BMI160_DATA_ENABLE;
    command[2]= 1;
    command[3]= 0;
}

void mbl_mw_acc_bmi160_disable_acceleration_sampling(uint8_t command[4]) {
    command[0]= MBL_MW_ACC_BMI160_MODULE;
    command[1]= MBL_MW_ACC_BMI160_DATA_ENABLE;
    command[2]= 0;
    command[3]= 1;
}

int mbl_mw_acc_bmi160_get_acceleration_data_g(CartesianFloat *data_g, const MblMwAccBmi160Config *config, uint8_t response[8]) {
    if (response[0] == MBL_MW_ACC_BMI160_MODULE && response[1] == MBL_MW_ACC_BMI160_DATA) {
        CartesianShort unscaled;
        std::memcpy(&unscaled, response + 2, sizeof(CartesianShort));

        data_g->x= unscaled.x / config->scale;
        data_g->y= unscaled.y / config->scale;
        data_g->z= unscaled.z / config->scale;
        return STATUS_OK;
    }
    return STATUS_INVALID_RESPONSE;
}

int mbl_mw_acc_bmi160_get_acceleration_data_mg(CartesianShort *data_mg, const MblMwAccBmi160Config *config, uint8_t response[8]) {
    if (response[0] == MBL_MW_ACC_BMI160_MODULE && response[1] == MBL_MW_ACC_BMI160_DATA) {
        CartesianShort unscaled;
        std::memcpy(&unscaled, response + 2, sizeof(CartesianShort));

        data_mg->x= int16_t ((unscaled.x / config->scale) * 1000);
        data_mg->y= int16_t ((unscaled.y / config->scale) * 1000);
        data_mg->z = int16_t((unscaled.z / config->scale) * 1000);
        return STATUS_OK;
    }
    return STATUS_INVALID_RESPONSE;
}
