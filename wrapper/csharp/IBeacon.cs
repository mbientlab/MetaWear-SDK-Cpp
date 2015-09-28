using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class IBeacon {
        public static byte[] GuidToByteArray(Guid guid) {
            byte[] guidBytes = guid.ToByteArray();

            // Implementation taken from SO: http://stackoverflow.com/a/16722909
            Array.Reverse(guidBytes, 0, 4);
            Array.Reverse(guidBytes, 4, 2);
            Array.Reverse(guidBytes, 6, 4);
            Array.Reverse(guidBytes);
            return guidBytes;
        }

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_ibeacon_set_major")]
        public static extern void SetMajor(byte[] command, ushort major);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_ibeacon_set_minor")]
        public static extern void SetMinor(byte[] command, ushort minor);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_ibeacon_set_period")]
        public static extern void SetPeriod(byte[] command, ushort period);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_ibeacon_set_tx_power")]
        public static extern void SetTxPower(byte[] command, sbyte txPower);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_ibeacon_set_rx_power")]
        public static extern void SetRxPower(byte[] command, sbyte rxPower);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_ibeacon_set_uuid")]
        public static extern void SetUuid(byte[] command, byte[] adUuid);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_ibeacon_enable")]
        public static extern void Enable(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_ibeacon_disable")]
        public static extern void Disable(byte[] command);
    }
}