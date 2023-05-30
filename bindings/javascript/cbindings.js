// Warning!!!
// This JavaScript module is generated from the MetaWear C++ API header files
// Do not edit
var ref = require('ref-napi');
var ffi = require('ffi-napi');
var Struct = require('ref-struct-di')(ref);
var ArrayType = require('ref-array-di')(ref);
var Enum = require('enum');
var LIBMETAWEAR_PATH = require('./libmetawear-path');

// TODO: These exist because arrays are not handled perfectly yet
var ArrayUByte_6 = ArrayType(ref.types.uint8, 6);
var ArrayFloat = ArrayType(ref.types.float);
var ArrayUByte_10 = ArrayType(ref.types.uint8, 10);
var ArrayUByte_16 = ArrayType(ref.types.uint8, 16);

var BleAdType = new Enum({
  'CONNECTED_UNDIRECTED': 0,
  'CONNECTED_DIRECTED': 1
}, ref.types.int);
BleAdType.alignment = 4;

var DataTypeId = new Enum({
  'UINT32': 0,
  'FLOAT': 1,
  'CARTESIAN_FLOAT': 2,
  'INT32': 3,
  'BYTE_ARRAY': 4,
  'BATTERY_STATE': 5,
  'TCS34725_ADC': 6,
  'EULER_ANGLE': 7,
  'QUATERNION': 8,
  'CORRECTED_CARTESIAN_FLOAT': 9,
  'OVERFLOW_STATE': 10,
  'SENSOR_ORIENTATION': 11,
  'STRING': 12,
  'LOGGING_TIME': 13,
  'BTLE_ADDRESS': 14,
  'BOSCH_ANY_MOTION': 15,
  'CALIBRATION_STATE': 16,
  'DATA_ARRAY': 17,
  'BOSCH_TAP': 18,
  'BOSCH_GESTURE': 19
}, ref.types.int);
DataTypeId.alignment = 4;

var MagBmm150Preset = new Enum({
  'LOW_POWER': 0,
  'REGULAR': 1,
  'ENHANCED_REGULAR': 2,
  'HIGH_ACCURACY': 3
}, ref.types.int);
MagBmm150Preset.alignment = 4;

var SpiMode = new Enum({
  '_0': 0,
  '_1': 1,
  '_2': 2,
  '_3': 3
}, ref.types.int);
SpiMode.alignment = 4;

var BaroBmp280StandbyTime = new Enum({
  '_0_5ms': 0,
  '_62_5ms': 1,
  '_125ms': 2,
  '_250ms': 3,
  '_500ms': 4,
  '_1000ms': 5,
  '_2000ms': 6,
  '_4000ms': 7
}, ref.types.int);
BaroBmp280StandbyTime.alignment = 4;

var AlsLtr329MeasurementRate = new Enum({
  '_50ms': 0,
  '_100ms': 1,
  '_200ms': 2,
  '_500ms': 3,
  '_1000ms': 4,
  '_2000ms': 5
}, ref.types.int);
AlsLtr329MeasurementRate.alignment = 4;

var AccBmi160Odr = new Enum({
  '_0_78125Hz': 0,
  '_1_5625Hz': 1,
  '_3_125Hz': 2,
  '_6_25Hz': 3,
  '_12_5Hz': 4,
  '_25Hz': 5,
  '_50Hz': 6,
  '_100Hz': 7,
  '_200Hz': 8,
  '_400Hz': 9,
  '_800Hz': 10,
  '_1600Hz': 11
}, ref.types.int);
AccBmi160Odr.alignment = 4;

var GattCharWriteType = new Enum({
  'WITH_RESPONSE': 0,
  'WITHOUT_RESPONSE': 1
}, ref.types.int);
GattCharWriteType.alignment = 4;

var Module = new Enum({
  'SWITCH': 1,
  'LED': 2,
  'ACCELEROMETER': 3,
  'TEMPERATURE': 4,
  'GPIO': 5,
  'NEO_PIXEL': 6,
  'IBEACON': 7,
  'HAPTIC': 8,
  'DATA_PROCESSOR': 9,
  'EVENT': 10,
  'LOGGING': 11,
  'TIMER': 12,
  'I2C': 13,
  'MACRO': 15,
  'CONDUCTANCE': 16,
  'SETTINGS': 17,
  'BAROMETER': 18,
  'GYRO': 19,
  'AMBIENT_LIGHT': 20,
  'MAGNETOMETER': 21,
  'HUMIDITY': 22,
  'COLOR_DETECTOR': 23,
  'PROXIMITY': 24,
  'SENSOR_FUSION': 25,
  'DEBUG': 254
}, ref.types.int);
Module.alignment = 4;

var TimeMode = new Enum({
  'ABSOLUTE': 0,
  'DIFFERENTIAL': 1
}, ref.types.int);
TimeMode.alignment = 4;

var Model = new Enum({
  'NA': -1,
  'METAWEAR_R': 0,
  'METAWEAR_RG': 1,
  'METAWEAR_RPRO': 2,
  'METAWEAR_C': 3,
  'METAWEAR_CPRO': 4,
  'METAENV': 5,
  'METADETECT': 6,
  'METAHEALTH': 7,
  'METATRACKER': 8,
  'METAMOTION_R': 9,
  'METAMOTION_RL': 10,
  'METAMOTION_C': 11,
  'METAMOTION_S': 12
}, ref.types.int);
Model.alignment = 4;

var SensorOrientation = new Enum({
  'FACE_UP_PORTRAIT_UPRIGHT': 0,
  'FACE_UP_PORTRAIT_UPSIDE_DOWN': 1,
  'FACE_UP_LANDSCAPE_LEFT': 2,
  'FACE_UP_LANDSCAPE_RIGHT': 3,
  'FACE_DOWN_PORTRAIT_UPRIGHT': 4,
  'FACE_DOWN_PORTRAIT_UPSIDE_DOWN': 5,
  'FACE_DOWN_LANDSCAPE_LEFT': 6,
  'FACE_DOWN_LANDSCAPE_RIGHT': 7
}, ref.types.int);
SensorOrientation.alignment = 4;

var SensorFusionGyroRange = new Enum({
  '_2000DPS': 0,
  '_1000DPS': 1,
  '_500DPS': 2,
  '_250DPS': 3
}, ref.types.int);
SensorFusionGyroRange.alignment = 4;

var AccBoschGesture = new Enum({
  'UNKNOWN': 0,
  'PUSH_ARM_DOWN': 1,
  'PIVOT_UP': 2,
  'SHAKE': 3,
  'ARM_FLICK_IN': 4,
  'ARM_FLICK_OUT': 5
}, ref.types.int);
AccBoschGesture.alignment = 4;

var AccBoschRange = new Enum({
  '_2G': 0,
  '_4G': 1,
  '_8G': 2,
  '_16G': 3
}, ref.types.int);
AccBoschRange.alignment = 4;

var PassthroughMode = new Enum({
  'ALL': 0,
  'CONDITIONAL': 1,
  'COUNT': 2
}, ref.types.int);
PassthroughMode.alignment = 4;

var BaroBoschIirFilter = new Enum({
  'OFF': 0,
  'AVG_2': 1,
  'AVG_4': 2,
  'AVG_8': 3,
  'AVG_16': 4
}, ref.types.int);
BaroBoschIirFilter.alignment = 4;

var DeltaMode = new Enum({
  'ABSOLUTE': 0,
  'DIFFERENTIAL': 1,
  'BINARY': 2
}, ref.types.int);
DeltaMode.alignment = 4;

var ThresholdMode = new Enum({
  'ABSOLUTE': 0,
  'BINARY': 1
}, ref.types.int);
ThresholdMode.alignment = 4;

var AccBoschAxisXyzSign = new Enum({
  '_000': 0,
  '_100': 1,
  '_110': 2,
  '_101': 3,
  '_010': 4,
  '_011': 5,
  '_001': 6,
  '_111': 7
}, ref.types.int);
AccBoschAxisXyzSign.alignment = 4;

var GpioAnalogReadMode = new Enum({
  'ABS_REF': 0,
  'ADC': 1
}, ref.types.int);
GpioAnalogReadMode.alignment = 4;

var ComparatorOperation = new Enum({
  'EQ': 0,
  'NEQ': 1,
  'LT': 2,
  'LTE': 3,
  'GT': 4,
  'GTE': 5
}, ref.types.int);
ComparatorOperation.alignment = 4;

var PulseOutput = new Enum({
  'WIDTH': 0,
  'AREA': 1,
  'PEAK': 2,
  'ON_DETECTION': 3
}, ref.types.int);
PulseOutput.alignment = 4;

var HumidityBme280Oversampling = new Enum({
  '_1X': 1,
  '_2X': 2,
  '_4X': 3,
  '_8X': 4,
  '_16X': 5
}, ref.types.int);
HumidityBme280Oversampling.alignment = 4;

var TemperatureSource = new Enum({
  'INVALID': -1,
  'NRF_DIE': 0,
  'EXT_THERM': 1,
  'BMP280': 2,
  'PRESET_THERM': 3
}, ref.types.int);
TemperatureSource.alignment = 4;

var AccBoschActivity = new Enum({
  'STILL': 0,
  'WALKING': 1,
  'RUNNING': 2,
  'UNKNOWN': 3
}, ref.types.int);
AccBoschActivity.alignment = 4;

var SensorFusionMode = new Enum({
  'SLEEP': 0,
  'NDOF': 1,
  'IMU_PLUS': 2,
  'COMPASS': 3,
  'M4G': 4
}, ref.types.int);
SensorFusionMode.alignment = 4;

var AccBoschMotion = new Enum({
  'SIGMOTION': 0,
  'NOMOTION': 1,
  'ANYMOTION': 2
}, ref.types.int);
AccBoschMotion.alignment = 4;

var AccBmi270Odr = new Enum({
  '_0_78125Hz': 0,
  '_1_5625Hz': 1,
  '_3_125Hz': 2,
  '_6_25Hz': 3,
  '_12_5Hz': 4,
  '_25Hz': 5,
  '_50Hz': 6,
  '_100Hz': 7,
  '_200Hz': 8,
  '_400Hz': 9,
  '_800Hz': 10,
  '_1600Hz': 11
}, ref.types.int);
AccBmi270Odr.alignment = 4;

var AccBoschDoubleTapWindow = new Enum({
  '_50ms': 0,
  '_100ms': 1,
  '_150ms': 2,
  '_200ms': 3,
  '_250ms': 4,
  '_375ms': 5,
  '_500ms': 6,
  '_700ms': 7
}, ref.types.int);
AccBoschDoubleTapWindow.alignment = 4;

var AccBoschAxisXyzRemap = new Enum({
  'XYZ': 0,
  'YZX': 1,
  'ZXY': 2,
  'XZY': 3,
  'YXZ': 4,
  'ZYX': 5
}, ref.types.int);
AccBoschAxisXyzRemap.alignment = 4;

var AccBmi160StepCounterMode = new Enum({
  'NORMAL': 0,
  'SENSITIVE': 1,
  'ROBUST': 2
}, ref.types.int);
AccBmi160StepCounterMode.alignment = 4;

var SensorFusionData = new Enum({
  'CORRECTED_ACC': 0,
  'CORRECTED_GYRO': 1,
  'CORRECTED_MAG': 2,
  'QUATERNION': 3,
  'EULER_ANGLE': 4,
  'GRAVITY_VECTOR': 5,
  'LINEAR_ACC': 6
}, ref.types.int);
SensorFusionData.alignment = 4;

var WhitelistFilter = new Enum({
  'ALLOW_FROM_ANY': 0,
  'SCAN_REQUESTS': 1,
  'CONNECTION_REQUESTS': 2,
  'SCAN_AND_CONNECTION_REQUESTS': 3
}, ref.types.int);
WhitelistFilter.alignment = 4;

var AccBoschOrientationMode = new Enum({
  'SYMMETRICAL': 0,
  'HIGH_ASYMMETRICAL': 1,
  'LOW_ASYMMETRICAL': 2
}, ref.types.int);
AccBoschOrientationMode.alignment = 4;

var AccBoschTypewrist = new Enum({
  'NONE': 0,
  'WEARK_WAKEUP': 1,
  'GESTURE': 2
}, ref.types.int);
AccBoschTypewrist.alignment = 4;

var GyroBoschOdr = new Enum({
  '_25Hz': 6,
  '_50Hz': 7,
  '_100Hz': 8,
  '_200Hz': 9,
  '_400Hz': 10,
  '_800Hz': 11,
  '_1600Hz': 12,
  '_3200Hz': 13
}, ref.types.int);
GyroBoschOdr.alignment = 4;

var AccBoschTapQuietTime = new Enum({
  '_30ms': 0,
  '_20ms': 1
}, ref.types.int);
AccBoschTapQuietTime.alignment = 4;

var AccBoschTapShockTime = new Enum({
  '_50ms': 0,
  '_75ms': 1
}, ref.types.int);
AccBoschTapShockTime.alignment = 4;

var BaroBoschOversampling = new Enum({
  'SKIP': 0,
  'ULTRA_LOW_POWER': 1,
  'LOW_POWER': 2,
  'STANDARD': 3,
  'HIGH': 4,
  'ULTRA_HIGH': 5
}, ref.types.int);
BaroBoschOversampling.alignment = 4;

var BaroBme280StandbyTime = new Enum({
  '_0_5ms': 0,
  '_62_5ms': 1,
  '_125ms': 2,
  '_250ms': 3,
  '_500ms': 4,
  '_1000ms': 5,
  '_10ms': 6,
  '_20ms': 7
}, ref.types.int);
BaroBme280StandbyTime.alignment = 4;

var ComparatorMode = new Enum({
  'ABSOLUTE': 0,
  'REFERENCE': 1,
  'ZONE': 2,
  'BINARY': 3
}, ref.types.int);
ComparatorMode.alignment = 4;

var LedPreset = new Enum({
  'BLINK': 0,
  'PULSE': 1,
  'SOLID': 2
}, ref.types.int);
LedPreset.alignment = 4;

var MetaWearRProChannel = new Enum({
  'ON_DIE': 0,
  'ON_BOARD_THERMISTOR': 1,
  'EXT_THERMISTOR': 2,
  'BMP280': 3
}, ref.types.int);
MetaWearRProChannel.alignment = 4;

var MetaWearRChannel = new Enum({
  'ON_DIE': 0,
  'EXT_THERMISTOR': 1
}, ref.types.int);
MetaWearRChannel.alignment = 4;

var LedColor = new Enum({
  'GREEN': 0,
  'RED': 1,
  'BLUE': 2
}, ref.types.int);
LedColor.alignment = 4;

var GpioPullMode = new Enum({
  'UP': 0,
  'DOWN': 1,
  'NONE': 2
}, ref.types.int);
GpioPullMode.alignment = 4;

var AlsLtr329IntegrationTime = new Enum({
  '_100ms': 0,
  '_50ms': 1,
  '_200ms': 2,
  '_400ms': 3,
  '_150ms': 4,
  '_250ms': 5,
  '_300ms': 6,
  '_350ms': 7
}, ref.types.int);
AlsLtr329IntegrationTime.alignment = 4;

var GpioPinChangeType = new Enum({
  'RISING': 1,
  'FALLING': 2,
  'ANY': 3
}, ref.types.int);
GpioPinChangeType.alignment = 4;

