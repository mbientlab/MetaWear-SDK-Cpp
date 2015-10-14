#include "metawear/datasignal.h"

#include "metawear/ambientlight_ltr329.h"
#include "metawear/barometer_bmp280.h"
#include "metawear/dataprocessor.h"
#include "metawear/switch.h"
#include "metawear/gyro_bmi160.h"
#include "metawear/accelerometer_bmi160.h"
#include "metawear/accelerometer_mma8452q.h"

#include "constant.h"
#include "datainterpreter.h"
#include "connection_def.h"
#include "metawearboard_def.h"
#include "responseheader.h"

struct DataSignalId {
    enum :uint8_t {
        SWITCH= 0, ACC_MMA8452Q, ACC_BMI160, BARO_BMP280_PRESSURE, BARO_BMP280_ALTITUDE, GYRO_BMI160, ALS_LTR329
    };
};

enum ProcessorId {
    ACCUMULATOR= 0x2,
    COMPARISON= 0x6,
    MATH= 0x9
};

const MblMwDataSignal* mbl_mw_acc_bmi160_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    return &board->sensor_data_signals.at(DataSignalId::ACC_BMI160);
}
 
const MblMwDataSignal* mbl_mw_acc_mma8452q_get_acceleration_data_signal(const MblMwMetaWearBoard *board) {
    return &board->sensor_data_signals.at(DataSignalId::ACC_MMA8452Q);
}
 
const MblMwDataSignal* mbl_mw_als_ltr329_get_illuminance_data_signal(const MblMwMetaWearBoard *board) {
    return &board->sensor_data_signals.at(DataSignalId::ALS_LTR329);
}
 
const MblMwDataSignal* mbl_mw_baro_bmp280_get_pressure_data_signal(const MblMwMetaWearBoard *board) {
    return &board->sensor_data_signals.at(DataSignalId::BARO_BMP280_PRESSURE);
}
 
const MblMwDataSignal* mbl_mw_baro_bmp280_get_altitude_data_signal(const MblMwMetaWearBoard *board) {
    return &board->sensor_data_signals.at(DataSignalId::BARO_BMP280_ALTITUDE);
}
 
const MblMwDataSignal* mbl_mw_gyro_bmi160_get_rotation_data_signal(const MblMwMetaWearBoard *board) {
    return &board->sensor_data_signals.at(DataSignalId::GYRO_BMI160);
}
 
const MblMwDataSignal* mbl_mw_switch_get_state_data_signal(const MblMwMetaWearBoard *board) {
    return &board->sensor_data_signals.at(DataSignalId::SWITCH);
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, uint8_t data_type_id, 
        uint8_t is_signed, uint8_t length, uint8_t offset, MblMwMetaWearBoard *owner) :
    header{header.module_id, header.register_id, header.data_id}, data_type_id(data_type_id), is_signed(is_signed), length(length), 
    offset(offset), owner(owner) {
}

void mbl_mw_datasignal_subscribe(MblMwDataSignal *signal) {
    signal->owner->active_data_signals.emplace(signal->header, signal);

    if (signal == mbl_mw_acc_bmi160_get_acceleration_data_signal(signal->owner)) {
        signal->owner->response_processors.emplace(BMI160_ACCEL_RESPONSE_HEADER, convert_to_bmi160_acceleration);
    } else if (signal == mbl_mw_acc_mma8452q_get_acceleration_data_signal(signal->owner)) {
        signal->owner->response_processors.emplace(MMA8452Q_ACCEL_RESPONSE_HEADER, convert_to_mma8452q_acceleration);
    }

    if (signal->header.data_id == NO_DATA_ID) {
        uint8_t command[3] = { signal->header.module_id, signal->header.register_id, 1 };
        SEND_COMMAND_BOARD(signal->owner);
    }
}

void mbl_mw_datasignal_unsubscribe(MblMwDataSignal *signal) {
    signal->owner->active_data_signals.erase(signal->header);

    if (signal->header.data_id == NO_DATA_ID) {
        uint8_t command[3] = { signal->header.module_id, signal->header.register_id, 0 };
        SEND_COMMAND_BOARD(signal->owner);
    }
}

// data processor functions
/*
void mbl_mw_dataprocessor_update_id(MblMwDataSignal *signal, uint8_t new_id) {
    signal->header.data_id= new_id;
}

#define INITIALIZE_DATA_ATTR(attr)\
    command[0]= DATA_PROCESSOR_MODULE;\
    command[1]= DATA_PROCESSOR_ADD;\
    command[2]= attr->header.module_id;\
    command[3]= attr->header.register_id;\
    command[4]= attr->header.data_id;\
    command[5]= attr->offset | ((attr->length - 1) << 5)

#define PACK_LENGTHS(index, in, out) command[index]= (out - 1) | (in - 1) << 2

DataSignal* mbl_mw_data_processor_accumulator(uint8_t command[8], const DataSignal *signal) {
    INITIALIZE_DATA_ATTR(signal);
    command[6]= ACCUMULATOR;
    command[7]= (signal->length - 1) | ((signal->length - 1) << 2);

    uint8_t signal_values[]= {DATA_PROCESSOR_MODULE, DATA_PROCESSOR_ADD, NO_DATA_ID, signal->is_signed, signal->length, 0};
    DataSignal *next_signal= new DataSignal;
    
    std::memcpy(next_signal, signal_values, sizeof(DataSignal));
    return next_signal;
}

DataSignal* mbl_mw_data_processor_comparison(uint8_t command[14], const DataSignal *signal, uint8_t is_signed, 
        Comparison op, int32_t reference) {
    INITIALIZE_DATA_ATTR(signal);
    command[6]= COMPARISON;
    command[7]= is_signed;
    command[8]= op;
    std::memcpy(command + 10, &reference, 4);

    uint8_t signal_values[]= {DATA_PROCESSOR_MODULE, DATA_PROCESSOR_ADD, NO_DATA_ID, is_signed, signal->length, 0};
    DataSignal *next_signal= new DataSignal;
    
    std::memcpy(next_signal, signal_values, sizeof(DataSignal));
    return next_signal;
}

DataSignal* mbl_mw_data_processor_math(uint8_t command[13], const DataSignal *signal, uint8_t is_signed, 
        Arithmetic op, int32_t rhs) {
    uint8_t output_size= 4;
    INITIALIZE_DATA_ATTR(signal);
    command[6]= MATH;
    PACK_LENGTHS(7, signal->length, output_size) | (is_signed << 4);
    command[8]= op;
    std::memcpy(command + 9, &rhs, 4);

    uint8_t signal_values[]= {DATA_PROCESSOR_MODULE, DATA_PROCESSOR_ADD, NO_DATA_ID, is_signed, output_size, 0};
    DataSignal *next_signal= new DataSignal;
    
    std::memcpy(next_signal, signal_values, sizeof(DataSignal));
    return next_signal;
}
*/
