#include "metawear/sensor/accelerometer.h"
#include "accelerometer_bosch_private.h"
#include "accelerometer_mma8452q_private.h"
#include "utils.h"

#include "metawear/core/cpp/constant.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"

#include <vector>

using std::vector;

static const vector<float> MMA8452Q_ODR_VALUES= {800.f, 400.f, 200.f, 100.f, 50.f, 12.5f, 6.25f, 1.56f},
    MMA8452Q_FSR_VALUES= {2.f, 4.f, 8.f},
    BMI160_ODR_VALUES= {0.78125f, 1.5625f, 3.125f, 6.25f, 12.5f, 25.f, 50.f, 100.f, 200.f, 400.f, 800.f, 1600.f},
    BMA255_ODR_VALUES= {15.62f, 31.26f, 62.5f, 125.f, 250.f, 500.f, 1000.f},
    BOSCH_FSR_VALUES= {2.f, 4.f, 8.f, 16.f};

void init_accelerometer_module(MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        init_accelerometer_mma8452q(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        init_accelerometer_bmi160(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        init_accelerometer_bma255(board);
        break;
    }
}

MblMwDataSignal* mbl_mw_acc_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        return mbl_mw_acc_mma8452q_get_acceleration_data_signal(board);
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        return mbl_mw_acc_bosch_get_acceleration_data_signal(board);
    }
    return NULL;
}

void mbl_mw_acc_set_odr(MblMwMetaWearBoard *board, float odr) {
    uint8_t index;
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        index= closest_index(MMA8452Q_ODR_VALUES, odr);
        mbl_mw_acc_mma8452q_set_odr(board, (MblMwAccMma8452qOdr) index);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        index= closest_index(BMI160_ODR_VALUES, odr);
        mbl_mw_acc_bmi160_set_odr(board, (MblMwAccBmi160Odr) index);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        index= closest_index(BMA255_ODR_VALUES, odr);
        mbl_mw_acc_bma255_set_odr(board, (MblMwAccBma255Odr) index);
        break;
    }
}

void mbl_mw_acc_set_range(MblMwMetaWearBoard *board, float range) {
    uint8_t index;
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        index= closest_index(MMA8452Q_FSR_VALUES, range);
        mbl_mw_acc_mma8452q_set_range(board, (MblMwAccMma8452qRange) index);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        index= closest_index(BOSCH_FSR_VALUES, range);
        mbl_mw_acc_bosch_set_range(board, (MblMwAccBoschRange) index);
        break;
    }
}

void mbl_mw_acc_write_acceleration_config(const MblMwMetaWearBoard* board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_write_acceleration_config(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        mbl_mw_acc_bosch_write_acceleration_config(board);
        break;
    }
}

void mbl_mw_acc_start(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_start(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        mbl_mw_acc_bosch_start(board);
        break;
    }
}

void mbl_mw_acc_stop(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_stop(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        mbl_mw_acc_bosch_stop(board);
        break;
    }
}

void mbl_mw_acc_enable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_enable_acceleration_sampling(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        mbl_mw_acc_bosch_enable_acceleration_sampling(board);
        break;
    }    
}

void mbl_mw_acc_disable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_disable_acceleration_sampling(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        mbl_mw_acc_bosch_disable_acceleration_sampling(board);
        break;
    }
}