var SensorFusionAccRange = new Enum({
  '_2G': 0,
  '_4G': 1,
  '_8G': 2,
  '_16G': 3
}, ref.types.int);
SensorFusionAccRange.alignment = 4;

var MathOperation = new Enum({
  'ADD': 1,
  'MULTIPLY': 2,
  'DIVIDE': 3,
  'MODULUS': 4,
  'EXPONENT': 5,
  'SQRT': 6,
  'LSHIFT': 7,
  'RSHIFT': 8,
  'SUBTRACT': 9,
  'ABS_VALUE': 10,
  'CONSTANT': 11
}, ref.types.int);
MathOperation.alignment = 4;

var MagBmm150Odr = new Enum({
  '_10Hz': 0,
  '_2Hz': 1,
  '_6Hz': 2,
  '_8Hz': 3,
  '_15Hz': 4,
  '_20Hz': 5,
  '_25Hz': 6,
  '_30Hz': 7
}, ref.types.int);
MagBmm150Odr.alignment = 4;

var SpiFrequency = new Enum({
  '_125KHz': 0,
  '_250KHz': 1,
  '_500KHz': 2,
  '_1MHz': 3,
  '_2MHz': 4,
  '_4MHz': 5,
  '_8MHz': 6
}, ref.types.int);
SpiFrequency.alignment = 4;

var GyroBoschRange = new Enum({
  '_2000dps': 0,
  '_1000dps': 1,
  '_500dps': 2,
  '_250dps': 3,
  '_125dps': 4
}, ref.types.int);
GyroBoschRange.alignment = 4;

var AlsLtr329Gain = new Enum({
  '_1X': 0,
  '_2X': 1,
  '_4X': 2,
  '_8X': 3,
  '_48X': 4,
  '_96X': 5
}, ref.types.int);
AlsLtr329Gain.alignment = 4;

var FnVoid_VoidP_Int = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.types.int32]);
var FnInt_VoidP_UByteP_UByte = ffi.Function(ref.types.int32, [ref.refType(ref.types.void), ref.refType(ref.types.uint8), ref.types.uint8]);
var MetaWearBoard = ref.types.void;
var FnVoid_MetaWearBoardP = ffi.Function(ref.types.void, [ref.refType(MetaWearBoard)]);
var Data = Struct({
  'epoch': ref.types.int64,
  'extra': ref.refType(ref.types.void),
  'value': ref.refType(ref.types.void),
  'type_id': DataTypeId,
  'length': ref.types.uint8
});

var FnVoid_VoidP_DataP = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(Data)]);
var FnVoid_VoidP_MetaWearBoardP_Int = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(MetaWearBoard), ref.types.int32]);
var CalibrationData = Struct({
  'acc': ArrayUByte_10,
  'gyro': ArrayUByte_10,
  'mag': ArrayUByte_10
});

var FnVoid_VoidP_MetaWearBoardP_CalibrationDataP = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(MetaWearBoard), ref.refType(CalibrationData)]);
var DataLogger = ref.types.void;
var FnVoid_VoidP_DataLoggerP = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(DataLogger)]);
var Event = ref.types.void;
var FnVoid_VoidP_EventP_Int = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(Event), ref.types.int32]);
var Timer = ref.types.void;
var FnVoid_VoidP_TimerP = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(Timer)]);
var AnonymousDataSignal = ref.types.void;
var FnVoid_VoidP_MetaWearBoardP_AnonymousDataSignalP_UInt = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(MetaWearBoard), ref.refType(AnonymousDataSignal), ref.types.uint32]);
var DataProcessor = ref.types.void;
var FnVoid_VoidP_DataProcessorP = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(DataProcessor)]);
var GattChar = Struct({
  'service_uuid_high': ref.types.uint64,
  'service_uuid_low': ref.types.uint64,
  'uuid_high': ref.types.uint64,
  'uuid_low': ref.types.uint64
});

var FnVoid_VoidP_VoidP_GattCharWriteType_GattCharP_UByteP_UByte = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(ref.types.void), GattCharWriteType, ref.refType(GattChar), ref.refType(ref.types.uint8), ref.types.uint8]);
var FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(ref.types.void), ref.refType(GattChar), FnInt_VoidP_UByteP_UByte]);
var FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(ref.types.void), ref.refType(GattChar), FnInt_VoidP_UByteP_UByte, FnVoid_VoidP_Int]);
var FnVoid_VoidP_VoidP_FnVoidVoidPtrInt = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(ref.types.void), FnVoid_VoidP_Int]);
var BtleConnection = Struct({
  'context': ref.refType(ref.types.void),
  'write_gatt_char': FnVoid_VoidP_VoidP_GattCharWriteType_GattCharP_UByteP_UByte,
  'read_gatt_char': FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray,
  'enable_notifications': FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt,
  'on_disconnect': FnVoid_VoidP_VoidP_FnVoidVoidPtrInt
});

var GpioAnalogReadParameters = Struct({
  'pullup_pin': ref.types.uint8,
  'pulldown_pin': ref.types.uint8,
  'virtual_pin': ref.types.uint8,
  'delay_us': ref.types.uint16
});

var BoschTap = Struct({
  'type': ref.types.uint8,
  'sign': ref.types.uint8
});

var BtleAddress = Struct({
  'address_type': ref.types.uint8,
  'address': ArrayUByte_6
});

var DataSignal = ref.types.void;
var BatteryState = Struct({
  'voltage': ref.types.uint16,
  'charge': ref.types.uint8
});

var CorrectedCartesianFloat = Struct({
  'x': ref.types.float,
  'y': ref.types.float,
  'z': ref.types.float,
  'accuracy': ref.types.uint8
});

var EulerAngles = Struct({
  'heading': ref.types.float,
  'pitch': ref.types.float,
  'roll': ref.types.float,
  'yaw': ref.types.float
});

var ModuleInfo = Struct({
  'name': ref.types.CString,
  'extra': ref.refType(ref.types.uint8),
  'extra_len': ref.types.uint8,
  'present': ref.types.uint8,
  'implementation': ref.types.uint8,
  'revision': ref.types.uint8
});

var OverflowState = Struct({
  'length': ref.types.uint16,
  'assert_en': ref.types.uint8
});

var CalibrationState = Struct({
  'accelrometer': ref.types.uint8,
  'gyroscope': ref.types.uint8,
  'magnetometer': ref.types.uint8
});

var DeviceInformation = Struct({
  'manufacturer': ref.types.CString,
  'model_number': ref.types.CString,
  'serial_number': ref.types.CString,
  'firmware_revision': ref.types.CString,
  'hardware_revision': ref.types.CString
});

var LoggingTime = Struct({
  'epoch': ref.types.int64,
  'reset_uid': ref.types.uint8
});

var FnVoid_VoidP_UInt_UInt = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.types.uint32, ref.types.uint32]);
var FnVoid_VoidP_UByte_Long_UByteP_UByte = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.types.uint8, ref.types.int64, ref.refType(ref.types.uint8), ref.types.uint8]);
var LogDownloadHandler = Struct({
  'context': ref.refType(ref.types.void),
  'received_progress_update': FnVoid_VoidP_UInt_UInt,
  'received_unknown_entry': FnVoid_VoidP_UByte_Long_UByteP_UByte,
  'received_unhandled_entry': FnVoid_VoidP_DataP
});

var FnVoid_VoidP_UByte_UByte_UInt_UInt = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.types.uint8, ref.types.uint8, ref.types.uint32, ref.types.uint32]);
var FnVoid_VoidP_MetaWearBoardP_FnBoardPtr = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(MetaWearBoard), FnVoid_MetaWearBoardP]);
var RawLogDownloadHandler = Struct({
  'context': ref.refType(ref.types.void),
  'received_entry': FnVoid_VoidP_UByte_UByte_UInt_UInt,
  'received_progress_update': FnVoid_VoidP_UInt_UInt,
  'logging_page_completed': FnVoid_VoidP_MetaWearBoardP_FnBoardPtr
});

var Quaternion = Struct({
  'w': ref.types.float,
  'x': ref.types.float,
  'y': ref.types.float,
  'z': ref.types.float
});

var CartesianFloat = Struct({
  'x': ref.types.float,
  'y': ref.types.float,
  'z': ref.types.float
});

var FnVoid_VoidP = ffi.Function(ref.types.void, [ref.refType(ref.types.void)]);
var FnVoid_VoidP_charP = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.types.CString]);
var DfuDelegate = Struct({
  'context': ref.refType(ref.types.void),
  'on_dfu_started': FnVoid_VoidP,
  'on_dfu_cancelled': FnVoid_VoidP,
  'on_transfer_percentage': FnVoid_VoidP_Int,
  'on_successful_file_transferred': FnVoid_VoidP,
  'on_error': FnVoid_VoidP_charP
});

var LedPattern = Struct({
  'high_intensity': ref.types.uint8,
  'low_intensity': ref.types.uint8,
  'rise_time_ms': ref.types.uint16,
  'high_time_ms': ref.types.uint16,
  'fall_time_ms': ref.types.uint16,
  'pulse_duration_ms': ref.types.uint16,
  'delay_time_ms': ref.types.uint16,
  'repeat_count': ref.types.uint8
});

var BoschAnyMotion = Struct({
  'sign': ref.types.uint8,
  'x_axis_active': ref.types.uint8,
  'y_axis_active': ref.types.uint8,
  'z_axis_active': ref.types.uint8
});

var BoschGestureType = Struct({
  'type': ref.types.uint8,
  'gesture_code': ref.types.uint8
});

var SpiParameters = Struct({
  'mode': SpiMode,
  'frequency': SpiFrequency,
  'data': ref.refType(ref.types.uint8),
  'data_length': ref.types.uint8,
  'slave_select_pin': ref.types.uint8,
  'clock_pin': ref.types.uint8,
  'mosi_pin': ref.types.uint8,
  'miso_pin': ref.types.uint8,
  'lsb_first': ref.types.uint8,
  'use_nrf_pins': ref.types.uint8
});

var I2cReadParameters = Struct({
  'device_addr': ref.types.uint8,
  'register_addr': ref.types.uint8
});

function Const() {
}
Const.SETTINGS_BATTERY_VOLTAGE_INDEX = 0;
Const.STATUS_WARNING_UNEXPECTED_SENSOR_DATA = 1;
Const.STATUS_WARNING_INVALID_PROCESSOR_TYPE = 2;
Const.STATUS_ERROR_UNSUPPORTED_PROCESSOR = 4;
Const.STATUS_OK = 0;
Const.STATUS_ERROR_SERIALIZATION_FORMAT = 32;
Const.STATUS_ERROR_ENABLE_NOTIFY = 64;
Const.SETTINGS_BATTERY_CHARGE_INDEX = 1;
Const.GYRO_ROTATION_X_AXIS_INDEX = 0;
Const.ADDRESS_TYPE_PUBLIC = 0;
Const.ADDRESS_TYPE_RANDOM_STATIC = 1;
Const.STATUS_WARNING_INVALID_RESPONSE = 8;
Const.ADDRESS_TYPE_PRIVATE_RESOLVABLE = 2;
Const.ADDRESS_TYPE_PRIVATE_NON_RESOLVABLE = 3;
Const.ACC_ACCEL_Z_AXIS_INDEX = 2;
Const.ACC_ACCEL_X_AXIS_INDEX = 0;
Const.SETTINGS_POWER_STATUS_UNSUPPORTED = -1;
Const.SENSOR_FUSION_CALIBRATION_ACCURACY_LOW = 1;
Const.SENSOR_FUSION_CALIBRATION_ACCURACY_MEDIUM = 2;
Const.MAG_BFIELD_X_AXIS_INDEX = 0;
Const.ACC_ACCEL_Y_AXIS_INDEX = 1;
Const.STATUS_ERROR_TIMEOUT = 16;
Const.SETTINGS_CHARGE_STATUS_UNSUPPORTED = -1;
Const.LED_REPEAT_INDEFINITELY = 255;
Const.SENSOR_FUSION_CALIBRATION_ACCURACY_UNRELIABLE = 0;
Const.SENSOR_FUSION_CALIBRATION_ACCURACY_HIGH = 3;
Const.GPIO_UNUSED_PIN = 255;
Const.MAG_BFIELD_Y_AXIS_INDEX = 1;
Const.MAG_BFIELD_Z_AXIS_INDEX = 2;
Const.GYRO_ROTATION_Y_AXIS_INDEX = 1;
Const.GYRO_ROTATION_Z_AXIS_INDEX = 2;

// TODO: This line exisits because the generator doesn't understand array of pointers
var ArrayAnonymousDataSignalP = ArrayType(ref.refType(AnonymousDataSignal));
FnVoid_VoidP_MetaWearBoardP_AnonymousDataSignalP_UInt = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(MetaWearBoard), ArrayAnonymousDataSignalP, ref.types.uint32]);
var ArrayDataSignalP = ArrayType(ref.refType(DataSignal));
var Lib = ffi.Library(LIBMETAWEAR_PATH, {
/**
 * Restarts the board in bootloader mode.
 * @param board     Calling object
 */
  'mbl_mw_debug_jump_to_bootloader': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Create a comparator processor where signed/unsigned is inferred.  
 * Only allows data through that satisfies a comparison operation.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute (=, !=, <, >)
 * @param reference             Reference value to compare the input to
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_comparator_create': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ref.types.float, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Sets connection parameters.
 * @param board                 Board to modify
 * @param min_conn_interval     Connection interval lower bound, min 7.5ms
 * @param max_conn_interval     Connection interval upper bound, max 4000ms
 * @param latency               Number of connection intervals to skip, betwen [0, 1000]
 * @param timeout               Max time between data exchanges until the connection is considered to be lost, between [10, 32000]ms
 */
  'mbl_mw_settings_set_connection_parameters': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float, ref.types.float, ref.types.uint16, ref.types.uint16]],

/**
 * Retrieves the id value identifying the processor.
 * @param processor         Processor to lookup
 * @return Numerical id of the processor
 */
  'mbl_mw_dataprocessor_get_id': [ref.types.uint8, [ref.refType(DataProcessor)]],

/**
 * Writes the acceleration settings to the sensor
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bosch_write_acceleration_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the maximum angle change for wrist wakeup
 * Cosine of minimum expected attitude change of the device within 1 second time window when moving from non-focus to focus position. 
 * The parameter is scaled by 2048 i.e. 2048 * cos(angle). Range is 1448 to 1856. Default value is 1774.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum angle
 */
  'mbl_mw_acc_bmi270_wrist_wakeup_angle_nonfocus': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Creates a fake button event with the data value.
 * Requires that the switch signal be streaming or logging to add a spoofed event in the logger.
 * @param board     Calling object
 * @param value     Value to spoof, 1 byte
 */
  'mbl_mw_debug_spoof_button_event': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Disables the BMI270 step counter
 * @param board     Board to modify
 */
  'mbl_mw_acc_bmi270_disable_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Get the device boot time for a given reset_uid.  This reference time
 * is automatically calulated at connection time.
 * @param board                 Board to use
 * @param reset_uid             Reset id
 * @return Number of milliseconds since epoch that the given reset_uid occured
 */
  'mbl_mw_logging_get_reference_time': [ref.types.int64, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Issues a soft reset.
 * @param board     Calling object
 */
  'mbl_mw_debug_reset': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the iBeacon advertising major number.
 * @param board     Pointer to the board to send the command to
 * @param major     DataSignal output to use as the new major number
 */
  'mbl_mw_ibeacon_set_major_signal': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(DataSignal)]],

