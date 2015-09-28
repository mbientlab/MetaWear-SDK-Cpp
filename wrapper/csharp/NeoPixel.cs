using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class NeoPixel {
        public enum ColorOrdering {
            /** Red, green, blue order */
            WS2811_RGB = 0,
            /** Red, blue, green order */
            WS2811_RBG,
            /** Green, red, blue order */
            WS2811_GRB,
            /** Green, blue, red order */
            WS2811_GBR
        };

        public enum RotationDirection {
            TOWARDS,
            AWAY
        };

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_init_slow_strand")]
        public static extern void InitSlowStrand(byte[] command, byte strand, byte gpioPin, byte nPixels, ColorOrdering ordering);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_init_fast_strand")]
        public static extern void InitFastStrand(byte[] command, byte strand, byte gpioPin, byte nPixels, ColorOrdering ordering);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_free_strand")]
        public static extern void FreeStrand(byte[] command, byte strand, byte gpioPin, byte nPixels, ColorOrdering ordering);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_enable_hold")]
        public static extern void EnableHold(byte[] command, byte strand);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_disable_hold")]
        public static extern void DisableHold(byte[] command, byte strand);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_clear")]
        public static extern void Clear(byte[] command, byte strand, byte start, byte end);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_set_color")]
        public static extern void SetColor(byte[] command, byte strand, byte pixel, byte red, byte green, byte blue);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_rotate")]
        public static extern void Rotate(byte[] command, byte strand, ushort count, ushort period, RotationDirection direction);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_rotate")]
        public static extern void RotateIndefinitely(byte[] command, byte strand, ushort period, RotationDirection direction);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_neopixel_stop_rotation")]
        public static extern void StopRotation(byte[] command, byte strand);
    }
}