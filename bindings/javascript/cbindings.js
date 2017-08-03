// Warning!!!
// This JavaScript module is generated from the MetaWear C++ API header files
// Do not edit
var ref = require('ref');
var ffi = require('ffi');
var Struct = require('ref-struct');
var Enum = require('enum');
var LIBMETAWEAR_PATH = require('./libmetawear-path');

var SpiFrequency = new Enum({
  '_125KHz': 0,
  '_250KHz': 1,
  '_500KHz': 2,
  '_1MHz': 3,
  '_2MHz': 4,
  '_4MHz': 5,
  '_8MHz': 6
}, ref.types.int);
SpiFrequency.alignment = 4

var ProximityTsl2671Current = new Enum({
  '_100mA': 0,
  '_50mA': 1,
  '_25mA': 2,
  '_12_5mA': 3
}, ref.types.int);
ProximityTsl2671Current.alignment = 4

var MetaWearRChannel = new Enum({
  'ON_DIE': 0,
  'EXT_THERMISTOR': 1
}, ref.types.int);
MetaWearRChannel.alignment = 4

var TemperatureSource = new Enum({
  'INVALID': -1,
  'NRF_DIE': 0,
  'EXT_THERM': 1,
  'BMP280': 2,
  'PRESET_THERM': 3
}, ref.types.int);
TemperatureSource.alignment = 4

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
MagBmm150Odr.alignment = 4

var GyroBmi160Odr = new Enum({
  '_25Hz': 6,
  '_50Hz': 7,
  '_100Hz': 8,
  '_200Hz': 9,
  '_400Hz': 10,
  '_800Hz': 11,
  '_1600Hz': 12,
  '_3200Hz': 13
}, ref.types.int);
GyroBmi160Odr.alignment = 4

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
BaroBme280StandbyTime.alignment = 4

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
BaroBmp280StandbyTime.alignment = 4

var SpiMode = new Enum({
  '_0': 0,
  '_1': 1,
  '_2': 2,
  '_3': 3
}, ref.types.int);
SpiMode.alignment = 4

var PassthroughMode = new Enum({
  'ALL': 0,
  'CONDITIONAL': 1,
  'COUNT': 2
}, ref.types.int);
PassthroughMode.alignment = 4

var MagBmm150Preset = new Enum({
  'LOW_POWER': 0,
  'REGULAR': 1,
  'ENHANCED_REGULAR': 2,
  'HIGH_ACCURACY': 3
}, ref.types.int);
MagBmm150Preset.alignment = 4

var ThresholdMode = new Enum({
  'ABSOLUTE': 0,
  'BINARY': 1
}, ref.types.int);
ThresholdMode.alignment = 4

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
  'METAMOTION_C': 10
}, ref.types.int);
Model.alignment = 4

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
AccBmi160Odr.alignment = 4

var ComparatorOperation = new Enum({
  'EQ': 0,
  'NEQ': 1,
  'LT': 2,
  'LTE': 3,
  'GT': 4,
  'GTE': 5
}, ref.types.int);
ComparatorOperation.alignment = 4

var LedPreset = new Enum({
  'BLINK': 0,
  'PULSE': 1,
  'SOLID': 2
}, ref.types.int);
LedPreset.alignment = 4

var MetaWearRProChannel = new Enum({
  'ON_DIE': 0,
  'ON_BOARD_THERMISTOR': 1,
  'EXT_THERMISTOR': 2,
  'BMP280': 3
}, ref.types.int);
MetaWearRProChannel.alignment = 4

var HumidityBme280Oversampling = new Enum({
  '_1X': 1,
  '_2X': 2,
  '_4X': 3,
  '_8X': 4,
  '_16X': 5
}, ref.types.int);
HumidityBme280Oversampling.alignment = 4

var ComparatorMode = new Enum({
  'ABSOLUTE': 0,
  'REFERENCE': 1,
  'ZONE': 2,
  'BINARY': 3
}, ref.types.int);
ComparatorMode.alignment = 4

var SensorFusionAccRange = new Enum({
  '_2G': 0,
  '_4G': 1,
  '_8G': 2,
  '_16G': 3
}, ref.types.int);
SensorFusionAccRange.alignment = 4

var DeltaMode = new Enum({
  'ABSOLUTE': 0,
  'DIFFERENTIAL': 1,
  'BINARY': 2
}, ref.types.int);
DeltaMode.alignment = 4

var AccBmi160StepCounterMode = new Enum({
  'NORMAL': 0,
  'SENSITIVE': 1,
  'ROBUST': 2
}, ref.types.int);
AccBmi160StepCounterMode.alignment = 4

var NeoPixelColorOrdering = new Enum({
  'WS2811_RGB': 0,
  'WS2811_RBG': 1,
  'WS2811_GRB': 2,
  'WS2811_GBR': 3
}, ref.types.int);
NeoPixelColorOrdering.alignment = 4

var GattCharWriteType = new Enum({
  'WITH_RESPONSE': 0,
  'WITHOUT_RESPONSE': 1
}, ref.types.int);
GattCharWriteType.alignment = 4

var GpioPinChangeType = new Enum({
  'RISING': 1,
  'FALLING': 2,
  'ANY': 3
}, ref.types.int);
GpioPinChangeType.alignment = 4