/**
 * Frees allocated memory
 * @param ptr   Pointer to the memory to free
 */
  'mbl_mw_memory_free': [ref.types.void, [ref.refType(ref.types.void)]],

/**
 * Downsampling for the BMI270 acc
 * See BMI270 datasheet for more details
 * @param board         Pointer to the board to send the command to
 * @param gyro_downs    Downsampling for Gyroscope (2**downs_gyro)
 * @param gyro_data     Selects filtered or unfiltered Gyroscope data for fifo
 * @param acc_downs     Downsampling for Accelerometer (2**downs_accel)
 * @param acc_data      Selects filtered or unfiltered Accelerometer data for fifo
 */
  'mbl_mw_acc_bmi270_fifo_downs': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint8, ref.types.uint8]],

/**
 * Sets the key register value.  This is a simple 4 byte scratch register.
 * @param board     Board to receive data from
 */
  'mbl_mw_debug_set_key_register': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint32]],

/**
 * Sends a raw command directly to the MetaWear.  Useful for testing.
 * @param board     Calling object
 * @param value     Value to send: [Module ID, Register ID, Optional Index, Data...]
 * @param lenght    Size of the value array
 */
  'mbl_mw_debug_send_command': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint8]],

/**
 * Retrieves the data signal representing battery state.
 * @param board         Calling object
 * @return Pointer to the battery state signal
 */
  'mbl_mw_settings_get_battery_state_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Retrieves supported characteristics from the Device Information service.  
 * The memory allocated by the function must be freed by calling mbl_mw_memory_free.
 * @return Struct holding the characteristics
 */
  'mbl_mw_metawearboard_get_device_information': [ref.refType(DeviceInformation), [ref.refType(MetaWearBoard)]],

/**
 * Writes the orientation detection settings to the board
 * Applies MODE and HYSTERESIS from set_orientation_hysteresis() and set_orientation_mode()
 * Not supported by the BMI270.
 * @param board     Calling object
 */
  'mbl_mw_acc_bosch_write_orientation_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Enables/disables stack overflow assertion.  Function will do nothing if feature is unsupported.
 * @param board     Calling object
 * @param enable    0 to disable, non-zero value to enable
 */
  'mbl_mw_debug_set_stack_overflow_assertion': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Creates an instance of the MblMwMetaWearBoard struct
 * @param connection    Connection struct the new MblMwMetaWearBoard variable will use for btle communication
 * @return Pointer to the newly created struct
 */
  'mbl_mw_metawearboard_create': [ref.refType(MetaWearBoard), [ref.refType(BtleConnection)]],

/**
 * Sets the advertisement name.
 * Can be used to rename the device.
 * @param board             Board to modify
 * @param device_name       Byte array containing the device name, max 8 ASCII characters
 * @param len               Length of the array
 */
  'mbl_mw_settings_set_device_name': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint8]],

/**
 * Remaps the BMI270 axis
 * If the coordinate system of the MetaWear differs from the sensor coordinate system described in the BMI270 datasheet,
 * the sensor axis must be remapped to use the orientation dependent features properly.
 * Axis remapping register allows the host to freely map individual axis to the coordinate system of the used platform. 
 * Individual axis can be mapped to any other defined axis. The sign value of the axis can be also configured. 
 * @param board     Pointer to the board to send the command to
 * @param map       X,Y,Z axis remap
 * @param sign      X,Y,Z axis flip/sign
 */
  'mbl_mw_acc_bmi270_axis_remap': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschAxisXyzRemap, AccBoschAxisXyzSign]],

/**
 * Retrieves the data signal representing the device GAP (MAC) address.
 * @param board         Calling object
 * @return Pointer to the mac signal
 */
  'mbl_mw_settings_get_mac_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Reads the internal queues' current usage statistics; data is returned as a byte array.  
 * If feature is unspported, nullptr will be passed to the `handler` parameter.
 * @param board     Calling object
 * @param context   Pointer to additional data for the callback function
 * @param handler   Callback function for handling the received data
 */
  'mbl_mw_debug_read_schedule_queue_usage': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_DataP]],

/**
 * Manually configure the sensor, only for advanced users.  
 * It is recommended that users use one of the preset configurations.
 * @param board         Calling object
 * @param xy_reps       Repetitions on the x/y-axis
 * @param z_reps        Repetitions on the z-axis
 * @param odr           Sensor data rate
 */
  'mbl_mw_mag_bmm150_configure': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16, ref.types.uint16, MagBmm150Odr]],

/**
 * Retrieves the data signal representing acceleration data from a Bosch accelerometer
 * This signal is timestamp,x,y,z acc data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 acceleration data signal
 * MblMwCartesianFloat is return signal data type
 */
  'mbl_mw_acc_bosch_get_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Create a sample delay processor.  
 * Holds data until a certain amount (bin_size) has been collected.
 * Can be used to delay the input into another processor.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param bin_size              Number of samples to hold before letting data through
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_sample_create': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Variant of acceleration data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z acc data (it packs three acc data points in one timestamp)
 * @return Pointer to the data signal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
  'mbl_mw_acc_bosch_get_packed_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * @deprecated In v0.14.0, use <code>mbl_mw_settings_set_ad_parameters</code> instead
 */
  'mbl_mw_settings_set_ad_interval': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16, ref.types.uint8]],

/**
 * Sets scan response.
 * @param board         Board to modify
 * @param response      Scan response as a byte array
 * @param len           Length of the array
 */
  'mbl_mw_settings_set_scan_response': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint8]],

/**
 * Write the module configuration to the board.
 * Applies the MODE and RANGE values set in set_*(). Must be preceeded by set_*() calls.
 * @param board         Calling object
 */
  'mbl_mw_sensor_fusion_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing B field strength in uT (micro Teslas)
 * This signal is timestamp,x,y,z mag data
 * @param board         Calling object
 * @return Pointer to the data signal
 * MblMwCartesianFloat is return signal data type
 */
  'mbl_mw_mag_bmm150_get_b_field_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Subscribes to responses from the data logger.
 * @param logger                Logger to subscribe to
 * @param context               Pointer to additional data for the callback function
 * @param received_data         Callback function to handle data received from the logger
 */
  'mbl_mw_logger_subscribe': [ref.types.void, [ref.refType(DataLogger), ref.refType(ref.types.void), FnVoid_VoidP_DataP]],

/**
 * Sets the iBeacon advertising minor number.
 * @param board     Pointer to the board to send the command to
 * @param minor     DataSignal output to use as the new minor number
 */
  'mbl_mw_ibeacon_set_minor_signal': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(DataSignal)]],

/**
 * Adds MAC Addresses for Whitelist filtering.
 * @param board         Board to modify
 * @param index         Whitelist MAC address in range [1, 8], must start at 1 and go in increasing order
 * @param address       Address to add
 */
  'mbl_mw_settings_get_whitelist_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Pulls the current accelerometer output data rate and data range from the sensor.
 * Reads the ODR and RANGE values set in the sensor.
 * This is a debug function, the data is return in the context ptr as AccBmi160Config->acc/AccBmi270Config->acc
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
  'mbl_mw_acc_read_config': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Retrieves the data signal representing the power status.
 * @param board         Calling object
 * @return Pointer to the power status signal, nullptr if unsupported  
 */
  'mbl_mw_settings_get_power_status_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the iBeacon advertising receiving power.
 * @param board         Pointer to the board to send the command to
 * @param rx_power      New advertising receiving power
 */
  'mbl_mw_ibeacon_set_rx_power': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.int8]],

/**
 * Retrieves the id value identifying the timer.
 * @param timer             Timer to lookup
 * @return Numerical id of the timer
 */
  'mbl_mw_timer_get_id': [ref.types.uint8, [ref.refType(Timer)]],

/**
 * Create a fuser processor which fuses signals or processors together.
 * Combine data from multiple data sources into 1 data packet. 
 * Popular for combining gyro and acc data into 1 packet.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param ops                   Array of data signals to combine into 1 message
 * @param n_ops                 Number of items in the array
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_fuser_create': [ref.types.int32, [ref.refType(DataSignal), ArrayDataSignalP, ref.types.uint32, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Retrieves an event pointer representing a disconnect event.
 * @param board         Board the event is fired on
 * @return Pointer to the disconnect event
 */
  'mbl_mw_settings_get_disconnect_event': [ref.refType(Event), [ref.refType(MetaWearBoard)]],

/**
 * Places the board in a powered down state after the next reset.  When in power save mode, press the switch 
 * or plug in the USB charger to wake the board up.
 * @param board     Calling object
 */
  'mbl_mw_debug_enable_power_save': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Adds a system timer (timestamp) to the input signal. 
 * Not typically used.
 * Adds additional information to the BTLE packet in the form of a counter
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_accounter_create': [ref.types.int32, [ref.refType(DataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Removes a data processor and its consumers from the board.
 * @param processor         Processor to remove
 */
  'mbl_mw_dataprocessor_remove': [ref.types.void, [ref.refType(DataProcessor)]],

/**
 * Check if the data signal can be explicitly read.
 * @param signal    Data signal to check
 * @return Zero if not readable, non-zero if it is
 */
  'mbl_mw_datasignal_is_readable': [ref.types.int32, [ref.refType(DataSignal)]],

/**
 * Drives a buzzer - Pulls down the HCD pin.
 * The MetaWear has a driver for motor or buzzers that are 3C compatible (check the datasheet)
 * @param board             Pointer to the board to send the command to
 * @param pulse_width_ms    How long to run the buzzer, in milliseconds
 */
  'mbl_mw_haptic_start_buzzer': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Starts data logging.
 * @param board                 Board to log data on
 * @param overwrite             Non-zero if older entries will be overwritten
 */
  'mbl_mw_logging_start': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Sets the rotation range
 * The range is in units of degrees per second (dps) for Bosch sensors
 * See MblMwGyroBoschRange for allowed values.
 * @param board     Pointer to the board to modify
 * @param range     New rotation range
 */
  'mbl_mw_gyro_bmi160_set_range': [ref.types.void, [ref.refType(MetaWearBoard), GyroBoschRange]],

/**
 * Modifies the magnitude that allows data through.
 * @param delta             Delta processor to modify
 * @param magnitude         Min distance from the reference value to allow the input to pass
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-delta processor was passed in
 */
  'mbl_mw_dataprocessor_delta_modify_magnitude': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float]],

/**
 * Instructs the board to terminate the connection.
 * @param board     Calling object
 */
  'mbl_mw_debug_disconnect': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the acceleration range
 * The range is in units of Gs between 2 and 16gs for Bosch sensors
 * See MblMwAccBoschRange for allowed values.
 * @param board     Pointer to the board to modify
 * @param range     Acceleration range to assign
 */
  'mbl_mw_acc_bosch_set_range': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschRange]],

/**
 * Clear all data enable bits.
 * Turns off the Kalman filter (sensor fusion).
 * @param board         Calling object
 */
  'mbl_mw_sensor_fusion_clear_enabled_mask': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieve the number of available channels
 * @return Number of channel IDs
 */
  'mbl_mw_multi_chnl_temp_get_num_channels': [ref.types.uint8, [ref.refType(MetaWearBoard)]],

/**
 * Reads the current power status if available.  The callback function will be called with:  
 * 1    - power source is attached  
 * 0    - no power source atached  
 * -1   - feature not supported  
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param handler       Callback function that is executed when the task is finished
 */
  'mbl_mw_settings_read_current_power_status': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Sets the minimum angle change for wrist wakeup
 * Cosine of minimum expected attitude change of the device within 1 second time window when moving within focus position. 
 * The parameter is scaled by 2048 i.e. 2048 * cos(angle). Range is 1024 to 1774. Default is 1448.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Minimum angle
 */
  'mbl_mw_acc_bmi270_wrist_wakeup_angle_focus': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Retrieves the data signal representing BMI270 rotation data
 * This signal is timestamp,x,y,z gyro data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 rotation data signal
 * MblMwCartesianFloat is return signal data type
 */
  'mbl_mw_gyro_bmi270_get_rotation_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the maximum time for the gesture recognition
 * Maximum time within which gesture movement has to be completed. 
 * Range is 150 to 250 samples at 50Hz (i.e. 3 to 5 seconds). Defualt value is 200 (i.e. 4 seconds).
 * @param board     Pointer to the board to send the command to 
 * @param duration      
 */
  'mbl_mw_acc_bmi270_wrist_gesture_duration': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Disables the BMI270 activity detector
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bmi270_disable_activity_detection': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Writes data via the i2c bus
 * @param board             Board the i2c bus resides on
 * @param device_addr       Device to write to
 * @param register_addr     Address of the register to write
 * @param value             Payload, as a byte array
 * @param length            Number of bytes
 */
  'mbl_mw_i2c_write': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.refType(ref.types.uint8), ref.types.uint8]],

/**
 * Generates a string identifying the data chain the anonymous data signal is receiving data from.
 * @return String identifying the data chain
 */
  'mbl_mw_anonymous_datasignal_get_identifier': [ref.types.CString, [ref.refType(AnonymousDataSignal)]],

/**
 * Determines the board model of the currently connected device.  
 * Only call this function after the board has been initialized.
 * @return Friendly name representing the board model 
 */
  'mbl_mw_metawearboard_get_model_name': [ref.types.CString, [ref.refType(MetaWearBoard)]],

/**
 * Disables rotation sampling
 * The board will stop gathering data from the gyroscope
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi270_disable_rotation_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Stops data logging.
 * @param board                 Board to stop logging
 */
  'mbl_mw_logging_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the output data rate for the BMI160 accelerometer
 * The ODR sets the output data frequency in Hz.
 * See MblMwAccBmi160Odr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
  'mbl_mw_acc_bmi270_set_odr': [ref.types.void, [ref.refType(MetaWearBoard), AccBmi270Odr]],

/**
 * Turns on the 3V regulator.
 * Needed if IOs / peripherals need 3V power from the MetaSensor
 * For MMS only, will be ignored for all others
 * @param board         Board to modify
 * @param index         0: Disable, 1: Enable
 */
  'mbl_mw_settings_enable_3V_regulator': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Modify the configuration of a pulse detector.
 * @param pulse             Pulse detector to modify
 * @param threshold         New threshold of the detector
 * @param width             New width of the detector
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non pulse detector was passed in
 */
  'mbl_mw_dataprocessor_pulse_modify': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float, ref.types.uint16]],

/**
 * Switches the accelerometer to active mode
 * When in active mode, the accelerometer cannot be configured.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bosch_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Force 1M PHY (Disables 2M PHY in BLE5.0+).
 * @param board         Board to modify
 * @param index         0: Use Auto Phy 1: Force 1Mhz Phy
 */
  'mbl_mw_settings_force_1M_phy': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Enables the BMI160 step detector 
 * A callback will notify the user when a step is detected but will NOT count steps
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bmi160_enable_step_detector': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Enables the BMI270 wrist gesture recognition
 * @param board     Board to modify
 */
  'mbl_mw_acc_bmi270_enable_wrist_gesture': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Ends command recording.  
 * This function is non-blocking and will asynchronously alert the caller when the operation is completed.
 * @param event                 Event to end recording for
 * @param context               Pointer to additional data for the callback function
 * @param commands_recorded     Callback function to be executed when commands have been recorded
 */
  'mbl_mw_event_end_record': [ref.types.void, [ref.refType(Event), ref.refType(ref.types.void), FnVoid_VoidP_EventP_Int]],

