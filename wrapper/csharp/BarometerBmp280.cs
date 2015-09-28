using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class BarometerBmp280 {
        public enum Oversampling {
            SKIP,
            ULTRA_LOW_POWER,
            LOW_POWER,
            STANDARD,
            HIGH,
            ULTRA_HIGH
        };

        public enum IirFilter {
            OFF = 0,
            AVG_2,
            AVG_4,
            AVG_8,
            AVG_16
        };

        public enum StandbyTime {
            STANDBY_0_5MS = 0,
            STANDBY_62_5MS,
            STANDBY_125MS,
            STANDBY_250MS,
            STANDBY_500MS,
            STANDBY_1000MS,
            STANDBY_2000MS,
            STANDBY_4000MS
        };

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_get_pressure_data_source")]
        public static extern IntPtr GetPressureDataSource();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_get_altitude_data_source")]
        public static extern IntPtr GetAltitudeDataSource();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_create_config")]
        public static extern IntPtr CreateConfig();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_free_config")]
        public static extern void FreeConfig(IntPtr config);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_set_oversampling")]
        public static extern void SetOversampling(IntPtr config, OVersampling oversample);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_set_iir_filter")]
        public static extern void SetIirFilter(IntPtr config, IirFilter filter);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_set_standby_time")]
        public static extern void SetStandbyTime(IntPtr config, StandbyTime time);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_write_config")]
        public static extern void WriteConfig(byte[] command, IntPtr config);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_start")]
        public static extern void Start(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_stop")]
        public static extern void Stop(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_get_pressure_data")]
        public static extern int GetPressureData(ref float data_pa, byte[] response);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_baro_bmp280_get_altitude_data")]
        public static extern int GetAltitudeData(ref float data_m, byte[] response);
    }
}
 