var PulseOutput = new Enum({
  'WIDTH': 0,
  'AREA': 1,
  'PEAK': 2,
  'ON_DETECTION': 3
}, ref.types.int);
PulseOutput.alignment = 4

var ProximityTsl2671Channel = new Enum({
  '_0': 1,
  '_1': 2,
  'BOTH': 3
}, ref.types.int);
ProximityTsl2671Channel.alignment = 4

var AlsLtr329Gain = new Enum({
  '_1X': 0,
  '_2X': 1,
  '_4X': 2,
  '_8X': 3,
  '_48X': 4,
  '_96X': 5
}, ref.types.int);
AlsLtr329Gain.alignment = 4

var NeoPixelRotDirection = new Enum({
  'TOWARDS': 0,
  'AWAY': 1
}, ref.types.int);
NeoPixelRotDirection.alignment = 4

var GyroBmi160Range = new Enum({
  '_2000dps': 0,
  '_1000dps': 1,
  '_500dps': 2,
  '_250dps': 3,
  '_125dps': 4
}, ref.types.int);
GyroBmi160Range.alignment = 4

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
  'GSR': 16,
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
Module.alignment = 4

var AccMma8452qRange = new Enum({
  '_2G': 0,
  '_4G': 1,
  '_8G': 2
}, ref.types.int);
AccMma8452qRange.alignment = 4

var ColorDetectorTcs34725Gain = new Enum({
  '_1X': 0,
  '_4X': 1,
  '_16X': 2,
  '_60X': 3
}, ref.types.int);
ColorDetectorTcs34725Gain.alignment = 4

var AccBma255Odr = new Enum({
  '_15_62Hz': 0,
  '_31_26Hz': 1,
  '_62_5Hz': 2,
  '_125Hz': 3,
  '_250Hz': 4,
  '_500Hz': 5,
  '_1000Hz': 6,
  '_2000Hz': 7
}, ref.types.int);
AccBma255Odr.alignment = 4

var GpioAnalogReadMode = new Enum({
  'ABS_REF': 0,
  'ADC': 1
}, ref.types.int);
GpioAnalogReadMode.alignment = 4

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
  'CORRECTED_CARTESIAN_FLOAT': 9
}, ref.types.int);
DataTypeId.alignment = 4

var SensorFusionMode = new Enum({
  'SLEEP': 0,
  'NDOF': 1,
  'IMU_PLUS': 2,
  'COMPASS': 3,
  'M4G': 4
}, ref.types.int);
SensorFusionMode.alignment = 4

var SensorFusionData = new Enum({
  'CORRECTED_ACC': 0,
  'CORRECTED_GYRO': 1,
  'CORRECTED_MAG': 2,
  'QUATERION': 3,
  'EULER_ANGLE': 4,
  'GRAVITY_VECTOR': 5,
  'LINEAR_ACC': 6
}, ref.types.int);
SensorFusionData.alignment = 4

var SensorFusionGyroRange = new Enum({
  '_2000DPS': 0,
  '_1000DPS': 1,
  '_500DPS': 2,
  '_250DPS': 3
}, ref.types.int);
SensorFusionGyroRange.alignment = 4

var AccBoschRange = new Enum({
  '_2G': 0,
  '_4G': 1,
  '_8G': 2,
  '_16G': 3
}, ref.types.int);
AccBoschRange.alignment = 4

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
AlsLtr329IntegrationTime.alignment = 4

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
MathOperation.alignment = 4

var AccMma8452qOdr = new Enum({
  '_800Hz': 0,
  '_400Hz': 1,
  '_200Hz': 2,
  '_100Hz': 3,
  '_50Hz': 4,
  '_12_5Hz': 5,
  '_6_25Hz': 6,
  '_1_56Hz': 7
}, ref.types.int);
AccMma8452qOdr.alignment = 4

var AlsLtr329MeasurementRate = new Enum({
  '_50ms': 0,
  '_100ms': 1,
  '_200ms': 2,
  '_500ms': 3,
  '_1000ms': 4,
  '_2000ms': 5
}, ref.types.int);
AlsLtr329MeasurementRate.alignment = 4

var GpioPullMode = new Enum({
  'UP': 0,
  'DOWN': 1,
  'NONE': 2
}, ref.types.int);
GpioPullMode.alignment = 4

var BaroBoschOversampling = new Enum({
  'SKIP': 0,
  'ULTRA_LOW_POWER': 1,
  'LOW_POWER': 2,
  'STANDARD': 3,
  'HIGH': 4,
  'ULTRA_HIGH': 5
}, ref.types.int);
BaroBoschOversampling.alignment = 4

var TimeMode = new Enum({
  'ABSOLUTE': 0,
  'DIFFERENTIAL': 1
}, ref.types.int);
TimeMode.alignment = 4

var LedColor = new Enum({
  'GREEN': 0,
  'RED': 1,
  'BLUE': 2
}, ref.types.int);
LedColor.alignment = 4

var BaroBoschIirFilter = new Enum({
  'OFF': 0,
  'AVG_2': 1,
  'AVG_4': 2,
  'AVG_8': 3,
  'AVG_16': 4
}, ref.types.int);
BaroBoschIirFilter.alignment = 4

