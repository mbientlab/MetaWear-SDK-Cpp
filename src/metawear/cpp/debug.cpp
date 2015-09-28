#include "metawear/debug.h"

static const uint8_t DEBUG_MODULE= 0xfe;

static const uint8_t DEBUG_RESET= 0x1, DEBUG_BOOTLOADER= 0x2, DEBUG_RESET_GC= 0x5, DEBUG_DISCONNECT= 0x6;

void mbl_mw_debug_reset(uint8_t command[2]) {
    command[0]= DEBUG_MODULE;
    command[1]= DEBUG_RESET;
}

void mbl_mw_debug_jump_to_bootloader(uint8_t command[2]) {
    command[0]= DEBUG_MODULE;
    command[1]= DEBUG_BOOTLOADER;
}

void mbl_mw_debug_disconnect(uint8_t command[2]) {
    command[0]= DEBUG_MODULE;
    command[1]= DEBUG_DISCONNECT;
}

void mbl_mw_debug_reset_after_gc(uint8_t command[2]) {
    command[0]= DEBUG_MODULE;
    command[1]= DEBUG_RESET_GC;
}
