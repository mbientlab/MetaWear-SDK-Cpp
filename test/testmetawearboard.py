from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.core import Status
from testdataprocessor import TestGpioFeedbackSetup

class TestMetaWearBoard(TestMetaWearBase):
    def test_no_response_handler(self):
        tests= [
            {
                'response': create_string_buffer(b'\x03\x04\x16\xc4\x94\xa2\x2a\xd0', 8),
                'status': Status.WARNING_UNEXPECTED_SENSOR_DATA,
                'name': 'acc'
            },
            {
                'response': create_string_buffer(b'\x14\x03\xed\x92\xb0\x00', 6),
                'status': Status.WARNING_INVALID_RESPONSE,
                'name': 'ambient_light'
            },
            {
                'response': create_string_buffer(b'\x12\x01\xd3\x35\x8b\x01', 6),
                'status': Status.WARNING_INVALID_RESPONSE,
                'name': 'baro_pressure'
            },
            {
                'response': create_string_buffer(b'\x12\x02\x1e\x1f\xfe\xff', 6),
                'status': Status.WARNING_INVALID_RESPONSE,
                'name': 'baro_altitude'
            },
            {
                'response': create_string_buffer(b'\x13\x05\x3e\x43\xff\x7f\x00\x80', 8),
                'status': Status.WARNING_INVALID_RESPONSE,
                'name': 'gyro'
            },
            {
                'response': create_string_buffer(b'\x01\x01\x01', 3),
                'status': Status.WARNING_UNEXPECTED_SENSOR_DATA,
                'name': 'switch'
            }
        ]

        for test in tests:
            with self.subTest(response=test['name']):
                status= self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, test['response'].raw, len(test['response'].raw))
                self.assertEqual(status, test['status'])

    def test_service_discovery(self):
        self.assertEqual(self.initialized, True)

class TestMetaWearBoardInitialize(TestMetaWearBase):
    def test_reinitialize(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80],
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80],
            [0xfe, 0x80], [0x0b, 0x84]
        ]

        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)
        self.assertEqual(self.full_history, expected_cmds)

    def test_reinitialize_new_firmware(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80], 
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80],
            [0xfe, 0x80], [0x0b, 0x84],
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80], 
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80],
            [0xfe, 0x80], [0x0b, 0x84]
        ]

        self.firmware_revision= create_string_buffer(b'1.1.4', 5)
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)
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

        self.libmetawear.mbl_mw_timer_create(self.board, 667408, -1, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create(self.board, 1000, -1, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create(self.board, 1000, 10, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 1000, 0, self.timer_signal_ready)

        self.libmetawear.mbl_mw_metawearboard_tear_down(self.board)