var FnVoid_VoidP_Int = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.types.int32]);
var FnInt_VoidP_UByteP_UByte = ffi.Function(ref.types.int32, [ref.refType(ref.types.void), ref.refType(ref.types.uint8), ref.types.uint8]);
var MetaWearBoard = ref.types.void;
var FnVoid_MetaWearBoardP_Int = ffi.Function(ref.types.void, [ref.refType(MetaWearBoard), ref.types.int32]);
var DataLogger = ref.types.void;
var FnVoid_DataLoggerP = ffi.Function(ref.types.void, [ref.refType(DataLogger)]);
var Timer = ref.types.void;
var FnVoid_TimerP = ffi.Function(ref.types.void, [ref.refType(Timer)]);
var Data = Struct({
  'epoch': ref.types.int64,
  'value': ref.refType(ref.types.void),
  'type_id': DataTypeId,
  'length': ref.types.uint8
});

var FnVoid_DataP = ffi.Function(ref.types.void, [ref.refType(Data)]);
var DataProcessor = ref.types.void;
var FnVoid_DataProcessorP = ffi.Function(ref.types.void, [ref.refType(DataProcessor)]);
var Event = ref.types.void;
var FnVoid_EventP_Int = ffi.Function(ref.types.void, [ref.refType(Event), ref.types.int32]);
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

var GpioAnalogReadParameters = Struct({
  'pullup_pin': ref.types.uint8,
  'pulldown_pin': ref.types.uint8,
  'virtual_pin': ref.types.uint8,
  'delay_us': ref.types.uint16
});

var FnVoid_UInt_UInt = ffi.Function(ref.types.void, [ref.types.uint32, ref.types.uint32]);
var FnVoid_UByte_Long_UByteP_UByte = ffi.Function(ref.types.void, [ref.types.uint8, ref.types.int64, ref.refType(ref.types.uint8), ref.types.uint8]);
var LogDownloadHandler = Struct({
  'received_progress_update': FnVoid_UInt_UInt,
  'received_unknown_entry': FnVoid_UByte_Long_UByteP_UByte,
  'received_unhandled_entry': FnVoid_DataP
});

var CartesianFloat = Struct({
  'x': ref.types.float,
  'y': ref.types.float,
  'z': ref.types.float
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

var CorrectedCartesianFloat = Struct({
  'x': ref.types.float,
  'y': ref.types.float,
  'z': ref.types.float,
  'accuracy': ref.types.uint8
});

var BatteryState = Struct({
  'voltage': ref.types.uint16,
  'charge': ref.types.uint8
});

var Tcs34725ColorAdc = Struct({
  'clear': ref.types.uint16,
  'red': ref.types.uint16,
  'green': ref.types.uint16,
  'blue': ref.types.uint16
});

var DataSignal = ref.types.void;
var EulerAngles = Struct({
  'heading': ref.types.float,
  'pitch': ref.types.float,
  'roll': ref.types.float,
  'yaw': ref.types.float
});

var GattChar = Struct({
  'service_uuid_high': ref.types.uint64,
  'service_uuid_low': ref.types.uint64,
  'uuid_high': ref.types.uint64,
  'uuid_low': ref.types.uint64
});

var FnVoid_VoidP_GattCharWriteType_GattCharP_UByteP_UByte = ffi.Function(ref.types.void, [ref.refType(ref.types.void), GattCharWriteType, ref.refType(GattChar), ref.refType(ref.types.uint8), ref.types.uint8]);
var FnVoid_VoidP_GattCharP_FnIntVoidPtrArray = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(GattChar), FnInt_VoidP_UByteP_UByte]);
var FnVoid_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(GattChar), FnInt_VoidP_UByteP_UByte, FnVoid_VoidP_Int]);
var BtleConnection = Struct({
  'write_gatt_char': FnVoid_VoidP_GattCharWriteType_GattCharP_UByteP_UByte,
  'read_gatt_char': FnVoid_VoidP_GattCharP_FnIntVoidPtrArray,
  'enable_notifications': FnVoid_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt
});

var Quaternion = Struct({
  'w': ref.types.float,
  'x': ref.types.float,
  'y': ref.types.float,
  'z': ref.types.float
});

var FnVoid = ffi.Function(ref.types.void, []);
var FnVoid_Int = ffi.Function(ref.types.void, [ref.types.int32]);
var FnVoid_charP = ffi.Function(ref.types.void, [ref.types.CString]);
var DfuDelegate = Struct({
  'on_dfu_started': FnVoid,
  'on_dfu_cancelled': FnVoid,
  'on_transfer_percentage': FnVoid_Int,
  'on_successful_file_transferred': FnVoid,
  'on_error': FnVoid_charP
});

