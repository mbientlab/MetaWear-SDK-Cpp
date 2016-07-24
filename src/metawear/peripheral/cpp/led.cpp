#include "metawear/core/module.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/peripheral/led.h"

#include <cstring>

using namespace std;

const uint8_t LED_PLAY= 1, LED_STOP= 2, LED_CONFIG= 3, DELAYED_REVISION= 1;

static inline void set_intensities(MblMwLedPattern *pattern, uint8_t high, uint8_t low) {
    pattern->high_intensity= high;
    pattern->low_intensity= low;
}

static inline void set_pulse_times(MblMwLedPattern *pattern, uint16_t rise_ms, uint16_t high_ms, uint16_t fall_ms, uint16_t duration_ms) {
    pattern->rise_time_ms= rise_ms;
    pattern->high_time_ms= high_ms;
    pattern->fall_time_ms= fall_ms;
    pattern->pulse_duration_ms= duration_ms;
}

void mbl_mw_led_load_preset_pattern(MblMwLedPattern* pattern, MblMwLedPreset preset) {
    switch (preset) {
    case MBL_MW_LED_PRESET_BLINK:
        set_intensities(pattern, 31, 0);
        set_pulse_times(pattern, 0, 50, 0, 500);
        break;
    case MBL_MW_LED_PRESET_PULSE:
        set_intensities(pattern, 31, 0);
        set_pulse_times(pattern, 725, 500, 725, 2000);
        break;
    case MBL_MW_LED_PRESET_SOLID:
        set_intensities(pattern, 31, 31);
        set_pulse_times(pattern, 0, 500, 0, 1000);
        break;
    }
}

void mbl_mw_led_write_pattern(const MblMwMetaWearBoard *board, const MblMwLedPattern *pattern, MblMwLedColor color) {
    uint8_t command[17]= { MBL_MW_MODULE_LED, LED_CONFIG, static_cast<uint8_t>(color), 2};

    // MblMwLedPattern is aligned to 14 bytes wide, individual elements total to 13 bytes
    memcpy(command + 4, pattern, 13);
    if (board->module_info.at(MBL_MW_MODULE_LED).revision < DELAYED_REVISION) {
        command[14]= 0;
        command[15]= 0;
    }
    SEND_COMMAND;
}

void mbl_mw_led_autoplay(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_LED, LED_PLAY, 2};
    SEND_COMMAND;
}

void mbl_mw_led_play(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_LED, LED_PLAY, 1};
    SEND_COMMAND;
}

void mbl_mw_led_pause(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_LED, LED_PLAY, 0};
    SEND_COMMAND;
}

void mbl_mw_led_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_LED, LED_STOP, 0};
    SEND_COMMAND;
}

void mbl_mw_led_stop_and_clear(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {MBL_MW_MODULE_LED, LED_STOP, 1};
    SEND_COMMAND;
}
