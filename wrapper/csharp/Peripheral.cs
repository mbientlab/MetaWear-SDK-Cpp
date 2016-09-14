using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear.Peripheral {
    public class IBeacon {
        public static byte[] GuidToByteArray(Guid guid) {
            byte[] guidBytes = guid.ToByteArray();

            // Implementation taken from SO: http://stackoverflow.com/a/16722909
            Array.Reverse(guidBytes, 0, 4);
            Array.Reverse(guidBytes, 4, 2);
            Array.Reverse(guidBytes, 6, 4);
            Array.Reverse(guidBytes);
            return guidBytes;
        }
    }

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
            public byte highIntensity, lowIntensity;
            public ushort riseTime, highTime, fallTime, pulseDuration, delayTime;
            public byte repeatCount;
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