/**
 * Retrieves the data signal representing data from the BMI160 step counter
 * This signal represents the number of steps a user has taken (steps = 275)
 * This signal can be read once but should not be streamed or logged.
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 step counter data signal
 * UINT32 is return signal data type
 */
  'mbl_mw_acc_bmi160_get_step_counter_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Remove all recorded events from the board.
 * @param board                 Calling object
 */
  'mbl_mw_event_remove_all': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the iBeacon advertising UUID.
 * @param board         Pointer to the board to send the command to
 * @param ad_uuid       Byte representation of the UUID in little endian ordering
 */
  'mbl_mw_ibeacon_set_uuid': [ref.types.void, [ref.refType(MetaWearBoard), ArrayUByte_16]],

/**
 * Ends macro recording.  
 * An numerical id representing the macro will be passed to the callback function when the operation is complete.
 * @param board                 Calling object
 * @param context               Pointer to additional data for the callback function
 * @param commands_recorded     Callback function to be executed when the commands are recorded
 */
  'mbl_mw_macro_end_record': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Sets advertising transmitting power.  If a non valid value is set, the nearest valid value will be used instead.
 * @param board         Board to set the TX power
 * @param tx_power      Valid values are: 4, 0, -4, -8, -12, -16, -20, -30
 */
  'mbl_mw_settings_set_tx_power': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.int8]],

/**
 * Create an rms processor.  
 * Computes the root mean square of the input.
 * Works on inputs such as acc, gyro, and magnetometer data (x,y,z)
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_rms_create': [ref.types.int32, [ref.refType(DataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Flushes logging data (pending writes) to the MMS memory.
 * Should be called for the MMS when done with logging and ready to download data
 * For MMS only.
 * @param board                 Board to stop logging
 */
  'mbl_mw_logging_flush_page': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Create a passthrough processor.  
 * On a pass-count, only the count # of samples will go through and then the processor will shut off.
 * On a pass-conditional, if the count=0, all data is blocked. if the count>0, all data is passed.
 * Gate that only allows data though based on a user configured internal state.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param mode                  Processor's operation mode
 * @param count                 Internal count to initial the processor with
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_passthrough_create': [ref.types.int32, [ref.refType(DataSignal), PassthroughMode, ref.types.uint16, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Disables acceleration sampling.
 * The board will stop gathering data from the accelerometer
 * @param board     Board to disable acceleration sampling on
 */
  'mbl_mw_acc_disable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Create a math processor using unsigned operations.  
 * Performs simple arithmetic on sensor data. See MblMwMathOperation for allowed ops.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Math operation to compute
 * @param rhs                   Right hand side of the operation that requires 2 inputs
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_math_create_unsigned': [ref.types.int32, [ref.refType(DataSignal), MathOperation, ref.types.float, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Adds MAC Addresses for Whitelist filtering.
 * @param board         Board to modify
 * @param index         Whitelist MAC address in range [1, 8], must start at 1 and go in increasing order
 * @param address       Address to add
 */
  'mbl_mw_settings_add_whitelist_address': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.refType(BtleAddress)]],

/**
 * Remove all macros on the flash memory.  
 * The erase operation will not be performed until you disconnect from the board.  
 * If you wish to reset the board after the erase operation, use the mbl_mw_debug_reset_after_gc method.
 * @param board     Calling object
 */
  'mbl_mw_macro_erase_all': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Begin macro recording.  
 * Used to command the board on boot. Commands will survive a reset.
 * For example, on boot, flash the LED red.
 * For example, when the user drops the metawear (freefall event), vibrate the coin motor.
 * For example, renaming the device permanently.
 * Every MetaWear command issued will be recorded to the flash memory.
 * @param board             Calling object
 * @param exec_on_boot      True if the commands should be executed when the board powers on
 */
  'mbl_mw_macro_record': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Plays any programmed patterns.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_led_play': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Reads the current stack state.  If feature is unspported, nullptr will be passed to the `handler` parameter.
 * @param board     Calling object
 * @param context   Pointer to additional data for the callback function
 * @param handler   Callback function for handling the received data
 */
  'mbl_mw_debug_read_stack_overflow_state': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_DataP]],

/**
 * Resets the BMI270 step counter (step counter = 0)
 * Does not disable the step counter
 * @param board     Board to reset
 */
  'mbl_mw_acc_bmi270_reset_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the sensor gain.
 * There are altogether six gain settings (1X, 2X, 4X, 8X, 48X and 96X) available for user to configure
 * See MblMwAlsLtr329Gain for allowed values
 * @param board     Pointer to the board to modify
 * @param gain      Sensor gain value to set
 */
  'mbl_mw_als_ltr329_set_gain': [ref.types.void, [ref.refType(MetaWearBoard), AlsLtr329Gain]],

/**
 * Retrieves the data signal representing switch state data.
 * The switch is either pushed (1) or not pushed (0)
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the switch data signal
 * UINT32 is return signal data type 
 */
  'mbl_mw_switch_get_state_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Removes the logger from the board.
 * @param logger                logger to remove
 */
  'mbl_mw_logger_remove': [ref.types.void, [ref.refType(DataLogger)]],

/**
 * Generates a string identifying the data chain the logger is receiving data from.  This string is matched with the 
 * output of mbl_mw_anonymous_datasignal_get_identifier.
 * The memory allocated by the function must be freed by calling mbl_mw_memory_free.
 * @param logger                Calling object
 */
  'mbl_mw_logger_generate_identifier': [ref.types.CString, [ref.refType(DataLogger)]],

/**
 * Downloads the log data.
 * @param board                     Board to download the log data from
 * @param n_notifies                How many progress updates to send
 * @param handler                   Handler for processing logger responses
 */
  'mbl_mw_logging_download': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.refType(LogDownloadHandler)]],

/**
 * Retrieves a data signal representing the length of the log, including timestamps.
 * @param board                 Board to get reset_uid from
 */
  'mbl_mw_logging_get_length_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * @deprecated As of v0.10.0, use mbl_mw_dataprocessor_lowpass_create
 */
  'mbl_mw_dataprocessor_average_create': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Retrieves a data signal representing the current logger time state.  This includes the
 * reset_uid and time of boot.
 * @param board                 Board to get time from
 */
  'mbl_mw_logging_get_time_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Stops a timer.
 * @param timer     Timer to stop
 */
  'mbl_mw_timer_stop': [ref.types.void, [ref.refType(Timer)]],

/**
 * Determines the board model of the currently connected device.  
 * Only call this function after the board has been initialized.
 * @return Board model, MBL_MW_MODEL_NA if unable to determine
 */
  'mbl_mw_metawearboard_get_model': [Model, [ref.refType(MetaWearBoard)]],

/**
 * Sets the 
 * Sine of the maximum allowed downward tilt angle in landscape right direction of the device, when it is in focus position. 
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). Range is 700 to 1024. Default value is 1024.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum tilt angle in Landscape Right mode 
 */
  'mbl_mw_acc_bmi270_wrist_wakeup_tilt_lr': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Initialize the API's internal state.  
 * This function is non-blocking and will alert the caller when the operation is complete.
 * @param board         Board to initialize
 * @param context       Pointer to additional data for the callback function
 * @param initialized   Callback function to be executed when the board is initialized
 */
  'mbl_mw_metawearboard_initialize': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Set the device boot time for a given reset_uid.  This reference time
 * is used to calcuated real timestamps from logged data.
 * @param board                 Board to use
 * @param reset_uid             Reset id
 * @param reference_epoch       New reference epoch (in milliseconds) to use
 */
  'mbl_mw_logging_set_reference_time': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.int64]],

/**
 * Retrieves an individual value from a multi-valued datasignal i.e. MblMwCartesianFloat datasignal is 3 float values.  
 * For example, the x component for the acc x,y,z can be retrieved with this function
 * These individual signals can use the full suite of 
 * @param signal                Data signal to lookup
 * @param index                 Index of the component to return
 * @return Signal component, null if signal is signle valued
 */
  'mbl_mw_datasignal_get_component': [ref.refType(DataSignal), [ref.refType(DataSignal), ref.types.uint8]],

/**
 * Restarts the board after performing garbage collection.
 * @param board     Calling object
 */
  'mbl_mw_debug_reset_after_gc': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Drives a motor - Pulls down the HCD pin.
 * The MetaWear has a driver for motor or buzzers that are 3C compatible (check the datasheet)
 * This is not a full PWM driver.
 * @param board             Pointer to the board to send the command to
 * @param duty_cycle_per    Strength of the motor, between [0, 100] percent
 * @param pulse_width_ms    How long to run the motor, in milliseconds
 */
  'mbl_mw_haptic_start_motor': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float, ref.types.uint16]],

/**
 * Pulls the current gyro output data rate and data range from the sensor
 * returns 0 if successful
 * data is on context ptr
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
  'mbl_mw_gyro_bmi270_read_config': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Enables iBeacon mode.  You will need to disconnect from the board to 
 * advertise as an iBeacon.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_ibeacon_enable': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing data from the motion detection algorithm
 * This signal retrieves either "no motion", "any motion" or "significant motion" depending on user settings
 * @param board     Calling object
 * @return Pointer to Bosch's motion detection data signal
 * UINT32 is return signal data type represented by MblMwAccMotionTypes
 */
  'mbl_mw_acc_bosch_get_motion_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the digital output state
 * The GPIO pin is set as an output pin and turned on (high voltage)
 * See MetaWear datasheet for allowed pin values
 * @param board     Board the pin is on
 * @param pin       GPIO pin to set
 */
  'mbl_mw_gpio_set_digital_output': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Downloads the raw log data
 * @param board                     Board to download the log data from
 * @param n_notifies                How many progress updates to send
 * @param handler                   Handler for processing logger responses
 */
  'mbl_mw_logging_raw_download': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.refType(RawLogDownloadHandler)]],

/**
 * Overwrites the current accumulator value with a new value.
 * Can be used to reset the running sum
 * @param accumulator           Accumulator processor to modify
 * @param new_running_sum       New running sum of the accumulator
 * @return  MBL_MW_STATUS_OK if processor state was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if a non-accumulator 
 *          was passed in
 */
  'mbl_mw_dataprocessor_set_accumulator_state': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float]],

/**
 * Sets connection parameters.
 * @param board          Board to modify
 * @param mode           Whitelist filter mode
 */
  'mbl_mw_settings_set_whitelist_filter_mode': [ref.types.void, [ref.refType(MetaWearBoard), WhitelistFilter]],

/**
 * Looks up the MblMwDataLogger object corresponding to the id.
 * @param board             Board to search on
 * @param id                Numerical id to lookup
 * @return Logger object identified by the id, null if no object is found
 */
  'mbl_mw_logger_lookup_id': [ref.refType(DataLogger), [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Configures Bluetooth LE ad parameters.
 * @param board         Calling object
 * @param interval      Advertisement interval, between [0, 65535] milliseconds
 * @param timeout       Advertisement timeout, between [0, 180] seconds where 0 indicates no timeout
 * @param type          Advertisement type, ignored if unsupported on current firmware
 */
  'mbl_mw_settings_set_ad_parameters': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16, ref.types.uint8, BleAdType]],

/**
 * Creates a timer that will run for a set number of repetitions.  
 * A pointer representing the timer will be passed to the user through a callback function
 * @param board             Board the timer belongs to
 * @param period            How often to 
 * @param repetitions       Number of events the timer will fire
 * @param delay             Zero if the tiemr should immediately fire, non-zero to delay the first event
 * @param context           Pointer to additional data for the callback function
 * @param received_timer    Callback function to be executed when the timer is created
 */
  'mbl_mw_timer_create': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint32, ref.types.uint16, ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_TimerP]],

/**
 * Create a multi-value comparator for signed comparisons. 
 * This feature is only available on firmware v1.2.3 and later.  A pointer representing the 
 * processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input
 * @param op                    Comparison operation to execute (=, !=, <, >)
 * @param mode                  Processor output mode
 * @param references            Array of reference values to compare against
 * @param references_length     Number of elements in the references array
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_multi_comparator_create_signed': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ComparatorMode, ArrayFloat, ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Enables the BMI270 step detector 
 * A callback will notify the user when a step is detected but will NOT count steps
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bmi270_enable_step_detector': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal the logger is recording data for.
 * @param logger            Logger to lookup
 * @return Pointer to owning MblMwDataSignal object
 */
  'mbl_mw_logger_get_signal': [ref.refType(DataSignal), [ref.refType(DataLogger)]],

/**
 * Sets how long the API should wait before a required response is received.  
 * You should increase this value if operations such as API initialization, creating timer, 
 * loggers, and data processors, and recording commands consistently time out.
 * @param board                 Board to configure
 * @param response_time_ms      How long to wait for a response, from [0, 4000]ms.  Use 0ms for indefinite timeout
 */
  'mbl_mw_metawearboard_set_time_for_response': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Variant of rotation data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z acc gyro (it packs three acc data points in one timestamp)
 * @return Pointer to the data singal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
  'mbl_mw_gyro_bmi270_get_packed_rotation_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Deserializes API state.  
 * This function must be executed before calling mbl_mw_metawearboard_initialize.
 * @param board         Board to deserialize
 * @param state         Byte array holding the the information state
 * @param size          Byte array size
 * @return MBL_MW_STATUS_OK if successful, MBL_MW_STATUS_ERROR_SERIALIZATION_FORMAT if failed
 */
  'mbl_mw_metawearboard_deserialize': [ref.types.int32, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint32]],

/**
 * Creates a timer that will run indefinitely.  
 * A pointer representing the timer will be passed to the user through a callback function
 * @param board             Board the timer belongs to
 * @param period            How often to 
 * @param delay             Zero if the tiemr should immediately fire, non-zero to delay the first event
 * @param context           Pointer to additional data for the callback function
 * @param received_timer    Callback function to be executed when the timer is created
 */
  'mbl_mw_timer_create_indefinite': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint32, ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_TimerP]],