function Const() {
}
Const.MAG_BFIELD_Z_AXIS_INDEX = 2
Const.GYRO_ROTATION_X_AXIS_INDEX = 0
Const.MODULE_BARO_TYPE_BMP280 = 0
Const.MODULE_ACC_TYPE_BMA255 = 3
Const.MODULE_ACC_TYPE_MMA8452Q = 0
Const.ACC_ACCEL_Y_AXIS_INDEX = 1
Const.SENSOR_FUSION_CALIBRATION_ACCURACY_HIGH = 3
Const.SENSOR_FUSION_CALIBRATION_ACCURACY_LOW = 1
Const.GYRO_ROTATION_Z_AXIS_INDEX = 2
Const.MODULE_ACC_TYPE_BMI160 = 1
Const.SENSOR_FUSION_CALIBRATION_ACCURACY_UNRELIABLE = 0
Const.ACC_ACCEL_Z_AXIS_INDEX = 2
Const.GPIO_UNUSED_PIN = 255
Const.MODULE_TYPE_NA = -1
Const.ACC_ACCEL_X_AXIS_INDEX = 0
Const.LED_REPEAT_INDEFINITELY = 255
Const.MAG_BFIELD_Y_AXIS_INDEX = 1
Const.STATUS_WARNING_INVALID_PROCESSOR_TYPE = 2
Const.STATUS_OK = 0
Const.SETTINGS_BATTERY_VOLTAGE_INDEX = 0
Const.SENSOR_FUSION_CALIBRATION_ACCURACY_MEDIUM = 2
Const.STATUS_ERROR_UNSUPPORTED_PROCESSOR = 4
Const.STATUS_WARNING_UNEXPECTED_SENSOR_DATA = 1
Const.STATUS_ERROR_TIMEOUT = 16
Const.SETTINGS_BATTERY_CHARGE_INDEX = 1
Const.MODULE_BARO_TYPE_BME280 = 1
Const.STATUS_WARNING_INVALID_RESPONSE = 8
Const.STATUS_ERROR_SERIALIZATION_FORMAT = 32
Const.STATUS_ERROR_ENABLE_NOTIFY = 64
Const.GYRO_ROTATION_Y_AXIS_INDEX = 1
Const.CD_TCS34725_ADC_CLEAR_INDEX = 0
Const.CD_TCS34725_ADC_RED_INDEX = 1
Const.CD_TCS34725_ADC_GREEN_INDEX = 2
Const.MAG_BFIELD_X_AXIS_INDEX = 0
Const.CD_TCS34725_ADC_BLUE_INDEX = 3

