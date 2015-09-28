using System;
using System.Runtime.InteropServices;

namespace MbientLab {
    namespace MetaWear {
        public sealed class AccelerometerMma8452q {
            public enum FullScaleRange {
                FSR_2G = 0,
                FSR_4G,
                FSR_8G,
            };

            public enum OutputDataRate {
                ODR_800HZ= 0,
                ODR_400HZ,
                ODR_200HZ,
                ODR_100HZ,
                ODR_50HZ,
                ODR_12_5HZ,
                ODR_6_25HZ,
                ODR_1_56HZ,
            };

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_get_acceleration_data_source")]
            public static extern IntPtr GetAccelerationDataSource();

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_create_config")]
            public static extern IntPtr CreateConfig();

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_free_config")]
            public static extern void FreeConfig(IntPtr config);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_set_odr")]
            public static extern void SetOutputDataRate(IntPtr config, OutputDataRate odr);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_set_range")]
            public static extern void SetFullScaleRange(IntPtr config, FullScaleRange range);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_write_acceleration_config")]
            public static extern void WriteConfig(byte[] command, IntPtr config);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_start")]
            public static extern void GlobalStart(byte[] command);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_stop")]
            public static extern void GlobalStop(byte[] command);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_enable_acceleration_sampling")]
            public static extern void EnableAccelerationSampling(byte[] command);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_disable_acceleration_sampling")]
            public static extern void DisableAccelerationSampling(byte[] command);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_get_acceleration_data_g")]
            public static extern int GetAccelerationData(ref CartesianFloat data_g, byte[] response);

            [DllImport(Constants.METAWEAR_DLL_NAME, EntryPoint = "mbl_mw_acc_mma8452q_get_acceleration_data_mg")]
            public static extern int GetAccelerationData(ref CartesianShort data_mg, byte[] response);
        }
    }
}
