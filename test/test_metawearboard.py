from common import TestMetaWearBase, to_string_buffer, to_ubyte_pointer
from ctypes import *
from logdata import *
from mbientlab.metawear.cbindings import *
from test_dataprocessor import TestGpioFeedbackSetup
from test_logging import TestAccelerometerLoggingBase, TestGyroYAxisLoggingBase
import serializedstate
import threading
import unittest

class TestMetaWearBoard(TestMetaWearBase):
    def test_no_response_handler(self):
        tests= [
            {
                'response': create_string_buffer(b'\x03\x04\x16\xc4\x94\xa2\x2a\xd0', 8),
                'status': Const.STATUS_WARNING_UNEXPECTED_SENSOR_DATA,
                'name': 'acc'
            },
            {
                'response': create_string_buffer(b'\x14\x03\xed\x92\xb0\x00', 6),
                'status': Const.STATUS_WARNING_INVALID_RESPONSE,
                'name': 'ambient_light'
            },
            {
                'response': create_string_buffer(b'\x12\x01\xd3\x35\x8b\x01', 6),
                'status': Const.STATUS_WARNING_INVALID_RESPONSE,
                'name': 'baro_pressure'
            },
            {
                'response': create_string_buffer(b'\x12\x02\x1e\x1f\xfe\xff', 6),
                'status': Const.STATUS_WARNING_INVALID_RESPONSE,
                'name': 'baro_altitude'
            },
            {
                'response': create_string_buffer(b'\x13\x05\x3e\x43\xff\x7f\x00\x80', 8),
                'status': Const.STATUS_WARNING_INVALID_RESPONSE,
                'name': 'gyro'
            },
            {
                'response': create_string_buffer(b'\x01\x01\x01', 3),
                'status': Const.STATUS_WARNING_UNEXPECTED_SENSOR_DATA,
                'name': 'switch'
            }
        ]

        for test in tests:
            with self.subTest(response=test['name']):
                status = self.notify_mw_char(test['response'])
                self.assertEqual(status, test['status'])

    def test_service_discovery(self):
        self.assertEqual(self.init_status, Const.STATUS_OK)

    def test_module_info(self):
        expected = [
            ModuleInfo(name = b"Switch", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"Led", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"Accelerometer", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 1),
            ModuleInfo(name = b"Temperature", extra = to_ubyte_pointer([0x00, 0x01]), extra_len = 2, present = 1, implementation = 1, revision = 0),
            ModuleInfo(name = b"Gpio", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"NeoPixel", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"IBeacon", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"Haptic", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"DataProcessor", extra = to_ubyte_pointer([0x1c]), extra_len = 1, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"Event", extra = to_ubyte_pointer([0x1c]), extra_len = 1, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"Logging", extra = to_ubyte_pointer([0x08, 0x80, 0x31, 0x00, 0x00]), extra_len = 5, present = 1, implementation = 0, revision = 2),
            ModuleInfo(name = b"Timer", extra = to_ubyte_pointer([0x08]), extra_len = 1, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"SerialPassthrough", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"Macro", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"Conductance", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"Settings", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 0),
            ModuleInfo(name = b"Barometer", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"Gyro", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"AmbientLight", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"Magnetometer", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"Humidity", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"Color", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"Proximity", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"SensorFusion", extra = to_ubyte_pointer([]), extra_len = 0, present = 0, implementation = 0xff, revision = 0xff),
            ModuleInfo(name = b"Debug", extra = to_ubyte_pointer([]), extra_len = 0, present = 1, implementation = 0, revision = 2)
        ]

        size= c_uint(0)
        temp = self.libmetawear.mbl_mw_metawearboard_get_module_info(self.board, byref(size))
        actual = [temp[i] for i in range(size.value)]

        self.maxDiff = None
        self.assertEqual(actual, expected)