var Lib = ffi.Library(LIBMETAWEAR_PATH, {
  'mbl_mw_spi_write': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(SpiParameters)]],

  'mbl_mw_proximity_tsl2671_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_proximity_tsl2671_get_adc_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_multi_chnl_temp_get_num_channels': [ref.types.uint8, [ref.refType(MetaWearBoard)]],

  'mbl_mw_multi_chnl_temp_get_source': [TemperatureSource, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_multi_chnl_temp_configure_ext_thermistor': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint8, ref.types.uint8]],

  'mbl_mw_multi_chnl_temp_get_temperature_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_mag_bmm150_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_mag_bmm150_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_mag_bmm150_enable_b_field_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_mag_bmm150_get_packed_b_field_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_mag_bmm150_get_b_field_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_gyro_bmi160_enable_rotation_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_gyro_bmi160_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_gyro_bmi160_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_proximity_tsl2671_set_transmitter_current': [ref.types.void, [ref.refType(MetaWearBoard), ProximityTsl2671Current]],

  'mbl_mw_gyro_bmi160_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_gyro_bmi160_set_odr': [ref.types.void, [ref.refType(MetaWearBoard), GyroBmi160Odr]],

  'mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_baro_bosch_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_baro_bosch_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_baro_bosch_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_baro_bosch_get_pressure_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_als_ltr329_set_measurement_rate': [ref.types.void, [ref.refType(MetaWearBoard), AlsLtr329MeasurementRate]],

  'mbl_mw_als_ltr329_set_gain': [ref.types.void, [ref.refType(MetaWearBoard), AlsLtr329Gain]],

  'mbl_mw_mag_bmm150_set_preset': [ref.types.void, [ref.refType(MetaWearBoard), MagBmm150Preset]],

  'mbl_mw_als_ltr329_get_illuminance_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_mma8452q_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_mma8452q_write_acceleration_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_mma8452q_set_range': [ref.types.void, [ref.refType(MetaWearBoard), AccMma8452qRange]],

  'mbl_mw_acc_mma8452q_get_packed_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bosch_disable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bosch_enable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_spi_get_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8]],

  'mbl_mw_acc_bosch_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bosch_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bmi160_disable_step_detector': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bmi160_enable_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bmi160_set_step_counter_mode': [ref.types.void, [ref.refType(MetaWearBoard), AccBmi160StepCounterMode]],

  'mbl_mw_acc_bmi160_write_step_counter_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bmi160_set_odr': [ref.types.void, [ref.refType(MetaWearBoard), AccBmi160Odr]],

  'mbl_mw_mag_bmm150_configure': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16, ref.types.uint16, MagBmm150Odr]],

  'mbl_mw_acc_bmi160_get_step_detector_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bmi160_get_step_counter_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bosch_get_packed_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bosch_get_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_enable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_baro_bosch_get_altitude_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_gyro_bmi160_disable_rotation_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_baro_bmp280_set_standby_time': [ref.types.void, [ref.refType(MetaWearBoard), BaroBmp280StandbyTime]],

  'mbl_mw_acc_write_acceleration_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_disable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_ibeacon_set_rx_power': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.int8]],

  'mbl_mw_ibeacon_set_tx_power': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.int8]],

  'mbl_mw_neopixel_stop_rotation': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_neopixel_set_color': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint8, ref.types.uint8, ref.types.uint8]],

  'mbl_mw_neopixel_init_fast_strand': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint8, NeoPixelColorOrdering]],

  'mbl_mw_timer_remove': [ref.types.void, [ref.refType(Timer)]],

  'mbl_mw_als_ltr329_set_integration_time': [ref.types.void, [ref.refType(MetaWearBoard), AlsLtr329IntegrationTime]],

  'mbl_mw_haptic_start_buzzer': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

  'mbl_mw_proximity_tsl2671_set_receiver_channel': [ref.types.void, [ref.refType(MetaWearBoard), ProximityTsl2671Channel]],

  'mbl_mw_metawearboard_perform_dfu': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(DfuDelegate), ref.types.CString]],

  'mbl_mw_led_stop_and_clear': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_metawearboard_deserialize': [ref.types.int32, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint32]],

  'mbl_mw_neopixel_clear': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint8]],

  'mbl_mw_metawearboard_get_model_name': [ref.types.CString, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_multi_comparator_create_signed': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ComparatorMode, ref.refType(ref.types.float), ref.types.uint8, FnVoid_DataProcessorP]],

  'mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_metawearboard_free': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_metawearboard_lookup_module': [ref.types.int32, [ref.refType(MetaWearBoard), Module]],

  'mbl_mw_sensor_fusion_set_mode': [ref.types.void, [ref.refType(MetaWearBoard), SensorFusionMode]],

  'mbl_mw_cd_tcs34725_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_led_write_pattern': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(LedPattern), LedColor]],

  'mbl_mw_metawearboard_notify_char_changed': [ref.types.int32, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint8]],

  'mbl_mw_dataprocessor_get_state_data_signal': [ref.refType(DataSignal), [ref.refType(DataProcessor)]],

  'mbl_mw_connection_notify_char_changed': [ref.types.int32, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint8]],

  'mbl_mw_metawearboard_char_read': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(GattChar), ref.refType(ref.types.uint8), ref.types.uint8]],

  'mbl_mw_macro_erase_all': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_remove': [ref.types.void, [ref.refType(DataProcessor)]],

  'mbl_mw_macro_execute': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_macro_end_record': [ref.types.void, [ref.refType(MetaWearBoard), FnVoid_MetaWearBoardP_Int]],

  'mbl_mw_timer_stop': [ref.types.void, [ref.refType(Timer)]],

  'mbl_mw_debug_disconnect': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_metawearboard_initialize': [ref.types.void, [ref.refType(MetaWearBoard), FnVoid_MetaWearBoardP_Int]],

  'mbl_mw_timer_start': [ref.types.void, [ref.refType(Timer)]],

  'mbl_mw_neopixel_free_strand': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_metawearboard_create': [ref.refType(MetaWearBoard), [ref.refType(BtleConnection)]],

  'mbl_mw_ibeacon_set_minor': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

  'mbl_mw_datasignal_read_with_parameters': [ref.types.void, [ref.refType(DataSignal), ref.refType(ref.types.void)]],

  'mbl_mw_debug_jump_to_bootloader': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bmi160_disable_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_set_odr': [ref.types.float, [ref.refType(MetaWearBoard), ref.types.float]],

  'mbl_mw_connection_char_read': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(GattChar), ref.refType(ref.types.uint8), ref.types.uint8]],

  'mbl_mw_logging_start': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_haptic_start_motor': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float, ref.types.uint16]],

  'mbl_mw_macro_record': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_baro_bme280_set_standby_time': [ref.types.void, [ref.refType(MetaWearBoard), BaroBme280StandbyTime]],

  'mbl_mw_datasignal_subscribe': [ref.types.void, [ref.refType(DataSignal), FnVoid_DataP]],

  'mbl_mw_acc_set_range': [ref.types.float, [ref.refType(MetaWearBoard), ref.types.float]],

  'mbl_mw_event_end_record': [ref.types.void, [ref.refType(Event), FnVoid_EventP_Int]],

  'mbl_mw_settings_set_device_name': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint8]],

  'mbl_mw_settings_get_disconnect_event': [ref.refType(Event), [ref.refType(MetaWearBoard)]],

  'mbl_mw_ibeacon_set_minor_signal': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(DataSignal)]],

  'mbl_mw_memory_free': [ref.types.void, [ref.refType(ref.types.void)]],

  'mbl_mw_neopixel_enable_hold': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_logging_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_event_record_commands': [ref.types.void, [ref.refType(Event)]],

  'mbl_mw_timer_get_id': [ref.types.uint8, [ref.refType(Timer)]],

  'mbl_mw_logger_remove': [ref.types.void, [ref.refType(DataLogger)]],

  'mbl_mw_timer_lookup_id': [ref.refType(Timer), [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_timer_create': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint32, ref.types.uint16, ref.types.uint8, FnVoid_TimerP]],

  'mbl_mw_logging_clear_entries': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_debug_reset': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_logging_download': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.refType(LogDownloadHandler)]],

  'mbl_mw_als_ltr329_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_sensor_fusion_enable_data': [ref.types.void, [ref.refType(MetaWearBoard), SensorFusionData]],

  'mbl_mw_logger_get_id': [ref.types.uint8, [ref.refType(DataLogger)]],

  'mbl_mw_als_ltr329_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_logger_get_signal': [ref.refType(DataSignal), [ref.refType(DataLogger)]],

  'mbl_mw_dataprocessor_math_create_signed': [ref.types.int32, [ref.refType(DataSignal), MathOperation, ref.types.float, FnVoid_DataProcessorP]],

  'mbl_mw_neopixel_init_slow_strand': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint8, NeoPixelColorOrdering]],

  'mbl_mw_dataprocessor_passthrough_modify': [ref.types.int32, [ref.refType(DataProcessor), PassthroughMode, ref.types.uint16]],

  'mbl_mw_logger_lookup_id': [ref.refType(DataLogger), [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_settings_get_battery_state_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_settings_set_ad_interval': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16, ref.types.uint8]],

  'mbl_mw_ibeacon_enable': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_delta_set_reference': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float]],

  'mbl_mw_settings_set_tx_power': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.int8]],

  'mbl_mw_gyro_bmi160_get_rotation_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_sample_create': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, FnVoid_DataProcessorP]],

  'mbl_mw_baro_bosch_get_pressure_read_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_sample_modify_bin_size': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint8]],

  'mbl_mw_ibeacon_set_major_signal': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(DataSignal)]],

  'mbl_mw_dataprocessor_time_create': [ref.types.int32, [ref.refType(DataSignal), TimeMode, ref.types.uint32, FnVoid_DataProcessorP]],

  'mbl_mw_settings_set_scan_response': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8), ref.types.uint8]],

  'mbl_mw_settings_start_advertising': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_ibeacon_set_major': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

  'mbl_mw_datasignal_unsubscribe': [ref.types.void, [ref.refType(DataSignal)]],

  'mbl_mw_dataprocessor_counter_set_state': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint32]],

  'mbl_mw_datasignal_get_component': [ref.refType(DataSignal), [ref.refType(DataSignal), ref.types.uint8]],

  'mbl_mw_acc_bmi160_enable_step_detector': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_gpio_set_digital_output': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_metawearboard_is_initialized': [ref.types.int32, [ref.refType(MetaWearBoard)]],

  'mbl_mw_metawearboard_tear_down': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bmi160_reset_step_counter': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_threshold_create': [ref.types.int32, [ref.refType(DataSignal), ThresholdMode, ref.types.float, ref.types.float, FnVoid_DataProcessorP]],

  'mbl_mw_timer_create_indefinite': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint32, ref.types.uint8, FnVoid_TimerP]],

  'mbl_mw_gpio_get_digital_input_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_ibeacon_set_uuid': [ref.types.void, [ref.refType(MetaWearBoard), ref.refType(ref.types.uint8)]],

  'mbl_mw_acc_mma8452q_get_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_datasignal_is_readable': [ref.types.int32, [ref.refType(DataSignal)]],

  'mbl_mw_dataprocessor_math_modify_rhs': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float]],

  'mbl_mw_led_pause': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_passthrough_create': [ref.types.int32, [ref.refType(DataSignal), PassthroughMode, ref.types.uint16, FnVoid_DataProcessorP]],

  'mbl_mw_dataprocessor_pulse_modify': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float, ref.types.uint16]],

  'mbl_mw_dataprocessor_counter_create': [ref.types.int32, [ref.refType(Event), FnVoid_DataProcessorP]],

  'mbl_mw_gyro_bmi160_get_packed_rotation_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_logger_subscribe': [ref.types.void, [ref.refType(DataLogger), FnVoid_DataP]],

  'mbl_mw_sensor_fusion_set_acc_range': [ref.types.void, [ref.refType(MetaWearBoard), SensorFusionAccRange]],

  'mbl_mw_led_load_preset_pattern': [ref.types.void, [ref.refType(LedPattern), LedPreset]],

  'mbl_mw_led_autoplay': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_neopixel_rotate': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.types.uint16, NeoPixelRotDirection]],

  'mbl_mw_led_play': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_ibeacon_disable': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_average_modify_size': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint8]],

  'mbl_mw_acc_bma255_set_odr': [ref.types.void, [ref.refType(MetaWearBoard), AccBma255Odr]],

  'mbl_mw_led_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_average_create': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, FnVoid_DataProcessorP]],

  'mbl_mw_dataprocessor_average_reset': [ref.types.int32, [ref.refType(DataProcessor)]],

  'mbl_mw_dataprocessor_buffer_create': [ref.types.int32, [ref.refType(DataSignal), FnVoid_DataProcessorP]],

  'mbl_mw_dataprocessor_delta_modify_magnitude': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float]],

  'mbl_mw_i2c_get_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8]],

  'mbl_mw_dataprocessor_delta_create': [ref.types.int32, [ref.refType(DataSignal), DeltaMode, ref.types.float, FnVoid_DataProcessorP]],

  'mbl_mw_dataprocessor_pulse_create': [ref.types.int32, [ref.refType(DataSignal), PulseOutput, ref.types.float, ref.types.uint16, FnVoid_DataProcessorP]],

  'mbl_mw_metawearboard_serialize': [ref.refType(ref.types.uint8), [ref.refType(MetaWearBoard), ref.refType(ref.types.uint32)]],

  'mbl_mw_i2c_write': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, ref.refType(ref.types.uint8), ref.types.uint8]],

  'mbl_mw_dataprocessor_rms_create': [ref.types.int32, [ref.refType(DataSignal), FnVoid_DataProcessorP]],

  'mbl_mw_acc_get_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_gpio_get_pin_monitor_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_dataprocessor_rss_create': [ref.types.int32, [ref.refType(DataSignal), FnVoid_DataProcessorP]],

  'mbl_mw_metawearboard_set_time_for_response': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

  'mbl_mw_debug_reset_after_gc': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_threshold_modify_boundary': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float, ref.types.float]],

  'mbl_mw_acc_bosch_set_range': [ref.types.void, [ref.refType(MetaWearBoard), AccBoschRange]],

  'mbl_mw_dataprocessor_time_modify_period': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint32]],

  'mbl_mw_dataprocessor_accumulator_create': [ref.types.int32, [ref.refType(DataSignal), FnVoid_DataProcessorP]],

  'mbl_mw_sensor_fusion_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_metawearboard_get_model': [Model, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_accumulator_create_size': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, FnVoid_DataProcessorP]],

  'mbl_mw_als_ltr329_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_set_accumulator_state': [ref.types.int32, [ref.refType(DataProcessor), ref.types.float]],

  'mbl_mw_gyro_bmi160_set_range': [ref.types.void, [ref.refType(MetaWearBoard), GyroBmi160Range]],

  'mbl_mw_dataprocessor_comparator_create': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ref.types.float, FnVoid_DataProcessorP]],

  'mbl_mw_gpio_get_analog_input_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), ref.types.uint8, GpioAnalogReadMode]],

  'mbl_mw_dataprocessor_comparator_create_signed': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ref.types.float, FnVoid_DataProcessorP]],

  'mbl_mw_proximity_tsl2671_set_integration_time': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float]],

  'mbl_mw_acc_mma8452q_enable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_dataprocessor_comparator_create_unsigned': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ref.types.float, FnVoid_DataProcessorP]],

  'mbl_mw_dataprocessor_comparator_modify': [ref.types.int32, [ref.refType(DataProcessor), ComparatorOperation, ref.types.float]],

  'mbl_mw_dataprocessor_comparator_modify_signal': [ref.types.int32, [ref.refType(DataProcessor), ComparatorOperation, ref.refType(DataSignal)]],

  'mbl_mw_dataprocessor_multi_comparator_create': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ComparatorMode, ref.refType(ref.types.float), ref.types.uint8, FnVoid_DataProcessorP]],

  'mbl_mw_gpio_set_pull_mode': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, GpioPullMode]],

  'mbl_mw_baro_bosch_set_iir_filter': [ref.types.void, [ref.refType(MetaWearBoard), BaroBoschIirFilter]],

  'mbl_mw_dataprocessor_multi_comparator_create_unsigned': [ref.types.int32, [ref.refType(DataSignal), ComparatorOperation, ComparatorMode, ref.refType(ref.types.float), ref.types.uint8, FnVoid_DataProcessorP]],

  'mbl_mw_dataprocessor_math_create': [ref.types.int32, [ref.refType(DataSignal), MathOperation, ref.types.float, FnVoid_DataProcessorP]],

  'mbl_mw_dataprocessor_multi_comparator_modify': [ref.types.int32, [ref.refType(DataProcessor), ComparatorOperation, ref.refType(ref.types.float), ref.types.uint8]],

  'mbl_mw_settings_set_connection_parameters': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float, ref.types.float, ref.types.uint16, ref.types.uint16]],

  'mbl_mw_dataprocessor_counter_create_size': [ref.types.int32, [ref.refType(Event), ref.types.uint8, FnVoid_DataProcessorP]],

  'mbl_mw_dataprocessor_get_id': [ref.types.uint8, [ref.refType(DataProcessor)]],

  'mbl_mw_dataprocessor_lookup_id': [ref.refType(DataProcessor), [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_dataprocessor_math_create_unsigned': [ref.types.int32, [ref.refType(DataSignal), MathOperation, ref.types.float, FnVoid_DataProcessorP]],

  'mbl_mw_datasignal_log': [ref.types.void, [ref.refType(DataSignal), FnVoid_DataLoggerP]],

  'mbl_mw_dataprocessor_math_modify_rhs_signal': [ref.types.int32, [ref.refType(DataProcessor), ref.refType(DataSignal)]],

  'mbl_mw_dataprocessor_passthrough_set_count': [ref.types.int32, [ref.refType(DataProcessor), ref.types.uint16]],

  'mbl_mw_dataprocessor_accounter_create': [ref.types.int32, [ref.refType(DataSignal), FnVoid_DataProcessorP]],

  'mbl_mw_ibeacon_set_period': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint16]],

  'mbl_mw_dataprocessor_packer_create': [ref.types.int32, [ref.refType(DataSignal), ref.types.uint8, FnVoid_DataProcessorP]],

  'mbl_mw_proximity_tsl2671_set_n_pulses': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_cd_tcs34725_enable_illuminator_led': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_switch_get_state_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_bosch_write_acceleration_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_sensor_fusion_write_config': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_event_get_owner': [ref.refType(MetaWearBoard), [ref.refType(Event)]],

  'mbl_mw_cd_tcs34725_get_adc_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_sensor_fusion_set_gyro_range': [ref.types.void, [ref.refType(MetaWearBoard), SensorFusionGyroRange]],

  'mbl_mw_acc_mma8452q_disable_acceleration_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_mma8452q_stop': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_cd_tcs34725_set_gain': [ref.types.void, [ref.refType(MetaWearBoard), ColorDetectorTcs34725Gain]],

  'mbl_mw_baro_bosch_set_standby_time': [ref.types.float, [ref.refType(MetaWearBoard), ref.types.float]],

  'mbl_mw_gpio_clear_digital_output': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_cd_tcs34725_set_integration_time': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.float]],

  'mbl_mw_gpio_set_pin_change_type': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, GpioPinChangeType]],

  'mbl_mw_baro_bosch_set_oversampling': [ref.types.void, [ref.refType(MetaWearBoard), BaroBoschOversampling]],

  'mbl_mw_neopixel_disable_hold': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_gpio_start_pin_monitoring': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_gpio_stop_pin_monitoring': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8]],

  'mbl_mw_mag_bmm150_disable_b_field_sampling': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_cd_tcs34725_disable_illuminator_led': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_humidity_bme280_get_percentage_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_sensor_fusion_get_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard), SensorFusionData]],

  'mbl_mw_sensor_fusion_clear_enabled_mask': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_neopixel_rotate_indefinitely': [ref.types.void, [ref.refType(MetaWearBoard), ref.types.uint8, ref.types.uint8, NeoPixelRotDirection]],

  'mbl_mw_sensor_fusion_start': [ref.types.void, [ref.refType(MetaWearBoard)]],

  'mbl_mw_acc_mma8452q_set_odr': [ref.types.void, [ref.refType(MetaWearBoard), AccMma8452qOdr]],

  'mbl_mw_humidity_bme280_set_oversampling': [ref.types.void, [ref.refType(MetaWearBoard), HumidityBme280Oversampling]],

  'mbl_mw_acc_get_high_freq_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]],

  'mbl_mw_datasignal_read': [ref.types.void, [ref.refType(DataSignal)]],

  'mbl_mw_acc_get_packed_acceleration_data_signal': [ref.refType(DataSignal), [ref.refType(MetaWearBoard)]]
});

