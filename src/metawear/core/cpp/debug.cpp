#include "metawearboard_def.h"
#include "metawear/core/debug.h"

static const uint8_t DEBUG_MODULE= 0xfe;
static const uint8_t DEBUG_RESET= 0x1, DEBUG_BOOTLOADER= 0x2, DEBUG_RESET_GC= 0x5, DEBUG_DISCONNECT= 0x6;

void mbl_mw_debug_reset(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {DEBUG_MODULE, DEBUG_RESET};
    SEND_COMMAND;
}

void mbl_mw_debug_jump_to_bootloader(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {DEBUG_MODULE, DEBUG_BOOTLOADER};
    SEND_COMMAND;
}

void mbl_mw_debug_disconnect(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {DEBUG_MODULE, DEBUG_DISCONNECT};
    SEND_COMMAND;
}

void mbl_mw_debug_reset_after_gc(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {DEBUG_MODULE, DEBUG_RESET_GC};
    SEND_COMMAND;
}