class TestDeviceInfo(TestMetaWearBase):
    def setUp(self):
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def test_read(self):
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        expected = DeviceInformation(manufacturer = b"deadbeef", model_number = b"0", serial_number = b"cafebabe", firmware_revision = b"1.1.3", hardware_revision = b"0.1")
        actual = self.libmetawear.mbl_mw_metawearboard_get_device_information(self.board)

        self.assertEqual(actual.contents, expected)

    def test_read_after_deserialize(self):
        state_buffer= to_string_buffer(TestMetaWearBoardSerialize.motion_r_state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))

        self.firmware_revision= create_string_buffer(b'1.3.1', 5)
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        expected = DeviceInformation(manufacturer = b"deadbeef", model_number = b"5", serial_number = b"cafebabe", firmware_revision = b"1.3.1", hardware_revision = b"0.1")
        actual = self.libmetawear.mbl_mw_metawearboard_get_device_information(self.board)

        self.assertEqual(actual.contents, expected)

class TestMetaWearBoardInitialize(TestMetaWearBase):
    def test_reinitialize(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80],
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80], [0x19, 0x80],
            [0xfe, 0x80], 
            [0x0b, 0x84], [0x0b, 0x84]
        ]

        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.assertEqual(self.full_history, expected_cmds)

    def test_reinitialize_new_firmware(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80], 
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80], [0x19, 0x80],
            [0xfe, 0x80], [0x0b, 0x84],
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80], 
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80], [0x19, 0x80],
            [0xfe, 0x80], [0x0b, 0x84]
        ]

        self.firmware_revision= create_string_buffer(b'1.1.4', 5)
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.assertEqual(self.full_history, expected_cmds)

class TestMetaWearBoardInitError(TestMetaWearBase):
    def setUp(self):
        self.module_limit = 0x3
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def initialized(self, context, board, status):
        super().initialized(context, board, status)

        self.e.set()

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        if (command[1] != 0x80 or command[1] == 0x80 and command[0] <= self.module_limit):
            TestMetaWearBase.commandLogger(self, context, board, writeType, characteristic, command, length)

    def test_timeout(self):
        self.e= threading.Event()
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.e.wait()

        self.assertEqual(self.init_status, Const.STATUS_ERROR_TIMEOUT)
    
    def test_resume(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80],
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80], [0x19, 0x80],
            [0xfe, 0x80], 
            [0x0b, 0x84]
        ]

        self.e= threading.Event()
        # this attempt will fail
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.e.wait()
        self.assertEqual(self.init_status, Const.STATUS_ERROR_TIMEOUT)

        self.e.clear()
        self.module_limit = 0xff
        # this attempt resumes where previous init failed
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.e.wait()

        self.assertEqual(self.full_history, expected_cmds)

class TestTearDown(TestGpioFeedbackSetup):
    def test_dataproc_event(self):
        expected_cmds= [
            [0x09, 0x08],
            [0x0a, 0x05],
            [0x0b, 0x0a]
        ]

        self.libmetawear.mbl_mw_metawearboard_tear_down(self.board)
        tear_down_cmds= self.command_history[22:].copy()

        self.assertEqual(tear_down_cmds, expected_cmds)

