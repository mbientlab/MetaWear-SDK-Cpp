#include "metawear/sensor/i2c.h"
#include "metawear/sensor/spi.h"
#include "serialpassthrough_private.h"
#include "serialpassthrough_register.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"

#include <cstring>
#include <vector>

using std::forward_as_tuple;
using std::memcpy;
using std::piecewise_construct;
using std::vector;

const uint8_t SPI_REVISION= 1;

struct SpiBitFields {
    uint8_t slave_select_pin, clock_pin, mosi_pin, miso_pin;
    uint8_t lsb_first:1;
    uint8_t mode:2;
    uint8_t frequency:3;
    uint8_t use_nrf_pins:1;
    uint8_t :1;

    SpiBitFields(const MblMwSpiParameters* parameters);
};

SpiBitFields::SpiBitFields(const MblMwSpiParameters* parameters) {
    memcpy(this, &parameters->slave_select_pin, 4);
    *(((uint8_t*) this) + 4)= 0;
    lsb_first= parameters->lsb_first;
    mode= parameters->mode;
    frequency= parameters->frequency;
    use_nrf_pins= parameters->use_nrf_pins;
}

MblMwI2cSignal::MblMwI2cSignal(ResponseHeader header, MblMwMetaWearBoard* owner, uint8_t length) : 
        MblMwDataSignal(header, owner, DataInterpreter::BYTE_ARRAY, 1, length, 0, 0) {
}

MblMwI2cSignal::MblMwI2cSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner) : MblMwDataSignal(state_stream, owner) {
}

void MblMwI2cSignal::read() const {
}

void MblMwI2cSignal::read(const void* parameters) const {
    const MblMwI2cReadParameters* read_parameters= (const MblMwI2cReadParameters*) parameters;
    uint8_t command[6]= { header.module_id, header.register_id, read_parameters->device_addr, read_parameters->register_addr, header.data_id, length() };
    SEND_COMMAND_BOARD(owner);
}

MblMwSpiSignal::MblMwSpiSignal(ResponseHeader header, MblMwMetaWearBoard* owner, uint8_t length) : 
        MblMwDataSignal(header, owner, DataInterpreter::BYTE_ARRAY, 1, length, 0, 0) {
}

MblMwSpiSignal::MblMwSpiSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner) : MblMwDataSignal(state_stream, owner) {
}

void MblMwSpiSignal::read() const {
}

void MblMwSpiSignal::read(const void* parameters) const {
    const MblMwSpiParameters* read_parameters= (const MblMwSpiParameters*) parameters;
    SpiBitFields fields(read_parameters);

    vector<uint8_t> command= { header.module_id, header.register_id };
    command.insert(command.end(), (uint8_t*) &fields, ((uint8_t*) &fields) + sizeof(SpiBitFields));
    command.insert(command.end(), (length() - 1) | (header.data_id << 4));
    if (read_parameters->data != nullptr) {
        command.insert(command.end(), read_parameters->data, read_parameters->data + read_parameters->data_length);
    }

    send_command(owner, command.data(), (uint8_t) command.size());
}

void init_serialpassthrough_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_I2C) && board->module_info.at(MBL_MW_MODULE_I2C).present) {
        board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_I2C, READ_REGISTER(ORDINAL(SerialPassthroughRegister::I2C_READ_WRITE))),
            forward_as_tuple(response_handler_data_with_id));

        if (board->module_info.at(MBL_MW_MODULE_I2C).revision >= SPI_REVISION) {
            board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_I2C, READ_REGISTER(ORDINAL(SerialPassthroughRegister::SPI_READ_WRITE))),
            forward_as_tuple(response_handler_data_with_id));
        }
    }
}

MblMwDataSignal* mbl_mw_i2c_get_data_signal(MblMwMetaWearBoard *board, uint8_t length, uint8_t id) {
    ResponseHeader header(MBL_MW_MODULE_I2C, READ_REGISTER(ORDINAL(SerialPassthroughRegister::I2C_READ_WRITE)), id);

    if (!board->module_events.count(header)) {
        board->module_events[header]= new MblMwI2cSignal(header, board, length);
    }
    return dynamic_cast<MblMwDataSignal*>(board->module_events.at(header));
}

MblMwDataSignal* mbl_mw_spi_get_data_signal(MblMwMetaWearBoard* board, uint8_t length, uint8_t id) {
    if (board->module_info.count(MBL_MW_MODULE_I2C) && board->module_info.at(MBL_MW_MODULE_I2C).present && 
            board->module_info.at(MBL_MW_MODULE_I2C).revision >= SPI_REVISION) {
        ResponseHeader header(MBL_MW_MODULE_I2C, READ_REGISTER(ORDINAL(SerialPassthroughRegister::SPI_READ_WRITE)), id);

        if (!board->module_events.count(header)) {
            board->module_events[header]= new MblMwSpiSignal(header, board, length);
        }
        return dynamic_cast<MblMwDataSignal*>(board->module_events.at(header));
    }
    return nullptr;
}

void mbl_mw_i2c_write(const MblMwMetaWearBoard *board, uint8_t device_addr, uint8_t register_addr, const uint8_t* value, uint8_t length) {
    vector<uint8_t> command= { MBL_MW_MODULE_I2C, ORDINAL(SerialPassthroughRegister::I2C_READ_WRITE), device_addr, register_addr, 0xff, length};
    command.insert(command.end(), value, value + length);

    send_command(board, command.data(), (uint8_t) command.size());
}

void mbl_mw_spi_write(const MblMwMetaWearBoard* board, const MblMwSpiParameters* parameters) {
    const MblMwSpiParameters* read_parameters= (const MblMwSpiParameters*) parameters;
    SpiBitFields fields(read_parameters);

    vector<uint8_t> command= { MBL_MW_MODULE_I2C, ORDINAL(SerialPassthroughRegister::SPI_READ_WRITE) };
    command.insert(command.end(), (uint8_t*) &fields, ((uint8_t*) &fields) + sizeof(SpiBitFields));
    command.insert(command.end(), read_parameters->data, read_parameters->data + read_parameters->data_length);

    send_command(board, command.data(), (uint8_t) command.size());
}
