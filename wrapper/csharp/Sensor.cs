using System;
using System.Runtime.InteropServices;

namespace MbientLab {
    namespace MetaWear {
        public sealed class Sensor {
            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_sensor_subscribe")]
            public static extern void Subscribe(byte[] command, IntPtr dataSource);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_sensor_unsubscribe")]
            public static extern void Unsubscribe(byte[] command, IntPtr dataSource);
        }
    }
}