class TestTimerTearDown(TestMetaWearBase):
    def timer_created(self, context, timer_signal):
        self.timerSignals.append(timer_signal)

    def timer_created_last(self, context, timer_signal):
        self.timerSignals.append(timer_signal)
        self.events["timer"].set()

    def test_timer(self):
        expected_cmds= [
            [0x0c, 0x05, 0x00],
            [0x0c, 0x05, 0x01],
            [0x0c, 0x05, 0x02],
            [0x0c, 0x05, 0x03],
            [0x09, 0x08],
            [0x0a, 0x05],
            [0x0b, 0x0a]
        ]

        last_timer_handler = FnVoid_VoidP_VoidP(self.timer_created_last)
        self.libmetawear.mbl_mw_timer_create(self.board, 667408, -1, 0, None, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create(self.board, 1000, -1, 0, None, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create(self.board, 1000, 10, 0, None, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 1000, 0, None, last_timer_handler)
        self.events["timer"].wait()

        self.libmetawear.mbl_mw_metawearboard_tear_down(self.board)
        self.assertEqual(self.command_history[4:], expected_cmds)

class TestMetaWearBoardSerialize(TestMetaWearBase):
    motion_r_state = [
        2, 
        1, 3, 1, 
        1, 53, 
        25, 
        1, 0, 0, 0, 
        2, 0, 0, 0, 
        3, 1, 1, 0, 
        4, 1, 0, 4, 0, 3, 1, 2, 
        5, 0, 1, 5, 3, 3, 3, 3, 1, 
        6, 0, 0, 0, 
        7, 0, 0, 0, 
        8, 0, 0, 0, 
        9, 0, 0, 1, 28, 
        10, 0, 0, 1, 28, 
        11, 0, 2, 5, 8, 128, 43, 0, 0, 
        12, 0, 0, 1, 8, 
        13, 0, 1, 0, 
        15, 0, 1, 1, 8, 
        16, 255, 255, 
        17, 0, 3, 0, 
        18, 0, 0, 0, 
        19, 0, 1, 0, 
        20, 0, 0, 0, 
        21, 0, 1, 0, 
        22, 255, 255, 
        23, 255, 255, 
        24, 255, 255, 
        25, 0, 0, 8, 3, 0, 6, 0, 2, 0, 1, 0, 
        254, 0, 2, 0, 
        45, 
        1, 1, 255, 0, 1, 0, 1, 1, 0, 0, 
        3, 4, 255, 0, 7, 2, 3, 2, 1, 0, 
        3, 4, 255, 0, 8, 2, 1, 2, 1, 0, 
        3, 4, 255, 0, 8, 2, 1, 2, 1, 2, 
        3, 4, 255, 0, 8, 2, 1, 2, 1, 4, 
        3, 11, 255, 0, 33, 0, 1, 1, 0, 0, 
        3, 14, 255, 0, 36, 0, 1, 1, 0, 0, 
        3, 17, 255, 0, 28, 0, 1, 1, 0, 0, 
        3, 25, 255, 0, 1, 0, 1, 1, 0, 0, 
        3, 28, 255, 0, 7, 2, 3, 2, 1, 0, 
        3, 218, 255, 0, 1, 0, 1, 2, 0, 0, 
        4, 193, 0, 0, 2, 6, 1, 2, 1, 0, 
        4, 193, 1, 0, 2, 6, 1, 2, 1, 0, 
        4, 193, 2, 0, 2, 6, 1, 2, 1, 0, 
        4, 193, 3, 0, 2, 6, 1, 2, 1, 0, 
        11, 196, 255, 0, 31, 0, 1, 5, 0, 0, 
        11, 197, 255, 0, 1, 0, 1, 4, 0, 0, 
        17, 10, 255, 0, 17, 203, 255, 0, 29, 0, 1, 6, 0, 0, 
        17, 204, 255, 0, 14, 0, 1, 3, 0, 0, 
        17, 204, 255, 0, 1, 0, 1, 2, 0, 1, 
        17, 204, 255, 0, 1, 0, 1, 1, 0, 0, 
        18, 1, 255, 0, 3, 3, 1, 4, 0, 0, 
        18, 2, 255, 0, 4, 3, 1, 4, 1, 0, 
        18, 193, 255, 0, 3, 3, 1, 4, 0, 0, 
        19, 5, 255, 0, 5, 4, 3, 2, 1, 0, 
        19, 5, 255, 0, 6, 4, 1, 2, 1, 0, 
        19, 5, 255, 0, 6, 4, 1, 2, 1, 2, 
        19, 5, 255, 0, 6, 4, 1, 2, 1, 4, 
        19, 7, 255, 0, 5, 4, 3, 2, 1, 0, 
        20, 3, 255, 0, 1, 0, 1, 4, 0, 0, 
        21, 5, 255, 0, 12, 8, 3, 2, 1, 0, 
        21, 5, 255, 0, 13, 8, 1, 2, 1, 0, 
        21, 5, 255, 0, 13, 8, 1, 2, 1, 2, 
        21, 5, 255, 0, 13, 8, 1, 2, 1, 4, 
        21, 9, 255, 0, 12, 8, 3, 2, 1, 0, 
        25, 4, 255, 0, 26, 0, 1, 13, 1, 0, 
        25, 5, 255, 0, 24, 0, 1, 13, 1, 0, 
        25, 6, 255, 0, 24, 0, 1, 13, 1, 0, 
        25, 7, 255, 0, 22, 0, 4, 4, 1, 0, 
        25, 8, 255, 0, 23, 0, 4, 4, 1, 0, 
        25, 9, 255, 0, 25, 0, 3, 4, 1, 0, 
        25, 10, 255, 0, 25, 0, 3, 4, 1, 0, 
        25, 203, 255, 0, 34, 0, 3, 1, 0, 0, 
        254, 196, 255, 0, 1, 0, 1, 4, 0, 0, 
        5, 
        3, 40, 3, 7, 48, 129, 11, 192, 0, 20, 20, 20, 64, 10, 24, 72, 8, 17, 0, 0, 
        18, 44, 0, 
        19, 40, 0, 
        20, 0, 3, 
        25, 0, 3, 0, 
        1, 
        59, 76, 90, 65, 99, 1, 0, 0, 5, 
        0
    ]

    def setUp(self):
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def test_serialize_motion_r(self):
        self.firmware_revision= create_string_buffer(b'1.3.1', 5)
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        self.python_array= []
        for i in range(0, state_array_size.value):
            self.python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.maxDiff= None
        self.assertEqual(self.python_array[0:568], TestMetaWearBoardSerialize.motion_r_state[0:568])

    def test_deserialize_motion_r(self):
        # just test that deserialization is successful
        state_buffer= to_string_buffer(TestMetaWearBoardSerialize.motion_r_state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))

        self.assertTrue(True)

    def test_deserialize_motion_r_new_fw(self):
        firmware_state = [0x03, 0x03, 0x01]

        self.firmware_revision= create_string_buffer(b'1.3.3', 5)
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        state_buffer= to_string_buffer(TestMetaWearBoardSerialize.motion_r_state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        python_array= []
        for i in range(0, state_array_size.value):
            python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.assertEqual(python_array[1:4], firmware_state)

    def test_serialize_readable_logger(self):
        logger_state= [
            0x1,
            0xff, 0x04, 0xc1, 0x00, 0x01, 0x00
        ]

        self.firmware_revision= create_string_buffer(b'1.2.5', 5)
        self.boardType = TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        readable_signal = self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 0)
        self.libmetawear.mbl_mw_datasignal_log(readable_signal, None, self.logger_created)
        self.events["log"].wait()

        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        python_array= []
        for i in range(0, state_array_size.value):
            python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.assertEqual(python_array[424:], logger_state)

    def test_deserialize_readable_logger(self):
        state = [
            1, 
            5, 2, 1, 
            1, 49, 
            25, 
            1, 0, 0, 0, 
            2, 0, 0, 0, 
            3, 1, 1, 0, 
            4, 1, 0, 4, 0, 3, 1, 2, 
            5, 0, 0, 0, 
            6, 0, 0, 0, 
            7, 0, 0, 0, 
            8, 0, 0, 0, 
            9, 0, 0, 1, 28, 
            10, 0, 0, 1, 28, 
            11, 0, 2, 5, 8, 128, 45, 0, 0, 
            12, 0, 0, 1, 8, 
            13, 0, 0, 0, 
            15, 0, 0, 0, 
            16, 255, 255, 
            17, 0, 0, 0, 
            18, 0, 0, 0, 
            19, 0, 1, 0, 
            20, 0, 0, 0, 
            21, 255, 255, 
            22, 255, 255, 
            23, 255, 255, 
            24, 255, 255, 
            25, 255, 255, 
            254, 0, 0, 0, 
            21, 
            1, 1, 255, 0, 1, 0, 1, 1, 0, 0, 
            3, 4, 255, 0, 7, 2, 3, 2, 1, 0, 
            3, 4, 255, 0, 8, 2, 1, 2, 1, 0, 
            3, 4, 255, 0, 8, 2, 1, 2, 1, 2, 
            3, 4, 255, 0, 8, 2, 1, 2, 1, 4, 
            3, 25, 255, 0, 1, 0, 1, 1, 0, 0, 
            3, 28, 255, 0, 7, 2, 3, 2, 1, 0, 
            3, 218, 255, 0, 1, 0, 1, 2, 0, 0, 
            4, 193, 0, 0, 2, 6, 1, 2, 1, 0, 
            4, 193, 1, 0, 2, 6, 1, 2, 1, 0, 
            4, 193, 2, 0, 2, 6, 1, 2, 1, 0, 
            4, 193, 3, 0, 2, 6, 1, 2, 1, 0, 
            18, 1, 255, 0, 3, 3, 1, 4, 0, 0, 
            18, 2, 255, 0, 4, 3, 1, 4, 1, 0, 
            18, 193, 255, 0, 3, 3, 1, 4, 0, 0, 
            19, 5, 255, 0, 5, 4, 3, 2, 1, 0, 
            19, 5, 255, 0, 6, 4, 1, 2, 1, 0, 
            19, 5, 255, 0, 6, 4, 1, 2, 1, 2, 
            19, 5, 255, 0, 6, 4, 1, 2, 1, 4, 
            19, 7, 255, 0, 5, 4, 3, 2, 1, 0, 
            20, 3, 255, 0, 1, 0, 1, 4, 0, 0, 
            4, 
            3, 40, 3, 7, 48, 129, 11, 192, 0, 20, 20, 20, 64, 10, 24, 72, 8, 17, 0, 0, 
            18, 44, 0, 
            19, 40, 0, 
            20, 0, 3, 
            1, 
            226, 118, 142, 47, 90, 1, 0, 0, 21, 4, 0, 0, 5, 
            1, 
            255, 4, 193, 0, 1, 0
        ]

        self.firmware_revision= create_string_buffer(b'1.2.5', 5)
        self.boardType = TestMetaWearBase.METAWEAR_RPRO_BOARD

        state_buffer= to_string_buffer(state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        logger = self.libmetawear.mbl_mw_logger_lookup_id(self.board, 0)
        self.libmetawear.mbl_mw_logger_subscribe(logger, None, self.sensor_data_handler)

        self.libmetawear.mbl_mw_logging_download(self.board, 0, cast(None, POINTER(LogDownloadHandler)))
        self.notify_mw_char(create_string_buffer(b'\x0b\x07\xa0\x37\x43\x00\x00\xc8\x00\x00\x00', 11))

        self.assertAlmostEqual(self.data_float.value, 25.0)

class TestMetaWearBoardTearDownSerialize(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.expected_state= [
            0x02, 
            0x03, 0x01, 0x01, 
            0x01, 0x31, 
            0x19, 
            0x01, 0xff, 0xff,
            0x02, 0xff, 0xff,
            0x03, 0x01, 0x00, 0x00, 
            0x04, 0xff, 0xff,
            0x05, 0xff, 0xff,
            0x06, 0xff, 0xff,
            0x07, 0xff, 0xff,
            0x08, 0xff, 0xff,
            0x09, 0xff, 0xff,
            0x0a, 0xff, 0xff,
            0x0b, 0xff, 0xff,
            0x0c, 0xff, 0xff,
            0x0d, 0xff, 0xff,
            0x0f, 0xff, 0xff,
            0x10, 0xff, 0xff,
            0x11, 0x00, 0x03, 0x00,
            0x12, 0xff, 0xff,
            0x13, 0xff, 0xff,
            0x14, 0xff, 0xff,
            0x15, 0xff, 0xff,
            0x16, 0xff, 0xff,
            0x17, 0xff, 0xff,
            0x18, 0xff, 0xff,
            0x19, 0xff, 0xff,
            0xfe, 0xff, 0xff,
            0x11, 
            0x03, 0x04, 0xff, 0x00, 0x07, 0x02, 0x03, 0x02, 0x01, 0x00,
            0x03, 0x04, 0xff, 0x00, 0x08, 0x02, 0x01, 0x02, 0x01, 0x00, 
            0x03, 0x04, 0xff, 0x00, 0x08, 0x02, 0x01, 0x02, 0x01, 0x02, 
            0x03, 0x04, 0xff, 0x00, 0x08, 0x02, 0x01, 0x02, 0x01, 0x04,
            0x03, 0x0b, 0xff, 0x00, 0x21, 0x00, 0x01, 0x01, 0x00, 0x00,
            0x03, 0x0e, 0xff, 0x00, 0x24, 0x00, 0x01, 0x01, 0x00, 0x00, 
            0x03, 0x11, 0xff, 0x00, 0x1c, 0x00, 0x01, 0x01, 0x00, 0x00,
            0x03, 0x19, 0xff, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00,
            0x03, 0x9a, 0xff, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x00,
            0x0b, 0x84, 0xff, 0x00, 0x1f, 0x00, 0x01, 0x05, 0x00, 0x00, 
            0x0b, 0x85, 0xff, 0x00, 0x01, 0x00, 0x01, 0x04, 0x00, 0x00, 
            0x11, 0x0a, 0xff, 0x00, 
            0x11, 0xcb, 0xff, 0x00, 0x1d, 0x00, 0x01, 0x06, 0x00, 0x00, 
            0x11, 0x8c, 0xff, 0x00, 0x0e, 0x00, 0x01, 0x03, 0x00, 0x00,
            0x11, 0x8c, 0xff, 0x00, 0x0e, 0x00, 0x01, 0x02, 0x00, 0x01,
            0x11, 0x8c, 0xff, 0x00, 0x0e, 0x00, 0x01, 0x01, 0x00, 0x00,
            0xfe, 196, 0xff, 0x00, 0x01, 0x00, 0x01, 0x04, 0x00, 0x00,
            0x01,
            0x03, 0x28, 0x03, 0x07, 0x30, 0x81, 0x0b, 0xc0, 0x00, 0x14, 0x14, 0x14, 0x40, 0x0a, 0x18, 0x48, 0x08, 0x11, 0x00, 0x00, 
            0x01, 
            0xf6, 0x0b, 0x99, 0x36, 0x55, 0x01, 0x00, 0x00, 0x05, 
            0x00
        ]

    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def test_teardown_serialize(self):
        state_buffer= to_string_buffer(self.expected_state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        acc_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_log(acc_signal, None, self.logger_created)
        self.events["log"].wait()

        self.libmetawear.mbl_mw_metawearboard_tear_down(self.board)

        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        python_array= []
        for i in range(0, state_array_size.value):
            python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.maxDiff= None
        self.assertEqual(python_array[0:271], self.expected_state[0:271])

class TestMetaWearBoardDeserialize(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.state= to_string_buffer(serializedstate.activity_with_buffer)
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def test_deserialize_same_firmware(self):
        # only thing to do is re-read the logging tick
        expected= [0xb, 0x84]

        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(self.state, POINTER(c_ubyte)), len(self.state.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        self.assertEqual(self.command, expected)

    def test_deserialize_diff_firmware(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80],
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80], [0x19, 0x80],
            [0xfe, 0x80], [0x0b, 0x84]
        ]

        self.firmware_revision= create_string_buffer(b'1.0.4', 5)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(self.state, POINTER(c_ubyte)), len(self.state.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        self.assertEqual(self.full_history, expected_cmds)

class TestDeserializeTimer(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= to_string_buffer(serializedstate.timer_two_events)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

        self.serialized_timer= self.libmetawear.mbl_mw_timer_lookup_id(self.board, 7);

    def test_start(self):
        expected= [0x0c, 0x03, 0x7]

        self.libmetawear.mbl_mw_timer_start(self.serialized_timer)
        self.assertEqual(self.command, expected)

    def test_stop(self):
        expected= [0x0c, 0x04, 0x7]

        self.libmetawear.mbl_mw_timer_stop(self.serialized_timer)
        self.assertEqual(self.command, expected)

    def test_remove(self):
        expected_cmds= [
            [0x0c, 0x05, 0x7],
            [0x0a, 0x04, 0x0],
            [0x0a, 0x04, 0x1]
        ]

        self.libmetawear.mbl_mw_timer_remove(self.serialized_timer)
        self.assertEqual(self.command_history, expected_cmds)

    def test_null_timer(self):
        invalid_ids= [0, 1, 2, 3, 4, 5, 6]

        for id in invalid_ids:
            with self.subTest(timer_id=id):
                self.assertIsNone(self.libmetawear.mbl_mw_timer_lookup_id(self.board, id))

class TestDeserializeAccelerometerLog(TestAccelerometerLoggingBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= to_string_buffer(serializedstate.accelerometer_log)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.acc_logger= self.libmetawear.mbl_mw_logger_lookup_id(self.board, 0)

    def test_acc_data(self):
        self.logger_ready(None, self.acc_logger)
        self.assertEqual(self.logged_data, Bmi160Accelerometer.expected_values)

    def test_epoch_calc(self):
        self.logger_ready(None, self.acc_logger)

        self.maxDiff = None
        self.assertEqual(self.data_time_offsets, Bmi160Accelerometer.expected_offsets)

    def test_remove_logger(self):
        expected_cmds= [
            [0x0b, 0x03, 0x00],
            [0x0b, 0x03, 0x01]
        ]

        self.libmetawear.mbl_mw_logger_remove(self.acc_logger)
        self.assertEqual(self.command_history, expected_cmds)

class TestDeserializeGyroYAxisLog(TestGyroYAxisLoggingBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= to_string_buffer(serializedstate.gyro_y_axis_logging_state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.gyro_y_logger= self.libmetawear.mbl_mw_logger_lookup_id(self.board, 0)

    def test_gyro_data(self):
        self.logger_ready(None, self.gyro_y_logger)

        # why doesn't unittest come with an assertAlmostEqual for list of floats?
        self.assertEqual(len(self.logged_data), len(Bmi160GyroYAxis.expected_values))
        for a, b in zip(self.logged_data, Bmi160GyroYAxis.expected_values):
            self.assertAlmostEqual(a, b, delta = 0.001)

    def test_remove_logger(self):
        expected_cmds= [
            [0x0b, 0x03, 0x00]
        ]

        self.libmetawear.mbl_mw_logger_remove(self.gyro_y_logger)
        self.assertEqual(self.command_history, expected_cmds)

class TestDeserializeActivityHandler(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= to_string_buffer(serializedstate.activity_with_buffer)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

    def test_read_state(self):
        ids= [1, 2]

        for id in ids:
            with self.subTest(processor_id=id):
                processor= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, id)
                state_signal= self.libmetawear.mbl_mw_dataprocessor_get_state_data_signal(processor)
                self.libmetawear.mbl_mw_datasignal_read(state_signal)

                expected= [0x9, 0xc4, id]
                self.assertEqual(self.command, expected)

    def test_remove_processor_chain(self):
        expected_cmds= [
            [0x9, 0x6, 0x2],
            [0x9, 0x6, 0x4],
            [0x9, 0x6, 0x3],
            [0x9, 0x6, 0x1]
        ]

        processor= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, 1)
        self.libmetawear.mbl_mw_dataprocessor_remove(processor)
        self.assertEqual(self.command_history, expected_cmds)

    def test_buffer_read(self):
        expected= 260.5125

        processor= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, 2)
        processor_state= self.libmetawear.mbl_mw_dataprocessor_get_state_data_signal(processor)
        self.libmetawear.mbl_mw_datasignal_subscribe(processor_state, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x09\x84\x02\xcd\x20\x41\x00', 7))

        self.assertAlmostEqual(self.data_float.value, expected, delta= 0.0001)

    def test_timer_data(self):
        response= create_string_buffer(b'\x09\x03\x03\xfe\x3f\xb7\x01', 7)
        expected= 1756.9999

        processor= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, 3)
        self.libmetawear.mbl_mw_datasignal_subscribe(processor, None, self.sensor_data_handler)
        self.notify_mw_char(response)

        self.assertAlmostEqual(self.data_float.value, expected, delta= 0.0001)

class TestSerializeMultiComparator(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.firmware_revision= create_string_buffer(b'1.2.3', 5)

        super().setUp()

    def test_initial_state(self):
        references= (c_float * 4)(1024, 512, 256, 128)
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x15, GpioAnalogReadMode.ADC)

        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(adc_signal, ComparatorOperation.GTE, ComparatorMode.REFERENCE, references, 
                len(references), None, self.processor_handler)
        self.events["processor"].wait()

        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        self.python_array= []
        for i in range(0, state_array_size.value):
            self.python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.maxDiff = None
        self.assertEqual(self.python_array[0:448], serializedstate.multi_comparator_state[0:448])

class TestDeserializeMultiComparator(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.firmware_revision= create_string_buffer(b'1.2.3', 5)
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= to_string_buffer(serializedstate.multi_comparator_state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, cast(state_buffer, POINTER(c_ubyte)), len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

    def test_modify_config_state(self):
        references= (c_float * 2)(128, 256)

        comparator= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, 0)
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_modify(comparator, ComparatorOperation.LT, references, len(references))

        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        python_array= []
        for i in range(0, state_array_size.value):
            python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.assertEqual(python_array[0:408], serializedstate.multi_comparator_modified_state[0:408])

class TestModel(TestMetaWearBase):
    def setUp(self):
        setup = True

        self.models = [
            ["metawear r", TestMetaWearBase.METAWEAR_R_BOARD, Model.METAWEAR_R, "MetaWear R"],
            ["metawear rg", TestMetaWearBase.METAWEAR_RG_BOARD, Model.METAWEAR_RG, "MetaWear RG"],
            ["metawear rpro", TestMetaWearBase.METAWEAR_RPRO_BOARD, Model.METAWEAR_RPRO, "MetaWear RPro"],
            ["metawear cpro", TestMetaWearBase.METAWEAR_CPRO_BOARD, Model.METAWEAR_CPRO, "MetaWear CPro"],
            ["metaenv", TestMetaWearBase.METAWEAR_ENV_BOARD, Model.METAENV, "MetaEnvironment"],
            ["metadetect", TestMetaWearBase.METAWEAR_DETECT_BOARD, Model.METADETECT, "MetaDetector"],
            ["metamotion r", TestMetaWearBase.METAWEAR_MOTION_R_BOARD, Model.METAMOTION_R, "MetaMotion R"]
        ]

    def test_get_model(self):
        for m in self.models:
            with self.subTest(model=m[0]):
                self.boardType = m[1]
                self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))
                self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

                actual = self.libmetawear.mbl_mw_metawearboard_get_model(self.board)
                self.assertEqual(m[2], actual)

    def test_get_model_name(self):
        for m in self.models:
            with self.subTest(model=m[0]):
                self.boardType = m[1]
                self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))
                self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

                actual = self.libmetawear.mbl_mw_metawearboard_get_model_name(self.board)
                self.assertEqual(m[3], actual.decode('ascii'))

class TestIndefiniteTimeout(TestMetaWearBase):
    def setUp(self):
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))
        self.init_event = threading.Event()

    def initialized(self, ctx, board, status):
        self.init_status= status
        self.init_event.set()

    def commandLogger(self, board, writeType, characteristic, command, length):
        pass

    @unittest.skip("not waiting forever")
    def test_infinite_initialize(self):
        self.libmetawear.mbl_mw_metawearboard_set_time_for_response(self.board, 0)
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.init_event.wait()

        self.assertEqual(self.init_status, Const.STATUS_ERROR_TIMEOUT)

class TestMissingModule(TestMetaWearBase):
    def setUp(self):
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))
        self.init_event = threading.Event()

    def initialized(self, ctx, board, status):
        self.init_status= status
        self.init_event.set()

    def test_no_modules(self):
        self.boardType = TestMetaWearBase.CUSTOM_BOARD
        self.custom_module_number = b'5'

        self.lookup_module_response = lambda x: to_string_buffer([x, 0x80])
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        self.init_event.wait()

        self.assertEqual(Const.STATUS_OK, self.init_status)
        # should not issue command to read current time
        self.assertNotEqual([0x0b, 0x84], self.command)