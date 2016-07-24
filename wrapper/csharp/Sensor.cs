using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear.Sensor {
    public class AccelerometerBosch {
        public enum FullScaleRange {
            FSR_2G = 0,
            FSR_4G,
            FSR_8G,
            FSR_16G
        };
    }
    public class AccelerometerBma255 {
        public const int MODULE_TYPE = 3;

        public enum OutputDataRate {
            ODR_15_62HZ = 0,
            ODR_31_26HZ,
            ODR_62_5HZ,
            ODR_125HZ,
            ODR_250HZ,
            ODR_500HZ,
            ODR_1000HZ,
            ODR_2000HZ,
        }
    }
    public class AccelerometerBmi160 {
        public const int MODULE_TYPE = 1;

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

        public enum StepCounterMode {
            NORMAL = 0,
            SENSITIVE,
            ROBUST
        };
    }
    public class AccelerometerMma8452q {
        public const int MODULE_TYPE = 0;

        public enum FullScaleRange {
            FSR_2G = 0,
            FSR_4G,
            FSR_8G
        }

        public enum OutputDataRate {
            ODR_800HZ = 0,
            ODR_400HZ,
            ODR_200HZ,
            ODR_100HZ,
            ODR_50HZ,
            ODR_12_5HZ,
            ODR_6_25HZ,
            ODR_1_56HZ
        }
    }

    public class AmbientLightLtr329 {
        public enum SensorGain {
            GAIN_1X = 0,            // Default setting
            GAIN_2X,
            GAIN_4X,
            GAIN_8X,
            GAIN_48X,
            GAIN_96X
        }

        public enum IntegrationTime {
            TIME_100MS = 0,         // Default setting
            TIME_50MS,
            TIME_200MS,
            TIME_400MS,
            TIME_150MS,
            TIME_250MS,
            TIME_300MS,
            TIME_350MS
        }


        public enum MeasurementRate {
            RATE_50MS = 0,
            RATE_100MS,
            RATE_200MS,
            RATE_500MS,             // Default setting
            RATE_1000MS,
            RATE_2000MS
        }
    }

    public class BarometerBosch {
        public enum Oversampling {
            SKIP = 0,
            ULTRA_LOW_POWER,
            LOW_POWER,
            STANDARD,
            HIGH,
            ULTRA_HIGH
        }

        public enum InfiniteImpluseFilter {
            OFF = 0,
            AVG_2,
            AVG_4,
            AVG_8,
            AVG_16
        }
    }
    public class BarometerBmp280 {
        public const int MODULE_TYPE = 0;

        public enum Standby {
            TIME_0_5MS = 0,
            TIME_62_5MS,
            TIME_125MS,
            TIME_250MS,
            TIME_500MS,
            TIME_1000MS,
            TIME_2000MS,
            TIME_4000MS
        }
    }
    public class BarometerBme280 {
        public const int MODULE_TYPE = 1;

        public enum Standby {
            TIME_0_5MS = 0,
            TIME_62_5MS,
            TIME_125MS,
            TIME_250MS,
            TIME_500MS,
            TIME_1000MS,
            TIME_10MS,
            TIME_20MS
        }
    }

    public class Gpio {
        public const byte UNUSED_PIN = 0xff;

        [StructLayout(LayoutKind.Sequential)]
        public struct AnalogReadParameters {
            public byte pullupPin;
            public byte pulldownPin;
            public byte virtualPin;
            public ushort delay;
        }

        public enum PullMode {
            UP = 0,
            DOWN,
            NONE
        }

        public enum ReadMode {
            ABS_REFERENCE = 0,
            ADC
        }

        public enum PinChangeType {
            RISING = 1,
            FALLING,
            ANY
        }
    }

    public class GyroBmi160 {
        public enum OutputDataRate {
            ODR_25HZ = 6,
            ODR_50HZ,
            ODR_100HZ,
            ODR_200HZ,
            ODR_400HZ,
            ODR_800HZ,
            ODR_1600HZ,
            ODR_3200HZ
        }


        public enum FullScaleRange {
            FSR_2000DPS = 0,
            FSR_1000DPS,
            FSR_500DPS,
            FSR_250DPS,
            FSR_125DPS
        }
    }

    public class MultiChannelTemperature {
        public enum Source {
            INVALID = -1,
            NRF_DIE,
            EXT_THERMISTOR,
            BMP280,
            PRESET_THERMISTOR
        }

        public enum MetaWearRChannel {
            ON_DIE = 0,
            EXT_THERMISTOR
        }

        public enum MetaWearRProChannel {
            ON_DIE = 0,
            ON_BOARD_THERMISTOR,
            EXT_THERMISTOR,
            BMP280
        }
    }

    public class ProximityTsl2671 {
        public enum TransmitterCurrent {
            CURRENT_100MA = 0,
            CURRENT_50MA,
            CURRENT_25MA,
            CURRENT_12_5MA
        }

        public enum RecieverChannel {
            CHANNEL_0= 1,
            CHANNEL_1,
            CHANNEL_BOTH
        }
    }

    public class HumidityBme280 {
        public enum Oversampling {
            OS_1X = 1,
            OS_2X,
            OS_4X,
            OS_8X,
            OS_16X,
        }
    }

    public class ColorDetectorTcs34725 {
        public enum Gain {
            GAIN_1X = 0,
            GAIN_4X,
            GAIN_16X,
            GAIN_60X
        }
    }

    public class MagnetometerBmm150 {
        public enum PowerPreset {
            LOW_POWER= 0,
            REGULAR,
            ENHANCED_REGULAR,
            HIGH_ACCURACY
        }
    }

    public class I2C {
        [StructLayout(LayoutKind.Sequential)]
        public struct ReadParameters {
            public byte deviceAddr;
            public byte registerAddr;
        }
    }

    public class SPI {
        public enum Mode {
            M_0= 0,
            M_1,
            M_2,
            M_3
        }

        public enum Frequency {
            F_125_KHZ= 0,
            F_250_KHZ,
            F_500_KHZ,
            F_1_MHZ,
            F_2_MHZ,
            F_4_MHZ,
            F_8_MHZ
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct ReadParameters {
            public Mode mode;
            public Frequency frequency;
            public IntPtr data;
            public byte data_length;
            public byte slaveSelectPin;
            public byte clockPin;
            public byte mosiPin;
            public byte misoPin;
            public byte lsbFirst;
            public byte useNrfPins;
        }
    }
}
