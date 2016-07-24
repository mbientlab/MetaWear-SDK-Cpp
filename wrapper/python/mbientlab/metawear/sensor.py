from ctypes import c_int, c_ushort, c_ubyte, c_void_p, Structure

class AccelerometerBosch:
    FSR_2G= 0
    FSR_4G= 1
    FSR_8G= 2
    FSR_16G= 3

# Constants for configuring the BMI160 accelerometer
class AccelerometerBmi160:
    MODULE_TYPE= 1

    ODR_0_78125HZ= 0
    ODR_1_5625HZ= 1
    ODR_3_125HZ= 2
    ODR_6_25HZ= 3
    ODR_12_5HZ= 4
    ODR_25HZ= 5
    ODR_50HZ= 6
    ODR_100HZ= 7
    ODR_200HZ= 8
    ODR_400HZ= 9
    ODR_800HZ= 10
    ODR_1600HZ= 11

    STEP_COUNTER_MODE_NORMAL= 0
    STEP_COUNTER_MODE_SENSITIVE= 1
    STEP_COUNTER_MODE_ROBUST= 2

class AccelerometerBma255:
    MODULE_TYPE= 3

    ODR_15_62HZ= 0
    ODR_31_26HZ= 1
    ODR_62_5HZ= 2
    ODR_125HZ= 3
    ODR_250HZ= 4
    ODR_500HZ= 5
    ODR_1000HZ= 6
    ODR_2000HZ= 7

# Constants for configuring the MMA8452Q accelerometer
class AccelerometerMma8452q:
    MODULE_TYPE= 0

    ODR_800HZ= 0
    ODR_400HZ= 1
    ODR_200HZ= 2
    ODR_100HZ= 3
    ODR_50HZ= 4
    ODR_12_5HZ= 5
    ODR_6_25HZ= 6
    ODR_1_56HZ= 7

    FSR_2G= 0
    FSR_4G= 1
    FSR_8G= 2

# Constants for configuring the LTR329 ambient light sensor
class AmbientLightLtr329:
    GAIN_1X= 0
    GAIN_2X= 1
    GAIN_4X= 2
    GAIN_8X= 3
    GAIN_48X= 4
    GAIN_96X= 5

    INTEGRATION_TIME_100MS= 0
    INTEGRATION_TIME_50MS= 1
    INTEGRATION_TIME_200MS= 2
    INTEGRATION_TIME_400MS= 3
    INTEGRATION_TIME_150MS= 4
    INTEGRATION_TIME_250MS= 5
    INTEGRATION_TIME_300MS= 6
    INTEGRATION_TIME_350MS= 7

    MEASUREMENT_RATE_50MS= 0
    MEASUREMENT_RATE_100MS= 1
    MEASUREMENT_RATE_200MS= 2
    MEASUREMENT_RATE_500MS= 3
    MEASUREMENT_RATE_1000MS= 4
    MEASUREMENT_RATE_2000MS= 5

# Constants for configuring the Bosch barometers
class BarometerBosch:
    OVERSAMPLING_SKIP= 0,
    OVERSAMPLING_ULTRA_LOW_POWER= 1
    OVERSAMPLING_LOW_POWER= 2
    OVERSAMPLING_STANDARD= 3
    OVERSAMPLING_HIGH= 4
    OVERSAMPLING_ULTRA_HIGH= 5

    IIR_FILTER_OFF= 0
    IIR_FILTER_AVG_2= 1
    IIR_FILTER_AVG_4= 2
    IIR_FILTER_AVG_8= 3
    IIR_FILTER_AVG_16= 4

class BarometerBmp280:
    STANDBY_TIME_0_5MS= 0
    STANDBY_TIME_62_5MS= 1
    STANDBY_TIME_125MS= 2
    STANDBY_TIME_250MS= 3
    STANDBY_TIME_500MS= 4
    STANDBY_TIME_1000MS= 5
    STANDBY_TIME_2000MS= 6
    STANDBY_TIME_4000MS= 7

class BarometerBme280:
    STANDBY_TIME_0_5MS= 0
    STANDBY_TIME_62_5MS= 1
    STANDBY_TIME_125MS= 2
    STANDBY_TIME_250MS= 3
    STANDBY_TIME_500MS= 4
    STANDBY_TIME_1000MS= 5
    STANDBY_TIME_10MS= 6
    STANDBY_TIME_20MS= 7

