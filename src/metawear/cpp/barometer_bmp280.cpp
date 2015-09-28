#include "metawear/barometer_bmp280.h"

#include <cstring>

static const float SCALE= 256.f;
static const uint8_t MBL_MW_BARO_BMP280_MODE= 0x3, MBL_MW_BARO_BMP280_ENABLE= 0x4;

struct MblMwBaroBmp280Config {
    uint8_t :2;
    uint8_t pressure_oversampling:3;
    uint8_t :3;
    uint8_t :2;
    uint8_t iir_filter:3;
    uint8_t standby_time: 3;
};

MblMwBaroBmp280Config* mbl_mw_baro_bmp280_create_config() {
    MblMwBaroBmp280Config *config= new MblMwBaroBmp280Config;

    std::memset(config, 0, sizeof(MblMwBaroBmp280Config));
    mbl_mw_baro_bmp280_set_oversampling(config, MBL_MW_BARO_BMP280_OVERSAMPLE_STANDARD);
    mbl_mw_baro_bmp280_set_iir_filter(config, MBL_MW_BARO_BMP280_IIR_FILTER_OFF);
    mbl_mw_baro_bmp280_set_standby_time(config, MBL_MW_BARO_BMP280_STANDBY_TIME_0_5MS);
    return config;
}

void mbl_mw_baro_bmp280_free_config(MblMwBaroBmp280Config *config) {
    delete config;
}

void mbl_mw_baro_bmp280_set_oversampling(MblMwBaroBmp280Config *config, MblMwBaroBmp280Oversampling oversampling) {
    config->pressure_oversampling= oversampling;
}

void mbl_mw_baro_bmp280_set_iir_filter(MblMwBaroBmp280Config *config, MblMwBaroBmp280IirFilter iir_filter) {
    config->iir_filter= iir_filter;
}

void mbl_mw_baro_bmp280_set_standby_time(MblMwBaroBmp280Config *config, MblMwBaroBmp280StandbyTime standby_time) {
    config->standby_time= standby_time;
}

void mbl_mw_baro_bmp280_write_config(uint8_t command[4], MblMwBaroBmp280Config *config) {
    command[0]= MBL_MW_BARO_BMP280_MODULE;
    command[1]= MBL_MW_BARO_BMP280_MODE;
    std::memcpy(command + 2, config, sizeof(MblMwBaroBmp280Config));
}

void mbl_mw_baro_bmp280_start(uint8_t command[4]) {
    command[0]= MBL_MW_BARO_BMP280_MODULE;
    command[1]= MBL_MW_BARO_BMP280_ENABLE;
    command[2]= 1;
    command[3]= 1;
}

void mbl_mw_baro_bmp280_stop(uint8_t command[4]) {
    command[0]= MBL_MW_BARO_BMP280_MODULE;
    command[1]= MBL_MW_BARO_BMP280_ENABLE;
    command[2]= 0;
    command[3]= 0;
}

int mbl_mw_baro_bmp280_get_pressure_data(float *data_pa, uint8_t response[6]) {
    if (response[0] == MBL_MW_BARO_BMP280_MODULE && response[1] == MBL_MW_BARO_BMP280_PRESSURE_DATA) {
        uint32_t unscaled_pa;
        std::memcpy(&unscaled_pa, response + 2, sizeof(uint32_t));

        *data_pa= unscaled_pa / SCALE;
        return STATUS_OK;
    }
    return STATUS_INVALID_RESPONSE;
}

int mbl_mw_baro_bmp280_get_altitude_data(float *data_m, uint8_t response[6]) {
    if (response[0] == MBL_MW_BARO_BMP280_MODULE && response[1] == MBL_MW_BARO_BMP280_ALTITUDE_DATA) {
        int32_t unscaled_m;
        std::memcpy(&unscaled_m, response + 2, sizeof(uint32_t));

        *data_m= unscaled_m / SCALE;
        return STATUS_OK;
    }
    return STATUS_INVALID_RESPONSE;
}
