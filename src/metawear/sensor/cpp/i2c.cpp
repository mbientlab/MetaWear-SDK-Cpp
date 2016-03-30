#include "metawear/sensor/i2c.h"
#include "i2c_register.h"

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <vector>

using std::vector;

MblMwDataSignal* mbl_mw_i2c_get_data_signal(MblMwMetaWearBoard *board, uint8_t length, uint8_t id) {
    ResponseHeader header(MBL_MW_MODULE_I2C, READ_REGISTER(ORDINAL(I2cRegister::READ_WRITE)), id);

    if (!board->sensor_data_signals.count(header)) {
        board->sensor_data_signals[header]= new MblMwDataSignal(header, board, DataInterpreter::BYTE_ARRAY, 1, length, 0, 0);
    }
    return board->sensor_data_signals.at(header);
}

void mbl_mw_i2c_write(const MblMwMetaWearBoard *board, uint8_t device_addr, uint8_t register_addr, const uint8_t* value, uint8_t length) {
    vector<uint8_t> command= { MBL_MW_MODULE_I2C, ORDINAL(I2cRegister::READ_WRITE), device_addr, register_addr, 0xff, length};
    command.insert(command.end(), value, value + length);

    send_command(board, command.data(), (uint8_t) command.size());
}

void mbl_mw_i2c_read(const MblMwMetaWearBoard *board, uint8_t device_addr, uint8_t register_addr, uint8_t length, uint8_t id) {
    uint8_t command[6]= { MBL_MW_MODULE_I2C, READ_REGISTER(ORDINAL(I2cRegister::READ_WRITE)), device_addr, register_addr, id, length };
    SEND_COMMAND;
}
