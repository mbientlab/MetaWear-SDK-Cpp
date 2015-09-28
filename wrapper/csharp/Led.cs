using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class Led {
        public enum Color {
            GREEN,
            RED,
            BLUE
        }

        public enum Preset{
            PATTERN_BLINK,
            PATTERN_PULSE,
            PATTERN_SOLID
        };

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_create_pattern")]
        public static extern IntPtr CreatePattern();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_create_preset_pattern")]
        public static extern IntPtr CreatePattern(Preset preset);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_free_pattern")]
        public static extern void FreePattern(IntPtr pattern);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_write_pattern")]
        public static extern void WritePattern(byte[] command, IntPtr pattern, Color ledColor);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_set_intensity")]
        public static extern void SetIntensity(IntPtr pattern, byte high, byte low);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_set_pulse_times")]
        public static extern void SetPulseTimes(IntPtr pattern, ushort riseTime, ushort highTime, ushort fallTime, ushort duration);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_set_repeat_count")]
        public static extern void SetRepeatCount(IntPtr pattern, byte count);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_autoplay")]
        public static extern void AutoPlay(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_play")]
        public static extern void Play(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_pause")]
        public static extern void Pause(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_led_stop")]
        public static extern void Stop(byte[] command);
    }
}