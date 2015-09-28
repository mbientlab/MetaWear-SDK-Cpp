using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class Switch {
        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_switch_get_data_source")]
        public static extern IntPtr GetDataSource();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_switch_get_data")]
        public static extern int GetData(ref byte state, byte[] response);
    }
}