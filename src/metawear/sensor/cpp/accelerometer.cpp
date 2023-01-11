#include "metawear/sensor/accelerometer.h"
#include "metawear/sensor/accelerometer_bosch.h"
#include "accelerometer_private.h"
#include "accelerometer_bosch_private.h"
#include "utils.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"

#include <vector>

const float INVALID_SETTING= -1;

using std::stringstream;
using std::vector;

const uint8_t MBL_MW_MODULE_ACC_TYPE_BMI160 = 1;            ///< Constant identifying the BMI160 accelerometer module type
const uint8_t MBL_MW_MODULE_ACC_TYPE_BMI270 = 4;            ///< Constant identifying the BMI270 accelerometer module type

static const vector<float> BMI270_ODR_VALUES= {0.78125f, 1.5625f, 3.125f, 6.25f, 12.5f, 25.f, 50.f, 100.f, 200.f, 400.f, 800.f, 1600.f},
    BMI160_ODR_VALUES= {0.78125f, 1.5625f, 3.125f, 6.25f, 12.5f, 25.f, 50.f, 100.f, 200.f, 400.f, 800.f, 1600.f},
    BOSCH_FSR_VALUES= {2.f, 4.f, 8.f, 16.f};

// Helper function - init module
void init_accelerometer_module(MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        init_accelerometer_bmi160(board);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        init_accelerometer_bmi270(board);
        break;
    }
}

// Helper function - Free module
void free_accelerometer_module(MblMwMetaWearBoard *board) {
    if (!board->module_info.count(MBL_MW_MODULE_ACCELEROMETER)) {
        return;
    }
    
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        free_accelerometer_bosch(board);
        break;
    }
}

// Helper function - serialize
void serialize_accelerometer_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        serialize_accelerometer_bmi160_config(board, state);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        serialize_accelerometer_bmi270_config(board, state);
        break;
    }
}

// Helper function - deserialize
void deserialize_accelerometer_config(MblMwMetaWearBoard *board, uint8_t** state_stream) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        deserialize_accelerometer_bmi160_config(board, state_stream);
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        deserialize_accelerometer_bmi270_config(board, state_stream);
        break;
    }
}

// Name for the loggers
void create_acc_uri(const MblMwDataSignal* signal, stringstream& uri) {
    switch(signal->owner->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        create_acc_bmi270_uri(signal, uri);
        break;
    }
}

// Get acc signal
MblMwDataSignal* mbl_mw_acc_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        return mbl_mw_acc_bosch_get_acceleration_data_signal(board);
    }
    return nullptr;
}

// Get acc signal
MblMwDataSignal* mbl_mw_acc_get_high_freq_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    return mbl_mw_acc_get_packed_acceleration_data_signal(board);
}

// Get packed signal
MblMwDataSignal* mbl_mw_acc_get_packed_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    switch(board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        return mbl_mw_acc_bosch_get_packed_acceleration_data_signal(board);
    }
    return nullptr;
}

// Set odr
float mbl_mw_acc_set_odr(MblMwMetaWearBoard *board, float odr) {
    uint8_t index;
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        index= closest_index(BMI160_ODR_VALUES, odr);
        mbl_mw_acc_bmi160_set_odr(board, (MblMwAccBmi160Odr) index);
        return BMI160_ODR_VALUES[index];
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        index= closest_index(BMI270_ODR_VALUES, odr);
        mbl_mw_acc_bmi270_set_odr(board, (MblMwAccBmi270Odr) index);
        return BMI270_ODR_VALUES[index];
    }

    return INVALID_SETTING;
}

// Set range
float mbl_mw_acc_set_range(MblMwMetaWearBoard *board, float range) {
    uint8_t index;
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        index= closest_index(BOSCH_FSR_VALUES, range);
        mbl_mw_acc_bosch_set_range(board, (MblMwAccBoschRange) index);
        return BOSCH_FSR_VALUES[index];
    }

    return INVALID_SETTING;
}

// Write config
void mbl_mw_acc_write_acceleration_config(const MblMwMetaWearBoard* board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        mbl_mw_acc_bosch_write_acceleration_config(board);
        break;
    }
}

// Read config
void mbl_mw_acc_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        read_accelerometer_bosch_acceleration_config(board, context, completed);
        break;
    }
}

// Start acc
void mbl_mw_acc_start(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        mbl_mw_acc_bosch_start(board);
        break;
    }
}

// Stop acc
void mbl_mw_acc_stop(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        mbl_mw_acc_bosch_stop(board);
        break;
    }
}

// Enable sampling
void mbl_mw_acc_enable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        mbl_mw_acc_bosch_enable_acceleration_sampling(board);
        break;
    }    
}

// Disable sampling
void mbl_mw_acc_disable_acceleration_sampling(const MblMwMetaWearBoard *board) {
    switch (board->module_info.at(MBL_MW_MODULE_ACCELEROMETER).implementation) {
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    case MBL_MW_MODULE_ACC_TYPE_BMI270:
        mbl_mw_acc_bosch_disable_acceleration_sampling(board);
        break;
    }
}
