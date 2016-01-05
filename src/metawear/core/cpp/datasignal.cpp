#include "metawear/core/datasignal.h"

#include "constant.h"
#include "connection_def.h"
#include "datainterpreter.h"
#include "datasignal_private.h"
#include "metawearboard_def.h"

static void subscribe_default(MblMwDataSignal* signal) {
    if ((signal->header.register_id & 0x80) != 0x80 && signal->header.data_id == NO_DATA_ID) {
        uint8_t command[3] = { signal->header.module_id, signal->header.register_id, 1 };
        SEND_COMMAND_BOARD(signal->owner);
    }
}

static void unsubscribe_default(MblMwDataSignal* signal) {
    if ((signal->header.register_id & 0x80) != 0x80 && signal->header.data_id == NO_DATA_ID) {
        uint8_t command[3] = { signal->header.module_id, signal->header.register_id, 0 };
        SEND_COMMAND_BOARD(signal->owner);
    }
}

float number_to_firmware_default(MblMwDataSignal* source, float value) {
    return value;
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, ResponseConvertor convertor, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset, FnDataSignalFloat number_to_firmware, 
        FnDataSignal subscribe, FnDataSignal unsubscribe) : MblMwEvent(header, owner),
        convertor(convertor), n_channels(n_channels), channel_size(channel_size), is_signed(is_signed), 
        offset(offset), number_to_firmware(number_to_firmware), subscribe(subscribe), unsubscribe(unsubscribe) {
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, ResponseConvertor convertor, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset) : 
        MblMwDataSignal(header, owner, convertor, n_channels, channel_size, is_signed, offset, number_to_firmware_default, 
        subscribe_default, unsubscribe_default) {
}

uint8_t MblMwDataSignal::length() {
    return n_channels * channel_size;
}

uint8_t MblMwDataSignal::get_data_ubyte() {
    return ((length() - 1) << 5) | offset;
}

void MblMwDataSignal::set_channel_attr(uint8_t n_channels, uint8_t channel_size) {
    this->n_channels= n_channels;
    this->channel_size= channel_size;
}

void mbl_mw_datasignal_subscribe(MblMwDataSignal *signal, MblMwFnData received_data) {
    ResponseHeader header(signal->header);
    header.register_id&= 0x7f;

    switch (signal->convertor) {
    case ResponseConvertor::INT32:
        signal->owner->response_processors.emplace(header, convert_to_int32);
        break;
    case ResponseConvertor::UINT32:
        signal->owner->response_processors.emplace(header, convert_to_uint32);
        break;
    case ResponseConvertor::TEMPERATURE:
        signal->owner->response_processors.emplace(header, convert_to_temperature);
        break;
    case ResponseConvertor::BMP280_PRESSURE:
        signal->owner->response_processors.emplace(header, convert_to_bmp280_pressure);
        break;
    case ResponseConvertor::BMP280_ALTITUDE:
        signal->owner->response_processors.emplace(header, convert_to_bmp280_altitude);
        break;
    case ResponseConvertor::BMI160_ROTATION:
        signal->owner->response_processors.emplace(header, convert_to_bmi160_rotation);
        break;
    case ResponseConvertor::BMI160_ROTATION_SINGLE_AXIS:
        signal->owner->response_processors.emplace(header, convert_to_bmi160_rotation_single_axis);
        break;
    case ResponseConvertor::BMI160_ACCELERATION:
        signal->owner->response_processors.emplace(header, convert_to_bmi160_acceleration);
        break;
    case ResponseConvertor::BMI160_ACCELERATION_SINGLE_AXIS:
        signal->owner->response_processors.emplace(header, convert_to_bmi160_acceleration_single_axis);
        break;
    case ResponseConvertor::MMA8452Q_ACCELERATION:
        signal->owner->response_processors.emplace(header, convert_to_mma8452q_acceleration);
        break;
    case ResponseConvertor::MMA8452Q_ACCELERATION_SINGLE_AXIS:
        signal->owner->response_processors.emplace(header, convert_to_mma8452q_acceleration_single_axis);
        break;
    }
    signal->owner->data_signal_handlers.emplace(header, received_data);
    signal->subscribe(signal);
}

void mbl_mw_datasignal_unsubscribe(MblMwDataSignal *signal) {
    ResponseHeader header(signal->header);
    header.register_id&= 0x7f;

    signal->owner->response_processors.erase(header);
    signal->owner->data_signal_handlers.erase(header);
    signal->unsubscribe(signal);
}