/**
 * Pauses the patterns.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_led_pause': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Looks up the MblMwTimer object corresponding to the id.
 * @param board             Board to search on
 * @param id                Numerical id to lookup
 * @return Timer object identified by the id, null if no object is found
 */
  'mbl_mw_timer_lookup_id': [ref.refType(Timer), [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Subscribes to a data stream, processing messages with the given handler.
 * @param signal                Data signal to subscribe to
 * @param context               Pointer to additional data for the callback function
 * @param received_data         Callback function to handle data received from the signal
 */
  'mbl_mw_datasignal_subscribe': [ref.types.void, [ref.refType(DataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataP]],

/**
 * Enables the tap detector
 * Not supported by the BMI270.
 * @param board             Calling object
 * @param enable_single     0 to ignore single tap detection, non-zero to detect
 * @param enable_double     0 to ignore double tap detection, non-zero to detect
 */
  'mbl_mw_acc_bosch_enable_tap_detection': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8]],

/**
 * Writes the configuration to the sendor
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi270_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the MblMwMetaWearBoard the event belongs to.
 * @param event     Event to lookup
 * @return Pointer to the owner
 */
  'mbl_mw_event_get_owner': [ref.refType(MetaWearBoard), [ref.refType(Event)]],

/**
 * Retrieves a data signal representing the key register value.  This is a simple
 * 4 byte scratch register.
 * @param board     Board to receive data from
 */
  'mbl_mw_debug_get_key_register_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Resets the running average (averager current value).
 * @param average       Average processor to reset
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-average processor was passed in
 */
  'mbl_mw_dataprocessor_average_reset': [ref.types.int32, [ref.refType(DataProcessor)]],

/**
 * Starts a timer.
 * @param timer     Timer to start
 */
  'mbl_mw_timer_start': [ref.types.void, [ref.refType(Timer)]],

/**
 * Unsubscribes from a data stream.
 * @param signal    Data signal to unsubscribe from
 */
  'mbl_mw_datasignal_unsubscribe': [ref.types.void, [ref.refType(DataSignal)]],

/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_acc_bosch_get_packed_acceleration_data_signal instead.
 */
  'mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the iBeacon advertising minor number.
 * @param board     Pointer to the board to send the command to
 * @param minor     New advertising minor number
 */
  'mbl_mw_ibeacon_set_minor': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Sets the 
 * Sine of the maximum allowed backward tilt angle in portrait down direction of the device, when it is in focus position.
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). Range is 0 to179. Default value is 179.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum tilt angle in Portrait Down mode 
 */
  'mbl_mw_acc_bmi270_wrist_wakeup_tilt_pd': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Reads data from sensor represented by the data signal.
 * Data is forwarded to the callback function assigned by the mbl_mw_datasignal_subscribe function.  
 * This variant is for reads that require additional parameters.  
 * @param signal        Data signal to read from
 * @param parameters    Additional parameters required for the read operation
 */
  'mbl_mw_datasignal_read_with_parameters': [ref.types.void, [ref.refType(DataSignal), ref.refType(ref.types.void)]],

/**
 * Sets the iBeacon advertising period.
 * @param board     Pointer to the board to send the command to
 * @param period    New advertising period, in milliseconds
 */
  'mbl_mw_ibeacon_set_period': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Create a delta processor which looks for changes in the input. 
 * Only allows data through that is a min distance from a reference value.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param mode                  Output mode of the processor
 * @param magnitude             Min distance from the reference value to allow the input to pass
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_delta_create': [ref.types.int32, [ref.refType(DataSignal), DeltaMode, ref.types.float, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Reset the default orientation of the board. 
 * Works while sensor fusion is running or off. Works for NDOF and IMUPLUS only. 
 * @param board         Calling object
 */
  'mbl_mw_sensor_fusion_reset_orientation': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the pin pull mode
 * A GPIO pin can be pulled up (to high voltage), pulled down (to 0V), or left floating.
 * See MblMwGpioPullMode for allowed mode values
 * @param board     Board the pin is on
 * @param pin       GPIO pin to modify
 * @param mode      New pull mode
 */
  'mbl_mw_gpio_set_pull_mode': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, GpioPullMode]],

/**
 * Plays any programmed patterns, and immediately plays any patterns programmed later.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_led_autoplay': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Get the latest reset_uid read duing connection.
 * @param board                 Board to get reset_uid from
 * @return value of reset_uid
 */
  'mbl_mw_logging_get_latest_reset_uid': [ref.types.uint8, [ref.refType(MetaWearBoard)]],


  'mbl_mw_macro_record_raw': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Reads data from sensor represented by the data signal.  
 * Data is forwarded to the callback function assigned by the mbl_mw_datasignal_subscribe function
 * @param signal    Data signal to read from
 */
  'mbl_mw_datasignal_read': [ref.types.void, [ref.refType(DataSignal)]],

/**
 * Modify the internal count of the passthrough processor.
 * @param passthrough           Passthrough processor to modify
 * @param new_count             New internal count
 * @return MBL_MW_STATUS_OK if processor state was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-passthrough processor was passed in
 */
  'mbl_mw_dataprocessor_passthrough_set_count': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint16]],

/**
 * Subscribe to the data produced by the signal.
 * @param signal                Calling object
 * @param context               Pointer to additional data for the callback function
 * @param data_handler          Callback function to handle data received from the signal
 */
  'mbl_mw_anonymous_datasignal_subscribe': [ref.types.void, [ref.refType(AnonymousDataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataP]],

/**
 * Retrieves the data signal representing the charge status.
 * @param board         Calling object
 * @return Pointer to the charge status signal, nullptr if unsupported
 */
  'mbl_mw_settings_get_charge_status_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the tap detector's shock time parameter
 * Selects a tap shock duration of ‘0’->50ms, ‘1’->75ms (i.e how long a tap can be)
 * This is the time it take the tap to "settle".
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param time      New shock time
 */
  'mbl_mw_acc_bosch_set_shock_time': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschTapShockTime]],

/**
 * Enables command recording.  
 * Commands can be used to execute functions on the sensor as a result of an event.
 * For example, when the time counts to 30 (the event), take a temperature reading (the command).
 * All MetaWear commands called after this point will be executed when the owning event is fired
 * @param event     Event to record commands for
 */
  'mbl_mw_event_record_commands': [ref.types.void, [ref.refType(Event)]],

/**
 * Starts illuminance sampling.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_als_ltr329_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Modify the bin size of a sample delay processor.
 * @param sample_delay          Sample processor to modify
 * @param bin_size              Number of samples to hold before letting data through
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non sample delay processor was passed in
 */
  'mbl_mw_dataprocessor_sample_modify_bin_size': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint8]],

/**
 * Clears the digital output state
 * The GPIO pin is set as an output pin and turned off (0 voltage)
 * See MetaWear datasheet for allowed pin values
 * @param board     Board the pin is on
 * @param pin       GPIO pin to clear
 */
  'mbl_mw_gpio_clear_digital_output': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Frees the memory allocated for the struct.
 * @param board     Pointer to the memory to free
 */
  'mbl_mw_metawearboard_free': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Checks if the board is initialized.
 * @param board     Board to check
 * @return Zero if not initialized, non-zero if it is
 */
  'mbl_mw_metawearboard_is_initialized': [ref.types.int32, [ref.refType(MetaWearBoard)]],

/**
 * Checks module type i.e. what kind of accelerometer is being used.
 * @param board         Board to check
 * @param module        Module to lookup
 * @return Module type used by the board, MBL_MW_MODULE_TYPE_NA if module is not available
 * @see MBL_MW_MODULE_ACC_TYPE_BMI270
 * @see MBL_MW_MODULE_ACC_TYPE_BMI160
 */
  'mbl_mw_metawearboard_lookup_module': [ref.types.int32, [ref.refType(MetaWearBoard), Module]],

/**
 * Starts the DFU process and updloads the given file to the device.
 * @param board         Calling object
 * @param delegate      Struct the function will forward DFU progress updates to
 * @param filename      Path to firmware bin file
 */
  'mbl_mw_metawearboard_perform_dfu': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(DfuDelegate), ref.types.CString]],

/**
 * Clear the logger of saved entries.
 * @param board                 Board to remove entries from
 */
  'mbl_mw_logging_clear_entries': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Overwrites the current count with a new value
 * @param counter           Counter processor to modify
 * @param new_count         New count value
 * @return  MBL_MW_STATUS_OK if processor state was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if a non-counter 
 *          was passed in
 */
  'mbl_mw_dataprocessor_counter_set_state': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint32]],

/**
 * Disables the BMI270 wrist gesture recognition
 * @param board     Board to modify
 */
  'mbl_mw_acc_bmi270_disable_wrist_gesture': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Returns information about the onboard modules.
 * @param board             Calling object
 * @param size              Pointer to where the size of the returned array will be written to
 * @return Array of info objects
 */
  'mbl_mw_metawearboard_get_module_info': [ref.refType(ModuleInfo), [ref.refType(MetaWearBoard), ref.refType(ref.types.uint32)]],

/**
 * Writes the step counter configuration to the sensor
 * Applies the MODE set by set_step_counter_mode()
 * @param board     Board to write to
 */
  'mbl_mw_acc_bmi160_write_step_counter_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Serializes the API state.  
 * The memory allocated by the function must be freed by calling mbl_mw_memory_free.
 * @param board         Board to serialize
 * @param size          Pointer to where the size of the returned byte array will be written to
 * @return Byte array of the serialized state
 */
  'mbl_mw_metawearboard_serialize': [ref.refType(ref.types.uint8), [ref.refType(MetaWearBoard), ref.refType(ref.types.uint32)]],

/**
 * Manual compensation for the BMI270 gyro
 * The offset compensation field for each axis has a width of 10 bit using two’s complement notation. 
 * The offset resolution (LSB) is 61 mdps and the offset range is +- 31 dps.
 * @param board     Pointer to the board to send the command to
 * @param x_offset  Offset compensation for Gyroscope X-axis 
 * @param y_offset  Offset compensation for Gyroscope Y-axis 
 * @param z_offset  Offset compensation for Gyroscope Z-axis
 */
  'mbl_mw_gyro_bmi270_offsets': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint8]],

/**
 * Writes the wrist gesture recognition configuration to the sensor
 * Applies the ARM SIDE, PEAK, SAMPLE and DURATION set by set_wrist_gesture_*()
 * @param board     Board to write to
 */
  'mbl_mw_acc_bmi270_write_wrist_gesture_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Reads the current state of the board and creates anonymous data signals based on what data is being logged.
 * If this task failed, a null pointer will be passed into the `anonymous_signals` parameter
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param created       Callback function to be executed once the task is completed.
 */
  'mbl_mw_metawearboard_create_anonymous_datasignals': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_AnonymousDataSignalP_UInt]],

/**
 * Sets the watermark level of the step counter
 * The Step-counter will trigger output every time this number of steps are counted.
 * Holds implicitly a 20x factor, so the range is 0 to 20460, with resolution of 20 steps. 
 * If 0, the output is disabled. If 1, it will count to 20 steps. 
 * @param board     Board to modify
 * @param trigger   Number of steps
 */
  'mbl_mw_acc_bmi270_set_step_counter_trigger': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Loads the struct with a preset configuration.
 * @param pattern       Pointer to the pattern to write the configuration to
 * @param preset        Preset pattern to load 
 */
  'mbl_mw_led_load_preset_pattern': [ref.types.void, [ref.refType(LedPattern), LedPreset]],

/**
 * Modifies the multi-value comparator, changing the operation and reference values.  
 * This feature is only available on firmware v1.2.3 and later.
 * @param comparator            Comparator processor to modify
 * @param op                    New comparison operation
 * @param references            Array of new reference values
 * @param references_length     Number of elements in the references array
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-comparator processor was passed in
 */
  'mbl_mw_dataprocessor_multi_comparator_modify': [ref.types.int32, [ref.refType(DataProcessor), ComparatorOperation, ArrayFloat, ref.types.uint8]],

/**
 * Retrieves the id value identifying the logger.
 * @param logger            Logger to lookup
 * @return Numerical id of the logger
 */
  'mbl_mw_logger_get_id': [ref.types.uint8, [ref.refType(DataLogger)]],

/**
 * Stop sensor fusion.
 * The board will stop gathering data from the gyroscope, accelerometer, and gyroscope and stop the sensor fusion.
 * @param board         Calling object
 */
  'mbl_mw_sensor_fusion_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Writes the led pattern to the board.
 * @param board     Pointer to the board to send the command to
 * @param pattern   Pointer to the pattern attributes to write to the board
 * @param color     Color the pattern is configuring
 */
  'mbl_mw_led_write_pattern': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(LedPattern), LedColor]],

/**
 * Modify the configuration of a math processor, changing the right side value of the operation.
 * @param math                  Math processor to modify
 * @param rhs                   New right hand side of the operation
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-math processor was passed in
 */
  'mbl_mw_dataprocessor_math_modify_rhs': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float]],

/**
 * Sets the pin change type to monitor
 * The monitor provides a callback anytime the value changes by MblMwGpioPinChangeType when pin monitoring is on
 * See MblMwGpioPinChangeType for allowed type values and see MetaWear datasheet for allowed pin values
 * @param board     Board the pin is on
 * @param pin       GPIO pin to set
 * @param type      Change type to monitor
 */
  'mbl_mw_gpio_set_pin_change_type': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, GpioPinChangeType]],

/**
 * Retrieves the data signal representing data from the wrist gesture algorithm
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI270 step detector data signal
 * UINT32 is return signal data type represented by MblMwAccGestureCode
 */
  'mbl_mw_acc_bmi270_get_wrist_detector_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Stops playing LED patterns and clears all pattern configurations.
 * @param board             Pointer to the board to send the command to
 */
  'mbl_mw_led_stop_and_clear': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Create a buffer processor.
 * Stores one entry. Great for temporarily storing the output of other processors.
 * Captures input data which can be retrieved at a later point in time
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_buffer_create': [ref.types.int32, [ref.refType(DataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Sets the iBeacon advertising major number.
 * @param board     Pointer to the board to send the command to
 * @param major     New advertising major number
 */
  'mbl_mw_ibeacon_set_major': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Set the standby time for the BME280 barometer.
 * When the sensor operates in normal mode, it continuously cycles between an active measurement period and an inactive standby period. 
 * See MblMwBaroBme280StandbyTime for allowed values.
 * @param board     Pointer to the board to modify
 * @param standby_time      Standby time value to set
 */
  'mbl_mw_baro_bme280_set_standby_time': [ref.types.void, [ref.refType(MetaWearBoard), BaroBme280StandbyTime]],

/**
 * Sets the iBeacon advertising transmitting power.
 * @param board         Pointer to the board to send the command to
 * @param tx_power      New advertising transmitting power
 */
  'mbl_mw_ibeacon_set_tx_power': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.int8]],

/**
 * Create a pulse detector. 
 * Detects and quantifies a pulse over the input values using the threshold and width specified.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param output                Output type of the processor
 * @param threshold             Value the data must exceed for a valid pulse
 * @param width                 Number of samples that must exceed the threshold for a valid pulse
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_pulse_create': [ref.types.int32, [ref.refType(DataSignal), PulseOutput, ref.types.float, ref.types.uint16, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Create a math processor where signed/unsigned operation is inferred.  
 * Performs simple arithmetic on sensor data. See MblMwMathOperation for allowed ops.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Math operation to compute
 * @param rhs                   Right hand side of the operation that requires 2 inputs
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_math_create': [ref.types.int32, [ref.refType(DataSignal), MathOperation, ref.types.float, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Creates a time delay processor.  
 * Can be used to periodically allow data through. 
 * Can be used to slowly (low freq/sampling rate) get data from sensors (i.e count to 30 and take a temp reading)
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param mode                  Operation mode of the processor
 * @param period                How often to allow data through, in milliseconds
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_time_create': [ref.types.int32, [ref.refType(DataSignal), TimeMode, ref.types.uint32, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Retrieves the data signal representing a temperature source
 * The temperature is in C by default
 * The temperature sensor can by the selected; it can be the on die sensor that is built in to the MCU, 
 * the on board thermistor, an external thermistor added to the GPIOs of the board or the BMP280.
 * Each board comes with a different configuration of temperature sensors, refer to the datasheet of your MetaWear.
 * @param board     Board to retrieve the signal from
 * @param channel   Channel ID of the temperature source
 * INT32 is return signal data type 
 */
  'mbl_mw_multi_chnl_temp_get_temperature_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Modify the configuration of the time delay processor.
 * @param time_delay            Time delay processor to modify
 * @param period                How often to allow data through, in milliseconds
 */
  'mbl_mw_dataprocessor_time_modify_period': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint32]],

