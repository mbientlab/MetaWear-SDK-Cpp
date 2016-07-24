from ctypes import c_ushort, c_ubyte, Structure

# Constants and types for configuring the Led
class Led:
    # Python wrapper for the MblMwLedPattern struct
    class Pattern(Structure):
        _fields_= [
            ("high_intensity", c_ubyte),
            ("low_intensity", c_ubyte),
            ("rise_time_ms", c_ushort),
            ("high_time_ms", c_ushort),
            ("fall_time_ms", c_ushort),
            ("pulse_duration_ms", c_ushort),
            ("delay_time_ms", c_ushort),
            ("repeat_count", c_ubyte)
        ]

    REPEAT_INDEFINITELY= 0xff

    COLOR_GREEN= 0
    COLOR_RED= 1
    COLOR_BLUE= 2

    PRESET_BLINK= 0
    PRESET_PULSE= 1
    PRESET_SOLID= 2

# Constants for configuring NeoPixel strands
class NeoPixel:
    COLOR_ORDERING_WS2811_RGB= 0
    COLOR_ORDERING_WS2811_RBG= 1
    COLOR_ORDERING_WS2811_GRB= 2
    COLOR_ORDERING_WS2811_GBR= 3

    ROT_DIRECTION_TOWARDS= 0
    ROT_DIRECTION_AWAY= 1
