using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear.Core {
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void FnVoid();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void FnVoidPtrByteArray(IntPtr pointer, IntPtr bytes, byte length);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void FnVoidPtr(IntPtr pointer);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void FnUintUint(uint first, uint second);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void FnUbyteLongByteArray(byte byteValue, long longValue, IntPtr bytes, byte length);

    public class Status {
        public const int OK = 0;
        public const int WARNING_UNEXPECTED_SENSOR_DATA = 1;
        public const int WARNING_INVALID_PROCESSOR_TYPE = 2;
        public const int ERROR_UNSUPPORTED_PROCESSOR = 4;
        public const int WARNING_INVALID_RESPONSE = 8;
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

        public static readonly GattCharGuid METAWEAR_NOTIFY_CHAR = new GattCharGuid(new Guid("326A9000-85CB-9195-D9DD-464CFBBAE75A"), 
                new Guid("326A9006-85CB-9195-D9DD-464CFBBAE75A"));
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct GattCharacteristic {
        public ulong service_uuid_high;
        public ulong service_uuid_low;
        public ulong uuid_high;
        public ulong uuid_low;

        public GattCharGuid toGattCharGuid() {
            return new GattCharGuid(toGuid(service_uuid_high, service_uuid_low), toGuid(uuid_high, uuid_low));
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
        public FnVoidPtrByteArray writeGattChar;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public FnVoidPtr readGattChar;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct LogDownloadHandler {
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public FnUintUint receivedProgressUpdate;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public FnUbyteLongByteArray receivedUnknownEntry;
    }

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
    public struct BatteryState {
        public ushort voltage;
        public byte charge;

        public override string ToString() {
            return string.Format("(voltage: {0:d}, charge: {1:d})", voltage, charge);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Tcs34725ColorAdc {
        public ushort clear, red, green, blue;

        public override string ToString() {
            return string.Format("(clear: {0:d}, red: {1:d}, green: {2:d}, blue: {3:d})", clear, red, green, blue);
        }
    }
}