/**
 * Create a threshold processor.  
 * Allows data through that crosses a boundary (threshold) according to MblMwThresholdMode
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param mode                  Processor output mode (absolute mode, output is value | binary mode output is 1 rising edge, -1 if falling)
 * @param boundary              Limit (threshold) that triggers an event when data crosses it
 * @param hysteresis            Min distance (error/diff) between the limit and value to signal a successful crossing
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_threshold_create': [ref.types.int32, [ref.refType(DataSignal), ThresholdMode, ref.types.float, ref.types.float, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Starts advertising.
 * @param board         Board to start btle advertisement
 */
  'mbl_mw_settings_start_advertising': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Get the data signal representing a processor's internal state.  
 * Processors that have an internal state are: accumulator, buffer, counter, delta, and passthrough.
 * @param processor         Processor to access
 * @return Pointer to the data signal, null if the processor does not have an internal state
 */
  'mbl_mw_dataprocessor_get_state_data_signal': [ref.refType(DataSignal), [ref.refType(DataProcessor)]],

/**
 * Disables the BMI270 step detector
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bmi270_disable_step_detector': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Looks up the MblMwDataProcessor object corresponding to the id.
 * @param board             Board to search on
 * @param id                Numerical id to lookup
 * @return Data processor object identified by the id, null if no object is found
 */
  'mbl_mw_dataprocessor_lookup_id': [ref.refType(DataProcessor), [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Modify the passthrough configuration.
 * @param passthrough           Passthrough processor to update
 * @param mode                  New operation mode to use
 * @param count                 New initial count
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-passthrough processor was passed in
 */
  'mbl_mw_dataprocessor_passthrough_modify': [ref.types.int32, [ref.refType(DataProcessor), PassthroughMode, ref.types.uint16]],

/**
 * Create a counter with an output size of 1 byte.  
 * Counts the number of times an event was fired.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Event the processor is counting
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_counter_create': [ref.types.int32, [ref.refType(Event), ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Enables the BMI270 activity detector 
 * A callback will notify the user when an activity is detected (walking, running, still)
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bmi270_enable_activity_detection': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Create a packer for the input signal.  
 * Combines multiple data values into 1 BLE packet.
 * Can be used to combine 3 accelerometer data entries into 1 packet. Used to ultra fast streaming.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param count                 Number of inputs to pack into 1 BLE packet
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_packer_create': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Create a high-pass filter.
 * Uses the averager to compute the difference of the current value from a running average of the previous (amount of) "size" samples.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param size                  Number of previous data samples to compare against, Recommended
 *                              to be a power of 2 for faster computation.
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_highpass_create': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Creates an MblMwDataLogger for the signal.
 * @param signal                Data signal to log
 * @param context               Pointer to additional data for the callback function
 * @param logger_ready          Callback function to be executed when the logger is created
 */
  'mbl_mw_datasignal_log': [ref.types.void, [ref.refType(DataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataLoggerP]],

/**
 * Sets the reference value of the processor.
 * @param delta                 Delta processor to modify
 * @param previous_value        Min distance from the reference value to allow the input to pass
 * @return MBL_MW_STATUS_OK if processor state was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-delta processor was passed in
 */
  'mbl_mw_dataprocessor_delta_set_reference': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float]],

/**
 * Execute the commands corresponding to the macro ID.
 * @param board     Calling object
 * @param id        Numerical ID of the macro to execute
 */
  'mbl_mw_macro_execute': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Modifies the threshold processor configuration.
 * @param threshold             Threshold processor to modify
 * @param boundary              Limit (threshold) that triggers an event when data crosses it
 * @param hysteresis            Min distance (error/diff) between the limit and value to signal a successful crossing
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-threshold processor was passed in
 */
  'mbl_mw_dataprocessor_threshold_modify_boundary': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float, ref.types.float]],

/**
 * Create an accumulator whose output is the same size as the input. 
 * Keeps a running sum of the input
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_accumulator_create': [ref.types.int32, [ref.refType(DataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Retrieve the temperature source type corresponding to a channel ID
 * @param board         Board to lookup the temperature source on
 * @param channel       Channel ID to lookup
 * @return Source type of the channel ID, MBL_MW_TEMP_SOURCE_INVALID if channel ID is out of range
 */
  'mbl_mw_multi_chnl_temp_get_source': [TemperatureSource, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Create an accumulator with a specific output size.  
 * Keeps a running sum of the input and returns the output as the specified size
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param output_size           Output size, between [1, 4] bytes
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_accumulator_create_size': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Create a comparator processor specifically for a signed comparison.  
 * Only allows data through that satisfies a comparison operation.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute (=, !=, <, >)
 * @param reference             Reference value to compare the input to
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_comparator_create_signed': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ref.types.float, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Enable magnetometer B field sampling
 * The board will start gathering data from the magnetometer
 * @param board         Calling object
 */
  'mbl_mw_mag_bmm150_enable_b_field_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing data from the BMI160 step detector
 * This signal simply detects a step, it does not count it (step recorded at time t)
 * This signal can be streamed or logged. This signal cannot be read or polled (read in loop).
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 step detector data signal
 * No return data type
 */
  'mbl_mw_acc_bmi160_get_step_detector_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Create a counter with a specific output size.  
 * Counts the number of times an event was fired with a specific output size
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Event the processor is counting
 * @param size                  Output size, between [1, 4] bytes
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_counter_create_size': [ref.types.int32, [ref.refType(Event), ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Create a comparator processor specifically for an unsigned comparison. 
 * Only allows data through that satisfies a comparison operation. 
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Comparison operation to execute (=, !=, <, >)
 * @param reference             Reference value to compare the input to
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_comparator_create_unsigned': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ref.types.float, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Modifies the comparator processor, changing the operation and reference value.
 * @param comparator            Comparator processor to modify
 * @param op                    New comparison operation (=, !=, <, >)
 * @param reference             New reference value
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-comparator processor was passed in
 */
  'mbl_mw_dataprocessor_comparator_modify': [ref.types.int32, [ref.refType(DataProcessor), ComparatorOperation, ref.types.float]],

/**
 * Set the iir filter coefficient.
 * The built-in IIR filter minimizes short-term disturbances in the output data caused by the slamming of a door or window. 
 * The filter coefficient ranges from 0 (off) to 16. 
 * See MblMwBaroBoschIirFilter for allowed values.
 * @param board     Pointer to the board to modify
 * @param iir_filter    IIR filter value to set
 */
  'mbl_mw_baro_bosch_set_iir_filter': [ref.types.void, [ref.refType(MetaWearBoard), BaroBoschIirFilter]],

/**
 * Create a multi-value comparator where a signed/unsigned comparison is inferred.  
 * This feature is only available on firmware v1.2.3 and later.  
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input
 * @param op                    Comparison operation to execute (=, !=, <, >)
 * @param mode                  Processor output mode
 * @param references            Array of reference values to compare against
 * @param references_length     Number of elements in the references array
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_multi_comparator_create': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ComparatorMode, ArrayFloat, ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Retrieves the data signal representing data from the orientation detection algorithm
 * This signal is identical to smartphone orientation (portrait, landscape)
 * The BMI270 does not support this function.
 * @param board     Calling object
 * @return Pointer to Bosch's orientation detection data signal
 * MblMwSensorOrientation is return signal data type
 */
  'mbl_mw_acc_bosch_get_orientation_detection_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Create a multi-value comparator for unsigned comparisons.  
 * This feature is only available on firmware v1.2.3 and later.  
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input
 * @param op                    Comparison operation to execute
 * @param mode                  Processor output mode
 * @param references            Array of reference values to compare against
 * @param references_length     Number of elements in the references array
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_multi_comparator_create_unsigned': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ComparatorMode, ArrayFloat, ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Modify the configuration of a math processor for a feedback or feedforward loop.
 * @param math                  Math processor to modify
 * @param rhs_signal            Data signal supplying the rhs value of the operation
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-math processor was passed in
 */
  'mbl_mw_dataprocessor_math_modify_rhs_signal': [ref.types.int32, [ref.refType(DataProcessor), ref.refType(DataSignal)]],

/**
 * Stops playing LED patterns.
 * @param board             Pointer to the board to send the command to
 */
  'mbl_mw_led_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Removes the timer from the board.
 * @param timer     Timer to remove
 */
  'mbl_mw_timer_remove': [ref.types.void, [ref.refType(Timer)]],

/**
 * Switches the gyro to active mode.  
 * While in active mode, the gyro cannot be configured
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi270_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Create a low-pass filter.
 * Uses the averager to create a moving average.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param size                  Number of previous data samples to compare against, Recommended
 *                              to be a power of 2 for faster computation.
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_lowpass_create': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Writes data via the SPI busm the parameters include the data, ss, frequency and more, see MblMwSpiParameters.
 * @param board             Board to communicate with
 * @param parameters        Parameters configuring the read
 */
  'mbl_mw_spi_write': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(SpiParameters)]],

/**
 * Modifies the sample size of the average processor
 * The sample size is the number of previous data samples to compare against
 * Recommended to be a power of 2 for faster computation.
 * @param average       Average processor to modify
 * @param size          New sample size to use
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-average processor was passed in
 */
  'mbl_mw_dataprocessor_average_modify_size': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint8]],

/**
 * Adds a packet counter to the input signal.
 * Great to make sure the packets are coming in order.
 * The count value is accessed through the MblMwData struct's <code>extra</code> field.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_accounter_create_count': [ref.types.int32, [ref.refType(DataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Retrieves the data signal representing data from the activity detection algorithm
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI270 step detector data signal
 * UINT32 is return signal data type represented by MblMwAccActivityCode
 */
  'mbl_mw_acc_bmi270_get_activity_detector_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Create a math processor using signed operations.  
 * Performs simple arithmetic on sensor data. See MblMwMathOperation for allowed ops.
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param op                    Math operation to compute
 * @param rhs                   Right hand side of the operation that requires 2 inputs
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_math_create_signed': [ref.types.int32, [ref.refType(DataSignal), MathOperation, ref.types.float, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Retrieves a data signal representing changes in digital data
 * The monitor provides a callback anytime the value changes
 * See MetaWear datasheet for allowed pin values
 * @param board     Board to receive data from
 * @param pin       GPIO pin to monitor
 * UINT32 is return signal data type 
 */
  'mbl_mw_gpio_get_pin_monitor_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Retrieves the data signal representing LTR329 illuminance data.
 * This signal represents the luminance in lux
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's LTR329 illuminance data signal
 * UINT32 is return signal data type 
 */
  'mbl_mw_als_ltr329_get_illuminance_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Create an rss processor.  
 * Computes the root sum square of the input.
 * Works on inputs such as acc, gyro, and magnetometer data (x,y,z)
 * A pointer representing the processor will be passed back to the user via a callback function.
 * @param source                Data signal providing the input for the processor
 * @param context               Pointer to additional data for the callback function
 * @param processor_created     Callback function to be executed when the processor is created
 */
  'mbl_mw_dataprocessor_rss_create': [ref.types.int32, [ref.refType(DataSignal), ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]],

/**
 * Removes all data processors and timers from the MetaWear board.
 * @param board         Board to tear down
 */
  'mbl_mw_metawearboard_tear_down': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing altitude data from a Bosch barometer.
 * This signal represent the altitude in Meters
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's altitude data signal
 * Int32 is return signal data type (can be negative)
 */
  'mbl_mw_baro_bosch_get_altitude_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing data from the BMI270 step counter
 * This signal represents the number of steps a user has taken (steps = 275)
 * This signal can be read once but should not be streamed or logged.
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI270 step counter data signal
 * UINT32 is return signal data type
 */
  'mbl_mw_acc_bmi270_get_step_counter_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing data from the BMI270 step detector
 * This signal simply detects a step, it does not count it (step recorded at time t)
 * This signal can be streamed or logged. This signal cannot be read or polled (read in loop).
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI270 step detector data signal
 * No return data type
 */
  'mbl_mw_acc_bmi270_get_step_detector_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing BMI160 rotation data
 * This signal is timestamp,x,y,z gyro data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 rotation data signal
 * MblMwCartesianFloat is return signal data type
 */
  'mbl_mw_gyro_bmi160_get_rotation_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing data from the tap detection algorithm
 * This signal retrieves the number of taps the board has been subjected to based on user settings
 * The BMI270 does not support this function.
 * @param board     Calling object
 * @return Pointer to Bosch's tap detection data signal
 * MblMwBoschTap is return signal data type
 */
  'mbl_mw_acc_bosch_get_tap_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the operational mode of the step counter
 * - Normal mode (default setting, recommended for most applications)
 * - Sensitive mode (can be used for light weighted, small persons)
 * - Robust mode (can be used, if many false positive detections are observed)
 * @param board     Board to modify
 * @param mode      New operation mode
 */
  'mbl_mw_acc_bmi160_set_step_counter_mode': [ref.types.void, [ref.refType(MetaWearBoard), AccBmi160StepCounterMode]],

/**
 * Enables the BMI160 step counter
 * @param board     Board to modify
 */
  'mbl_mw_acc_bmi160_enable_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Modifies the comparator processor for a feedback or feedforward loop.
 * @param comparator            Comparator processor to modify
 * @param op                    New comparison operation (=, !=, <, >)
 * @param reference_signal      Data signal output to be used for the reference value
 * @return MBL_MW_STATUS_OK if processor configuration was updated, MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE if 
 * a non-comparator processor was passed in
 */
  'mbl_mw_dataprocessor_comparator_modify_signal': [ref.types.int32, [ref.refType(DataProcessor), ComparatorOperation, ref.refType(DataSignal)]],

/**
 * Sets the output data rate for the BMI160 accelerometer
 * The ODR sets the output data frequency in Hz.
 * See MblMwAccBmi160Odr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
  'mbl_mw_acc_bmi160_set_odr': [ref.types.void, [ref.refType(MetaWearBoard), AccBmi160Odr]],

/**
 * Disables iBeacon mode.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_ibeacon_disable': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Disables the BMI160 step counter
 * @param board     Board to modify
 */
  'mbl_mw_acc_bmi160_disable_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Set the orientation calculation mode
 * Options include: Symmetrical, High asymmetrical, Low asymmetrical
 * See the BMI160 datasheet for more information. 
 * Not supported by the BMI270.
 * @param board         Calling object
 * @param mode          New calculation mode
 */
  'mbl_mw_acc_bosch_set_orientation_mode': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschOrientationMode]],

/**
 * Writes the configuration to the sendor
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi160_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Resets the BMI160 step counter (step counter = 0)
 * Does not disable the step counter
 * @param board     Board to reset
 */
  'mbl_mw_acc_bmi160_reset_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Writes the step counter configuration to the sensor
 * Applies the TRIGGER set by set_step_counter_trigger()
 * @param board     Board to write to
 */
  'mbl_mw_acc_bmi270_write_step_counter_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Pulls the current gyro output data rate and data range from the sensor
 * returns 0 if successful
 * data is on context ptr
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
  'mbl_mw_gyro_bmi160_read_config': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Disables the BMI160 step detector
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bmi160_disable_step_detector': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Reads the current charge status.  The callback function will be called with:  
 * 1    - battery is charging  
 * 0    - battery is not charging  
 * -1   - feature not supported
 */
  'mbl_mw_settings_read_current_charge_status': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Enables the BMI270 step counter 
 * The trigger count will trigger the callback signal with the # of steps
 * @param board     Board to modify
 */
  'mbl_mw_acc_bmi270_enable_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Reads the current step count.  The callback function will be called with:  
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param handler       Callback function that is executed when the task is finished
 */
  'mbl_mw_acc_bmi270_read_step_counter': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Sets the arm side the MetaWear is worn on for gesture recognition
 * Device in left (0 - false) or right (1 - true) arm. 
 * By default, the wearable device is assumed to be in left arm i.e. default value is 0 - false.
 * @param board     Pointer to the board to send the command to 
 * @param side      1 = Right arm, 0 = Left arm
 */
  'mbl_mw_acc_bmi270_wrist_gesture_armside': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Sets the tilt angle for gesture recognition
 * Sine of the minimum tilt angle in portrait down direction of the device when wrist is rolled away (roll-out) from user. 
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). 
 * Range is 1448 to 1774. Default value is 1774.
 * @param board     Pointer to the board to send the command to 
 * @param peak      Tilt Angle
 */
  'mbl_mw_acc_bmi270_wrist_gesture_peak': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Retrieves the firmware build id, used for identifying custom firmware build variants.
 * @param board         Calling object
 */
  'mbl_mw_settings_get_firmware_build_id': [ref.types.uint8, [ref.refType(MetaWearBoard)]],

