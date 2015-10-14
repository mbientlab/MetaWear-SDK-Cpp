from ctypes import *
import uuid

# Constants indicating status codes
class Status:
    OK= 0
    WARNING_UNEXPECTED_SENSOR_DATA= 1

# Constants identifying the message data types
class DataTypeId:
    UINT32= 0
    FLOAT= 1
    CARTESIAN_FLOAT= 2

# Python wrapper for the MblMwMessage struct
class Data(Structure):
    _fields_= [
        ("value", c_void_p),
        ("type_id", c_int)
    ]

# Python wrapper for the send_command variable type
SendCommand= CFUNCTYPE(None, c_void_p, POINTER(c_ubyte), c_ubyte);
# Python wrapper for the data_handler variable type
ReceivedSensorData= CFUNCTYPE(None, c_void_p, POINTER(Data))

# Python wrapper for the MblMwConnection struct
class MetaWearConnection(Structure):
    _fields_= [
        ("send_command", SendCommand),
        ("received_sensor_data", ReceivedSensorData)
    ]

# UUIDs for the MetaWear gatt services and characteristics
class Gatt:
    SERVICE_UUID= uuid.UUID('{326A9000-85CB-9195-D9DD-464CFBBAE75A}')
    CHAR_COMMAND_UUID= uuid.UUID('{326A9001-85CB-9195-D9DD-464CFBBAE75A}')
    CHAR_NOTIFICATION_UUID= uuid.UUID('{326A9006-85CB-9195-D9DD-464CFBBAE75A}')

# Constants for configuring the BMI160 accelerometer
class AccelerometerBmi160:
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

    FSR_2G= 0
    FSR_4G= 1
    FSR_8G= 2
    FSR_16G= 3

# Constants for configuring the MMA8452Q accelerometer
class AccelerometerMma8452q:
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

# Constants for configuring the BMP280 barometer
class BarometerBmp280:
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

    STANDBY_TIME_0_5MS= 0
    STANDBY_TIME_62_5MS= 1
    STANDBY_TIME_125MS= 2
    STANDBY_TIME_250MS= 3
    STANDBY_TIME_500MS= 4
    STANDBY_TIME_1000MS= 5
    STANDBY_TIME_2000MS= 6
    STANDBY_TIME_4000MS= 7

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

# Constants and types for configuring the Led
class Led:
    # Python wrapper for the MblMwLedPattern struct
    class Pattern(Structure):
        _fields_= [
            ("rise_time_ms", c_ushort),
            ("high_time_ms", c_ushort),
            ("fall_time_ms", c_ushort),
            ("pulse_duration_ms", c_ushort),
            ("high_intensity", c_ubyte),
            ("low_intensity", c_ubyte),
            ("repeat_count", c_ubyte)
        ]

    COLOR_GREEN= 0
    COLOR_RED= 1
    COLOR_BLUE= 2

    PRESET_BLINK= 0
    PRESET_PULSE= 1
    PRESET_SOLID= 2

# Constants for configuring NeoPixel strands
class NeoPixel:
    COLOR_ORDERING_WS2811_RGB= 0
    COLOR_ORDERING_WS2811_RBG= 1
    COLOR_ORDERING_WS2811_GRB= 2
    COLOR_ORDERING_WS2811_GBR= 3

    ROT_DIRECTION_TOWARDS= 0
    ROT_DIRECTION_AWAY= 1

# Python wrapper for the MblMwDataCartesianFloat struct
class CartesianFloat(Structure):
    _fields_= [
        ("x", c_float),
        ("y", c_float),
        ("z", c_float)
    ]

    # taken from https://www.python.org/dev/peps/pep-0485/#proposed-implementation
    def is_close(fst, snd):
        return abs(fst - snd) <= max( 0.001 * max(abs(fst), abs(snd)), 0.0 )

    def __eq__(self, other):
        return CartesianFloat.is_close(self.x, other.x) and CartesianFloat.is_close(self.y, other.y) and \
                CartesianFloat.is_close(self.z, other.z)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "(%.3f, %.3f, %.3f)" % (self.x, self.y, self.z)

    def __deepcopy__(self, memo):
        return CartesianFloat(x= self.x, y= self.y, z= self.z)

# Python wrapper for the MblMwDataCartesianShort struct
class CartesianShort(Structure):
    _fields_= [
        ("x", c_short),
        ("y", c_short),
        ("z", c_short)
    ]

    def __eq__(self, other):
        return (self.x == other.x) and (self.y == other.y) and (self.z == other.z)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "(%d, %d, %d)" % (self.x, self.y, self.z)

    def __deepcopy__(self, memo):
        return CartesianFloat(x= self.x, y= self.y, z= self.z)
