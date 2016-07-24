/**
 * @copyright MbientLab License 
 * @file neopixel.h
 * @brief Interacts with NeoPixel strands
 */
#pragma once

#include "peripheral_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Color ordering for the NeoPixel color values
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
 * Initializes memory on the MetaWear board for a NeoPixel strand with a slow operating speed (400 KHz)
 * @param board     Pointer to the board to send the command to
 * @param strand    Strand number (id) to initialize, can be in the range [0, 2]
 * @param ordering  Color ordering format
 * @param gpio_pin  GPIO pin the strand's data pin is connected to
 * @param n_pixels  Number of pixels to allocate memory for
 */
METAWEAR_API void mbl_mw_neopixel_init_slow_strand(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels, 
        MblMwNeoPixelColorOrdering ordering);
/**
 * Initializes memory on the MetaWear board for a NeoPixel strand with a fast operating speed (800 KHz)
 * @param board     Pointer to the board to send the command to
 * @param strand    Strand number (id) to initialize, can be in the range [0, 2]
 * @param ordering  Color ordering format
 * @param gpio_pin  GPIO pin the strand's data pin is connected to
 * @param n_pixels  Number of pixels to allocate memory for
 */
METAWEAR_API void mbl_mw_neopixel_init_fast_strand(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t gpio_pin, uint8_t n_pixels,
        MblMwNeoPixelColorOrdering ordering);
/**
 * Frees NeoPixel resources on the MetaWeard board for a specific strand
 * @param board     Pointer to the board to send the command to
 * @param strand    Strand index to free
 */
METAWEAR_API void mbl_mw_neopixel_free_strand(const MblMwMetaWearBoard *board, uint8_t strand);

/**
 * Enables strand holding which will not refresh with any LED changes until the hold is disabled.  
 * This let you to form complex LED patterns without having the strand refresh with partial changes.
 * @param board     Pointer to the board to send the command to
 * @param strand    Strand number (id) to hold
 */
METAWEAR_API void mbl_mw_neopixel_enable_hold(const MblMwMetaWearBoard *board, uint8_t strand);
/**
 * Disables strand holding.  The strand will be refreshed with any LED changes programmed 
 * while the hold was active
 * @param board     Pointer to the board to send the command to
 * @param strand    Strand number (id) to release
 */
METAWEAR_API void mbl_mw_neopixel_disable_hold(const MblMwMetaWearBoard *board, uint8_t strand);
/**
 * Clears pixel states on a strand
 * @param board     Pointer to the board to send the command to
 * @param strand    Strand number to clear
 * @param start     Pixel index to start clearing from
 * @param end       Pixel index to clear to, inclusive
 */
METAWEAR_API void mbl_mw_neopixel_clear(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t start, uint8_t end);
/**
 * Sets pixel color
 * @param board     Pointer to the board to send the command to
 * @param strand    Strand number the pixel is on
 * @param pixel     Index of the pixel
 * @param red       Red value, between [0, 255]
 * @param green     Green value, between [0, 255]
 * @param blue      Blue value, between [0, 255]
 */
METAWEAR_API void mbl_mw_neopixel_set_color(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Rotates the pixels on a strand
 * @param board         Pointer to the board to send the command to
 * @param strand        Strand to rotate
 * @param direction     Rotation direction
 * @param count         Number of times to repeat the rotation
 * @param period_ms     Amount of time, in milliseconds, between rotations
 */
METAWEAR_API void mbl_mw_neopixel_rotate(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t count, uint16_t period_ms, MblMwNeoPixelRotDirection direction);
/**
 * Rotates the pixels on a strand indefinitely
 * @param board         Pointer to the board to send the command to
 * @param strand        Strand to rotate
 * @param direction     Rotation direction
 * @param period_ms     Amount of time, in milliseconds, between rotations
 */
METAWEAR_API void mbl_mw_neopixel_rotate_indefinitely(const MblMwMetaWearBoard *board, uint8_t strand, uint8_t period_ms, MblMwNeoPixelRotDirection direction);
/**
 * Stops the pixel rotation
 * @param board     Pointer to the board to send the command to
 * @param strand    Strand to stop LED rotation
 */
METAWEAR_API void mbl_mw_neopixel_stop_rotation(const MblMwMetaWearBoard *board, uint8_t strand);

#ifdef	__cplusplus
}
#endif
