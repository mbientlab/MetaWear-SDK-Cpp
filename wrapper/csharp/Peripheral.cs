using System.Runtime.InteropServices;

namespace MbientLab.MetaWear.Peripheral {
    public class Led {
        public const byte REPEAT_INDEFINITELY = 0xff;

        public enum Color {
            GREEN,
            RED,
            BLUE
        }

        public enum PatternPreset {
            BLINK,
            PULSE,
            SOLID
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Pattern {
            ushort riseTime;
            ushort highTime;
            ushort fallTime;
            ushort pulseDuration;
            byte highIntensity;
            byte lowIntensity;
            byte repeatCount;
        }
    }

    public class NeoPixel {
        public enum Ordering {
            WS2811_RGB = 0,
            WS2811_RBG,
            WS2811_GRB,
            WS2811_GBR,
        }

        public enum Direction {
            TOWARDS = 0,
            AWAY
        }
    }
}