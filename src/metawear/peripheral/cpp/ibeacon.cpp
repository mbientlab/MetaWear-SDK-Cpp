#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "metawear/peripheral/ibeacon.h"

#include <cstring>

static const uint8_t IBEACON_MODULE = 0x7;

static const uint8_t IBEACON_ENABLE = 0x1, IBEACON_UUID = 0x2, IBEACON_MAJOR = 0x3, IBEACON_MINOR = 0x4,
IBEACON_RX = 0x5, IBEACON_TX = 0x6, IBEACON_PERIOD = 0x7;

#define SET_PARAM_NBYTES(reg, addr, len)\
    uint8_t command[len + 2]= {IBEACON_MODULE, reg};\
    std::memcpy(command + 2, addr, len);\
    SEND_COMMAND;

#define SET_PARAM_USHORT(reg, addr) SET_PARAM_NBYTES(reg, addr, 2)

#define SET_PARAM_UBYTE(reg, value)\
    uint8_t command[3]= {IBEACON_MODULE, reg, value};\
    SEND_COMMAND;

void mbl_mw_ibeacon_set_major(const MblMwMetaWearBoard *board, uint16_t major) {
    SET_PARAM_USHORT(IBEACON_MAJOR, &major);
}

void mbl_mw_ibeacon_set_major_signal(MblMwMetaWearBoard *board, const MblMwDataSignal* major) {
    EventDataParameter signal_data_token= {major->length(), major->offset, 0};

    set_data_token(board, &signal_data_token);
    mbl_mw_ibeacon_set_major(board, 0);
    clear_data_token(board);
}

void mbl_mw_ibeacon_set_minor(const MblMwMetaWearBoard *board, uint16_t minor) {
    SET_PARAM_USHORT(IBEACON_MINOR, &minor);
}

void mbl_mw_ibeacon_set_minor_signal(MblMwMetaWearBoard *board, const MblMwDataSignal* minor) {
    EventDataParameter signal_data_token= {minor->length(), minor->offset, 0};

    set_data_token(board, &signal_data_token);
    mbl_mw_ibeacon_set_minor(board, 0);
    clear_data_token(board);
}

void mbl_mw_ibeacon_set_period(const MblMwMetaWearBoard *board, uint16_t period) {
    SET_PARAM_USHORT(IBEACON_PERIOD, &period);
}

void mbl_mw_ibeacon_set_tx_power(const MblMwMetaWearBoard *board, int8_t tx_power) {
    SET_PARAM_UBYTE(IBEACON_TX, static_cast<uint8_t>(tx_power));
}

void mbl_mw_ibeacon_set_rx_power(const MblMwMetaWearBoard *board, int8_t rx_power) {
    SET_PARAM_UBYTE(IBEACON_RX, static_cast<uint8_t>(rx_power));
}

void mbl_mw_ibeacon_set_uuid(const MblMwMetaWearBoard *board, uint8_t ad_uuid[16]) {
    SET_PARAM_NBYTES(IBEACON_UUID, ad_uuid, 16);
}

void mbl_mw_ibeacon_enable(const MblMwMetaWearBoard *board) {
    SET_PARAM_UBYTE(IBEACON_ENABLE, 1);
}

void mbl_mw_ibeacon_disable(const MblMwMetaWearBoard *board) {
    SET_PARAM_UBYTE(IBEACON_ENABLE, 0);
}
