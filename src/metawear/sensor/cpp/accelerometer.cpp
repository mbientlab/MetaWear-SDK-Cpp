#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>

#include "accelerometer_bmi160_private.h"
#include "accelerometer_mma8452q_private.h"
#include "metawear/sensor/accelerometer.h"

#include "metawear/core/cpp/constant.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"

using std::distance;
using std::fabs;
using std::min_element;
using std::transform;
using std::vector;

static const vector<float> MMA8452Q_ODR_VALUES= {800.f, 400.f, 200.f, 100.f, 50.f, 12.5f, 6.25f, 1.56f},
    MMA8452Q_FSR_VALUES= {2.f, 4.f, 8.f},
    BMI160_ODR_VALUES= {0.78125f, 1.5625f, 3.125f, 6.25f, 12.5f, 25.f, 50.f, 100.f, 200.f, 400.f, 800.f, 1600.f},
    BMI160_FSR_VALUES= {2.f, 4.f, 8.f, 16.f};

static inline uint8_t closest_index(const vector<float>& values, float key) {
    vector<float> differences;
    differences.resize(values.size());

    transform(values.begin(), values.end(), differences.begin(), [key](const float val) {
        return fabs(val - key);
    });
    return distance(differences.begin(), min_element(differences.begin(), differences.end()));
}

void init_accelerometer_module(MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        init_accelerometer_mma8452q(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        init_accelerometer_bmi160(board);
        break;
    }
}

MblMwDataSignal* mbl_mw_acc_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        return mbl_mw_acc_mma8452q_get_acceleration_data_signal(board);
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        return mbl_mw_acc_bmi160_get_acceleration_data_signal(board);
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
        index= closest_index(BMI160_FSR_VALUES, range);
        mbl_mw_acc_bmi160_set_range(board, (MblMwAccBmi160Range) index);
        break;
    }
}

void mbl_mw_acc_write_acceleration_config(const MblMwMetaWearBoard* board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_write_acceleration_config(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        mbl_mw_acc_bmi160_write_acceleration_config(board);
        break;
    }
}

void mbl_mw_acc_start(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_start(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        mbl_mw_acc_bmi160_start(board);
        break;
    }
}

void mbl_mw_acc_stop(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_stop(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        mbl_mw_acc_bmi160_stop(board);
        break;
    }
}

void mbl_mw_acc_enable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_enable_acceleration_sampling(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        mbl_mw_acc_bmi160_enable_acceleration_sampling(board);
        break;
    }    
}

void mbl_mw_acc_disable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_disable_acceleration_sampling(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        mbl_mw_acc_bmi160_disable_acceleration_sampling(board);
        break;
    }
}
