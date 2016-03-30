from ctypes import *
import uuid

# Constants identifying the message data types
class DataTypeId:
    UINT32= 0
    FLOAT= 1
    CARTESIAN_FLOAT= 2
    INT32= 3
    BYTE_ARRAY= 4
    BATTERY_STATE= 5
    TCS34725_ADC= 6

# Python wrapper for the MblMwMessage struct
class Data(Structure):
    _fields_= [
        ("epoch", c_long),
        ("value", c_void_p),
        ("type_id", c_int),
        ("length", c_ubyte)
    ]

class GattCharacteristic(Structure):
    _fields_= [
        ("service_uuid_high", c_ulong),
        ("service_uuid_low", c_ulong),
        ("uuid_high", c_ulong),
        ("uuid_low", c_ulong)
    ]

FnByteArray= CFUNCTYPE(None, POINTER(c_ubyte), c_ubyte)
FnVoid= CFUNCTYPE(None)
FnVoidPtr= CFUNCTYPE(None, c_void_p)
FnDataPtr= CFUNCTYPE(None, POINTER(Data))
FnGattCharPtrByteArray= CFUNCTYPE(None, POINTER(GattCharacteristic), POINTER(c_ubyte), c_ubyte)
FnGattCharPtr= CFUNCTYPE(None, POINTER(GattCharacteristic))
FnUintUint= CFUNCTYPE(None, c_uint, c_uint)
FnUbyteUlongByteArray= CFUNCTYPE(None, c_ubyte, c_long, POINTER(c_ubyte), c_ubyte)

# UUIDs for the MetaWear gatt services and characteristics
class Gatt:
    SERVICE_UUID= uuid.UUID('{326A9000-85CB-9195-D9DD-464CFBBAE75A}')
    CHAR_NOTIFICATION_UUID= uuid.UUID('{326A9006-85CB-9195-D9DD-464CFBBAE75A}')

class BtleConnection(Structure):
    _fields_= [
        ("write_gatt_char", FnGattCharPtrByteArray),
        ("read_gatt_char", FnGattCharPtr)
    ]

# Constants indicating status codes
class Status:
    OK= 0
    WARNING_UNEXPECTED_SENSOR_DATA= 1
    WARNING_INVALID_PROCESSOR_TYPE= 2
    ERROR_UNSUPPORTED_PROCESSOR = 4
    WARNING_INVALID_RESPONSE = 8

class LogDownloadHandler(Structure):
    _fields_= [
        ("received_progress_update", FnUintUint),
        ("received_unknown_entry", FnUbyteUlongByteArray)
    ]

# Python wrapper for the MblMwDataCartesianFloat struct
class CartesianFloat(Structure):
    _fields_= [
        ("x", c_float),
        ("y", c_float),
        ("z", c_float)
    ]

    # taken from https://www.python.org/dev/peps/pep-0485/#proposed-implementation
    def is_close(fst, snd):
        return abs(fst - snd) <= max( 0.001 * max(abs(fst), abs(snd)), 0.001 )

    def __eq__(self, other):
        return CartesianFloat.is_close(self.x, other.x) and CartesianFloat.is_close(self.y, other.y) and CartesianFloat.is_close(self.z, other.z)

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
