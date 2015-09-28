using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class AccelerometerBmi160 {
        public enum FullScaleRange {
            FSR_2G = 0,
            FSR_4G,
            FSR_8G,
            FSR_16G
        };

        public enum OutputDataRate {
            ODR_0_78125HZ = 0,
            ODR_1_5625HZ,
            ODR_3_125HZ,
            ODR_6_25HZ,
            ODR_12_5HZ,
            ODR_25HZ,
            ODR_50HZ,
            ODR_100HZ,
            ODR_200HZ,
            ODR_400HZ,
            ODR_800HZ,
            ODR_1600HZ
        };

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_get_acceleration_data_source")]
        public static extern IntPtr GetAccelerationDataSource();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_create_config")]
        public static extern IntPtr CreateConfig();

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_free_config")]
        public static extern void FreeConfig(IntPtr config);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_set_odr")]
        public static extern void SetOutputDataRate(IntPtr config, OutputDataRate odr);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_set_range")]
        public static extern void SetFullScaleRange(IntPtr config, FullScaleRange range);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_write_acceleration_config")]
        public static extern void WriteConfig(byte[] command, IntPtr config);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_start")]
        public static extern void GlobalStart(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_stop")]
        public static extern void GlobalStop(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_enable_acceleration_sampling")]
        public static extern void EnableAccelerationSampling(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_disable_acceleration_sampling")]
        public static extern void DisableAccelerationSampling(byte[] command);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_get_acceleration_data_g")]
        public static extern int GetAccelerationData(ref CartesianFloat data_g, IntPtr config, byte[] response);

        [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_bmi160_get_acceleration_data_mg")]
        public static extern int GetAccelerationData(ref CartesianShort data_mg, IntPtr config, byte[] response);
    }
}
 
