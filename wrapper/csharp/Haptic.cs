using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class Haptic {
        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_haptic_start_motor")]
        public static extern void StartMotor(byte[] command, float dutyCycle, ushort pulseWidth);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_haptic_start_buzzer")]
        public static extern void StartBuzzer(byte[] command, ushort pulseWidth);
    }
}