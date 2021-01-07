.. highlight:: cpp

NeoPixel
========
Neopixels are a specific brand of individually addressable RGB LED (typically sold as a strip). Individually addressable means you can program the fourth LED in the strip to do one thing and the first to do another.

The firmware can communicate with `WS2811 <http://www.world-semi.com/en/Driver/Lighting_LED_driver_chip/WS2811/>`_ powered NeoPixel strands.  

NeoPixel functions are defined in the `neopixel.h <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html>`_ header file.

Strand Initialization
---------------------
By default, the firmware does not allocate memory for NeoPixels. You will need to allocate memory for a strand by calling 
`mbl_mw_neopixel_init_slow_strand <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html>`_ or 
`mbl_mw_neopixel_init_fast_strand <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html#aca6ce3ab1c3b4a94f4881fd0ce01b616>`_ depending on 
your strand's operating speed (fast = 800KHz, slow = 400KHz).  When allocating memory, you also need to tell the firmware your strand's operating 
speed and color ordering.  

To free the allocated memory, call 
`mbl_mw_neopixel_free_strand <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html#a885fe3a5249236967593c1ece79b3e14>`_. ::

    #include "metawear/peripheral/neopixel.h"
    
    void initialize_strand(MblMwMetaWearBoard* board) {
        uint8_t strand = 0, gpio_data_pin = 0;
        // allocate memory for a 30 pixel strand with a slow operating speed
        mbl_mw_neopixel_init_slow_strand(board, strand, gpio_data_pin, 30, MBL_MW_NP_WS2811_RGB);
    }

Setting Colors
--------------
The individual pixels of the strand are controlled with 
`mbl_mw_neopixel_set_color <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html#ad21361cad77aca09812eb41ca5d1c7b5>`_.  The color is 
determine by the combination of RGB values.  When setting the pixel colors, you can enable a hold with 
`mbl_mw_neopixel_set_color <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html#ad21361cad77aca09812eb41ca5d1c7b5>`_ so color changes are 
not applied until the hold is released by calling 
`mbl_mw_neopixel_disable_hold <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html#ad6aa99e50b700941ad011d798ff06e1a>`_. ::

    #include "metawear/peripheral/neopixel.h"
    
    void color_strand(MblMwMetaWearBoard* board) {
        uint8_t strand = 0;
        
        mbl_mw_neopixel_enable_hold(board, strand);
    
        mbl_mw_neopixel_set_color(board, strand, 0, 255, 0, 0);
        mbl_mw_neopixel_set_color(board, strand, 1, 0, 255, 0);
        mbl_mw_neopixel_set_color(board, strand, 2, 0, 0, 255);
    
        mbl_mw_neopixel_disable_hold(board, strand);
    }

Pattern Rotation
----------------
When you have a color pattern setup, you can rotate the pattern either indefinitely with 
`mbl_mw_neopixel_rotate_indefinitely <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html#aedd1ceec8ed5346666bfe7e43ad2b58c>`_ or for a 
fixed number of times using 
`mbl_mw_neopixel_rotate <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html#a5a91cecfdc500a3b9f6833e071fdbd0d>`_.  Rotations can be 
manually terminated with 
`mbl_mw_neopixel_stop_rotation <https://mbientlab.com/docs/metawear/cpp/latest/neopixel_8h.html#a6e8b06a9b3f9c22258628ec69c991771>`_. ::

    #include "metawear/peripheral/neopixel.h"
    
    void rotate_strand(MblMwMetaWearBoard* board) {
        uint8_t strand = 0;
        
        // rotate the pattern indefinitely, trigger rotation every 250ms
        mbl_mw_neopixel_rotate_indefinitely(board, strand, 250, MBL_MW_NP_ROT_DIR_TOWARDS);
    }

