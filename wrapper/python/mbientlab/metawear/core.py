from ctypes import *
import uuid

# Constants identifying the message data types
class DataTypeId:
    UINT32= 0
    FLOAT= 1
    CARTESIAN_FLOAT= 2
    INT32= 3

# Python wrapper for the MblMwMessage struct
class Data(Structure):
    _fields_= [
        ("value", c_void_p),
        ("type_id", c_int)
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
