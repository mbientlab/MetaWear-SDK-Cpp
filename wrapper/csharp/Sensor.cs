namespace MbientLab.MetaWear.Sensor {
    public class AccelerometerBmi160 {
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
    }

    public class AccelerometerMma8452q {
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
            GAIN_1X = 0,            // Defaut setting
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

    public class BarometerBmp280 {
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

    public class Gpio {
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
}