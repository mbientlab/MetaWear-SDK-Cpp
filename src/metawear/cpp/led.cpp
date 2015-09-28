#include "metawear/led.h"

#include <cstring>

static const uint8_t LED_REPEAT_INDEFINITELY = 0xff;
static const uint8_t LED_MODULE= 2;
static const uint8_t LED_PLAY= 1, LED_STOP= 2, LED_CONFIG= 3;

struct MblMwLedPattern {
    uint8_t high_intensity;
    uint8_t low_intensity;
    uint16_t rise_time_ms;
    uint16_t high_time_ms;
    uint16_t fall_time_ms;
    uint16_t pulse_duration_ms;
    uint8_t padding[2];
    uint8_t repeat_count;
};

MblMwLedPattern* mbl_mw_led_create_pattern() {
    MblMwLedPattern *pattern= new MblMwLedPattern;
    std::memset(pattern, 0, sizeof(MblMwLedPattern));
    mbl_mw_led_set_repeat_count(pattern, LED_REPEAT_INDEFINITELY);
    return pattern;
}

MblMwLedPattern* mbl_mw_led_create_preset_pattern(MblMwLedPreset preset) {
    MblMwLedPattern *pattern = mbl_mw_led_create_pattern();

    switch (preset) {
    case LED_PRESET_BLINK:
        mbl_mw_led_set_intensity(pattern, 31, 0);
        mbl_mw_led_set_pulse_times(pattern, 0, 50, 0, 500);
        break;
    case LED_PRESET_PULSE:
        mbl_mw_led_set_intensity(pattern, 31, 0);
        mbl_mw_led_set_pulse_times(pattern, 725, 500, 725, 2000);
        break;
    case LED_PRESET_SOLID:
        mbl_mw_led_set_intensity(pattern, 31, 31);
        mbl_mw_led_set_pulse_times(pattern, 0, 500, 0, 1000);
        break;
    }

    return pattern;
}

void mbl_mw_led_free_pattern(MblMwLedPattern *pattern) {
    delete pattern;
}

void mbl_mw_led_write_pattern(uint8_t command[17], MblMwLedPattern *pattern, MblMwLedColor color) {
    command[0] = LED_MODULE;
    command[1] = LED_CONFIG;
    command[2] = color;
    command[3] = 2;

    memcpy(command + 4, pattern, sizeof(MblMwLedPattern));
}

void mbl_mw_led_autoplay(uint8_t command[3]) {
    command[0]= LED_MODULE;
    command[1]= LED_PLAY;
    command[2]= 2;
}

void mbl_mw_led_play(uint8_t command[3]) {
    command[0]= LED_MODULE;
    command[1]= LED_PLAY;
    command[2]= 1;
}

void mbl_mw_led_pause(uint8_t command[3]) {
    command[0]= LED_MODULE;
    command[1]= LED_PLAY;
    command[2]= 0;
}

void mbl_mw_led_stop(uint8_t command[3], uint8_t clear_config) {
    command[0]= LED_MODULE;
    command[1]= LED_STOP;
    command[2]= clear_config;
}

void mbl_mw_led_set_intensity(MblMwLedPattern *pattern, uint8_t high, uint8_t low) {
    pattern->high_intensity= high;
    pattern->low_intensity= low;
}

void mbl_mw_led_set_pulse_times(MblMwLedPattern *pattern, uint16_t rise_ms, uint16_t high_ms, uint16_t fall_ms, uint16_t duration_ms) {
    pattern->rise_time_ms= rise_ms;
    pattern->high_time_ms= high_ms;
    pattern->fall_time_ms= fall_ms;
    pattern->pulse_duration_ms= duration_ms;
}

void mbl_mw_led_set_repeat_count(MblMwLedPattern *pattern, uint8_t count) {
    pattern->repeat_count= count;
}
