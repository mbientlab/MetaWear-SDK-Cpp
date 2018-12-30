# Warning!!!
# This python module is generated from the MetaWear C++ API header files
# Do not edit
from ctypes import * 
import sys
import copy

if sys.version_info[0] == 2:
    range = xrange

def array_ubyte_eq(a, a_len, b, b_len):
    if (a_len != b_len):
        return False
    for i in range(a_len):
        if (a[i] != b[i]):
            return False
    return True

def array_ubyte_to_hex_string(a, a_len):
    if (a_len == 0):
        return "[]"
    string = "[" + "0x%02x" % (a[0])
    for i in range(a_len - 1):
        string+= ", " + "0x%02x" % (a[i + 1])
    string+= "]"
    return string

def array_ubyte_deep_copy(a, a_len):
    ptr = cast(a, POINTER(c_ubyte * a_len))
    return cast(copy.deepcopy(ptr.contents), POINTER(c_ubyte))

def module_info_eq(this, that):
    return (this.name == that.name and array_ubyte_eq(this.extra, this.extra_len, that.extra, that.extra_len) and this.present == that.present and this.implementation == that.implementation and this.revision == that.revision)

def module_info_repr(this):
    return "{name : %s, extra : %s, extra_len : %d, present : %d, implementation : %d, revision : %d}" % (this.name, array_ubyte_to_hex_string(this.extra, this.extra_len), this.extra_len, this.present, this.implementation, this.revision)

def module_info_deepcopy(this, memo):
    return ModuleInfo(name = this.name, extra = array_ubyte_deep_copy(this.extra, this.extra_len), extra_len = this.extra_len, present = this.present, implementation = this.implementation, revision = this.revision)

# taken from https://www.python.org/dev/peps/pep-0485/#proposed-implementation
def is_close(fst, snd):
    return abs(fst - snd) <= max( 0.001 * max(abs(fst), abs(snd)), 0.001 )

class SensorFusionData:
    CORRECTED_ACC = 0
    CORRECTED_GYRO = 1
    CORRECTED_MAG = 2
    QUATERNION = 3
    EULER_ANGLE = 4
    GRAVITY_VECTOR = 5
    LINEAR_ACC = 6

class SensorFusionGyroRange:
    _2000DPS = 0
    _1000DPS = 1
    _500DPS = 2
    _250DPS = 3

class AccBoschDoubleTapWindow:
    _50ms = 0
    _100ms = 1
    _150ms = 2
    _200ms = 3
    _250ms = 4
    _375ms = 5
    _500ms = 6
    _700ms = 7

class AccBoschTapShockTime:
    _50ms = 0
    _75ms = 1

class AccBoschOrientationMode:
    SYMMETRICAL = 0
    HIGH_ASYMMETRICAL = 1
    LOW_ASYMMETRICAL = 2

class AccBmi160StepCounterMode:
    NORMAL = 0
    SENSITIVE = 1
    ROBUST = 2

class AccBmi160Odr:
    _0_78125Hz = 0
    _1_5625Hz = 1
    _3_125Hz = 2
    _6_25Hz = 3
    _12_5Hz = 4
    _25Hz = 5
    _50Hz = 6
    _100Hz = 7
    _200Hz = 8
    _400Hz = 9
    _800Hz = 10
    _1600Hz = 11

class AccBoschRange:
    _2G = 0
    _4G = 1
    _8G = 2
    _16G = 3

class SpiFrequency:
    _125KHz = 0
    _250KHz = 1
    _500KHz = 2
    _1MHz = 3
    _2MHz = 4
    _4MHz = 5
    _8MHz = 6

class ProximityTsl2671Current:
    _100mA = 0
    _50mA = 1
    _25mA = 2
    _12_5mA = 3

class MetaWearRChannel:
    ON_DIE = 0
    EXT_THERMISTOR = 1

class TemperatureSource:
    INVALID = -1
    NRF_DIE = 0
    EXT_THERM = 1
    BMP280 = 2
    PRESET_THERM = 3

class AccBoschTapQuietTime:
    _30ms = 0
    _20ms = 1

class MagBmm150Odr:
    _10Hz = 0
    _2Hz = 1
    _6Hz = 2
    _8Hz = 3
    _15Hz = 4
    _20Hz = 5
    _25Hz = 6
    _30Hz = 7

class SpiMode:
    _0 = 0
    _1 = 1
    _2 = 2
    _3 = 3

class PassthroughMode:
    ALL = 0
    CONDITIONAL = 1
    COUNT = 2

class MagBmm150Preset:
    LOW_POWER = 0
    REGULAR = 1
    ENHANCED_REGULAR = 2
    HIGH_ACCURACY = 3

class ThresholdMode:
    ABSOLUTE = 0
    BINARY = 1

class ComparatorOperation:
    EQ = 0
    NEQ = 1
    LT = 2
    LTE = 3
    GT = 4
    GTE = 5

class BleAdType:
    CONNECTED_UNDIRECTED = 0
    CONNECTED_DIRECTED = 1

class NeoPixelColorOrdering:
    WS2811_RGB = 0
    WS2811_RBG = 1
    WS2811_GRB = 2
    WS2811_GBR = 3

class LedPreset:
    BLINK = 0
    PULSE = 1
    SOLID = 2

class DeltaMode:
    ABSOLUTE = 0
    DIFFERENTIAL = 1
    BINARY = 2

class SensorFusionAccRange:
    _2G = 0
    _4G = 1
    _8G = 2
    _16G = 3

class ComparatorMode:
    ABSOLUTE = 0
    REFERENCE = 1
    ZONE = 2
    BINARY = 3

class GattCharWriteType:
    WITH_RESPONSE = 0
    WITHOUT_RESPONSE = 1

class MetaWearRProChannel:
    ON_DIE = 0
    ON_BOARD_THERMISTOR = 1
    EXT_THERMISTOR = 2
    BMP280 = 3

class HumidityBme280Oversampling:
    _1X = 1
    _2X = 2
    _4X = 3
    _8X = 4
    _16X = 5

class ConductanceRange:
    _50uS = 0
    _100uS = 1
    _150uS = 2
    _200uS = 3

class GpioPinChangeType:
    RISING = 1
    FALLING = 2
    ANY = 3

class NeoPixelRotDirection:
    TOWARDS = 0
    AWAY = 1

class SensorFusionMode:
    SLEEP = 0
    NDOF = 1
    IMU_PLUS = 2
    COMPASS = 3
    M4G = 4

class DataTypeId:
    UINT32 = 0
    FLOAT = 1
    CARTESIAN_FLOAT = 2
    INT32 = 3
    BYTE_ARRAY = 4
    BATTERY_STATE = 5
    TCS34725_ADC = 6
    EULER_ANGLE = 7
    QUATERNION = 8
    CORRECTED_CARTESIAN_FLOAT = 9
    OVERFLOW_STATE = 10
    SENSOR_ORIENTATION = 11
    STRING = 12
    LOGGING_TIME = 13
    BTLE_ADDRESS = 14
    BOSCH_ANY_MOTION = 15
    CALIBRATION_STATE = 16
    DATA_ARRAY = 17
    BOSCH_TAP = 18

class Model:
    NA = -1
    METAWEAR_R = 0
    METAWEAR_RG = 1
    METAWEAR_RPRO = 2
    METAWEAR_C = 3
    METAWEAR_CPRO = 4
    METAENV = 5
    METADETECT = 6
    METAHEALTH = 7
    METATRACKER = 8
    METAMOTION_R = 9
    METAMOTION_C = 10

class SensorOrientation:
    FACE_UP_PORTRAIT_UPRIGHT = 0
    FACE_UP_PORTRAIT_UPSIDE_DOWN = 1
    FACE_UP_LANDSCAPE_LEFT = 2
    FACE_UP_LANDSCAPE_RIGHT = 3
    FACE_DOWN_PORTRAIT_UPRIGHT = 4
    FACE_DOWN_PORTRAIT_UPSIDE_DOWN = 5
    FACE_DOWN_LANDSCAPE_LEFT = 6
    FACE_DOWN_LANDSCAPE_RIGHT = 7

class PulseOutput:
    WIDTH = 0
    AREA = 1
    PEAK = 2
    ON_DETECTION = 3

class ProximityTsl2671Channel:
    _0 = 1
    _1 = 2
    BOTH = 3

class AlsLtr329Gain:
    _1X = 0
    _2X = 1
    _4X = 2
    _8X = 3
    _48X = 4
    _96X = 5

class Module:
    SWITCH = 1
    LED = 2
    ACCELEROMETER = 3
    TEMPERATURE = 4
    GPIO = 5
    NEO_PIXEL = 6
    IBEACON = 7
    HAPTIC = 8
    DATA_PROCESSOR = 9
    EVENT = 10
    LOGGING = 11
    TIMER = 12
    I2C = 13
    MACRO = 15
    CONDUCTANCE = 16
    SETTINGS = 17
    BAROMETER = 18
    GYRO = 19
    AMBIENT_LIGHT = 20
    MAGNETOMETER = 21
    HUMIDITY = 22
    COLOR_DETECTOR = 23
    PROXIMITY = 24
    SENSOR_FUSION = 25
    DEBUG = 254

class AccMma8452qRange:
    _2G = 0
    _4G = 1
    _8G = 2

class GyroBmi160Range:
    _2000dps = 0
    _1000dps = 1
    _500dps = 2
    _250dps = 3
    _125dps = 4

class AlsLtr329IntegrationTime:
    _100ms = 0
    _50ms = 1
    _200ms = 2
    _400ms = 3
    _150ms = 4
    _250ms = 5
    _300ms = 6
    _350ms = 7

class MathOperation:
    ADD = 1
    MULTIPLY = 2
    DIVIDE = 3
    MODULUS = 4
    EXPONENT = 5
    SQRT = 6
    LSHIFT = 7
    RSHIFT = 8
    SUBTRACT = 9
    ABS_VALUE = 10
    CONSTANT = 11

