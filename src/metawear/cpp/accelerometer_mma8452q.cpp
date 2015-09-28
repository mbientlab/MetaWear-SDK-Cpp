#include "metawear/accelerometer_mma8452q.h"

#include <cstring>

const uint8_t MBL_MW_ACC_MMA8452Q_GLOBAL_START= 0x1, MBL_MW_ACC_MMA8452Q_DATA_ENABLE= 0x2, 
        MBL_MW_ACC_MMA8452Q_DATA_CONFIG= 0x3;

struct AccelerationBitField {
    uint8_t fs:2;
    uint8_t :2;
    uint8_t hpf_out:1;
    uint8_t :3;
    
    uint8_t hp_filter_cutoff;
    
    uint8_t active:1;
    uint8_t f_read:1;
    uint8_t lnoise:1;
    uint8_t dr:3;
    uint8_t aslp_rate:2;
    
    uint8_t ctrl_reg2;
    uint8_t aslp_count;
};

struct MblMwAccMma8452qConfig {
    AccelerationBitField accel_field;
};


MblMwAccMma8452qConfig* mbl_mw_acc_mma8452q_create_config() {
    MblMwAccMma8452qConfig *config= new MblMwAccMma8452qConfig;

    std::memset(config, 0, sizeof(MblMwAccMma8452qConfig));
    mbl_mw_acc_mma8452q_set_odr(config, MBL_MW_ACC_MMA8452Q_ODR_100HZ);
    mbl_mw_acc_mma8452q_set_range(config, MBL_MW_ACC_MMA8452Q_FSR_2G);
    return config;
}

void mbl_mw_acc_mma8452q_free_config(MblMwAccMma8452qConfig *config) {
    delete config;
}

void mbl_mw_acc_mma8452q_set_odr(MblMwAccMma8452qConfig *config, MblMwAccMma8452qOdr odr) {
    config->accel_field.dr= odr;
}

void mbl_mw_acc_mma8452q_set_range(MblMwAccMma8452qConfig *config, MblMwAccMma8452qRange range) {
    config->accel_field.fs= range;
}

void mbl_mw_acc_mma8452q_start(uint8_t command[3]) {
    command[0]= MBL_MW_ACC_MMA8452Q_MODULE;
    command[1]= MBL_MW_ACC_MMA8452Q_GLOBAL_START;
    command[2]= 1;
}

void mbl_mw_acc_mma8452q_stop(uint8_t command[3]) {
    command[0]= MBL_MW_ACC_MMA8452Q_MODULE;
    command[1]= MBL_MW_ACC_MMA8452Q_GLOBAL_START;
    command[2]= 0;
}

void mbl_mw_acc_mma8452q_enable_acceleration_sampling(uint8_t command[3]) {
    command[0]= MBL_MW_ACC_MMA8452Q_MODULE;
    command[1]= MBL_MW_ACC_MMA8452Q_DATA_ENABLE;
    command[2]= 1;
}

void mbl_mw_acc_mma8452q_disable_acceleration_sampling(uint8_t command[3]) {
    command[0]= MBL_MW_ACC_MMA8452Q_MODULE;
    command[1]= MBL_MW_ACC_MMA8452Q_DATA_ENABLE;
    command[2]= 0;
}

void mbl_mw_acc_mma8452q_write_acceleration_config(uint8_t command[7], const MblMwAccMma8452qConfig *config) {
    command[0]= MBL_MW_ACC_MMA8452Q_MODULE;
    command[1]= MBL_MW_ACC_MMA8452Q_DATA_CONFIG;
    std::memcpy(command + 2, &config->accel_field, sizeof(AccelerationBitField));
}

int mbl_mw_acc_mma8452q_get_acceleration_data_mg(CartesianShort *data_mg, const uint8_t response[8]) {
    if (response[0] == MBL_MW_ACC_MMA8452Q_MODULE && response[1] == MBL_MW_ACC_MMA8452Q_DATA) {
        std::memcpy(data_mg, response + 2, sizeof(CartesianShort));
        return STATUS_OK;
    }
    return STATUS_INVALID_RESPONSE;
}

int mbl_mw_acc_mma8452q_get_acceleration_data_g(CartesianFloat *data_g, const uint8_t response[8]) {
    if (response[0] == MBL_MW_ACC_MMA8452Q_MODULE && response[1] == MBL_MW_ACC_MMA8452Q_DATA) {
        CartesianShort milli_g;
        std::memcpy(&milli_g, response + 2, sizeof(CartesianShort));

        data_g->x= milli_g.x / 1000.f;
        data_g->y= milli_g.y / 1000.f;
        data_g->z= milli_g.z / 1000.f;
        return STATUS_OK;
    }
    return STATUS_INVALID_RESPONSE;
}
