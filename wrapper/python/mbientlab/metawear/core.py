from ctypes import *
import uuid

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
    EVENT = 0xa
    LOGGING = 0xb
    TIMER = 0xc
    I2C = 0xd
    MACRO = 0xf
    GSR = 0x10
    SETTINGS = 0x11
    BAROMETER = 0x12
    GYRO = 0x13
    AMBIENT_LIGHT = 0x14
    MAGNETOMETER = 0x15
    HUMIDITY = 0x16
    COLOR_DETECTOR = 0x17
    PROXIMITY = 0x18
    SENSOR_FUSION = 0x19
    DEBUG = 0xfe

# Constants identifying the message data types
class DataTypeId:
    UINT32= 0
    FLOAT= 1
    CARTESIAN_FLOAT= 2
    INT32= 3
    BYTE_ARRAY= 4
    BATTERY_STATE= 5
    TCS34725_ADC= 6
    EULER_ANGLES= 7
    QUATERNION= 8
    CORRECTED_CARTESIAN_FLOAT= 9

# Python wrapper for the MblMwMessage struct
class Data(Structure):
    _fields_= [
        ("epoch", c_longlong),
        ("value", c_void_p),
        ("type_id", c_int),
        ("length", c_ubyte)
    ]

class GattCharacteristic(Structure):
    _fields_= [
        ("service_uuid_high", c_ulonglong),
        ("service_uuid_low", c_ulonglong),
        ("uuid_high", c_ulonglong),
        ("uuid_low", c_ulonglong)
    ]

Fn_ByteArray= CFUNCTYPE(None, POINTER(c_ubyte), c_ubyte)
Fn_VoidPtr= CFUNCTYPE(None, c_void_p)
Fn_VoidPtr_Int= CFUNCTYPE(None, c_void_p, c_int)
Fn_DataPtr= CFUNCTYPE(None, POINTER(Data))
Fn_VoidPtr_GattCharPtr_ByteArray= CFUNCTYPE(None, c_void_p, POINTER(GattCharacteristic), POINTER(c_ubyte), c_ubyte)
Fn_VoidPtr_GattCharPtr= CFUNCTYPE(None, c_void_p, POINTER(GattCharacteristic))
Fn_Uint_Uint= CFUNCTYPE(None, c_uint, c_uint)
Fn_Ubyte_LongLong_ByteArray= CFUNCTYPE(None, c_ubyte, c_longlong, POINTER(c_ubyte), c_ubyte)

# UUIDs for the MetaWear gatt services and characteristics
class Gatt:
    SERVICE_UUID= uuid.UUID('{326A9000-85CB-9195-D9DD-464CFBBAE75A}')
    CHAR_NOTIFICATION_UUID= uuid.UUID('{326A9006-85CB-9195-D9DD-464CFBBAE75A}')

class BtleConnection(Structure):
    _fields_= [
        ("write_gatt_char", Fn_VoidPtr_GattCharPtr_ByteArray),
        ("read_gatt_char", Fn_VoidPtr_GattCharPtr)
    ]

# Constants indicating status codes
class Status:
    OK= 0
    WARNING_UNEXPECTED_SENSOR_DATA= 1
    WARNING_INVALID_PROCESSOR_TYPE= 2
    ERROR_UNSUPPORTED_PROCESSOR = 4
    WARNING_INVALID_RESPONSE = 8
    ERROR_TIMEOUT = 16

class LogDownloadHandler(Structure):
    _fields_= [
        ("received_progress_update", Fn_Uint_Uint),
        ("received_unknown_entry", Fn_Ubyte_LongLong_ByteArray),
        ("received_unhandled_entry", Fn_DataPtr)
    ]

class Settings:
    BATTERY_VOLTAGE_INDEX = 0
    BATTERY_CHARGE_INDEX = 1

# taken from https://www.python.org/dev/peps/pep-0485/#proposed-implementation
def is_close(fst, snd):
    return abs(fst - snd) <= max( 0.001 * max(abs(fst), abs(snd)), 0.001 )

# Python wrapper for the MblMwDataCartesianFloat struct
class CartesianFloat(Structure):
    _fields_= [
        ("x", c_float),
        ("y", c_float),
        ("z", c_float)
    ]

    def __eq__(self, other):
        return is_close(self.x, other.x) and is_close(self.y, other.y) and is_close(self.z, other.z)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "(%.3f, %.3f, %.3f)" % (self.x, self.y, self.z)

    def __deepcopy__(self, memo):
        return CartesianFloat(x= self.x, y= self.y, z= self.z)

class BatteryState(Structure):
    _fields_= [
        ("voltage", c_ushort),
        ("charge", c_ubyte)
    ]

    def __eq__(self, other):
        return (self.voltage == other.voltage) and (self.charge == other.charge)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "{voltage: %d, charge: %d}" % (self.voltage, self.charge)

    def __deepcopy__(self, memo):
        return BatteryState(voltage= self.voltage, charge= self.charge)

class Tcs34725ColorAdc(Structure):
    _fields_= [
        ("clear", c_ushort),
        ("red", c_ushort),
        ("green", c_ushort),
        ("blue", c_ushort)
    ]

    def __eq__(self, other):
        return (self.clear == other.clear) and (self.red == other.red) and (self.green == other.green) and (self.blue == other.blue)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "{clear: %d, red: %d, green: %d, blue: %d}" % (self.clear, self.red, self.green, self.blue)

    def __deepcopy__(self, memo):
        return Tcs34725ColorAdc(clear= self.clear, red= self.red, green= self.green, blue= self.blue)

class EulerAngle(Structure):
    _fields_= [
        ("heading", c_float),
        ("pitch", c_float),
        ("roll", c_float),
        ("yaw", c_float)
    ]

    def __eq__(self, other):
        return is_close(self.heading, other.heading) and is_close(self.pitch, other.pitch) and is_close(self.roll, other.roll) and is_close(self.yaw, other.yaw)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "(%.3f, %.3f, %.3f, %.3f)" % (self.heading, self.pitch, self.roll, self.yaw)

    def __deepcopy__(self, memo):
        return EulerAngle(heading = self.heading, pitch = self.pitch, roll = self.roll, yaw = self.yaw)

class Quaternion(Structure):
    _fields_= [
        ("w", c_float),
        ("x", c_float),
        ("y", c_float),
        ("z", c_float)
    ]

    def __eq__(self, other):
        return is_close(self.w, other.w) and is_close(self.x, other.x) and is_close(self.y, other.y) and is_close(self.z, other.z)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "(%.3f, %.3f, %.3f, %.3f)" % (self.w, self.x, self.y, self.z)

    def __deepcopy__(self, memo):
        return Quaternion(w = self.w, x = self.x, y = self.y, z = self.z)

class CorrectedCartesianFloat(Structure):
    _fields_= [
        ("x", c_float),
        ("y", c_float),
        ("z", c_float),
        ("accuracy", c_ubyte)
    ]

    def __eq__(self, other):
        return is_close(self.x, other.x) and is_close(self.y, other.y) and is_close(self.z, other.z) and self.accuracy == other.accuracy

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "(%.3f, %.3f, %.3f, %d)" % (self.x, self.y, self.z, self.accuracy)

    def __deepcopy__(self, memo):
        return CorrectedCartesianFloat(x = self.x, y = self.y, z = self.z, accuracy = self.accuracy)
