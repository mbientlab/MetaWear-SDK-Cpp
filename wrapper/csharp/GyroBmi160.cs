using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class GyroBmi160 {
        public enum OutputDataRate {
            ODR_25HZ = 6,
            ODR_50HZ,
            ODR_100HZ,
            ODR_200HZ,
            ODR_400HZ,
            ODR_800HZ,
            ODR_1600HZ,
            ODR_3200HZ
        };

        public enum FullScaleRange {
            FSR_2000DPS = 0,
            FSR_1000DPS,
            FSR_500DPS,
            FSR_250DPS,
            FSR_125DPS
        };

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_get_rotation_data_source")]
        public static extern IntPtr GetRotationDataSource();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_create_config")]
        public static extern IntPtr CreateConfig();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_free_config")]
        public static extern void FreeConfig(IntPtr config);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_set_odr")]
        public static extern void SetOutputDataRate(IntPtr config, OutputDataRate odr);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_set_range")]
        public static extern void SetFullScaleRange(IntPtr config, FullScaleRange range);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_write_config")]
        public static extern void WriteConfig(byte[] command, IntPtr config);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_start")]
        public static extern void GlobalStart(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_stop")]
        public static extern void GlobalStop(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_enable_rotation_sampling")]
        public static extern void EnableRotationSampling(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_disable_rotation_sampling")]
        public static extern void DisableRotationSampling(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_gyro_bmi160_get_rotation_data")]
        public static extern int GetAccelerationData(ref CartesianFloat data_dps, IntPtr config, byte[] response);
    }
}
 
