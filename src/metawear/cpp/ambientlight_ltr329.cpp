#include "metawear/ambientlight_ltr329.h"

#include <cstring>

const uint8_t MBL_MW_ALS_LTR329_ENABLE= 0x1, MBL_MW_ALS_LTR329_CONFIG= 0x2;

struct MblMwAlsLtr329Config {
    uint8_t :2;
    uint8_t als_gain:3;
    uint8_t :3;
    uint8_t als_measurement_rate:3;
    uint8_t als_integration_time:3;
    uint8_t :2;
};

MblMwAlsLtr329Config* mbl_mw_als_ltr329_create_config() {
    MblMwAlsLtr329Config *config= new MblMwAlsLtr329Config;

    std::memset(config, 0, sizeof(MblMwAlsLtr329Config));
    config->als_measurement_rate= MBL_MW_ALS_LTR329_RATE_500MS;
    return config;
}

void mbl_mw_als_ltr329_free_config(MblMwAlsLtr329Config *config) {
    delete config;
}

void mbl_mw_als_ltr329_set_gain(MblMwAlsLtr329Config *config, MblMwAlsLtr329Gain gain) {
    switch(gain) {
        case MBL_MW_ALS_LTR329_GAIN_48X:
        case MBL_MW_ALS_LTR329_GAIN_96X:
            config->als_gain= gain + 2;
            break;
        default:
            config->als_gain= gain;
            break;
    }
}

void mbl_mw_als_ltr329_set_integration_time(MblMwAlsLtr329Config *config, MblMwAlsLtr329IntegrationTime integration_time) {
    config->als_integration_time= integration_time;
}

void mbl_mw_als_ltr329_set_measurement_rate(MblMwAlsLtr329Config *config, MblMwAlsLtr329MeasurementRate measurement_rate) {
    config->als_measurement_rate= measurement_rate;
}

void mbl_mw_als_ltr329_write_config(uint8_t command[4], const MblMwAlsLtr329Config *config) {
    command[0]= MBL_MW_ALS_LTR329_MODULE;
    command[1]= MBL_MW_ALS_LTR329_CONFIG;
    std::memcpy(command + 2, config, sizeof(MblMwAlsLtr329Config));
}

void mbl_mw_als_ltr329_start(uint8_t command[3]) {
    command[0]= MBL_MW_ALS_LTR329_MODULE;
    command[1]= MBL_MW_ALS_LTR329_ENABLE;
    command[2]= 1;
}

void mbl_mw_als_ltr329_stop(uint8_t command[3]) {
    command[0]= MBL_MW_ALS_LTR329_MODULE;
    command[1]= MBL_MW_ALS_LTR329_ENABLE;
    command[2]= 0;
}

int mbl_mw_als_ltr329_get_illuminance_data(uint32_t *data_mlx, uint8_t response[6]) {
    if (response[0] == MBL_MW_ALS_LTR329_MODULE && response[1] == MBL_MW_ALS_LTR329_DATA) {
        std::memcpy(data_mlx, response + 2, sizeof(uint32_t));
        return STATUS_OK;
    }

    return STATUS_INVALID_RESPONSE;
}