# Constants for configuring the BMI160 gyro
class GyroBmi160:
    ODR_25HZ= 6
    ODR_50HZ= 7
    ODR_100HZ= 8
    ODR_200HZ= 9
    ODR_400HZ= 10
    ODR_800HZ= 11
    ODR_1600HZ= 12
    ODR_3200HZ= 13

    FSR_2000DPS= 0
    FSR_1000DPS= 1
    FSR_500DPS= 2
    FSR_250DPS= 3
    FSR_125DPS= 4

class MultiChannelTemperature:
    SOURCE_INVALID= -1
    SOURCE_NRF_DIE= 0
    SOURCE_EXT_THERM= 1
    SOURCE_BMP280= 2
    SOURCE_PRESET_THERM= 3

    METAWEAR_R_CHANNEL_ON_DIE= 0
    METAWEAR_R_CHANNEL_EXT_THERMISTOR= 1

    METAWEAR_RPRO_CHANNEL_ON_DIE= 0
    METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR= 1
    METAWEAR_RPRO_CHANNEL_EXT_THERMISTOR= 2
    METAWEAR_RPRO_CHANNEL_BMP280= 3

class Gpio:
    # Python wrapper for the MblMwGpioAnalogReadParameters struct
    class AnalogReadParameters(Structure):
        _fields_= [
            ("pullup_pin", c_ubyte),
            ("pulldown_pin", c_ubyte),
            ("virtual_pin", c_ubyte),
            ("delay_us", c_ushort)
        ]

    UNUSED_PIN = 0xff

    PULL_MODE_UP= 0
    PULL_MODE_DOWN= 1
    PULL_MODE_NONE= 2

    ANALOG_READ_MODE_ABS_REF= 0
    ANALOG_READ_MODE_ADC= 1

    PIN_CHANGE_TYPE_RISING= 1
    PIN_CHANGE_TYPE_FALLING= 2
    PIN_CHANGE_TYPE_ANY= 3

class MagnetometerBmm150:
    POWER_PRESET_LOW_POWER= 0
    POWER_PRESET_REGULAR= 1
    POWER_PRESET_ENHANCED_REGULAR= 2
    POWER_PRESET_HIGH_ACCURACY= 3;

class ColorDetectorTcs34725:
    GAIN_1X= 0
    GAIN_4X= 1
    GAIN_16X= 2
    GAIN_60X= 3

class ProximityTsl2671:
    CHANNEL_0= 1
    CHANNEL_1= 2
    CHANNEL_BOTH= 3

    CURRENT_100MA= 0
    CURRENT_50MA= 1
    CURRENT_25MA= 2
    CURRENT_12_5MA= 3

class HumidityBme280:
    OVERSAMPLING_1X= 1
    OVERSAMPLING_2X= 2
    OVERSAMPLING_4X= 3
    OVERSAMPLING_8X= 4
    OVERSAMPLING_16X= 5

class I2C:
    # Python wrapper for the MblMwI2cParameters struct
    class ReadParameters(Structure):
        _fields_= [
            ("device_addr", c_ubyte),
            ("register_addr", c_ubyte)
        ]

class SPI:
    # Python wrapper for the MblMwI2cParameters struct
    class Parameters(Structure):
        _fields_= [
            ("mode", c_int),
            ("frequency", c_int),
            ("data", c_void_p),
            ("data_length", c_ubyte),
            ("slave_select_pin", c_ubyte),
            ("clock_pin", c_ubyte),
            ("mosi_pin", c_ubyte),
            ("miso_pin", c_ubyte),
            ("lsb_first", c_ubyte),
            ("use_nrf_pins", c_ubyte),
        ]

    MODE_0= 0
    MODE_1= 1
    MODE_2= 2
    MODE_3= 3

    FREQUENCY_125_KHZ= 0
    FREQUENCY_250_KHZ= 1
    FREQUENCY_500_KHZ= 2
    FREQUENCY_1_MHZ= 3
    FREQUENCY_2_MHZ= 4
    FREQUENCY_4_MHZ= 5
    FREQUENCY_8_MHZ= 6
