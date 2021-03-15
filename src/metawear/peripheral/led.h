/**
 * @copyright MbientLab License 
 * @file led.h
 * @brief Communicates with the ultra-bright LED
 */
#pragma once

#include "peripheral_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/** Value indicating the led pulse will repeat indefinitely */
const uint8_t MBL_MW_LED_REPEAT_INDEFINITELY = 0xff;

/**
 * Available colors on the LED
 */
typedef enum {
    MBL_MW_LED_COLOR_GREEN,
    MBL_MW_LED_COLOR_RED,
    MBL_MW_LED_COLOR_BLUE
} MblMwLedColor;

/**
 * Enumeration of preset patterns available in the library
 * Patterns are based on the led blog post, http://projects.mbientlab.com/?p=656
 */
typedef enum {
    MBL_MW_LED_PRESET_BLINK,
    MBL_MW_LED_PRESET_PULSE,
    MBL_MW_LED_PRESET_SOLID
} MblMwLedPreset;

/**
 * Attributes describing a light pulse
 */
typedef struct {
    uint8_t high_intensity;             ///< Intensity when the pulse is in a high state, between [0, 31]
    uint8_t low_intensity;              ///< Intensity when the pulse is in a low state, between [0, 31]
    uint16_t rise_time_ms;              ///< Transition time from low to high state, in milliseconds
    uint16_t high_time_ms;              ///< Length of time the pulse spends in the high state, in milliseconds
    uint16_t fall_time_ms;              ///< Transition time from high to low state, in milliseconds
    uint16_t pulse_duration_ms;         ///< Length of time for one pulse, in milliseconds
    uint16_t delay_time_ms;             ///< How long to wait before starting the pulse, only used on firmware v1.2.3 or later
    uint8_t repeat_count;               ///< Number of repetitions
} MblMwLedPattern;

/**
 * Loads the struct with a preset configuration
 * @param pattern       Pointer to the pattern to write the configuration to
 * @param preset        Preset pattern to load 
 */
METAWEAR_API void mbl_mw_led_load_preset_pattern(MblMwLedPattern* pattern, MblMwLedPreset preset);

/**
 * Writes the led pattern to the board
 * @param board     Pointer to the board to send the command to
 * @param pattern   Pointer to the pattern attributes to write to the board
 * @param color     Color the pattern is configuring
 */
METAWEAR_API void mbl_mw_led_write_pattern(const MblMwMetaWearBoard* board, const MblMwLedPattern* pattern, MblMwLedColor color);
/**
 * Plays any programmed patterns, and immediately plays any patterns programmed later
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_led_autoplay(const MblMwMetaWearBoard* board);
/**
 * Plays any programmed patterns
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_led_play(const MblMwMetaWearBoard *board);
/**
 * Pauses the patterns
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_led_pause(const MblMwMetaWearBoard *board);
/**
 * Stops playing LED patterns
 * @param board             Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_led_stop(const MblMwMetaWearBoard *board);
/**
 * Stops playing LED patterns and clears all pattern configurations
 * @param board             Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_led_stop_and_clear(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
