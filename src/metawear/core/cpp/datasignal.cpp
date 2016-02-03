#include "metawear/core/datasignal.h"

#include "constant.h"
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

MblMwDataSignal::MblMwDataSignal(const MblMwEvent& event) : MblMwDataSignal(event.header, event.owner, DataInterpreter::UINT32, 0, 0, 0, 0) {
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset, FnDataSignalFloat number_to_firmware, 
        FnDataSignal subscribe, FnDataSignal unsubscribe) : MblMwEvent(header, owner),
        interpreter(interpreter), n_channels(n_channels), channel_size(channel_size), is_signed(is_signed), 
        offset(offset), number_to_firmware(number_to_firmware), subscribe(subscribe), unsubscribe(unsubscribe) {
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset) : 
        MblMwDataSignal(header, owner, interpreter, n_channels, channel_size, is_signed, offset, number_to_firmware_default, 
        subscribe_default, unsubscribe_default) {
}
MblMwDataSignal::~MblMwDataSignal() {
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
    signal->owner->data_signal_handlers.emplace(signal->header, received_data);
    signal->subscribe(signal);
}

void mbl_mw_datasignal_unsubscribe(MblMwDataSignal *signal) {
    signal->owner->data_signal_handlers.erase(signal->header);
    signal->unsubscribe(signal);
}