class AccMma8452qOdr:
    _800Hz = 0
    _400Hz = 1
    _200Hz = 2
    _100Hz = 3
    _50Hz = 4
    _12_5Hz = 5
    _6_25Hz = 6
    _1_56Hz = 7

class AccMma8452qCutoffFreq:
    HIGHEST = 0
    HIGH = 1
    MEDIUM = 2
    LOW = 3

class AlsLtr329MeasurementRate:
    _50ms = 0
    _100ms = 1
    _200ms = 2
    _500ms = 3
    _1000ms = 4
    _2000ms = 5

class GpioPullMode:
    UP = 0
    DOWN = 1
    NONE = 2

class BaroBoschOversampling:
    SKIP = 0
    ULTRA_LOW_POWER = 1
    LOW_POWER = 2
    STANDARD = 3
    HIGH = 4
    ULTRA_HIGH = 5

class LedColor:
    GREEN = 0
    RED = 1
    BLUE = 2

class TimeMode:
    ABSOLUTE = 0
    DIFFERENTIAL = 1

class BaroBoschIirFilter:
    OFF = 0
    AVG_2 = 1
    AVG_4 = 2
    AVG_8 = 3
    AVG_16 = 4

class BaroBmp280StandbyTime:
    _0_5ms = 0
    _62_5ms = 1
    _125ms = 2
    _250ms = 3
    _500ms = 4
    _1000ms = 5
    _2000ms = 6
    _4000ms = 7

class WhitelistFilter:
    ALLOW_FROM_ANY = 0
    SCAN_REQUESTS = 1
    CONNECTION_REQUESTS = 2
    SCAN_AND_CONNECTION_REQUESTS = 3

class BaroBme280StandbyTime:
    _0_5ms = 0
    _62_5ms = 1
    _125ms = 2
    _250ms = 3
    _500ms = 4
    _1000ms = 5
    _10ms = 6
    _20ms = 7

class ColorDetectorTcs34725Gain:
    _1X = 0
    _4X = 1
    _16X = 2
    _60X = 3

class AccBma255Odr:
    _15_62Hz = 0
    _31_26Hz = 1
    _62_5Hz = 2
    _125Hz = 3
    _250Hz = 4
    _500Hz = 5
    _1000Hz = 6
    _2000Hz = 7

class GpioAnalogReadMode:
    ABS_REF = 0
    ADC = 1

class GyroBmi160Odr:
    _25Hz = 6
    _50Hz = 7
    _100Hz = 8
    _200Hz = 9
    _400Hz = 10
    _800Hz = 11
    _1600Hz = 12
    _3200Hz = 13