/**
 * Switches the accelerometer to active mode.
 * @param board     Board the accelerometer is on
 */
  'mbl_mw_acc_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Value of minimum time difference between wrist roll-out and roll-in movement during flick gesture. 
 * Range is 3 to 5 samples at 50Hz (i.e. 0.06 to 0.1 seconds). Default value is 4 (i.e. 0.08 seconds).
 * @param board     Pointer to the board to send the command to 
 * @param sample      
 */
  'mbl_mw_acc_bmi270_wrist_gesture_samples': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Creates a synthetic notification internally to the MetaWear system.  Useful for testing.
 * @param board     Calling object
 * @param value     Value to spoof: [Module ID, Register ID, Notifcation En, Optional Index, Data...]
 * @param lenght    Size of the value array
 */
  'mbl_mw_debug_spoof_notification': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint8]],

/**
 * Stop pin monitoring
 * @param board     Board the pin is on
 * @param pin       GPIO pin to stop monitoring
 */
  'mbl_mw_gpio_stop_pin_monitoring': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Sets the 
 * Sine of the maximum allowed downward tilt angle in landscape left direction of the device, when it is in focus position.
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). Range is 700 to 1024. Default value is 700.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum tilt angle in Landscape Left mode 
 */
  'mbl_mw_acc_bmi270_wrist_wakeup_tilt_ll': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Sets the 
 * Sine of the maximum allowed forward tilt angle in portrait up direction of the device, when it is in focus position. 
 * The configuration parameter is scaled by 2048 i.e. 2048 * sin(angle). Range is 1774 to 1978. Default value is 1925.
 * @param board     Pointer to the board to send the command to 
 * @param angle     Maximum tilt angle in Portrait Up mode 
 */
  'mbl_mw_acc_bmi270_wrist_wakeup_tilt_pu': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Writes the wrist gesture recognition configuration to the sensor
 * Applies the ARM SIDE, PEAK, SAMPLE and DURATION set by set_wrist_gesture_*()
 * @param board     Board to write to
 */
  'mbl_mw_acc_bmi270_write_wrist_wakeup_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Enables the BMI270 wrist wakeup recognition
 * @param board     Board to modify
 */
  'mbl_mw_acc_bmi270_enable_wrist_wakeup': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Disables the BMI270 wrist wakeup recognition
 * @param board     Board to modify
 */
  'mbl_mw_acc_bmi270_disable_wrist_wakeup': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Manual compensation for the BMI270 acc
 * The offset compensation registers have a width of 8 bit using two’s complement notation.
 * The offset resolution (LSB) is 3.9 mg and the offset range is +- 0.5 g
 * @param board     Pointer to the board to send the command to
 * @param x_offset  Offset compensation for Accelerometer X-axis
 * @param y_offset  Offset compensation for Accelerometer Y-axis
 * @param z_offset  Offset compensation for Accelerometer Z-axis
 */
  'mbl_mw_acc_bmi270_acc_offsets': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16, ref.types.uint16, ref.types.uint16]],

/**
 * Set the hysteresis offset (degrees) for portrait/landscape detection
 * Upside/downside recognition hysteresis is not configurable.
 * See the BMI160 datasheet for more information. 
 * Not supported by the BMI270.
 * @param board         Calling object
 * @param hysteresis    New calculation mode
 */
  'mbl_mw_acc_bosch_set_orientation_hysteresis': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float]],

/**
 * Enables orientation detection
 * Not supported by the BMI270.
 * @param board     Calling object
 */
  'mbl_mw_acc_bosch_enable_orientation_detection': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Write IMU calibration data.  The data will be reloaded everytime the mode changes.
 * This function can only be used with firmware v1.4.3+.
 * See the MblMwCalibrationData struct which is composed of 10 bytes of acc data, 10 bytes of gyro data, and 10 bytes of mag data.
 * @param board         Calling object
 * @param data          Calibration data to load
 */
  'mbl_mw_sensor_fusion_write_calibration_data': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(CalibrationData)]],

/**
 * Disables orientation detection
 * Not supported by the BMI270.
 * @param board     Calling object
 */
  'mbl_mw_acc_bosch_disable_orientation_detection': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the tap detector's quiet time parameter
 * Selects a tap quiet duration of ‘0’->30 ms, ‘1’->20 ms (i.e how long to wait for another tap)
 * The criteria for a double-tap are fulfilled if the second tap occurs after the time quiet time and within the tap window. 
 * If during the quiet time period (30/20ms) a tap occurs, it will be considered as a new tap.
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param time      New quiet time
 */
  'mbl_mw_acc_bosch_set_quiet_time': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschTapQuietTime]],

/**
 * Retrieves a data signal representing digital data
 * See MetaWear datasheet for allowed pin values
 * @param board     Board to receive data from
 * @param pin       GPIO pin to read
 * UINT32 is return signal data type 
 */
  'mbl_mw_gpio_get_digital_input_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Sets the tap detector's double tap window
 * The time window between two taps is between 12.5ms and 500ms. 
 * The criteria for a double-tap are fulfilled if the second tap occurs after the quiet time and within the tap window.
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param window    New double tap window time
 */
  'mbl_mw_acc_bosch_set_double_tap_window': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschDoubleTapWindow]],

/**
 * Sets the tap detector's threshold
 * Threshold of the single/double-tap interrupt corresponding to an acceleration difference.
 * The threshold is typically between 0.7g and 1.5g in 2g measurement range.
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param window    New threshold level
 */
  'mbl_mw_acc_bosch_set_threshold': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float]],

/**
 * Sets the tap detector's double tap window
 * Applies the duration, quiet window, shock, and threshold from set_threshold(), set_double_tap_window(), set_shock_time(), set_quiet_time().
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param window    New double tap window time
 */
  'mbl_mw_acc_bosch_write_tap_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Disable the tap detector
 * Not supported by the BMI270.
 * @param board     Calling object
 * @param window    New double tap window time
 */
  'mbl_mw_acc_bosch_disable_tap_detection': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Stop pressure and altitude sensing.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_baro_bosch_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the any motion detector's count parameter
 * Any-motion generates an interrupt when the absolute value of the acceleration exceeds a "threshold"
 * for a certain "count" of consecutive data points.
 * For the BMI160, it is a simple counter.
 * For the BMI270, it is expressed in 50 Hz samples (20 ms). Range is 0 to 163sec. Default value is 5=100ms.
 * @param board     Calling object
 * @param count     Number of consecutive slope data points that must be above the threshold
 */
  'mbl_mw_acc_bosch_set_any_motion_count': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Configure the external thermistor
 * If a thermistor is added to the GPIOs of the MetaWear, this function will enable the configuration
 * @param board             Board the external thermistor is attached to
 * @param channel           Channel ID of the external thermistor 
 * @param data_pin          GPIO pin reading the data
 * @param pulldown_pin      GPIO pin the pulldown resistor is connected to
 * @param active_high       Zero if the pulldown pin is not active high, non-zero if active high
 */
  'mbl_mw_multi_chnl_temp_configure_ext_thermistor': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint8, ref.types.uint8]],

/**
 * Sets the any motion detector's threshold parameter
 * Any-motion generates an interrupt when the absolute value of the acceleration exceeds a "threshold"
 * for a certain "count" of consecutive data points.
 * For the BMI160, it is range-dependent (see datasheet). Default value is 0x14 = 5.11mg in 2g mode.
 * For the BMI270, range is 0 to 1g. Default value is 0xAA = 83mg.
 * @param board     Calling object
 * @param threshold Value that the slope data points must be above
 */
  'mbl_mw_acc_bosch_set_any_motion_threshold': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float]],

/**
 * Sets the no motion detector's count parameter
 * @param board     Calling object
 * @param count     Number of consecutive slope data points that must be above the threshold
 */
  'mbl_mw_acc_bosch_set_no_motion_count': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Sets the no motion detector's threshold parameter
 * @param board     Calling object
 * @param threshold Value that the slope data points must be above
 */
  'mbl_mw_acc_bosch_set_no_motion_threshold': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float]],

/**
 * Sets the sig motion detector's blocksize parameter
 * @param board     Calling object
 * @param threshold Value that the slope data points must be above
 */
  'mbl_mw_acc_bosch_set_sig_motion_blocksize': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

/**
 * Switches the accelerometer to standby mode.
 * @param board     Board the accelerometer is on
 */
  'mbl_mw_acc_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Writes the motion configuration to the remote device
 * Applies the threshold and count parameter or blocksize depending on motion type and settings from set_*_*()
 * For the BMI270, types include no motion, significant motion, and any motion. 
 * Ignored for other Bosch sensors.
 * @param board     Calling object
 * @param type      Type of motion requested
 */
  'mbl_mw_acc_bosch_write_motion_config': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschMotion]],

/**
 * Enables motion detection
 * The signal will callback if any motion is sensed based on the motion config
 * For the BMI270, signals include no motion, significant motion, and any motion. 
 * Ignored for other Bosch sensors.
 * @param board     Calling object
 * @param type      Type of motion requested
 */
  'mbl_mw_acc_bosch_enable_motion_detection': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschMotion]],

/**
 * Disables motion detection
 * @param board     Calling object
 * @param type      Type of motion requested
 */
  'mbl_mw_acc_bosch_disable_motion_detection': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschMotion]],

/**
 * Switches the accelerometer to standby mode (low power state)
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bosch_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Enables acceleration sampling
 * The board will start gathering data from the accelerometer
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bosch_enable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Disables acceleration sampling
 * The board will stop gathering data from the accelerometer
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_acc_bosch_disable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Switches the magnetometer into normal mode
 * @param board         Calling object
 */
  'mbl_mw_mag_bmm150_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Get the data signal object representing data from the sensor fusion algorithm
 * The sensor fusion algo is a kalman filter that combines acc, gyro, and mag data into outputs such as correct acceleration, euler angles or quaternions
 * @param board         Calling object
 * @param data          Desired sensor fusion data
 * @return Data signal object  
 * Return type can be MblMwCorrectedCartesianFloat, MblMwQuaternion, MblMwEulerAngl, MblMwCartesianFloat
 */
  'mbl_mw_sensor_fusion_get_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), SensorFusionData]],

/**
 * Get the data signal object representing the calibration state.  
 * This signal can only be used while the sensor fusion algorithm is running.
 * See CalibrationState: SENSOR_FUSION_CALIBRATION_ACCURACY_UNRELIABLE/LOW/MEDIUM/HIGH
 * @param board         Calling object
 * @return Data signal object  
 */
  'mbl_mw_sensor_fusion_calibration_state_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing acceleration data.
 * This signal is timestamp,x,y,z acc data
 * @param board     Board to retrieve the signal from
 * @return Pointer to the acceleration data signal
 * MblMwCartesianFloat is return signal data type
 */
  'mbl_mw_acc_get_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Set the operation mode.
 * See MblMwSensorFusionMode for allowed values:
 * MBL_MW_SENSOR_FUSION_MODE_NDOF - Calculates absolute orientation from accelerometer, gyro, and magnetometer - uses mag
 * MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS - Calculates relative orientation in space from accelerometer and gyro data - doesn't use mag
 * MBL_MW_SENSOR_FUSION_MODE_COMPASS - Determines geographic direction from th Earth’s magnetic field - uses mag
 * MBL_MW_SENSOR_FUSION_MODE_M4G - Similar to IMUPlus except rotation is detected with the magnetometer - uses mag
 * @param board         Calling object
 * @param mode          New operation mode
 */
  'mbl_mw_sensor_fusion_set_mode': [ref.types.void, [ref.refType(MetaWearBoard), SensorFusionMode]],

/**
 * Pulls the current sensor fusion configuration from the board by reading the mode register.
 * Returns status 0 if successful.
 * Config data is in context ptr.
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
  'mbl_mw_sensor_fusion_read_config': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_Int]],

/**
 * Writes the configuration to the LTR329 sensor.
 * Applies the INTEGRATION TIME, MEASUREMENT RATE, and GAIN values set in set_*().
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_als_ltr329_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Set the accelerometer data range.
 * Sets the range of the acc in Gs, see MblMwSensorFusionAccRange for allowed values
 * @param board         Calling object
 * @param range         New data range of the accelerometer
 */
  'mbl_mw_sensor_fusion_set_acc_range': [ref.types.void, [ref.refType(MetaWearBoard), SensorFusionAccRange]],

/**
 * Set the gyroscope data range.
 * Sets the range of the gyro in DPS, see MblMwSensorFusionGyroRange for allowed values
 * @param board         Calling object
 * @param range         New data range of the gyroscope
 */
  'mbl_mw_sensor_fusion_set_gyro_range': [ref.types.void, [ref.refType(MetaWearBoard), SensorFusionGyroRange]],

/**
 * Retrieve IMU calibration data; free the memory allocated for the MblMwCalibrationData pointer with mbl_mw_memory_free.
 * Only call this function when the calibration state of the IMUs is at high accuracy.  
 * This function can only be used with firmware v1.4.3+.  
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
  'mbl_mw_sensor_fusion_read_calibration_data': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.void), FnVoid_VoidP_MetaWearBoardP_CalibrationDataP]],

/**
 * Retrieves a data signal representing analog data
 * See MblMwGpioAnalogReadMode for allowed mode values and see MetaWear datasheet for allowed pin values
 * @param board     Board to receive data from
 * @param pin       GPIO pin to read
 * @param mode      Read mode to use
 * UINT32 is return signal data type 
 */
  'mbl_mw_gpio_get_analog_input_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8, GpioAnalogReadMode]],

/**
 * Set a data enable bit.
 * Turns on the Kalman filter (sensor fusion).
 * @param board         Calling object
 * @param data          Sensor fuson data to enable
 */
  'mbl_mw_sensor_fusion_enable_data': [ref.types.void, [ref.refType(MetaWearBoard), SensorFusionData]],

/**
 * Start sensor fusion.
 * The board will start gathering data from the gyroscope, accelerometer, and gyroscope and run the sensor fusion.
 * @param board         Calling object
 */
  'mbl_mw_sensor_fusion_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_acc_get_packed_acceleration_data_signal instead.
 */
  'mbl_mw_acc_get_high_freq_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Variant of acceleration data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z acc data (it packs three acc data points in one timestamp)
 * @return Pointer to the data signal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
  'mbl_mw_acc_get_packed_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the output data rate.  If an invalid odr is used, the closest valid value will be used.
 * The ODR sets the output data frequency in Hz.
 * @param board     Board to configure
 * @param odr       Output data rate, in Hz
 */
  'mbl_mw_acc_set_odr': [ref.types.float, [ref.refType(MetaWearBoard), ref.types.float]],

