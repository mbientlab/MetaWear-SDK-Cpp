#include "metawear/core/datasignal.h"

#include "datainterpreter.h"
#include "datasignal_private.h"
#include "metawearboard_def.h"

using std::vector;

MblMwDataSignal::MblMwDataSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner) : MblMwEvent(state_stream, owner), handler(nullptr) {
    interpreter = static_cast<DataInterpreter>(**state_stream);
    converter = static_cast<FirmwareConverter>(*(++(*state_stream)));
    n_channels = *(++(*state_stream));
    channel_size = *(++(*state_stream));
    is_signed = *(++(*state_stream));
    offset = *(++(*state_stream));

    (*state_stream)++;
}

MblMwDataSignal::MblMwDataSignal(const MblMwEvent& event) : MblMwDataSignal(event.header, event.owner, DataInterpreter::UINT32, 0, 0, 0, 0) {
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, FirmwareConverter converter, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset) : 
        MblMwEvent(header, owner), handler(nullptr), interpreter(interpreter), converter(converter), n_channels(n_channels), 
        channel_size(channel_size), is_signed(is_signed), offset(offset) {
    // Initialize readable signals to silent reads
    if (this->header.is_readable()) {
        this->header.enable_silent();
    }
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset) : 
        MblMwDataSignal(header, owner, interpreter, FirmwareConverter::DEFAULT, n_channels, channel_size, is_signed, offset) {
}

MblMwDataSignal::~MblMwDataSignal() {
}

void MblMwDataSignal::subscribe() {
    if (this->header.is_readable()) {
        this->header.disable_silent();
    } else {
        uint8_t command[3] = { header.module_id, header.register_id, 1 };
        SEND_COMMAND_BOARD(owner);
    }
}

void MblMwDataSignal::unsubscribe() {
    if (this->header.is_readable()) {
        this->header.enable_silent();
    } else {
        uint8_t command[3] = { header.module_id, header.register_id, 0 };
        SEND_COMMAND_BOARD(owner);
    }
}

void MblMwDataSignal::read() const {
    if (header.has_data_id()) {
        uint8_t command[2]= { header.module_id, header.register_id };
        SEND_COMMAND_BOARD(owner);
    } else {
        uint8_t command[3]= { header.module_id, header.register_id, header.data_id };
        SEND_COMMAND_BOARD(owner);
    }
}

void MblMwDataSignal::read(const void* parameters) const {
    read();
}

void MblMwDataSignal::serialize(vector<uint8_t>& state) const {
    MblMwEvent::serialize(state);

    state.push_back(static_cast<uint8_t>(interpreter));
    state.push_back(static_cast<uint8_t>(converter));
    state.push_back(n_channels);
    state.push_back(channel_size);
    state.push_back(is_signed);
    state.push_back(offset);
}

uint8_t MblMwDataSignal::length() const {
    return n_channels * channel_size;
}

uint8_t MblMwDataSignal::get_data_ubyte() const {
    return ((length() - 1) << 5) | offset;
}

void MblMwDataSignal::set_channel_attr(uint8_t n_channels, uint8_t channel_size) {
    this->n_channels= n_channels;
    this->channel_size= channel_size;
}

void MblMwDataSignal::make_signed() {
    is_signed= 1;
    if (unsigned_to_signed.count(interpreter)) {
        interpreter= unsigned_to_signed.at(interpreter);
    }
}

void MblMwDataSignal::make_unsigned() {
    is_signed= 0;
    if (signed_to_unsigned.count(interpreter)) {
        interpreter= signed_to_unsigned.at(interpreter);
    }
}

void mbl_mw_datasignal_subscribe(MblMwDataSignal *signal, MblMwFnData received_data) {
    signal->handler= received_data;
    signal->subscribe();
}

void mbl_mw_datasignal_unsubscribe(MblMwDataSignal *signal) {
    signal->handler= nullptr;
    signal->unsubscribe();
}

void mbl_mw_datasignal_read(const MblMwDataSignal* signal) {
    signal->read();
}

void mbl_mw_datasignal_read_with_parameters(const MblMwDataSignal* signal, const void* parameters) {
    signal->read(parameters);
}

int32_t mbl_mw_datasignal_is_readable(const MblMwDataSignal* signal) {
    return signal->header.is_readable();
}
