using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class AmbientLightLtr329 {
        public enum Gain {
            G_1X = 0,
            G_2X,
            G_4X,
            G_8X,
            G_48X,
            G_96X
        };

        public enum IntegrationTime {
            IT_100MS = 0,
            IT_50MS,
            IT_200MS,
            IT_400MS,
            IT_150MS,
            IT_250MS,
            IT_300MS,
            IT_350MS
        };

        public enum MeasurementRate {
            MR_50MS = 0,
            MR_100MS,
            MR_200MS,
            MR_500MS,
            MR_1000MS,
            MR_2000MS
        };

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_get_data_source")]
        public static extern IntPtr GetIlluminanceDataSource();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_create_config")]
        public static extern IntPtr CreateConfig();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_free_config")]
        public static extern void FreeConfig(IntPtr config);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_set_gain")]
        public static extern void SetGain(IntPtr config, Gain gain);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_set_integration_time")]
        public static extern void SetIntegrationTime(IntPtr config, IntegrationTime time);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_set_measurement_rate")]
        public static extern void SetMeasurementRate(IntPtr config, MeasurementRate rate);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_write_config")]
        public static extern void WriteConfig(byte[] command, IntPtr config);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_start")]
        public static extern void Start(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_stop")]
        public static extern void Stop(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_als_ltr329_get_illuminance_data")]
        public static extern int GetAccelerationData(ref uint data_dps, byte[] response);
    }
}
 
