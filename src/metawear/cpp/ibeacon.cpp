#include "metawear/ibeacon.h"

#include <cstring>

static const uint8_t IBEACON_MODULE = 0x7;

static const uint8_t IBEACON_ENABLE = 0x1, IBEACON_UUID = 0x2, IBEACON_MAJOR = 0x3, IBEACON_MINOR = 0x4, 
        IBEACON_RX = 0x5, IBEACON_TX = 0x6, IBEACON_PERIOD = 0x7;


#define SET_PARAM_NBYTES(reg, addr, len)\
    command[0]= IBEACON_MODULE;\
    command[1]= reg;\
    std::memcpy(command + 2, addr, len)

#define SET_PARAM_USHORT(reg, addr) SET_PARAM_NBYTES(reg, addr, 2)

#define SET_PARAM_UBYTE(reg, value)\
    command[0]= IBEACON_MODULE;\
    command[1]= reg;\
    command[2]= value;


void mbl_mw_ibeacon_set_major(uint8_t command[4], uint16_t major) {
    SET_PARAM_USHORT(IBEACON_MAJOR, &major);
}

void mbl_mw_ibeacon_set_minor(uint8_t command[4], uint16_t minor) {
    SET_PARAM_USHORT(IBEACON_MINOR, &minor);
}

void mbl_mw_ibeacon_set_period(uint8_t command[4], uint16_t period) {
    SET_PARAM_USHORT(IBEACON_PERIOD, &period);
}

void mbl_mw_ibeacon_set_tx_power(uint8_t command[3], int8_t tx_power) {
    SET_PARAM_UBYTE(IBEACON_TX, tx_power);
}

void mbl_mw_ibeacon_set_rx_power(uint8_t command[3], int8_t rx_power) {
    SET_PARAM_UBYTE(IBEACON_RX, rx_power);
}

void mbl_mw_ibeacon_set_uuid(uint8_t command[18], uint8_t ad_uuid[16]) {
    SET_PARAM_NBYTES(IBEACON_UUID, ad_uuid, 16);
}

void mbl_mw_ibeacon_enable(uint8_t command[3]) {
    SET_PARAM_UBYTE(IBEACON_ENABLE, 1);
}

void mbl_mw_ibeacon_disable(uint8_t command[3]) {
    SET_PARAM_UBYTE(IBEACON_ENABLE, 0);
}
