/**
 * @copyright MbientLab License 
 * @file led.h
 * @brief Communicates with the ultra-bright LED
 */
#pragma once

#include <stdint.h>
#include "dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Available colors on the LED
 */
typedef enum {
    GREEN,
    RED,
    BLUE
} MblMwLedColor;

/**
 * Enumeration of preset patterns available in the library
 * Patterns are based on the led blog post, http://projects.mbientlab.com/?p=656
 */
typedef enum {
    LED_PRESET_BLINK,
    LED_PRESET_PULSE,
    LED_PRESET_SOLID
} MblMwLedPreset;

/**
 * Container for the atteributes of an led pattern
 */
typedef struct MblMwLedPattern MblMwLedPattern;

/**
 * Instantiates a blank MblMwLedPattern.  Users will need to deallocate the memory by calling mbl_mw_led_free_pattern()
 * @return Pointer to the newly instantiated struct
 */
METAWEAR_API MblMwLedPattern* mbl_mw_led_create_pattern();
/**
 * Instantiates a MblMwLedPattern with the attributes set to one of the preset patterns.  Users will need to deallocate 
 * the memory by calling mbl_mw_led_free_pattern()
 * @return Pointer to the newly instantiated struct
 */
METAWEAR_API MblMwLedPattern* mbl_mw_led_create_preset_pattern(MblMwLedPreset preset);
/**
 * Frees the memory allocated for an led pattern
 * @param pattern   Pointer to memory to free
 */
METAWEAR_API void mbl_mw_led_free_pattern(MblMwLedPattern *pattern);

/**
 * Constructs the command that writes a led pattern to the board
 * @param command   Byte array (len 17) tfor the function to write the command to
 * @param pattern   Pointer to the pattern attributes to write to the board
 * @param color     Color the pattern is configuring
 */
METAWEAR_API void mbl_mw_led_write_pattern(uint8_t command[17], MblMwLedPattern *pattern, MblMwLedColor color);
/**
 * Sets the high and low intensities of the pulse
 * @param pattern   Pointer to the pattern attributes 
 * @param high      Intensity when the pulse is in a high state, between [0, 31]
 * @param low       Intensity when the pulse is in a low state, between [0, 31]
 */
METAWEAR_API void mbl_mw_led_set_intensity(MblMwLedPattern *pattern, uint8_t high, uint8_t low);
/**
 * Sets the timing parameters of the pulse
 * @param pattern   Pointer to the pattern attributes
 * @param rise_ms   Transition time from low to high state, in milliseconds
 * @param high_ms   Length of time the pulse spends in the high state, in milliseconds
 * @param fall_ms   Transition time from high to low state, in milliseconds
 * @param duration_ms   Length of one pulse, in milliseconds
 */
METAWEAR_API void mbl_mw_led_set_pulse_times(MblMwLedPattern *pattern, uint16_t rise_ms, uint16_t high_ms, uint16_t fall_ms, uint16_t duration_ms);
/**
 * Sets the number of times the pattern will repeat
 * @param pattern   Pointer to the pattern attributes
 * @param count     Number of repetitions
 */
METAWEAR_API void mbl_mw_led_set_repeat_count(MblMwLedPattern *pattern, uint8_t count);

/**
 * Constructs the command that plays any programmed patterns, and immediately plays any patterns programmed later
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_led_autoplay(uint8_t command[3]);
/**
 * Constructs the command that plays any programmed patterns
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_led_play(uint8_t command[3]);
/**
 * Construct the command that pauses the patterns
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_led_pause(uint8_t command[3]);
/**
 * Constructs the command that stops playing LED patterns
 * @param command           Byte array (len 3) for the function to write the command to
 * @param clear_pattern     0 to keep patterns, 1 to clear all patterns
 */
METAWEAR_API void mbl_mw_led_stop(uint8_t command[3], uint8_t clear_pattern);

#ifdef	__cplusplus
}
#endif
