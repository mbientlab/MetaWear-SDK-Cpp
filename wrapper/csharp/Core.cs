using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear.Core {
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void Fn_IntPtr(IntPtr pointer);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void Fn_IntPtr_Int(IntPtr pointer, int second);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void Fn_IntPtr_IntPtr_ByteArray(IntPtr fstPtr, IntPtr sndPtr, IntPtr bytes, byte length);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void Fn_IntPtr_IntPtr(IntPtr first, IntPtr second);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void Fn_Uint_Uint(uint first, uint second);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void Fn_Ubyte_Long_ByteArray(byte byteValue, long longValue, IntPtr bytes, byte length);

    public class Status {
        public const int OK = 0;
        public const int WARNING_UNEXPECTED_SENSOR_DATA = 1;
        public const int WARNING_INVALID_PROCESSOR_TYPE = 2;
        public const int ERROR_UNSUPPORTED_PROCESSOR = 4;
        public const int WARNING_INVALID_RESPONSE = 8;
        public const int ERROR_TIMEOUT = 16;
    }

    public enum Module {
        SWITCH = 1,
        LED,
        ACCELEROMETER,
        TEMPERATURE,
        GPIO,
        NEO_PIXEL,
        IBEACON,
        HAPTIC,
        DATA_PROCESSOR,
        EVENT,
        LOGGING,
        TIMER,
        I2C,
        MACRO = 0xf,
        GSR,
        SETTINGS,
        BAROMETER,
        GYRO,
        AMBIENT_LIGHT,
        MAGNETOMETER,
        HUMIDITY,
        COLOR_DETECTOR,
        PROXIMITY,
        DEBUG = 0xfe
    }

    public enum DataTypeId {
        UINT32 = 0,
        FLOAT,
        CARTESIAN_FLOAT,
        INT32,
        BYTE_ARRAY,
        BATTERY_STATE,
        TCS34725_ADC
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Data {
        public long epoch;
        public IntPtr value;
        public DataTypeId typeId;
        public byte length;
    }

    public class GattCharGuid {
        public Guid serviceGuid { get; }
        public Guid guid { get; }

        public GattCharGuid(Guid serviceGuid, Guid guid) {
            this.serviceGuid = serviceGuid;
            this.guid = guid;
        }

        public override string ToString() {
            return string.Format("{{service: {0}, characteristic: {1}{2}", serviceGuid.ToString(), guid.ToString(), "}");
        }

        public static readonly GattCharGuid METAWEAR_NOTIFY_CHAR = new GattCharGuid(new Guid("326A9000-85CB-9195-D9DD-464CFBBAE75A"), 
                new Guid("326A9006-85CB-9195-D9DD-464CFBBAE75A"));
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct GattCharacteristic {
        public ulong service_uuid_high, service_uuid_low;
        public ulong uuid_high, uuid_low;

        public GattCharGuid toGattCharGuid() {
            return new GattCharGuid(toGuid(service_uuid_high, service_uuid_low), toGuid(uuid_high, uuid_low));
        }

        public override string ToString() {
            return string.Format("{{service_uuid_high: 0x{0:X}, service_uuid_low: 0x{1:X}, uuid_high: 0x{2:X}, uuid_low: 0x{3:X}{4}", 
                service_uuid_high, service_uuid_low, uuid_high, uuid_low, "}");
        }

        private static Guid toGuid(ulong high, ulong low) {
            byte[] bytes = new byte[16];

            Array.Copy(BitConverter.GetBytes(low), bytes, 8);
            Array.Copy(BitConverter.GetBytes(high), 0, bytes, 8, 8);

            Array.Reverse(bytes);
            Array.Reverse(bytes, 0, 4);
            Array.Reverse(bytes, 4, 2);
            Array.Reverse(bytes, 6, 2);

            return new Guid(bytes);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct BtleConnection {
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public Fn_IntPtr_IntPtr_ByteArray writeGattChar;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public Fn_IntPtr_IntPtr readGattChar;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct LogDownloadHandler {
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public Fn_Uint_Uint receivedProgressUpdate;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public Fn_Ubyte_Long_ByteArray receivedUnknownEntry;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public Fn_IntPtr receivedUnhandledEntry;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CartesianFloat {
        public float x, y, z;

        public override string ToString() {
            return string.Format("{{x: {0:F3}, y: {1:F3}, z: {2:F3}{3}", x, y, z, "}");
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct BatteryState {
        public ushort voltage;
        public byte charge;

        public override string ToString() {
            return string.Format("{{voltage: {0:d}mv, charge: {1:d}%{2}", voltage, charge, "}");
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Tcs34725ColorAdc {
        public ushort clear, red, green, blue;

        public override string ToString() {
            return string.Format("{{clear: {0:d}, red: {1:d}, green: {2:d}, blue: {3:d}{4}", clear, red, green, blue, "}");
        }
    }
}
