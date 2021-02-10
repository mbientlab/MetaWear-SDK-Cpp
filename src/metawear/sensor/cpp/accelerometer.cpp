#include "metawear/sensor/accelerometer.h"
#include "metawear/sensor/accelerometer_bosch.h"
#include "metawear/sensor/accelerometer_mma8452q.h"
#include "accelerometer_private.h"
#include "accelerometer_bosch_private.h"
#include "accelerometer_mma8452q_private.h"
#include "utils.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"

#include <vector>

const float INVALID_SETTING= -1;

using std::stringstream;
using std::vector;

static const vector<float> MMA8452Q_ODR_VALUES= {800.f, 400.f, 200.f, 100.f, 50.f, 12.5f, 6.25f, 1.56f},
    MMA8452Q_FSR_VALUES= {2.f, 4.f, 8.f},
    BMI270_ODR_VALUES= {0.78125f, 1.5625f, 3.125f, 6.25f, 12.5f, 25.f, 50.f, 100.f, 200.f, 400.f, 800.f, 1600.f},
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        init_accelerometer_bmi270(board);
        break;
    }
}

void free_accelerometer_module(MblMwMetaWearBoard *board) {
    if (!board->module_info.count(MBL_MW_MODULE_ACCELEROMETER)) {
        return;
    }
    
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        free_accelerometer_mma8452q(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        free_accelerometer_bosch(board);
        break;
    }
}

void serialize_accelerometer_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        serialize_accelerometer_mma8452q_config(board, state);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        serialize_accelerometer_bmi160_config(board, state);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        serialize_accelerometer_bma255_config(board, state);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        serialize_accelerometer_bmi270_config(board, state);
        break;
    }
}

void deserialize_accelerometer_config(MblMwMetaWearBoard *board, uint8_t** state_stream) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        deserialize_accelerometer_mma8452q_config(board, state_stream);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        deserialize_accelerometer_bmi160_config(board, state_stream);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        deserialize_accelerometer_bma255_config(board, state_stream);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        deserialize_accelerometer_bmi270_config(board, state_stream);
        break;
    }
}

void create_acc_uri(const MblMwDataSignal* signal, stringstream& uri) {
    switch(signal->owner->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        create_acc_mma8452q_uri(signal, uri);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        create_acc_bosch_uri(signal, uri);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        create_acc_bmi270_uri(signal, uri);
        break;
    }
}

MblMwDataSignal* mbl_mw_acc_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        return mbl_mw_acc_mma8452q_get_acceleration_data_signal(board);
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        return mbl_mw_acc_bosch_get_acceleration_data_signal(board);
    }
    return nullptr;
}

MblMwDataSignal* mbl_mw_acc_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    return mbl_mw_acc_get_packed_acceleration_data_signal(board);
}

MblMwDataSignal* mbl_mw_acc_get_packed_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        return mbl_mw_acc_mma8452q_get_packed_acceleration_data_signal(board);
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        return mbl_mw_acc_bosch_get_packed_acceleration_data_signal(board);
    }
    return nullptr;
}

float mbl_mw_acc_set_odr(MblMwMetaWearBoard *board, float odr) {
    uint8_t index;
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        index= closest_index(MMA8452Q_ODR_VALUES, odr);
        mbl_mw_acc_mma8452q_set_odr(board, (MblMwAccMma8452qOdr) index);
        return MMA8452Q_ODR_VALUES[index];
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        index= closest_index(BMI160_ODR_VALUES, odr);
        mbl_mw_acc_bmi160_set_odr(board, (MblMwAccBmi160Odr) index);
        return BMI160_ODR_VALUES[index];
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        index= closest_index(BMA255_ODR_VALUES, odr);
        mbl_mw_acc_bma255_set_odr(board, (MblMwAccBma255Odr) index);
        return BMA255_ODR_VALUES[index];
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        index= closest_index(BMI270_ODR_VALUES, odr);
        mbl_mw_acc_bmi270_set_odr(board, (MblMwAccBmi270Odr) index);
        return BMI270_ODR_VALUES[index];
    }

    return INVALID_SETTING;
}

float mbl_mw_acc_set_range(MblMwMetaWearBoard *board, float range) {
    uint8_t index;
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        index= closest_index(MMA8452Q_FSR_VALUES, range);
        mbl_mw_acc_mma8452q_set_range(board, (MblMwAccMma8452qRange) index);
        return MMA8452Q_FSR_VALUES[index];
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        index= closest_index(BOSCH_FSR_VALUES, range);
        mbl_mw_acc_bosch_set_range(board, (MblMwAccBoschRange) index);
        return BOSCH_FSR_VALUES[index];
    }

    return INVALID_SETTING;
}

void mbl_mw_acc_write_acceleration_config(const MblMwMetaWearBoard* board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        mbl_mw_acc_mma8452q_write_acceleration_config(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        mbl_mw_acc_bosch_write_acceleration_config(board);
        break;
    }
}

void mbl_mw_acc_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        read_accelerometer_mma8452q_acceleration_config(board, context, completed);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        read_accelerometer_bosch_acceleration_config(board, context, completed);
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
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
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        mbl_mw_acc_bosch_disable_acceleration_sampling(board);
        break;
    }
}
