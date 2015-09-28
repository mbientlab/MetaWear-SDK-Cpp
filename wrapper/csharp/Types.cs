using System.Runtime.InteropServices;

namespace MbientLab {
    namespace MetaWear {
        [StructLayout(LayoutKind.Sequential)]
        public struct CartesianFloat {
            public float x;
            public float y;
            public float z;

            public override string ToString() {
                return string.Format("({0:F3}, {1:F3}, {2:F3})", x, y, z);
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct CartesianShort {
            public short x;
            public short y;
            public short z;

            public override string ToString() {
                return string.Format("({0:D}, {1:D}, {2:D})", x, y, z);
            }
        }
    }
}