FnVoid_VoidP_VoidP = CFUNCTYPE(None, c_void_p, c_void_p)
FnVoid_VoidP_VoidP_Int = CFUNCTYPE(None, c_void_p, c_void_p, c_int)
FnInt_VoidP_UByteP_UByte = CFUNCTYPE(c_int, c_void_p, POINTER(c_ubyte), c_ubyte)
FnVoid_VoidP_Int = CFUNCTYPE(None, c_void_p, c_int)
class Data(Structure):
    _fields_ = [
        ("epoch" , c_longlong),
        ("extra" , c_void_p),
        ("value" , c_void_p),
        ("type_id" , c_int),
        ("length" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.epoch == other.epoch and self.extra == other.extra and self.value == other.value and self.type_id == other.type_id and self.length == other.length)

    def __repr__(self):
        return "{epoch : %d, extra : %d, value : %d, type_id : %d, length : %d}" % (self.epoch, self.extra, self.value, self.type_id, self.length)

    def __deepcopy__(self, memo):
        return Data(epoch = self.epoch, extra = self.extra, value = self.value, type_id = self.type_id, length = self.length)

FnVoid_VoidP_DataP = CFUNCTYPE(None, c_void_p, POINTER(Data))
class CalibrationData(Structure):
    _fields_ = [
        ("acc" , (c_ubyte * 10)),
        ("gyro" , (c_ubyte * 10)),
        ("mag" , (c_ubyte * 10))
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (array_ubyte_eq(self.acc, 10, other.acc, 10) and array_ubyte_eq(self.gyro, 10, other.gyro, 10) and array_ubyte_eq(self.mag, 10, other.mag, 10))

    def __repr__(self):
        return "{acc : %s, gyro : %s, mag : %s}" % (array_ubyte_to_hex_string(self.acc, 10), array_ubyte_to_hex_string(self.gyro, 10), array_ubyte_to_hex_string(self.mag, 10))

    def __deepcopy__(self, memo):
        return CalibrationData(acc = copy.deepcopy(self.acc), gyro = copy.deepcopy(self.gyro), mag = copy.deepcopy(self.mag))

FnVoid_VoidP_VoidP_CalibrationDataP = CFUNCTYPE(None, c_void_p, c_void_p, POINTER(CalibrationData))
FnVoid_VoidP_VoidP_VoidP_UInt = CFUNCTYPE(None, c_void_p, c_void_p, c_void_p, c_uint)
FnVoid_VoidP = CFUNCTYPE(None, c_void_p)
class SpiParameters(Structure):
    _fields_ = [
        ("mode" , c_int),
        ("frequency" , c_int),
        ("data" , POINTER(c_ubyte)),
        ("data_length" , c_ubyte),
        ("slave_select_pin" , c_ubyte),
        ("clock_pin" , c_ubyte),
        ("mosi_pin" , c_ubyte),
        ("miso_pin" , c_ubyte),
        ("lsb_first" , c_ubyte),
        ("use_nrf_pins" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.mode == other.mode and self.frequency == other.frequency and self.data == other.data and self.data_length == other.data_length and self.slave_select_pin == other.slave_select_pin and self.clock_pin == other.clock_pin and self.mosi_pin == other.mosi_pin and self.miso_pin == other.miso_pin and self.lsb_first == other.lsb_first and self.use_nrf_pins == other.use_nrf_pins)

    def __repr__(self):
        return "{mode : %d, frequency : %d, data : %d, data_length : %d, slave_select_pin : %d, clock_pin : %d, mosi_pin : %d, miso_pin : %d, lsb_first : %d, use_nrf_pins : %d}" % (self.mode, self.frequency, self.data, self.data_length, self.slave_select_pin, self.clock_pin, self.mosi_pin, self.miso_pin, self.lsb_first, self.use_nrf_pins)

    def __deepcopy__(self, memo):
        return SpiParameters(mode = self.mode, frequency = self.frequency, data = self.data, data_length = self.data_length, slave_select_pin = self.slave_select_pin, clock_pin = self.clock_pin, mosi_pin = self.mosi_pin, miso_pin = self.miso_pin, lsb_first = self.lsb_first, use_nrf_pins = self.use_nrf_pins)

class I2cReadParameters(Structure):
    _fields_ = [
        ("device_addr" , c_ubyte),
        ("register_addr" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.device_addr == other.device_addr and self.register_addr == other.register_addr)

    def __repr__(self):
        return "{device_addr : %d, register_addr : %d}" % (self.device_addr, self.register_addr)

    def __deepcopy__(self, memo):
        return I2cReadParameters(device_addr = self.device_addr, register_addr = self.register_addr)

class GpioAnalogReadParameters(Structure):
    _fields_ = [
        ("pullup_pin" , c_ubyte),
        ("pulldown_pin" , c_ubyte),
        ("virtual_pin" , c_ubyte),
        ("delay_us" , c_ushort)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.pullup_pin == other.pullup_pin and self.pulldown_pin == other.pulldown_pin and self.virtual_pin == other.virtual_pin and self.delay_us == other.delay_us)

    def __repr__(self):
        return "{pullup_pin : %d, pulldown_pin : %d, virtual_pin : %d, delay_us : %d}" % (self.pullup_pin, self.pulldown_pin, self.virtual_pin, self.delay_us)

    def __deepcopy__(self, memo):
        return GpioAnalogReadParameters(pullup_pin = self.pullup_pin, pulldown_pin = self.pulldown_pin, virtual_pin = self.virtual_pin, delay_us = self.delay_us)

FnVoid_VoidP_UInt_UInt = CFUNCTYPE(None, c_void_p, c_uint, c_uint)
FnVoid_VoidP_UByte_Long_UByteP_UByte = CFUNCTYPE(None, c_void_p, c_ubyte, c_longlong, POINTER(c_ubyte), c_ubyte)
class LogDownloadHandler(Structure):
    _fields_ = [
        ("context" , c_void_p),
        ("received_progress_update" , FnVoid_VoidP_UInt_UInt),
        ("received_unknown_entry" , FnVoid_VoidP_UByte_Long_UByteP_UByte),
        ("received_unhandled_entry" , FnVoid_VoidP_DataP)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.context == other.context and self.received_progress_update == other.received_progress_update and self.received_unknown_entry == other.received_unknown_entry and self.received_unhandled_entry == other.received_unhandled_entry)

    def __repr__(self):
        return "{context : %d, received_progress_update : %d, received_unknown_entry : %d, received_unhandled_entry : %d}" % (self.context, self.received_progress_update, self.received_unknown_entry, self.received_unhandled_entry)

    def __deepcopy__(self, memo):
        return LogDownloadHandler(context = self.context, received_progress_update = self.received_progress_update, received_unknown_entry = self.received_unknown_entry, received_unhandled_entry = self.received_unhandled_entry)

class BtleAddress(Structure):
    _fields_ = [
        ("address_type" , c_ubyte),
        ("address" , (c_ubyte * 6))
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.address_type == other.address_type and array_ubyte_eq(self.address, 6, other.address, 6))

    def __repr__(self):
        return "{address_type : %d, address : %s}" % (self.address_type, array_ubyte_to_hex_string(self.address, 6))

    def __deepcopy__(self, memo):
        return BtleAddress(address_type = self.address_type, address = copy.deepcopy(self.address))

class CalibrationState(Structure):
    _fields_ = [
        ("accelrometer" , c_ubyte),
        ("gyroscope" , c_ubyte),
        ("magnetometer" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.accelrometer == other.accelrometer and self.gyroscope == other.gyroscope and self.magnetometer == other.magnetometer)

    def __repr__(self):
        return "{accelrometer : %d, gyroscope : %d, magnetometer : %d}" % (self.accelrometer, self.gyroscope, self.magnetometer)

    def __deepcopy__(self, memo):
        return CalibrationState(accelrometer = self.accelrometer, gyroscope = self.gyroscope, magnetometer = self.magnetometer)

class BoschAnyMotion(Structure):
    _fields_ = [
        ("sign" , c_ubyte),
        ("x_axis_active" , c_ubyte),
        ("y_axis_active" , c_ubyte),
        ("z_axis_active" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.sign == other.sign and self.x_axis_active == other.x_axis_active and self.y_axis_active == other.y_axis_active and self.z_axis_active == other.z_axis_active)

    def __repr__(self):
        return "{sign : %d, x_axis_active : %d, y_axis_active : %d, z_axis_active : %d}" % (self.sign, self.x_axis_active, self.y_axis_active, self.z_axis_active)

    def __deepcopy__(self, memo):
        return BoschAnyMotion(sign = self.sign, x_axis_active = self.x_axis_active, y_axis_active = self.y_axis_active, z_axis_active = self.z_axis_active)

FnVoid_VoidP_UByte_UByte_UInt_UInt = CFUNCTYPE(None, c_void_p, c_ubyte, c_ubyte, c_uint, c_uint)
FnVoid_VoidP_VoidP_FnBoardPtr = CFUNCTYPE(None, c_void_p, c_void_p, FnVoid_VoidP)
class RawLogDownloadHandler(Structure):
    _fields_ = [
        ("context" , c_void_p),
        ("received_entry" , FnVoid_VoidP_UByte_UByte_UInt_UInt),
        ("received_progress_update" , FnVoid_VoidP_UInt_UInt),
        ("logging_page_completed" , FnVoid_VoidP_VoidP_FnBoardPtr)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.context == other.context and self.received_entry == other.received_entry and self.received_progress_update == other.received_progress_update and self.logging_page_completed == other.logging_page_completed)

    def __repr__(self):
        return "{context : %d, received_entry : %d, received_progress_update : %d, logging_page_completed : %d}" % (self.context, self.received_entry, self.received_progress_update, self.logging_page_completed)

    def __deepcopy__(self, memo):
        return RawLogDownloadHandler(context = self.context, received_entry = self.received_entry, received_progress_update = self.received_progress_update, logging_page_completed = self.logging_page_completed)

class OverflowState(Structure):
    _fields_ = [
        ("length" , c_ushort),
        ("assert_en" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.length == other.length and self.assert_en == other.assert_en)

    def __repr__(self):
        return "{length : %d, assert_en : %d}" % (self.length, self.assert_en)

    def __deepcopy__(self, memo):
        return OverflowState(length = self.length, assert_en = self.assert_en)

class CartesianFloat(Structure):
    _fields_ = [
        ("x" , c_float),
        ("y" , c_float),
        ("z" , c_float)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (is_close(self.x, other.x) and is_close(self.y, other.y) and is_close(self.z, other.z))

    def __repr__(self):
        return "{x : %.3f, y : %.3f, z : %.3f}" % (self.x, self.y, self.z)

    def __deepcopy__(self, memo):
        return CartesianFloat(x = self.x, y = self.y, z = self.z)

class Quaternion(Structure):
    _fields_ = [
        ("w" , c_float),
        ("x" , c_float),
        ("y" , c_float),
        ("z" , c_float)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (is_close(self.w, other.w) and is_close(self.x, other.x) and is_close(self.y, other.y) and is_close(self.z, other.z))

    def __repr__(self):
        return "{w : %.3f, x : %.3f, y : %.3f, z : %.3f}" % (self.w, self.x, self.y, self.z)

    def __deepcopy__(self, memo):
        return Quaternion(w = self.w, x = self.x, y = self.y, z = self.z)

class GattChar(Structure):
    _fields_ = [
        ("service_uuid_high" , c_ulonglong),
        ("service_uuid_low" , c_ulonglong),
        ("uuid_high" , c_ulonglong),
        ("uuid_low" , c_ulonglong)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.service_uuid_high == other.service_uuid_high and self.service_uuid_low == other.service_uuid_low and self.uuid_high == other.uuid_high and self.uuid_low == other.uuid_low)

    def __repr__(self):
        return "{service_uuid_high : %d, service_uuid_low : %d, uuid_high : %d, uuid_low : %d}" % (self.service_uuid_high, self.service_uuid_low, self.uuid_high, self.uuid_low)

    def __deepcopy__(self, memo):
        return GattChar(service_uuid_high = self.service_uuid_high, service_uuid_low = self.service_uuid_low, uuid_high = self.uuid_high, uuid_low = self.uuid_low)

FnVoid_VoidP_VoidP_GattCharWriteType_GattCharP_UByteP_UByte = CFUNCTYPE(None, c_void_p, c_void_p, c_int, POINTER(GattChar), POINTER(c_ubyte), c_ubyte)
FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray = CFUNCTYPE(None, c_void_p, c_void_p, POINTER(GattChar), FnInt_VoidP_UByteP_UByte)
FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt = CFUNCTYPE(None, c_void_p, c_void_p, POINTER(GattChar), FnInt_VoidP_UByteP_UByte, FnVoid_VoidP_Int)
FnVoid_VoidP_VoidP_FnVoidVoidPtrInt = CFUNCTYPE(None, c_void_p, c_void_p, FnVoid_VoidP_Int)
class BtleConnection(Structure):
    _fields_ = [
        ("context" , c_void_p),
        ("write_gatt_char" , FnVoid_VoidP_VoidP_GattCharWriteType_GattCharP_UByteP_UByte),
        ("read_gatt_char" , FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray),
        ("enable_notifications" , FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt),
        ("on_disconnect" , FnVoid_VoidP_VoidP_FnVoidVoidPtrInt)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.context == other.context and self.write_gatt_char == other.write_gatt_char and self.read_gatt_char == other.read_gatt_char and self.enable_notifications == other.enable_notifications and self.on_disconnect == other.on_disconnect)

    def __repr__(self):
        return "{context : %d, write_gatt_char : %d, read_gatt_char : %d, enable_notifications : %d, on_disconnect : %d}" % (self.context, self.write_gatt_char, self.read_gatt_char, self.enable_notifications, self.on_disconnect)

    def __deepcopy__(self, memo):
        return BtleConnection(context = self.context, write_gatt_char = self.write_gatt_char, read_gatt_char = self.read_gatt_char, enable_notifications = self.enable_notifications, on_disconnect = self.on_disconnect)

class DeviceInformation(Structure):
    _fields_ = [
        ("manufacturer" , c_char_p),
        ("model_number" , c_char_p),
        ("serial_number" , c_char_p),
        ("firmware_revision" , c_char_p),
        ("hardware_revision" , c_char_p)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.manufacturer == other.manufacturer and self.model_number == other.model_number and self.serial_number == other.serial_number and self.firmware_revision == other.firmware_revision and self.hardware_revision == other.hardware_revision)

    def __repr__(self):
        return "{manufacturer : %s, model_number : %s, serial_number : %s, firmware_revision : %s, hardware_revision : %s}" % (self.manufacturer, self.model_number, self.serial_number, self.firmware_revision, self.hardware_revision)

    def __deepcopy__(self, memo):
        return DeviceInformation(manufacturer = self.manufacturer, model_number = self.model_number, serial_number = self.serial_number, firmware_revision = self.firmware_revision, hardware_revision = self.hardware_revision)

class LedPattern(Structure):
    _fields_ = [
        ("high_intensity" , c_ubyte),
        ("low_intensity" , c_ubyte),
        ("rise_time_ms" , c_ushort),
        ("high_time_ms" , c_ushort),
        ("fall_time_ms" , c_ushort),
        ("pulse_duration_ms" , c_ushort),
        ("delay_time_ms" , c_ushort),
        ("repeat_count" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.high_intensity == other.high_intensity and self.low_intensity == other.low_intensity and self.rise_time_ms == other.rise_time_ms and self.high_time_ms == other.high_time_ms and self.fall_time_ms == other.fall_time_ms and self.pulse_duration_ms == other.pulse_duration_ms and self.delay_time_ms == other.delay_time_ms and self.repeat_count == other.repeat_count)

    def __repr__(self):
        return "{high_intensity : %d, low_intensity : %d, rise_time_ms : %d, high_time_ms : %d, fall_time_ms : %d, pulse_duration_ms : %d, delay_time_ms : %d, repeat_count : %d}" % (self.high_intensity, self.low_intensity, self.rise_time_ms, self.high_time_ms, self.fall_time_ms, self.pulse_duration_ms, self.delay_time_ms, self.repeat_count)

    def __deepcopy__(self, memo):
        return LedPattern(high_intensity = self.high_intensity, low_intensity = self.low_intensity, rise_time_ms = self.rise_time_ms, high_time_ms = self.high_time_ms, fall_time_ms = self.fall_time_ms, pulse_duration_ms = self.pulse_duration_ms, delay_time_ms = self.delay_time_ms, repeat_count = self.repeat_count)

class LoggingTime(Structure):
    _fields_ = [
        ("epoch" , c_longlong),
        ("reset_uid" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.epoch == other.epoch and self.reset_uid == other.reset_uid)

    def __repr__(self):
        return "{epoch : %d, reset_uid : %d}" % (self.epoch, self.reset_uid)

    def __deepcopy__(self, memo):
        return LoggingTime(epoch = self.epoch, reset_uid = self.reset_uid)

class CorrectedCartesianFloat(Structure):
    _fields_ = [
        ("x" , c_float),
        ("y" , c_float),
        ("z" , c_float),
        ("accuracy" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (is_close(self.x, other.x) and is_close(self.y, other.y) and is_close(self.z, other.z) and self.accuracy == other.accuracy)

    def __repr__(self):
        return "{x : %.3f, y : %.3f, z : %.3f, accuracy : %d}" % (self.x, self.y, self.z, self.accuracy)

    def __deepcopy__(self, memo):
        return CorrectedCartesianFloat(x = self.x, y = self.y, z = self.z, accuracy = self.accuracy)

class BatteryState(Structure):
    _fields_ = [
        ("voltage" , c_ushort),
        ("charge" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.voltage == other.voltage and self.charge == other.charge)

    def __repr__(self):
        return "{voltage : %d, charge : %d}" % (self.voltage, self.charge)

    def __deepcopy__(self, memo):
        return BatteryState(voltage = self.voltage, charge = self.charge)

class ModuleInfo(Structure):
    _fields_ = [
        ("name" , c_char_p),
        ("extra" , POINTER(c_ubyte)),
        ("extra_len" , c_ubyte),
        ("present" , c_ubyte),
        ("implementation" , c_ubyte),
        ("revision" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return module_info_eq(self, other)

    def __repr__(self):
        return module_info_repr(self)

    def __deepcopy__(self, memo):
        return module_info_deepcopy(self, memo)

FnVoid_VoidP_charP = CFUNCTYPE(None, c_void_p, c_char_p)
class DfuDelegate(Structure):
    _fields_ = [
        ("context" , c_void_p),
        ("on_dfu_started" , FnVoid_VoidP),
        ("on_dfu_cancelled" , FnVoid_VoidP),
        ("on_transfer_percentage" , FnVoid_VoidP_Int),
        ("on_successful_file_transferred" , FnVoid_VoidP),
        ("on_error" , FnVoid_VoidP_charP)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.context == other.context and self.on_dfu_started == other.on_dfu_started and self.on_dfu_cancelled == other.on_dfu_cancelled and self.on_transfer_percentage == other.on_transfer_percentage and self.on_successful_file_transferred == other.on_successful_file_transferred and self.on_error == other.on_error)

    def __repr__(self):
        return "{context : %d, on_dfu_started : %d, on_dfu_cancelled : %d, on_transfer_percentage : %d, on_successful_file_transferred : %d, on_error : %d}" % (self.context, self.on_dfu_started, self.on_dfu_cancelled, self.on_transfer_percentage, self.on_successful_file_transferred, self.on_error)

    def __deepcopy__(self, memo):
        return DfuDelegate(context = self.context, on_dfu_started = self.on_dfu_started, on_dfu_cancelled = self.on_dfu_cancelled, on_transfer_percentage = self.on_transfer_percentage, on_successful_file_transferred = self.on_successful_file_transferred, on_error = self.on_error)

class BoschTap(Structure):
    _fields_ = [
        ("type" , c_ubyte),
        ("sign" , c_ubyte)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.type == other.type and self.sign == other.sign)

    def __repr__(self):
        return "{type : %d, sign : %d}" % (self.type, self.sign)

    def __deepcopy__(self, memo):
        return BoschTap(type = self.type, sign = self.sign)

class Tcs34725ColorAdc(Structure):
    _fields_ = [
        ("clear" , c_ushort),
        ("red" , c_ushort),
        ("green" , c_ushort),
        ("blue" , c_ushort)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (self.clear == other.clear and self.red == other.red and self.green == other.green and self.blue == other.blue)

    def __repr__(self):
        return "{clear : %d, red : %d, green : %d, blue : %d}" % (self.clear, self.red, self.green, self.blue)

    def __deepcopy__(self, memo):
        return Tcs34725ColorAdc(clear = self.clear, red = self.red, green = self.green, blue = self.blue)

class EulerAngles(Structure):
    _fields_ = [
        ("heading" , c_float),
        ("pitch" , c_float),
        ("roll" , c_float),
        ("yaw" , c_float)
    ]

    def __neq__(self, other):
        return not self.__eq__(other)

    def __eq__(self, other):
        return (is_close(self.heading, other.heading) and is_close(self.pitch, other.pitch) and is_close(self.roll, other.roll) and is_close(self.yaw, other.yaw))

    def __repr__(self):
        return "{heading : %.3f, pitch : %.3f, roll : %.3f, yaw : %.3f}" % (self.heading, self.pitch, self.roll, self.yaw)

    def __deepcopy__(self, memo):
        return EulerAngles(heading = self.heading, pitch = self.pitch, roll = self.roll, yaw = self.yaw)

class Const:
    SENSOR_FUSION_CALIBRATION_ACCURACY_LOW = 1
    MODULE_ACC_TYPE_BMA255 = 3
    ADDRESS_TYPE_PRIVATE_NON_RESOLVABLE = 3
    STATUS_ERROR_ENABLE_NOTIFY = 64
    SETTINGS_BATTERY_CHARGE_INDEX = 1
    MODULE_BARO_TYPE_BMP280 = 0
    SENSOR_FUSION_CALIBRATION_ACCURACY_MEDIUM = 2
    STATUS_ERROR_SERIALIZATION_FORMAT = 32
    STATUS_WARNING_UNEXPECTED_SENSOR_DATA = 1
    STATUS_ERROR_TIMEOUT = 16
    STATUS_OK = 0
    SETTINGS_BATTERY_VOLTAGE_INDEX = 0
    SENSOR_FUSION_CALIBRATION_ACCURACY_HIGH = 3
    SENSOR_FUSION_CALIBRATION_ACCURACY_UNRELIABLE = 0
    MODULE_TYPE_NA = -1
    CD_TCS34725_ADC_GREEN_INDEX = 2
    ADDRESS_TYPE_PUBLIC = 0
    ADDRESS_TYPE_RANDOM_STATIC = 1
    SETTINGS_POWER_STATUS_UNSUPPORTED = -1
    STATUS_WARNING_INVALID_PROCESSOR_TYPE = 2
    STATUS_WARNING_INVALID_RESPONSE = 8
    MODULE_BARO_TYPE_BME280 = 1
    ADDRESS_TYPE_PRIVATE_RESOLVABLE = 2
    SETTINGS_CHARGE_STATUS_UNSUPPORTED = -1
    ACC_ACCEL_Z_AXIS_INDEX = 2
    GPIO_UNUSED_PIN = 255
    LED_REPEAT_INDEFINITELY = 255
    ACC_ACCEL_X_AXIS_INDEX = 0
    MODULE_ACC_TYPE_MMA8452Q = 0
    MAG_BFIELD_Y_AXIS_INDEX = 1
    CD_TCS34725_ADC_CLEAR_INDEX = 0
    STATUS_ERROR_UNSUPPORTED_PROCESSOR = 4
    GYRO_ROTATION_Y_AXIS_INDEX = 1
    CD_TCS34725_ADC_RED_INDEX = 1
    ACC_ACCEL_Y_AXIS_INDEX = 1
    CD_TCS34725_ADC_BLUE_INDEX = 3
    MAG_BFIELD_X_AXIS_INDEX = 0
    GYRO_ROTATION_X_AXIS_INDEX = 0
    MODULE_ACC_TYPE_BMI160 = 1
    GYRO_ROTATION_Z_AXIS_INDEX = 2
    MAG_BFIELD_Z_AXIS_INDEX = 2

def init_libmetawear(libmetawear):
    libmetawear.mbl_mw_sensor_fusion_stop.restype = None
    libmetawear.mbl_mw_sensor_fusion_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_sensor_fusion_clear_enabled_mask.restype = None
    libmetawear.mbl_mw_sensor_fusion_clear_enabled_mask.argtypes = [c_void_p]

    libmetawear.mbl_mw_sensor_fusion_write_calibration_data.restype = None
    libmetawear.mbl_mw_sensor_fusion_write_calibration_data.argtypes = [c_void_p, POINTER(CalibrationData)]

    libmetawear.mbl_mw_sensor_fusion_set_gyro_range.restype = None
    libmetawear.mbl_mw_sensor_fusion_set_gyro_range.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_sensor_fusion_set_mode.restype = None
    libmetawear.mbl_mw_sensor_fusion_set_mode.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_sensor_fusion_calibration_state_data_signal.restype = c_void_p
    libmetawear.mbl_mw_sensor_fusion_calibration_state_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_sensor_fusion_get_data_signal.restype = c_void_p
    libmetawear.mbl_mw_sensor_fusion_get_data_signal.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bosch_disable_acceleration_sampling.restype = None
    libmetawear.mbl_mw_acc_bosch_disable_acceleration_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_enable_acceleration_sampling.restype = None
    libmetawear.mbl_mw_acc_bosch_enable_acceleration_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_start.restype = None
    libmetawear.mbl_mw_acc_bosch_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_write_motion_config.restype = None
    libmetawear.mbl_mw_acc_bosch_write_motion_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_set_shock_time.restype = None
    libmetawear.mbl_mw_acc_bosch_set_shock_time.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bosch_set_quiet_time.restype = None
    libmetawear.mbl_mw_acc_bosch_set_quiet_time.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bosch_disable_orientation_detection.restype = None
    libmetawear.mbl_mw_acc_bosch_disable_orientation_detection.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_enable_orientation_detection.restype = None
    libmetawear.mbl_mw_acc_bosch_enable_orientation_detection.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_write_orientation_config.restype = None
    libmetawear.mbl_mw_acc_bosch_write_orientation_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_set_orientation_hysteresis.restype = None
    libmetawear.mbl_mw_acc_bosch_set_orientation_hysteresis.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_acc_bmi160_disable_step_detector.restype = None
    libmetawear.mbl_mw_acc_bmi160_disable_step_detector.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_enable_step_counter.restype = None
    libmetawear.mbl_mw_acc_bmi160_enable_step_counter.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_set_step_counter_mode.restype = None
    libmetawear.mbl_mw_acc_bmi160_set_step_counter_mode.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bmi160_write_step_counter_config.restype = None
    libmetawear.mbl_mw_acc_bmi160_write_step_counter_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_set_odr.restype = None
    libmetawear.mbl_mw_acc_bmi160_set_odr.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bosch_get_motion_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_bosch_get_motion_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_get_orientation_detection_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_bosch_get_orientation_detection_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_get_step_counter_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_bmi160_get_step_counter_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_get_packed_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_bosch_get_packed_acceleration_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_stop.restype = None
    libmetawear.mbl_mw_acc_bosch_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_spi_get_data_signal.restype = c_void_p
    libmetawear.mbl_mw_spi_get_data_signal.argtypes = [c_void_p, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_proximity_tsl2671_write_config.restype = None
    libmetawear.mbl_mw_proximity_tsl2671_write_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_proximity_tsl2671_get_adc_data_signal.restype = c_void_p
    libmetawear.mbl_mw_proximity_tsl2671_get_adc_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_multi_chnl_temp_get_num_channels.restype = c_ubyte
    libmetawear.mbl_mw_multi_chnl_temp_get_num_channels.argtypes = [c_void_p]

    libmetawear.mbl_mw_multi_chnl_temp_configure_ext_thermistor.restype = None
    libmetawear.mbl_mw_multi_chnl_temp_configure_ext_thermistor.argtypes = [c_void_p, c_ubyte, c_ubyte, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal.restype = c_void_p
    libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_acc_bosch_set_double_tap_window.restype = None
    libmetawear.mbl_mw_acc_bosch_set_double_tap_window.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_mag_bmm150_stop.restype = None
    libmetawear.mbl_mw_mag_bmm150_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_start.restype = None
    libmetawear.mbl_mw_mag_bmm150_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_enable_b_field_sampling.restype = None
    libmetawear.mbl_mw_mag_bmm150_enable_b_field_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_get_step_detector_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_bmi160_get_step_detector_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_configure.restype = None
    libmetawear.mbl_mw_mag_bmm150_configure.argtypes = [c_void_p, c_ushort, c_ushort, c_int]

    libmetawear.mbl_mw_mag_bmm150_get_packed_b_field_data_signal.restype = c_void_p
    libmetawear.mbl_mw_mag_bmm150_get_packed_b_field_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal.restype = c_void_p
    libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_enable_rotation_sampling.restype = None
    libmetawear.mbl_mw_gyro_bmi160_enable_rotation_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_stop.restype = None
    libmetawear.mbl_mw_gyro_bmi160_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_start.restype = None
    libmetawear.mbl_mw_gyro_bmi160_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_set_odr.restype = None
    libmetawear.mbl_mw_gyro_bmi160_set_odr.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal.restype = c_void_p
    libmetawear.mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_gpio_stop_pin_monitoring.restype = None
    libmetawear.mbl_mw_gpio_stop_pin_monitoring.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gpio_set_pull_mode.restype = None
    libmetawear.mbl_mw_gpio_set_pull_mode.argtypes = [c_void_p, c_ubyte, c_int]

    libmetawear.mbl_mw_gpio_get_analog_input_data_signal.restype = c_void_p
    libmetawear.mbl_mw_gpio_get_analog_input_data_signal.argtypes = [c_void_p, c_ubyte, c_int]

    libmetawear.mbl_mw_multi_chnl_temp_get_source.restype = c_int
    libmetawear.mbl_mw_multi_chnl_temp_get_source.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_conductance_calibrate.restype = None
    libmetawear.mbl_mw_conductance_calibrate.argtypes = [c_void_p]

    libmetawear.mbl_mw_conductance_get_data_signal.restype = c_void_p
    libmetawear.mbl_mw_conductance_get_data_signal.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_cd_tcs34725_write_config.restype = None
    libmetawear.mbl_mw_cd_tcs34725_write_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_disable_b_field_sampling.restype = None
    libmetawear.mbl_mw_mag_bmm150_disable_b_field_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_humidity_bme280_get_percentage_data_signal.restype = c_void_p
    libmetawear.mbl_mw_humidity_bme280_get_percentage_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_cd_tcs34725_disable_illuminator_led.restype = None
    libmetawear.mbl_mw_cd_tcs34725_disable_illuminator_led.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_set_any_motion_count.restype = None
    libmetawear.mbl_mw_acc_bosch_set_any_motion_count.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_switch_get_state_data_signal.restype = c_void_p
    libmetawear.mbl_mw_switch_get_state_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_proximity_tsl2671_set_n_pulses.restype = None
    libmetawear.mbl_mw_proximity_tsl2671_set_n_pulses.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_cd_tcs34725_enable_illuminator_led.restype = None
    libmetawear.mbl_mw_cd_tcs34725_enable_illuminator_led.argtypes = [c_void_p]

    libmetawear.mbl_mw_gpio_set_pin_change_type.restype = None
    libmetawear.mbl_mw_gpio_set_pin_change_type.argtypes = [c_void_p, c_ubyte, c_int]

    libmetawear.mbl_mw_cd_tcs34725_set_integration_time.restype = None
    libmetawear.mbl_mw_cd_tcs34725_set_integration_time.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_acc_bosch_enable_tap_detection.restype = None
    libmetawear.mbl_mw_acc_bosch_enable_tap_detection.argtypes = [c_void_p, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_baro_bosch_stop.restype = None
    libmetawear.mbl_mw_baro_bosch_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_sensor_fusion_read_config.restype = None
    libmetawear.mbl_mw_sensor_fusion_read_config.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_Int]

    libmetawear.mbl_mw_baro_bosch_start.restype = None
    libmetawear.mbl_mw_baro_bosch_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_baro_bosch_write_config.restype = None
    libmetawear.mbl_mw_baro_bosch_write_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_gpio_clear_digital_output.restype = None
    libmetawear.mbl_mw_gpio_clear_digital_output.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_baro_bosch_set_standby_time.restype = c_float
    libmetawear.mbl_mw_baro_bosch_set_standby_time.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal.restype = c_void_p
    libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_als_ltr329_set_measurement_rate.restype = None
    libmetawear.mbl_mw_als_ltr329_set_measurement_rate.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_als_ltr329_set_gain.restype = None
    libmetawear.mbl_mw_als_ltr329_set_gain.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_mag_bmm150_set_preset.restype = None
    libmetawear.mbl_mw_mag_bmm150_set_preset.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_als_ltr329_get_illuminance_data_signal.restype = c_void_p
    libmetawear.mbl_mw_als_ltr329_get_illuminance_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_proximity_tsl2671_set_transmitter_current.restype = None
    libmetawear.mbl_mw_proximity_tsl2671_set_transmitter_current.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_gyro_bmi160_write_config.restype = None
    libmetawear.mbl_mw_gyro_bmi160_write_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_get_orientation_detection_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_mma8452q_get_orientation_detection_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_cd_tcs34725_set_gain.restype = None
    libmetawear.mbl_mw_cd_tcs34725_set_gain.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_mma8452q_disable_acceleration_sampling.restype = None
    libmetawear.mbl_mw_acc_mma8452q_disable_acceleration_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_stop.restype = None
    libmetawear.mbl_mw_acc_mma8452q_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config.restype = None
    libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_write_tap_config.restype = None
    libmetawear.mbl_mw_acc_bosch_write_tap_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_set_high_pass_cutoff.restype = None
    libmetawear.mbl_mw_acc_mma8452q_set_high_pass_cutoff.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_acc_mma8452q_set_range.restype = None
    libmetawear.mbl_mw_acc_mma8452q_set_range.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bosch_enable_motion_detection.restype = None
    libmetawear.mbl_mw_acc_bosch_enable_motion_detection.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_get_packed_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_mma8452q_get_packed_acceleration_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_enable_acceleration_sampling.restype = None
    libmetawear.mbl_mw_acc_enable_acceleration_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_stop.restype = None
    libmetawear.mbl_mw_acc_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_baro_bosch_get_altitude_data_signal.restype = c_void_p
    libmetawear.mbl_mw_baro_bosch_get_altitude_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_start.restype = None
    libmetawear.mbl_mw_acc_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_disable_rotation_sampling.restype = None
    libmetawear.mbl_mw_gyro_bmi160_disable_rotation_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_baro_bmp280_set_standby_time.restype = None
    libmetawear.mbl_mw_baro_bmp280_set_standby_time.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_write_acceleration_config.restype = None
    libmetawear.mbl_mw_acc_write_acceleration_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_sensor_fusion_set_acc_range.restype = None
    libmetawear.mbl_mw_sensor_fusion_set_acc_range.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_gyro_bmi160_get_packed_rotation_data_signal.restype = c_void_p
    libmetawear.mbl_mw_gyro_bmi160_get_packed_rotation_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_logger_subscribe.restype = None
    libmetawear.mbl_mw_logger_subscribe.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_DataP]

    libmetawear.mbl_mw_debug_send_command.restype = None
    libmetawear.mbl_mw_debug_send_command.argtypes = [c_void_p, POINTER(c_ubyte), c_ubyte]

    libmetawear.mbl_mw_als_ltr329_write_config.restype = None
    libmetawear.mbl_mw_als_ltr329_write_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_logger_get_signal.restype = c_void_p
    libmetawear.mbl_mw_logger_get_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_logging_stop.restype = None
    libmetawear.mbl_mw_logging_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_settings_set_ad_interval.restype = None
    libmetawear.mbl_mw_settings_set_ad_interval.argtypes = [c_void_p, c_ushort, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_delta_set_reference.restype = c_int
    libmetawear.mbl_mw_dataprocessor_delta_set_reference.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_mag_bmm150_suspend.restype = None
    libmetawear.mbl_mw_mag_bmm150_suspend.argtypes = [c_void_p]

    libmetawear.mbl_mw_gpio_get_pin_monitor_data_signal.restype = c_void_p
    libmetawear.mbl_mw_gpio_get_pin_monitor_data_signal.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_acc_get_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_get_acceleration_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_settings_read_current_charge_status.restype = None
    libmetawear.mbl_mw_settings_read_current_charge_status.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_Int]

    libmetawear.mbl_mw_cd_tcs34725_get_adc_data_signal.restype = c_void_p
    libmetawear.mbl_mw_cd_tcs34725_get_adc_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_event_get_owner.restype = c_void_p
    libmetawear.mbl_mw_event_get_owner.argtypes = [c_void_p]

    libmetawear.mbl_mw_settings_get_disconnect_event.restype = c_void_p
    libmetawear.mbl_mw_settings_get_disconnect_event.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_settings_set_whitelist_filter_mode.restype = None
    libmetawear.mbl_mw_settings_set_whitelist_filter_mode.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_settings_set_device_name.restype = None
    libmetawear.mbl_mw_settings_set_device_name.argtypes = [c_void_p, POINTER(c_ubyte), c_ubyte]

    libmetawear.mbl_mw_acc_get_packed_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_get_packed_acceleration_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_datasignal_read.restype = None
    libmetawear.mbl_mw_datasignal_read.argtypes = [c_void_p]

    libmetawear.mbl_mw_macro_record.restype = None
    libmetawear.mbl_mw_macro_record.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_acc_mma8452q_start.restype = None
    libmetawear.mbl_mw_acc_mma8452q_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_settings_get_mac_data_signal.restype = c_void_p
    libmetawear.mbl_mw_settings_get_mac_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_logger_remove.restype = None
    libmetawear.mbl_mw_logger_remove.argtypes = [c_void_p]

    libmetawear.mbl_mw_timer_get_id.restype = c_ubyte
    libmetawear.mbl_mw_timer_get_id.argtypes = [c_void_p]

    libmetawear.mbl_mw_settings_get_battery_state_data_signal.restype = c_void_p
    libmetawear.mbl_mw_settings_get_battery_state_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_led_stop_and_clear.restype = None
    libmetawear.mbl_mw_led_stop_and_clear.argtypes = [c_void_p]

    libmetawear.mbl_mw_metawearboard_deserialize.restype = c_int
    libmetawear.mbl_mw_metawearboard_deserialize.argtypes = [c_void_p, POINTER(c_ubyte), c_uint]

    libmetawear.mbl_mw_neopixel_clear.restype = None
    libmetawear.mbl_mw_neopixel_clear.argtypes = [c_void_p, c_ubyte, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_acc_read_config.restype = None
    libmetawear.mbl_mw_acc_read_config.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_Int]

    libmetawear.mbl_mw_metawearboard_get_module_info.restype = POINTER(ModuleInfo)
    libmetawear.mbl_mw_metawearboard_get_module_info.argtypes = [c_void_p, POINTER(c_uint)]

    libmetawear.mbl_mw_logging_get_reference_time.restype = c_longlong
    libmetawear.mbl_mw_logging_get_reference_time.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_acc_bosch_set_orientation_mode.restype = None
    libmetawear.mbl_mw_acc_bosch_set_orientation_mode.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_metawearboard_get_model_name.restype = c_char_p
    libmetawear.mbl_mw_metawearboard_get_model_name.argtypes = [c_void_p]

    libmetawear.mbl_mw_logging_set_reference_time.restype = None
    libmetawear.mbl_mw_logging_set_reference_time.argtypes = [c_void_p, c_ubyte, c_longlong]

    libmetawear.mbl_mw_dataprocessor_multi_comparator_create_signed.restype = c_int
    libmetawear.mbl_mw_dataprocessor_multi_comparator_create_signed.argtypes = [c_void_p, c_int, c_int, POINTER(c_float), c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_i2c_write.restype = None
    libmetawear.mbl_mw_i2c_write.argtypes = [c_void_p, c_ubyte, c_ubyte, POINTER(c_ubyte), c_ubyte]

    libmetawear.mbl_mw_metawearboard_serialize.restype = POINTER(c_ubyte)
    libmetawear.mbl_mw_metawearboard_serialize.argtypes = [c_void_p, POINTER(c_uint)]

    libmetawear.mbl_mw_acc_mma8452q_set_orientation_delay.restype = None
    libmetawear.mbl_mw_acc_mma8452q_set_orientation_delay.argtypes = [c_void_p, c_ushort]

    libmetawear.mbl_mw_settings_get_power_status_data_signal.restype = c_void_p
    libmetawear.mbl_mw_settings_get_power_status_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_metawearboard_lookup_module.restype = c_int
    libmetawear.mbl_mw_metawearboard_lookup_module.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_sensor_fusion_enable_data.restype = None
    libmetawear.mbl_mw_sensor_fusion_enable_data.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_als_ltr329_start.restype = None
    libmetawear.mbl_mw_als_ltr329_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_logger_get_id.restype = c_ubyte
    libmetawear.mbl_mw_logger_get_id.argtypes = [c_void_p]

    libmetawear.mbl_mw_debug_read_stack_overflow_state.restype = None
    libmetawear.mbl_mw_debug_read_stack_overflow_state.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_DataP]

    libmetawear.mbl_mw_acc_bmi160_disable_step_counter.restype = None
    libmetawear.mbl_mw_acc_bmi160_disable_step_counter.argtypes = [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_read_config.restype = None
    libmetawear.mbl_mw_gyro_bmi160_read_config.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_Int]

    libmetawear.mbl_mw_acc_set_odr.restype = c_float
    libmetawear.mbl_mw_acc_set_odr.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_logging_start.restype = None
    libmetawear.mbl_mw_logging_start.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_debug_jump_to_bootloader.restype = None
    libmetawear.mbl_mw_debug_jump_to_bootloader.argtypes = [c_void_p]

    libmetawear.mbl_mw_macro_end_record.restype = None
    libmetawear.mbl_mw_macro_end_record.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_Int]

    libmetawear.mbl_mw_datasignal_read_with_parameters.restype = None
    libmetawear.mbl_mw_datasignal_read_with_parameters.argtypes = [c_void_p, c_void_p]

    libmetawear.mbl_mw_metawearboard_create.restype = c_void_p
    libmetawear.mbl_mw_metawearboard_create.argtypes = [POINTER(BtleConnection)]

    libmetawear.mbl_mw_ibeacon_set_minor.restype = None
    libmetawear.mbl_mw_ibeacon_set_minor.argtypes = [c_void_p, c_ushort]

    libmetawear.mbl_mw_neopixel_enable_hold.restype = None
    libmetawear.mbl_mw_neopixel_enable_hold.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_memory_free.restype = None
    libmetawear.mbl_mw_memory_free.argtypes = [c_void_p]

    libmetawear.mbl_mw_neopixel_set_color.restype = None
    libmetawear.mbl_mw_neopixel_set_color.argtypes = [c_void_p, c_ubyte, c_ubyte, c_ubyte, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_anonymous_datasignal_subscribe.restype = None
    libmetawear.mbl_mw_anonymous_datasignal_subscribe.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_DataP]

    libmetawear.mbl_mw_logging_download.restype = None
    libmetawear.mbl_mw_logging_download.argtypes = [c_void_p, c_ubyte, POINTER(LogDownloadHandler)]

    libmetawear.mbl_mw_debug_reset.restype = None
    libmetawear.mbl_mw_debug_reset.argtypes = [c_void_p]

    libmetawear.mbl_mw_macro_erase_all.restype = None
    libmetawear.mbl_mw_macro_erase_all.argtypes = [c_void_p]

    libmetawear.mbl_mw_conductance_set_range.restype = None
    libmetawear.mbl_mw_conductance_set_range.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_event_remove_all.restype = None
    libmetawear.mbl_mw_event_remove_all.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_remove.restype = None
    libmetawear.mbl_mw_dataprocessor_remove.argtypes = [c_void_p]

    libmetawear.mbl_mw_metawearboard_create_anonymous_datasignals.restype = None
    libmetawear.mbl_mw_metawearboard_create_anonymous_datasignals.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_VoidP_UInt]

    libmetawear.mbl_mw_logger_lookup_id.restype = c_void_p
    libmetawear.mbl_mw_logger_lookup_id.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_settings_get_whitelist_data_signal.restype = c_void_p
    libmetawear.mbl_mw_settings_get_whitelist_data_signal.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_accounter_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_accounter_create.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_settings_set_scan_response.restype = None
    libmetawear.mbl_mw_settings_set_scan_response.argtypes = [c_void_p, POINTER(c_ubyte), c_ubyte]

    libmetawear.mbl_mw_timer_lookup_id.restype = c_void_p
    libmetawear.mbl_mw_timer_lookup_id.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_settings_set_tx_power.restype = None
    libmetawear.mbl_mw_settings_set_tx_power.argtypes = [c_void_p, c_byte]

    libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal.restype = c_void_p
    libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_sample_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_sample_create.argtypes = [c_void_p, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_baro_bosch_get_pressure_read_data_signal.restype = c_void_p
    libmetawear.mbl_mw_baro_bosch_get_pressure_read_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_sample_modify_bin_size.restype = c_int
    libmetawear.mbl_mw_dataprocessor_sample_modify_bin_size.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_acc_bosch_set_any_motion_threshold.restype = None
    libmetawear.mbl_mw_acc_bosch_set_any_motion_threshold.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_timer_remove.restype = None
    libmetawear.mbl_mw_timer_remove.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_set_threshold.restype = None
    libmetawear.mbl_mw_acc_bosch_set_threshold.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_neopixel_init_fast_strand.restype = None
    libmetawear.mbl_mw_neopixel_init_fast_strand.argtypes = [c_void_p, c_ubyte, c_ubyte, c_ubyte, c_int]

    libmetawear.mbl_mw_settings_get_charge_status_data_signal.restype = c_void_p
    libmetawear.mbl_mw_settings_get_charge_status_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_logging_clear_entries.restype = None
    libmetawear.mbl_mw_logging_clear_entries.argtypes = [c_void_p]

    libmetawear.mbl_mw_timer_create.restype = None
    libmetawear.mbl_mw_timer_create.argtypes = [c_void_p, c_uint, c_ushort, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_settings_read_current_power_status.restype = None
    libmetawear.mbl_mw_settings_read_current_power_status.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_Int]

    libmetawear.mbl_mw_acc_mma8452q_disable_orientation_detection.restype = None
    libmetawear.mbl_mw_acc_mma8452q_disable_orientation_detection.argtypes = [c_void_p]

    libmetawear.mbl_mw_led_autoplay.restype = None
    libmetawear.mbl_mw_led_autoplay.argtypes = [c_void_p]

    libmetawear.mbl_mw_macro_execute.restype = None
    libmetawear.mbl_mw_macro_execute.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_baro_bme280_set_standby_time.restype = None
    libmetawear.mbl_mw_baro_bme280_set_standby_time.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_datasignal_subscribe.restype = None
    libmetawear.mbl_mw_datasignal_subscribe.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_DataP]

    libmetawear.mbl_mw_timer_stop.restype = None
    libmetawear.mbl_mw_timer_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_debug_disconnect.restype = None
    libmetawear.mbl_mw_debug_disconnect.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_enable_step_detector.restype = None
    libmetawear.mbl_mw_acc_bmi160_enable_step_detector.argtypes = [c_void_p]

    libmetawear.mbl_mw_gpio_set_digital_output.restype = None
    libmetawear.mbl_mw_gpio_set_digital_output.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_metawearboard_is_initialized.restype = c_int
    libmetawear.mbl_mw_metawearboard_is_initialized.argtypes = [c_void_p]

    libmetawear.mbl_mw_metawearboard_tear_down.restype = None
    libmetawear.mbl_mw_metawearboard_tear_down.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_reset_step_counter.restype = None
    libmetawear.mbl_mw_acc_bmi160_reset_step_counter.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_threshold_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_threshold_create.argtypes = [c_void_p, c_int, c_float, c_float, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_timer_create_indefinite.restype = None
    libmetawear.mbl_mw_timer_create_indefinite.argtypes = [c_void_p, c_uint, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_anonymous_datasignal_get_identifier.restype = c_char_p
    libmetawear.mbl_mw_anonymous_datasignal_get_identifier.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_comparator_create_signed.restype = c_int
    libmetawear.mbl_mw_dataprocessor_comparator_create_signed.argtypes = [c_void_p, c_int, c_float, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_metawearboard_initialize.restype = None
    libmetawear.mbl_mw_metawearboard_initialize.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_Int]

    libmetawear.mbl_mw_timer_start.restype = None
    libmetawear.mbl_mw_timer_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_ibeacon_enable.restype = None
    libmetawear.mbl_mw_ibeacon_enable.argtypes = [c_void_p]

    libmetawear.mbl_mw_debug_set_key_register.restype = None
    libmetawear.mbl_mw_debug_set_key_register.argtypes = [c_void_p, c_uint]

    libmetawear.mbl_mw_settings_set_ad_parameters.restype = None
    libmetawear.mbl_mw_settings_set_ad_parameters.argtypes = [c_void_p, c_ushort, c_ubyte, c_int]

    libmetawear.mbl_mw_event_record_commands.restype = None
    libmetawear.mbl_mw_event_record_commands.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_average_reset.restype = c_int
    libmetawear.mbl_mw_dataprocessor_average_reset.argtypes = [c_void_p]

    libmetawear.mbl_mw_debug_get_key_register_data_signal.restype = c_void_p
    libmetawear.mbl_mw_debug_get_key_register_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_proximity_tsl2671_set_receiver_channel.restype = None
    libmetawear.mbl_mw_proximity_tsl2671_set_receiver_channel.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_metawearboard_perform_dfu.restype = None
    libmetawear.mbl_mw_metawearboard_perform_dfu.argtypes = [c_void_p, POINTER(DfuDelegate), c_char_p]

    libmetawear.mbl_mw_debug_set_stack_overflow_assertion.restype = None
    libmetawear.mbl_mw_debug_set_stack_overflow_assertion.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gyro_bmi160_set_range.restype = None
    libmetawear.mbl_mw_gyro_bmi160_set_range.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_logger_generate_identifier.restype = c_char_p
    libmetawear.mbl_mw_logger_generate_identifier.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_comparator_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_comparator_create.argtypes = [c_void_p, c_int, c_float, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_metawearboard_free.restype = None
    libmetawear.mbl_mw_metawearboard_free.argtypes = [c_void_p]

    libmetawear.mbl_mw_datasignal_unsubscribe.restype = None
    libmetawear.mbl_mw_datasignal_unsubscribe.argtypes = [c_void_p]

    libmetawear.mbl_mw_datasignal_get_component.restype = c_void_p
    libmetawear.mbl_mw_datasignal_get_component.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_counter_set_state.restype = c_int
    libmetawear.mbl_mw_dataprocessor_counter_set_state.argtypes = [c_void_p, c_uint]

    libmetawear.mbl_mw_acc_set_range.restype = c_float
    libmetawear.mbl_mw_acc_set_range.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_event_end_record.restype = None
    libmetawear.mbl_mw_event_end_record.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_Int]

    libmetawear.mbl_mw_debug_read_schedule_queue_usage.restype = None
    libmetawear.mbl_mw_debug_read_schedule_queue_usage.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_DataP]

    libmetawear.mbl_mw_settings_start_advertising.restype = None
    libmetawear.mbl_mw_settings_start_advertising.argtypes = [c_void_p]

    libmetawear.mbl_mw_metawearboard_get_device_information.restype = POINTER(DeviceInformation)
    libmetawear.mbl_mw_metawearboard_get_device_information.argtypes = [c_void_p]

    libmetawear.mbl_mw_debug_enable_power_save.restype = None
    libmetawear.mbl_mw_debug_enable_power_save.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_multi_comparator_modify.restype = c_int
    libmetawear.mbl_mw_dataprocessor_multi_comparator_modify.argtypes = [c_void_p, c_int, POINTER(c_float), c_ubyte]

    libmetawear.mbl_mw_led_write_pattern.restype = None
    libmetawear.mbl_mw_led_write_pattern.argtypes = [c_void_p, POINTER(LedPattern), c_int]

    libmetawear.mbl_mw_dataprocessor_math_create_signed.restype = c_int
    libmetawear.mbl_mw_dataprocessor_math_create_signed.argtypes = [c_void_p, c_int, c_float, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_debug_spoof_notification.restype = None
    libmetawear.mbl_mw_debug_spoof_notification.argtypes = [c_void_p, POINTER(c_ubyte), c_ubyte]

    libmetawear.mbl_mw_logging_get_length_data_signal.restype = c_void_p
    libmetawear.mbl_mw_logging_get_length_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_spi_write.restype = None
    libmetawear.mbl_mw_spi_write.argtypes = [c_void_p, POINTER(SpiParameters)]

    libmetawear.mbl_mw_logging_get_time_data_signal.restype = c_void_p
    libmetawear.mbl_mw_logging_get_time_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bma255_set_odr.restype = None
    libmetawear.mbl_mw_acc_bma255_set_odr.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_led_stop.restype = None
    libmetawear.mbl_mw_led_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_enable_orientation_detection.restype = None
    libmetawear.mbl_mw_acc_mma8452q_enable_orientation_detection.argtypes = [c_void_p]

    libmetawear.mbl_mw_logging_raw_download.restype = None
    libmetawear.mbl_mw_logging_raw_download.argtypes = [c_void_p, c_ubyte, POINTER(RawLogDownloadHandler)]

    libmetawear.mbl_mw_settings_set_connection_parameters.restype = None
    libmetawear.mbl_mw_settings_set_connection_parameters.argtypes = [c_void_p, c_float, c_float, c_ushort, c_ushort]

    libmetawear.mbl_mw_dataprocessor_counter_create_size.restype = c_int
    libmetawear.mbl_mw_dataprocessor_counter_create_size.argtypes = [c_void_p, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_als_ltr329_set_integration_time.restype = None
    libmetawear.mbl_mw_als_ltr329_set_integration_time.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_haptic_start_buzzer.restype = None
    libmetawear.mbl_mw_haptic_start_buzzer.argtypes = [c_void_p, c_ushort]

    libmetawear.mbl_mw_i2c_get_data_signal.restype = c_void_p
    libmetawear.mbl_mw_i2c_get_data_signal.argtypes = [c_void_p, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_delta_modify_magnitude.restype = c_int
    libmetawear.mbl_mw_dataprocessor_delta_modify_magnitude.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_ibeacon_set_major.restype = None
    libmetawear.mbl_mw_ibeacon_set_major.argtypes = [c_void_p, c_ushort]

    libmetawear.mbl_mw_dataprocessor_time_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_time_create.argtypes = [c_void_p, c_int, c_uint, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_ibeacon_set_major_signal.restype = None
    libmetawear.mbl_mw_ibeacon_set_major_signal.argtypes = [c_void_p, c_void_p]

    libmetawear.mbl_mw_ibeacon_set_minor_signal.restype = None
    libmetawear.mbl_mw_ibeacon_set_minor_signal.argtypes = [c_void_p, c_void_p]

    libmetawear.mbl_mw_ibeacon_set_tx_power.restype = None
    libmetawear.mbl_mw_ibeacon_set_tx_power.argtypes = [c_void_p, c_byte]

    libmetawear.mbl_mw_acc_disable_acceleration_sampling.restype = None
    libmetawear.mbl_mw_acc_disable_acceleration_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_ibeacon_set_rx_power.restype = None
    libmetawear.mbl_mw_ibeacon_set_rx_power.argtypes = [c_void_p, c_byte]

    libmetawear.mbl_mw_gpio_get_digital_input_data_signal.restype = c_void_p
    libmetawear.mbl_mw_gpio_get_digital_input_data_signal.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_ibeacon_set_uuid.restype = None
    libmetawear.mbl_mw_ibeacon_set_uuid.argtypes = [c_void_p, (c_ubyte * 16)]

    libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_datasignal_is_readable.restype = c_int
    libmetawear.mbl_mw_datasignal_is_readable.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_math_modify_rhs.restype = c_int
    libmetawear.mbl_mw_dataprocessor_math_modify_rhs.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_led_pause.restype = None
    libmetawear.mbl_mw_led_pause.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_passthrough_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_passthrough_create.argtypes = [c_void_p, c_int, c_ushort, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_counter_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_counter_create.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_conductance_get_num_channels.restype = c_ubyte
    libmetawear.mbl_mw_conductance_get_num_channels.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_pulse_modify.restype = c_int
    libmetawear.mbl_mw_dataprocessor_pulse_modify.argtypes = [c_void_p, c_float, c_ushort]

    libmetawear.mbl_mw_sensor_fusion_read_calibration_data.restype = None
    libmetawear.mbl_mw_sensor_fusion_read_calibration_data.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP_CalibrationDataP]

    libmetawear.mbl_mw_led_load_preset_pattern.restype = None
    libmetawear.mbl_mw_led_load_preset_pattern.argtypes = [POINTER(LedPattern), c_int]

    libmetawear.mbl_mw_neopixel_rotate.restype = None
    libmetawear.mbl_mw_neopixel_rotate.argtypes = [c_void_p, c_ubyte, c_ubyte, c_ushort, c_int]

    libmetawear.mbl_mw_led_play.restype = None
    libmetawear.mbl_mw_led_play.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_disable_tap_detection.restype = None
    libmetawear.mbl_mw_acc_bosch_disable_tap_detection.argtypes = [c_void_p]

    libmetawear.mbl_mw_ibeacon_disable.restype = None
    libmetawear.mbl_mw_ibeacon_disable.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_average_modify_size.restype = c_int
    libmetawear.mbl_mw_dataprocessor_average_modify_size.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_neopixel_init_slow_strand.restype = None
    libmetawear.mbl_mw_neopixel_init_slow_strand.argtypes = [c_void_p, c_ubyte, c_ubyte, c_ubyte, c_int]

    libmetawear.mbl_mw_dataprocessor_passthrough_modify.restype = c_int
    libmetawear.mbl_mw_dataprocessor_passthrough_modify.argtypes = [c_void_p, c_int, c_ushort]

    libmetawear.mbl_mw_logging_get_latest_reset_uid.restype = c_ubyte
    libmetawear.mbl_mw_logging_get_latest_reset_uid.argtypes = [c_void_p]

    libmetawear.mbl_mw_neopixel_free_strand.restype = None
    libmetawear.mbl_mw_neopixel_free_strand.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gpio_start_pin_monitoring.restype = None
    libmetawear.mbl_mw_gpio_start_pin_monitoring.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_baro_bosch_set_oversampling.restype = None
    libmetawear.mbl_mw_baro_bosch_set_oversampling.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_neopixel_disable_hold.restype = None
    libmetawear.mbl_mw_neopixel_disable_hold.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_sensor_fusion_start.restype = None
    libmetawear.mbl_mw_sensor_fusion_start.argtypes = [c_void_p]

    libmetawear.mbl_mw_neopixel_rotate_indefinitely.restype = None
    libmetawear.mbl_mw_neopixel_rotate_indefinitely.argtypes = [c_void_p, c_ubyte, c_ubyte, c_int]

    libmetawear.mbl_mw_neopixel_stop_rotation.restype = None
    libmetawear.mbl_mw_neopixel_stop_rotation.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_accounter_create_count.restype = c_int
    libmetawear.mbl_mw_dataprocessor_accounter_create_count.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_accumulator_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_accumulator_create.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_metawearboard_get_model.restype = c_int
    libmetawear.mbl_mw_metawearboard_get_model.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_accumulator_create_size.restype = c_int
    libmetawear.mbl_mw_dataprocessor_accumulator_create_size.argtypes = [c_void_p, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_als_ltr329_stop.restype = None
    libmetawear.mbl_mw_als_ltr329_stop.argtypes = [c_void_p]

    libmetawear.mbl_mw_settings_add_whitelist_address.restype = None
    libmetawear.mbl_mw_settings_add_whitelist_address.argtypes = [c_void_p, c_ubyte, POINTER(BtleAddress)]

    libmetawear.mbl_mw_dataprocessor_set_accumulator_state.restype = c_int
    libmetawear.mbl_mw_dataprocessor_set_accumulator_state.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_dataprocessor_average_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_average_create.argtypes = [c_void_p, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_sensor_fusion_write_config.restype = None
    libmetawear.mbl_mw_sensor_fusion_write_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_acc_bosch_write_acceleration_config.restype = None
    libmetawear.mbl_mw_acc_bosch_write_acceleration_config.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_highpass_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_highpass_create.argtypes = [c_void_p, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_get_state_data_signal.restype = c_void_p
    libmetawear.mbl_mw_dataprocessor_get_state_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_haptic_start_motor.restype = None
    libmetawear.mbl_mw_haptic_start_motor.argtypes = [c_void_p, c_float, c_ushort]

    libmetawear.mbl_mw_dataprocessor_lowpass_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_lowpass_create.argtypes = [c_void_p, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_buffer_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_buffer_create.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_proximity_tsl2671_set_integration_time.restype = None
    libmetawear.mbl_mw_proximity_tsl2671_set_integration_time.argtypes = [c_void_p, c_float]

    libmetawear.mbl_mw_acc_mma8452q_enable_acceleration_sampling.restype = None
    libmetawear.mbl_mw_acc_mma8452q_enable_acceleration_sampling.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_comparator_create_unsigned.restype = c_int
    libmetawear.mbl_mw_dataprocessor_comparator_create_unsigned.argtypes = [c_void_p, c_int, c_float, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_comparator_modify.restype = c_int
    libmetawear.mbl_mw_dataprocessor_comparator_modify.argtypes = [c_void_p, c_int, c_float]

    libmetawear.mbl_mw_dataprocessor_comparator_modify_signal.restype = c_int
    libmetawear.mbl_mw_dataprocessor_comparator_modify_signal.argtypes = [c_void_p, c_int, c_void_p]

    libmetawear.mbl_mw_dataprocessor_multi_comparator_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_multi_comparator_create.argtypes = [c_void_p, c_int, c_int, POINTER(c_float), c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_acc_bosch_get_tap_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_bosch_get_tap_data_signal.argtypes = [c_void_p]

    libmetawear.mbl_mw_baro_bosch_set_iir_filter.restype = None
    libmetawear.mbl_mw_baro_bosch_set_iir_filter.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_dataprocessor_multi_comparator_create_unsigned.restype = c_int
    libmetawear.mbl_mw_dataprocessor_multi_comparator_create_unsigned.argtypes = [c_void_p, c_int, c_int, POINTER(c_float), c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_math_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_math_create.argtypes = [c_void_p, c_int, c_float, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_get_id.restype = c_ubyte
    libmetawear.mbl_mw_dataprocessor_get_id.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_lookup_id.restype = c_void_p
    libmetawear.mbl_mw_dataprocessor_lookup_id.argtypes = [c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_delta_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_delta_create.argtypes = [c_void_p, c_int, c_float, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_acc_bosch_disable_motion_detection.restype = None
    libmetawear.mbl_mw_acc_bosch_disable_motion_detection.argtypes = [c_void_p]

    libmetawear.mbl_mw_dataprocessor_math_create_unsigned.restype = c_int
    libmetawear.mbl_mw_dataprocessor_math_create_unsigned.argtypes = [c_void_p, c_int, c_float, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_datasignal_log.restype = None
    libmetawear.mbl_mw_datasignal_log.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_math_modify_rhs_signal.restype = c_int
    libmetawear.mbl_mw_dataprocessor_math_modify_rhs_signal.argtypes = [c_void_p, c_void_p]

    libmetawear.mbl_mw_ibeacon_set_period.restype = None
    libmetawear.mbl_mw_ibeacon_set_period.argtypes = [c_void_p, c_ushort]

    libmetawear.mbl_mw_dataprocessor_packer_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_packer_create.argtypes = [c_void_p, c_ubyte, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_passthrough_set_count.restype = c_int
    libmetawear.mbl_mw_dataprocessor_passthrough_set_count.argtypes = [c_void_p, c_ushort]

    libmetawear.mbl_mw_dataprocessor_pulse_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_pulse_create.argtypes = [c_void_p, c_int, c_float, c_ushort, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_rms_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_rms_create.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_dataprocessor_rss_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_rss_create.argtypes = [c_void_p, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_debug_reset_after_gc.restype = None
    libmetawear.mbl_mw_debug_reset_after_gc.argtypes = [c_void_p]

    libmetawear.mbl_mw_metawearboard_set_time_for_response.restype = None
    libmetawear.mbl_mw_metawearboard_set_time_for_response.argtypes = [c_void_p, c_ushort]

    libmetawear.mbl_mw_dataprocessor_threshold_modify_boundary.restype = c_int
    libmetawear.mbl_mw_dataprocessor_threshold_modify_boundary.argtypes = [c_void_p, c_float, c_float]

    libmetawear.mbl_mw_acc_bosch_set_range.restype = None
    libmetawear.mbl_mw_acc_bosch_set_range.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_dataprocessor_time_modify_period.restype = c_int
    libmetawear.mbl_mw_dataprocessor_time_modify_period.argtypes = [c_void_p, c_uint]

    libmetawear.mbl_mw_dataprocessor_fuser_create.restype = c_int
    libmetawear.mbl_mw_dataprocessor_fuser_create.argtypes = [c_void_p, c_void_p, c_uint, c_void_p, FnVoid_VoidP_VoidP]

    libmetawear.mbl_mw_humidity_bme280_set_oversampling.restype = None
    libmetawear.mbl_mw_humidity_bme280_set_oversampling.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_mma8452q_set_odr.restype = None
    libmetawear.mbl_mw_acc_mma8452q_set_odr.argtypes = [c_void_p, c_int]

    libmetawear.mbl_mw_acc_get_high_freq_acceleration_data_signal.restype = c_void_p
    libmetawear.mbl_mw_acc_get_high_freq_acceleration_data_signal.argtypes = [c_void_p]

