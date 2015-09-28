#include "metawear/neopixel.h"

#include <cstring>

static const uint8_t NEOPIXEL_MODULE= 6;
static const uint8_t NEOPIXEL_INITIALIZE= 1, NEOPIXEL_HOLD= 2, NEOPIXEL_CLEAR= 3, NEOPIXEL_PIXEL= 4,
        NEOPIXEL_ROTATE= 5, NEOPIXEL_DEINITIALIZE= 6;

static const uint8_t SPEED_SLOW= 0, SPEED_FAST= 1;
static const uint8_t HOLD_ENABLE= 1, HOLD_DISABLE= 0;

static const uint8_t ROTATE_INDEFINITELY= 0xff;

static void inline init_strand(uint8_t command[6], uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, uint8_t speed, 
        MblMwNeoPixelColorOrdering ordering) {
    command[0]= NEOPIXEL_MODULE;
    command[1]= NEOPIXEL_INITIALIZE;
    command[2]= strand;
    command[3]= ordering | (speed << 2);
    command[4]= gpio_pin;
    command[5]= n_pixels;
}

void mbl_mw_neopixel_init_slow_strand(uint8_t command[6], uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, MblMwNeoPixelColorOrdering ordering) {
    init_strand(command, strand, gpio_pin, n_pixels, SPEED_SLOW, ordering);
}

void mbl_mw_neopixel_init_fast_strand(uint8_t command[6], uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, MblMwNeoPixelColorOrdering ordering) {
    init_strand(command, strand, gpio_pin, n_pixels, SPEED_FAST, ordering);
}

void mbl_mw_neopixel_free_strand(uint8_t command[3], uint8_t strand) {
    command[0]= NEOPIXEL_MODULE;
    command[1]= NEOPIXEL_DEINITIALIZE;
    command[2]= strand;
}

void mbl_mw_neopixel_enable_hold(uint8_t command[4], uint8_t strand) {
    command[0]= NEOPIXEL_MODULE;
    command[1]= NEOPIXEL_HOLD;
    command[2]= strand;
    command[3]= HOLD_ENABLE;
}

void mbl_mw_neopixel_disable_hold(uint8_t command[4], uint8_t strand) {
    command[0]= NEOPIXEL_MODULE;
    command[1]= NEOPIXEL_HOLD;
    command[2]= strand;
    command[3]= HOLD_DISABLE;
}

void mbl_mw_neopixel_clear(uint8_t command[5], uint8_t strand, uint8_t start, uint8_t end) {
    command[0]= NEOPIXEL_MODULE;
    command[1]= NEOPIXEL_CLEAR;
    command[2]= strand;
    command[3]= start;
    command[4]= end;
}

void mbl_mw_neopixel_set_color(uint8_t command[7], uint8_t strand, uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
    command[0]= NEOPIXEL_MODULE;
    command[1]= NEOPIXEL_PIXEL;
    command[2]= strand;
    command[3]= pixel;
    command[4]= red;
    command[5]= green;
    command[6]= blue;
}

void mbl_mw_neopixel_rotate(uint8_t command[7], uint8_t strand, uint8_t count, uint16_t period_ms, MblMwNeoPixelRotDirection direction) {
    command[0]= NEOPIXEL_MODULE;
    command[1]= NEOPIXEL_ROTATE;
    command[2]= strand;
    command[3]= (direction != MBL_MW_NP_ROT_DIR_TOWARDS) ? MBL_MW_NP_ROT_DIR_AWAY : MBL_MW_NP_ROT_DIR_TOWARDS;
    command[4]= count;

    memcpy(command + 5, &period_ms, 2);
}

void mbl_mw_neopixel_rotate_indefinitely(uint8_t command[7], uint8_t strand, uint8_t period_ms, MblMwNeoPixelRotDirection direction) {
    mbl_mw_neopixel_rotate(command, strand, ROTATE_INDEFINITELY, period_ms, direction);
}

void mbl_mw_neopixel_stop_rotation(uint8_t command[7], uint8_t strand) {
    command[0]= NEOPIXEL_MODULE;
    command[1]= NEOPIXEL_ROTATE;
    command[2]= strand;

    std::memset(command + 3, 0, 4);
}