/**
 * Sets the full scale range.  IF an invalid range is used, the closet valid value will be used.
 * The range is in units of Gs 
 * @param board     Board to configure
 * @param range     Sampling range, in g's
 */
  'mbl_mw_acc_set_range': [ref.types.float, [ref.refType(MetaWearBoard), ref.types.float]],

/**
 * Writes the configuration to the sensor.
 * Applies the STANDBY, IIR and OVERSAMPLING values set in set_standbytime(), set_oversampling(), and set_iir_filter().
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_baro_bosch_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Writes the acceleration settings to the board.
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Board to configure
 */
  'mbl_mw_acc_write_acceleration_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Enables acceleration sampling.
 * The board will start gathering data from the accelerometer
 * @param board     Board to enable acceleration sampling on
 */
  'mbl_mw_acc_enable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Enables rotation sampling
 * The board will start gathering data from the gyroscope
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi160_enable_rotation_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing pressure data from a Bosch barometer.
 * This signal represent the pressure in Pascals
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's pressure data signal
 * uInt32 is return signal data type
 */
  'mbl_mw_baro_bosch_get_pressure_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing a single read from a Bosch barometer.
 * This signal represent the pressure in Pascals
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's pressure data signal
 * uInt32 is return signal data type
 */
  'mbl_mw_baro_bosch_get_pressure_read_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Set the oversampling mode.
 * Settings available from ultra low power to ultra high resolution to adapt the sensor to the target application. 
 * See MblMwBaroBoschOversampling for allowed values.
 * The settings are predefined combinations of pressure measurement oversampling and temperature measurement oversampling. 
 * @param board     Pointer to the board to modify
 * @param oversampling      Oversampling value to set
 */
  'mbl_mw_baro_bosch_set_oversampling': [ref.types.void, [ref.refType(MetaWearBoard), BaroBoschOversampling]],

/**
 * Sets the rotation range for the BMI270 gyroscope
 * The range is in units of degrees per second (dps) for Bosch sensors
 * See MblMwGyroBoschRange for allowed values.
 * @param board     Pointer to the board to modify
 * @param range     New rotation range
 */
  'mbl_mw_gyro_bmi270_set_range': [ref.types.void, [ref.refType(MetaWearBoard), GyroBoschRange]],

/**
 * Set the standby time for the BMP280 barometer.
 * When the sensor operates in normal mode, it perpetualy cycles between an active measurement period and an inactive standby period. 
 * See MblMwBaroBmp280StandbyTime for allowed values.
 * @param board     Pointer to the board to modify
 * @param standby_time      Standby time value to set
 */
  'mbl_mw_baro_bmp280_set_standby_time': [ref.types.void, [ref.refType(MetaWearBoard), BaroBmp280StandbyTime]],

/**
 * Set the standby time.  If an invalid standby time is used, the closest valid value will be chosen.
 * When the sensor operates in normal mode, it continuously cycles between an active measurement period and an inactive standby period. 
 * @param board                 Pointer to the board to modify
 * @param standby_time_ms       Standby time value to set, in milliseconds
 * @return Selected standby time, in milliseconds, or -1 if barometer not present
 */
  'mbl_mw_baro_bosch_set_standby_time': [ref.types.float, [ref.refType(MetaWearBoard), ref.types.float]],

/**
 * Start pressure and altitude sensing.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_baro_bosch_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing humidity data
 * Relative humidity is returned as a percentage.
 * @param board         Board the humidity sensor resides on
 * @return Pointer to the data signal
 * UINT32 is return signal data type 
 */
  'mbl_mw_humidity_bme280_get_percentage_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the oversampling mode
 * For the humidity measurement, oversampling is possible to reduce the noise. 
 * The resolution of the humidity measurement is fixed at 16 bit ADC output.
 * See MblMwHumidityBme280Oversampling for allowed oversampling values
 * @param board             Board the humidity sensor resides on
 * @param oversampling      New oversampling mode
 */
  'mbl_mw_humidity_bme280_set_oversampling': [ref.types.void, [ref.refType(MetaWearBoard), HumidityBme280Oversampling]],

/**
 * Start pin monitoring
 * @param board     Board the pin is on
 * @param pin       GPIO pin to monitor
 */
  'mbl_mw_gpio_start_pin_monitoring': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

/**
 * Variant of B field data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z mag data (it packs three mag data points in one timestamp)
 * @param board         Calling object
 * @return Pointer to the data signal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
  'mbl_mw_mag_bmm150_get_packed_b_field_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the power mode to one of the recommended presets
 * The BMM150 magnetometer part has four power modes, see MblMwMagBmm150Preset for values
 * @param board         Calling object
 * @param preset        New preset power mode to use
 */
  'mbl_mw_mag_bmm150_set_preset': [ref.types.void, [ref.refType(MetaWearBoard), MagBmm150Preset]],

/**
 * Disable B field sampling
 * The board will stop gathering data from the magnetometer
 * @param board         Calling object
 */
  'mbl_mw_mag_bmm150_disable_b_field_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Switches the magnetometer into sleep mode
 * @param board         Calling object
 */
  'mbl_mw_mag_bmm150_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Puts the magnetometer in suspend mode.  This function will not issuee the command unless the 
 * board is running minimum firmware v1.3.4.
 * @param board         Calling object
 */
  'mbl_mw_mag_bmm150_suspend': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing SPI data.  
 * The data signal is identified by the id value and if the id is not present, a new data signal will be created using the length parameter. 
 * @param board         Board to communicate with
 * @param length        Number of bytes to read
 * @param id            Numerical id identifying the data
 * @return Pointer to the spi data signal
 */
  'mbl_mw_spi_get_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8]],

/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_gyro_bmi160_get_packed_rotation_data_signal instead.
 */
  'mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Variant of rotation data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z gyro data (it packs three acc data points in one timestamp)
 * @return Pointer to the data singal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
  'mbl_mw_gyro_bmi160_get_packed_rotation_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

/**
 * Sets the output data rate for the BMI160 gyroscope
 * The ODR sets the output data frequency in Hz.
 * See MblMwGyroBoschOdr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
  'mbl_mw_gyro_bmi160_set_odr': [ref.types.void, [ref.refType(MetaWearBoard), GyroBoschOdr]],

/**
 * Sets the output data rate for the BMI270 gyroscope
 * The ODR sets the output data frequency in Hz.
 * See MblMwGyroBoschOdr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
  'mbl_mw_gyro_bmi270_set_odr': [ref.types.void, [ref.refType(MetaWearBoard), GyroBoschOdr]],

/**
 * Switches the gyro to active mode.  
 * While in active mode, the gyro cannot be configured
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi160_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Switches the gyro to standby mode.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi160_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Switches the gyro to standby mode.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi270_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Disables rotation sampling
 * The board will stop gathering data from the gyroscope
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi160_disable_rotation_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Enables rotation sampling
 * The board will start gathering data from the gyroscope
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_gyro_bmi270_enable_rotation_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Sets the sensor integration time.
 * Measurement time for each full light measurement (ALS) cycle - 100ms (default) to 350 ms
 * See MblMwAlsLtr329IntegrationTime for allowed values
 * @param board     Pointer to the board to modify
 * @param integration_time      Integration time value to set
 */
  'mbl_mw_als_ltr329_set_integration_time': [ref.types.void, [ref.refType(MetaWearBoard), AlsLtr329IntegrationTime]],

/**
 * Sets the sensor measurement rate.
 * Frequency of light measurement - 50ms (default) to 2000 ms
 * See MblMwAlsLtr329MeasurementRate for allowed values
 * @param board     Pointer to the board to modify
 * @param measurement_rate      Measurement rate value to set
 */
  'mbl_mw_als_ltr329_set_measurement_rate': [ref.types.void, [ref.refType(MetaWearBoard), AlsLtr329MeasurementRate]],

/**
 * Stops illuminance sampling.
 * @param board     Pointer to the board to send the command to
 */
  'mbl_mw_als_ltr329_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

/**
 * Retrieves the data signal representing i2c data
 * The data signal is identified by the id value and if the id is not present, a new data signal will be created using the length parameter. 
 * @param board         Board the i2c bus resides on
 * @param length        Number of bytes to read
 * @param id            Numerical value identifying the data
 * @return Pointer to the i2c data signal
 */
  'mbl_mw_i2c_get_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8]]
});

module.exports = {
  Const: Const,
  I2cReadParameters: I2cReadParameters,
  BoschGestureType: BoschGestureType,
  BoschAnyMotion: BoschAnyMotion,
  LedPattern: LedPattern,
  FnVoid_VoidP_charP: FnVoid_VoidP_charP,
  FnVoid_VoidP: FnVoid_VoidP,
  CartesianFloat: CartesianFloat,
  LogDownloadHandler: LogDownloadHandler,
  SpiParameters: SpiParameters,
  FnVoid_VoidP_UByte_Long_UByteP_UByte: FnVoid_VoidP_UByte_Long_UByteP_UByte,
  FnVoid_VoidP_UInt_UInt: FnVoid_VoidP_UInt_UInt,
  FnVoid_VoidP_UByte_UByte_UInt_UInt: FnVoid_VoidP_UByte_UByte_UInt_UInt,
  LoggingTime: LoggingTime,
  ArrayFloat: ArrayFloat,
  DeviceInformation: DeviceInformation,
  FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt: FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt,
  OverflowState: OverflowState,
  AccBoschGesture: AccBoschGesture,
  AccBoschTapQuietTime: AccBoschTapQuietTime,
  CorrectedCartesianFloat: CorrectedCartesianFloat,
  GpioAnalogReadParameters: GpioAnalogReadParameters,
  WhitelistFilter: WhitelistFilter,
  AlsLtr329MeasurementRate: AlsLtr329MeasurementRate,
  ModuleInfo: ModuleInfo,
  FnVoid_VoidP_VoidP_FnVoidVoidPtrInt: FnVoid_VoidP_VoidP_FnVoidVoidPtrInt,
  FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray: FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray,
  FnVoid_VoidP_VoidP_GattCharWriteType_GattCharP_UByteP_UByte: FnVoid_VoidP_VoidP_GattCharWriteType_GattCharP_UByteP_UByte,
  AlsLtr329Gain: AlsLtr329Gain,
  GattChar: GattChar,
  FnVoid_VoidP_DataProcessorP: FnVoid_VoidP_DataProcessorP,
  FnVoid_VoidP_TimerP: FnVoid_VoidP_TimerP,
  Event: Event,
  FnVoid_VoidP_DataLoggerP: FnVoid_VoidP_DataLoggerP,
  DataLogger: DataLogger,
  ArrayAnonymousDataSignalP: ArrayAnonymousDataSignalP,
  AccBmi270Odr: AccBmi270Odr,
  FnVoid_VoidP_MetaWearBoardP_CalibrationDataP: FnVoid_VoidP_MetaWearBoardP_CalibrationDataP,
  FnVoid_VoidP_MetaWearBoardP_Int: FnVoid_VoidP_MetaWearBoardP_Int,
  Data: Data,
  SensorFusionData: SensorFusionData,
  AccBoschActivity: AccBoschActivity,
  FnVoid_MetaWearBoardP: FnVoid_MetaWearBoardP,
  FnVoid_VoidP_Int: FnVoid_VoidP_Int,
  Module: Module,
  LedPreset: LedPreset,
  SpiFrequency: SpiFrequency,
  RawLogDownloadHandler: RawLogDownloadHandler,
  BatteryState: BatteryState,
  ArrayUByte_6: ArrayUByte_6,
  FnVoid_VoidP_DataP: FnVoid_VoidP_DataP,
  MagBmm150Odr: MagBmm150Odr,
  MathOperation: MathOperation,
  GpioPullMode: GpioPullMode,
  MetaWearRChannel: MetaWearRChannel,
  AccBmi160StepCounterMode: AccBmi160StepCounterMode,
  CalibrationState: CalibrationState,
  AccBoschRange: AccBoschRange,
  MetaWearRProChannel: MetaWearRProChannel,
  AlsLtr329IntegrationTime: AlsLtr329IntegrationTime,
  BaroBoschIirFilter: BaroBoschIirFilter,
  AccBoschTapShockTime: AccBoschTapShockTime,
  GyroBoschOdr: GyroBoschOdr,
  BoschTap: BoschTap,
  MetaWearBoard: MetaWearBoard,
  FnInt_VoidP_UByteP_UByte: FnInt_VoidP_UByteP_UByte,
  TemperatureSource: TemperatureSource,
  BtleAddress: BtleAddress,
  EulerAngles: EulerAngles,
  PulseOutput: PulseOutput,
  SensorOrientation: SensorOrientation,
  AccBoschAxisXyzRemap: AccBoschAxisXyzRemap,
  Quaternion: Quaternion,
  FnVoid_VoidP_MetaWearBoardP_AnonymousDataSignalP_UInt: FnVoid_VoidP_MetaWearBoardP_AnonymousDataSignalP_UInt,
  DfuDelegate: DfuDelegate,
  AccBoschMotion: AccBoschMotion,
  ComparatorOperation: ComparatorOperation,
  ArrayUByte_10: ArrayUByte_10,
  PassthroughMode: PassthroughMode,
  GpioPinChangeType: GpioPinChangeType,
  AccBoschAxisXyzSign: AccBoschAxisXyzSign,
  FnVoid_VoidP_EventP_Int: FnVoid_VoidP_EventP_Int,
  ComparatorMode: ComparatorMode,
  GpioAnalogReadMode: GpioAnalogReadMode,
  SensorFusionAccRange: SensorFusionAccRange,
  MagBmm150Preset: MagBmm150Preset,
  DataTypeId: DataTypeId,
  Model: Model,
  SensorFusionGyroRange: SensorFusionGyroRange,
  TimeMode: TimeMode,
  BtleConnection: BtleConnection,
  Timer: Timer,
  BaroBmp280StandbyTime: BaroBmp280StandbyTime,
  AccBmi160Odr: AccBmi160Odr,
  GyroBoschRange: GyroBoschRange,
  AnonymousDataSignal: AnonymousDataSignal,
  SensorFusionMode: SensorFusionMode,
  DeltaMode: DeltaMode,
  ArrayDataSignalP: ArrayDataSignalP,
  HumidityBme280Oversampling: HumidityBme280Oversampling,
  BleAdType: BleAdType,
  ArrayUByte_16: ArrayUByte_16,
  BaroBme280StandbyTime: BaroBme280StandbyTime,
  BaroBoschOversampling: BaroBoschOversampling,
  AccBoschOrientationMode: AccBoschOrientationMode,
  ThresholdMode: ThresholdMode,
  AccBoschDoubleTapWindow: AccBoschDoubleTapWindow,
  AccBoschTypewrist: AccBoschTypewrist,
  Lib: Lib,
  LedColor: LedColor,
  GattCharWriteType: GattCharWriteType,
  DataSignal: DataSignal,
  FnVoid_VoidP_MetaWearBoardP_FnBoardPtr: FnVoid_VoidP_MetaWearBoardP_FnBoardPtr,
  DataProcessor: DataProcessor,
  SpiMode: SpiMode,
  CalibrationData: CalibrationData
};
