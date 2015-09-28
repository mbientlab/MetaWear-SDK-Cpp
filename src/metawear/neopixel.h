/**
 * @copyright MbientLab License 
 * @file neopixel.h
 * @brief Interacts with NeoPixel strands
 */
#pragma once

#include <stdint.h>
#include "dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Color ordering for the NeoPixel color values
 * @author Eric Tsai
 */
typedef enum {
    MBL_MW_NP_WS2811_RGB= 0,    ///< Red, green, blue order
    MBL_MW_NP_WS2811_RBG,       ///< Red, blue, green order
    MBL_MW_NP_WS2811_GRB,       ///< Green, red, blue order
    MBL_MW_NP_WS2811_GBR        ///< Green, blue, red order
} MblMwNeoPixelColorOrdering;

/**
 * Enumeration of rotation directions
 */
typedef enum {
    MBL_MW_NP_ROT_DIR_TOWARDS,         ///< Pixels move towards the board
    MBL_MW_NP_ROT_DIR_AWAY             ///< Pixels move away from the board
} MblMwNeoPixelRotDirection;

/**
 * Constructs the command that initializes memory on the MetaWear board for a NeoPixel strand with a 
 * slow operating speed (400 KHz)
 * @param command   Byte array (len 6) for the function to write the command to
 * @param strand    Strand number (id) to initialize, can be in the range [0, 2]
 * @param ordering  Color ordering format
 * @param gpio_pin  GPIO pin the strand's data pin is connected to
 * @param n_pixels  Number of pixels to allocate memory for
 */
METAWEAR_API void mbl_mw_neopixel_init_slow_strand(uint8_t command[6], uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, 
        MblMwNeoPixelColorOrdering ordering);
/**
 * Constructs the command that initialize memory on the MetaWear board for a NeoPixel strand with a 
 * fast operating speed (800 KHz)
 * @param command   Byte array (len 6) for the function to write the command to
 * @param strand    Strand number (id) to initialize, can be in the range [0, 2]
 * @param ordering  Color ordering format
 * @param gpio_pin  GPIO pin the strand's data pin is connected to
 * @param n_pixels  Number of pixels to allocate memory for
 */
METAWEAR_API void mbl_mw_neopixel_init_fast_strand(uint8_t command[6], uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, 
        MblMwNeoPixelColorOrdering ordering);
/**
 * Constructs the command that frees NeoPixel resources on the MetaWeard board for a specific strand
 * @param command   Byte array (len 3) for the function to write the command to
 * @param strand    Strand index to free
 */
METAWEAR_API void mbl_mw_neopixel_free_strand(uint8_t command[3], uint8_t strand);

/**
 * Constructs the command that enables strand holding.  When enabled, the strand will not refresh with any LED changes 
 * until the hold is disabled.  This allows you to form complex LED patterns without having the strand refresh with 
 * partial changes.
 * @param command   Byte array (len 4) for the function to write the command to
 * @param strand    Strand number (id) to hold
 */
METAWEAR_API void mbl_mw_neopixel_enable_hold(uint8_t command[4], uint8_t strand);
/**
 * Constructs the command that disables strand holding.  The strand will be refreshed with any LED changes programmed 
 * while the hold was active
 * @param command   Byte array (len 4) for the function to write the command to
 * @param strand    Strand number (id) to release
 */
METAWEAR_API void mbl_mw_neopixel_disable_hold(uint8_t command[4], uint8_t strand);
/**
 * CConstructs the command that clears pixel states on a strand
 * @param command   Byte array (len 5) for the function to write the command to
 * @param strand    Strand number to clear
 * @param start     Pixel index to start clearing from
 * @param end       Pixel index to clear to, inclusive
 */
METAWEAR_API void mbl_mw_neopixel_clear(uint8_t command[5], uint8_t strand, uint8_t start, uint8_t end);
/**
 * Constructs the command that sets pixel color
 * @param command   Byte array (len 7) for the function to write the command to
 * @param strand    Strand number the pixel is on
 * @param pixel     Index of the pixel
 * @param red       Red value, between [0, 255]
 * @param green     Green value, between [0, 255]
 * @param blue      Blue value, between [0, 255]
 */
METAWEAR_API void mbl_mw_neopixel_set_color(uint8_t command[7], uint8_t strand, uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Constructs the command that rotates the pixels on a strand
 * @param command       Byte array (len 7) for the function to write the command to
 * @param strand        Strand to rotate
 * @param direction     Rotation direction
 * @param count         Number of times to repeat the rotation
 * @param period_ms     Amount of time, in milliseconds, between rotations
 */
METAWEAR_API void mbl_mw_neopixel_rotate(uint8_t command[7], uint8_t strand, uint8_t count, uint16_t period_ms, MblMwNeoPixelRotDirection direction);
/**
 * Constructs the command that Rotates the pixels on a strand indefinitely
 * @param command       Byte array (len 7) for the function to write the command to
 * @param strand        Strand to rotate
 * @param direction     Rotation direction
 * @param period_ms     Amount of time, in milliseconds, between rotations
 */
METAWEAR_API void mbl_mw_neopixel_rotate_indefinitely(uint8_t command[7], uint8_t strand, uint8_t period_ms, MblMwNeoPixelRotDirection direction);
/**
 * Constructs the command that stops the pixel rotation
 * @param command   Byte array (len 7) for the function to write the command to
 * @param strand    Strand to stop LED rotation
 */
METAWEAR_API void mbl_mw_neopixel_stop_rotation(uint8_t command[7], uint8_t strand);

#ifdef	__cplusplus
}
#endif