module.exports = {
  Lib,
  FnVoid,
  Quaternion,
  BtleConnection,
  FnVoid_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt,
  FnVoid_TimerP,
  DeltaMode,
  AccBmi160Odr,
  GattChar,
  AlsLtr329Gain,
  DataTypeId,
  ColorDetectorTcs34725Gain,
  FnVoid_charP,
  GattCharWriteType,
  NeoPixelColorOrdering,
  AccBmi160StepCounterMode,
  FnVoid_UByte_Long_UByteP_UByte,
  FnVoid_DataProcessorP,
  SensorFusionAccRange,
  NeoPixelRotDirection,
  ComparatorMode,
  MetaWearRProChannel,
  AccMma8452qRange,
  MagBmm150Odr,
  FnVoid_UInt_UInt,
  SensorFusionData,
  TemperatureSource,
  GpioPinChangeType,
  MetaWearRChannel,
  MathOperation,
  GpioAnalogReadMode,
  Const,
  SpiFrequency,
  GyroBmi160Range,
  FnVoid_MetaWearBoardP_Int,
  BaroBme280StandbyTime,
  FnVoid_VoidP_GattCharP_FnIntVoidPtrArray,
  ProximityTsl2671Channel,
  SpiParameters,
  FnVoid_DataLoggerP,
  Model,
  DfuDelegate,
  SpiMode,
  MagBmm150Preset,
  ThresholdMode,
  AccBma255Odr,
  BaroBmp280StandbyTime,
  Tcs34725ColorAdc,
  PassthroughMode,
  HumidityBme280Oversampling,
  ComparatorOperation,
  ProximityTsl2671Current,
  Module,
  SensorFusionMode,
  DataProcessor,
  GyroBmi160Odr,
  LedPreset,
  LogDownloadHandler,
  SensorFusionGyroRange,
  CartesianFloat,
  AccBoschRange,
  AccMma8452qOdr,
  FnVoid_VoidP_GattCharWriteType_GattCharP_UByteP_UByte,
  GpioPullMode,
  PulseOutput,
  DataLogger,
  Event,
  DataSignal,
  BaroBoschOversampling,
  AlsLtr329IntegrationTime,
  Data,
  TimeMode,
  LedColor,
  I2cReadParameters,
  BaroBoschIirFilter,
  FnVoid_VoidP_Int,
  FnInt_VoidP_UByteP_UByte,
  MetaWearBoard,
  Timer,
  AlsLtr329MeasurementRate,
  FnVoid_DataP,
  FnVoid_EventP_Int,
  GpioAnalogReadParameters,
  FnVoid_Int,
  LedPattern,
  CorrectedCartesianFloat,
  BatteryState,
  EulerAngles
};
