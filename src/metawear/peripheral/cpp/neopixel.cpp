#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/peripheral/neopixel.h"

#include <cstring>

static const uint8_t NEOPIXEL_MODULE= 6;
static const uint8_t NEOPIXEL_INITIALIZE= 1, NEOPIXEL_HOLD= 2, NEOPIXEL_CLEAR= 3, NEOPIXEL_PIXEL= 4,
        NEOPIXEL_ROTATE= 5, NEOPIXEL_DEINITIALIZE= 6;

static const uint8_t SPEED_SLOW= 0, SPEED_FAST= 1;
static const uint8_t HOLD_ENABLE= 1, HOLD_DISABLE= 0;

static const uint8_t ROTATE_INDEFINITELY= 0xff;

static void inline init_strand(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, uint8_t speed, MblMwNeoPixelColorOrdering ordering) {
    uint8_t command[6]= {NEOPIXEL_MODULE, NEOPIXEL_INITIALIZE, strand, static_cast<uint8_t>(ordering | (speed << 2)), gpio_pin, n_pixels};
    SEND_COMMAND;
}

void mbl_mw_neopixel_init_slow_strand(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, MblMwNeoPixelColorOrdering ordering) {
    init_strand(board, strand, gpio_pin, n_pixels, SPEED_SLOW, ordering);
}

void mbl_mw_neopixel_init_fast_strand(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, MblMwNeoPixelColorOrdering ordering) {
    init_strand(board, strand, gpio_pin, n_pixels, SPEED_FAST, ordering);
}

void mbl_mw_neopixel_free_strand(const MblMwMetaWearBoard *board, uint8_t strand) {
    uint8_t command[3]= {NEOPIXEL_MODULE, NEOPIXEL_DEINITIALIZE, strand};
    SEND_COMMAND;
}

void mbl_mw_neopixel_enable_hold(const MblMwMetaWearBoard *board, uint8_t strand) {
    uint8_t command[4]= {NEOPIXEL_MODULE, NEOPIXEL_HOLD, strand, HOLD_ENABLE};
    SEND_COMMAND;
}

void mbl_mw_neopixel_disable_hold(const MblMwMetaWearBoard *board, uint8_t strand) {
    uint8_t command[4]= {NEOPIXEL_MODULE, NEOPIXEL_HOLD, strand, HOLD_DISABLE};
    SEND_COMMAND;
}

void mbl_mw_neopixel_clear(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t start, uint8_t end) {
    uint8_t command[5]= {NEOPIXEL_MODULE, NEOPIXEL_CLEAR, strand, start, end};
    SEND_COMMAND;
}

void mbl_mw_neopixel_set_color(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t command[7]= {NEOPIXEL_MODULE, NEOPIXEL_PIXEL, strand, pixel, red, green, blue};
    SEND_COMMAND;
}

void mbl_mw_neopixel_rotate(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t count, uint16_t period_ms, MblMwNeoPixelRotDirection direction) {
    uint8_t command[7]= {NEOPIXEL_MODULE, NEOPIXEL_ROTATE, strand, 
            static_cast<uint8_t>((direction != MBL_MW_NP_ROT_DIR_TOWARDS) ? MBL_MW_NP_ROT_DIR_AWAY : MBL_MW_NP_ROT_DIR_TOWARDS),
            count};

    memcpy(command + 5, &period_ms, 2);
    SEND_COMMAND;
}

void mbl_mw_neopixel_rotate_indefinitely(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t period_ms, MblMwNeoPixelRotDirection direction) {
    mbl_mw_neopixel_rotate(board, strand, ROTATE_INDEFINITELY, period_ms, direction);
}

void mbl_mw_neopixel_stop_rotation(const MblMwMetaWearBoard *board, uint8_t strand) {
    uint8_t command[7]= {NEOPIXEL_MODULE, NEOPIXEL_ROTATE, strand};

    std::memset(command + 3, 0, 4);
    SEND_COMMAND;
}
