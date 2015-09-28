#include "metawear/gyro_bmi160.h"

#include <cstring>

static const uint8_t MBL_MW_GYRO_BMI160_POWER_MODE= 0x1, MBL_MW_GYRO_BMI160_DATA_ENABLE= 0x2, MBL_MW_GYRO_BMI160_CONFIG= 0x3;
static const float FSR_SCALE[5]= {16.4f, 32.8f, 65.6f, 131.2f, 262.4f};

struct MblMwGyroBmi160Config {
    uint8_t gyr_odr:4;
    uint8_t gyr_bwp:2;
    uint8_t :2;
    uint8_t gyr_range:3;
    uint8_t :5;
};

MblMwGyroBmi160Config* mbl_mw_gyro_bmi160_create_config() {
    MblMwGyroBmi160Config *config= new MblMwGyroBmi160Config;

    std::memset(config, 0, sizeof(MblMwGyroBmi160Config));
    config->gyr_bwp= 2;
    mbl_mw_gyro_bmi160_set_odr(config, MBL_MW_GYRO_BMI160_ODR_100HZ);
    mbl_mw_gyro_bmi160_set_range(config, MBL_MW_GYRO_BMI160_FSR_2000DPS);
    return config;
}

void mbl_mw_gyro_bmi160_free_config(MblMwGyroBmi160Config *config) {
    delete config;
}

void mbl_mw_gyro_bmi160_set_odr(MblMwGyroBmi160Config *config, MblMwGyroBmi160Odr odr) {
    config->gyr_odr= odr;
}

void mbl_mw_gyro_bmi160_set_range(MblMwGyroBmi160Config *config, MblMwGyroBmi160Range range) {
    config->gyr_range= range;
}

void mbl_mw_gyro_bmi160_write_config(uint8_t command[4], const MblMwGyroBmi160Config *config) {
    command[0]= MBL_MW_GYRO_BMI160_MODULE;
    command[1]= MBL_MW_GYRO_BMI160_CONFIG;
    std::memcpy(command + 2, config, sizeof(MblMwGyroBmi160Config));
}

void mbl_mw_gyro_bmi160_start(uint8_t command[3]) {
    command[0]= MBL_MW_GYRO_BMI160_MODULE;
    command[1]= MBL_MW_GYRO_BMI160_POWER_MODE;
    command[2]= 1;
}

void mbl_mw_gyro_bmi160_stop(uint8_t command[3]) {
    command[0]= MBL_MW_GYRO_BMI160_MODULE;
    command[1]= MBL_MW_GYRO_BMI160_POWER_MODE;
    command[2]= 0;
}

void mbl_mw_gyro_bmi160_enable_rotation_sampling(uint8_t command[4]) {
    command[0]= MBL_MW_GYRO_BMI160_MODULE;
    command[1]= MBL_MW_GYRO_BMI160_DATA_ENABLE;
    command[2]= 0x1;
    command[3]= 0x0;
}

void mbl_mw_gyro_bmi160_disable_rotation_sampling(uint8_t command[4]) {
    command[0]= MBL_MW_GYRO_BMI160_MODULE;
    command[1]= MBL_MW_GYRO_BMI160_DATA_ENABLE;
    command[2]= 0x0;
    command[3]= 0x1;
}

int mbl_mw_gyro_bmi160_get_rotation_data(CartesianFloat *data_dps, const MblMwGyroBmi160Config *config, uint8_t response[8]) {
    if (response[0] == MBL_MW_GYRO_BMI160_MODULE && response[1] == MBL_MW_GYRO_BMI160_DATA) {
        CartesianShort unscaled;
        std::memcpy(&unscaled, response + 2, sizeof(CartesianShort));

        data_dps->x= unscaled.x / FSR_SCALE[config->gyr_range];
        data_dps->y= unscaled.y / FSR_SCALE[config->gyr_range];
        data_dps->z= unscaled.z / FSR_SCALE[config->gyr_range];
        return STATUS_OK;
    }

    return STATUS_INVALID_RESPONSE;
}
