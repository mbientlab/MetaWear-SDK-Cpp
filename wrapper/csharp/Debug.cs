using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class Debug {
        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_debug_reset")]
        public static extern void Reset(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_debug_jump_to_bootloader")]
        public static extern void JumpToBootloader(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_debug_disconnect")]
        public static extern void Disconnect(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_debug_reset_after_gc")]
        public static extern void ResetAfterGc(byte[] command);
    }
}
 