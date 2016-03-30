from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.core import CartesianFloat
from mbientlab.metawear.sensor import MagnetometerBmm150

class TestMagnetometerBmm150(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_CPRO_BOARD

        super().setUp()

    def test_power_preset(self):
        tests= [
            {
                'expected': [[0x15, 0x04, 0x01, 0x02], [0x15, 0x03, 0x0]] ,
                'preset': MagnetometerBmm150.POWER_PRESET_LOW_POWER,
                'preset_name': 'low power',
                'offset': 0
            },
            {
                'expected': [[0x15, 0x04, 0x04, 0x0e], [0x15, 0x03, 0x00]], 
                'preset': MagnetometerBmm150.POWER_PRESET_REGULAR,
                'preset_name': 'regular',
                'offset': 2
            },
            {
                'expected': [[0x15, 0x04, 0x07, 0x1a], [0x15, 0x03, 0x00]], 
                'preset': MagnetometerBmm150.POWER_PRESET_ENHANCED_REGULAR,
                'preset_name': 'enhanced regular',
                'offset': 4
            },
            {
                'expected': [[0x15, 0x04, 0x17, 0x52], [0x15, 0x03, 0x05]], 
                'preset': MagnetometerBmm150.POWER_PRESET_HIGH_ACCURACY,
                'preset_name': 'high accuracy',
                'offset': 6
            },
        ]

        for test in tests:
            with self.subTest(preset= test['preset_name']):
                offset= test['offset']
                self.libmetawear.mbl_mw_mag_bmm150_set_power_preset(self.board, test['preset'])
                self.assertEqual(self.command_history[offset:offset + 2], test['expected'])

    def test_enable_b_field_sampling(self):
        expected= [0x15, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_mag_bmm150_enable_b_field_sampling(self.board)
        self.assertEqual(self.command, expected)

    def test_disable_b_field_sampling(self):
        expected= [0x15, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_mag_bmm150_disable_b_field_sampling(self.board)
        self.assertEqual(self.command, expected)

    def test_start(self):
        expected= [0x15, 0x01, 0x01]

        self.libmetawear.mbl_mw_mag_bmm150_start(self.board)
        self.assertEqual(self.command, expected)

    def test_start(self):
        expected= [0x15, 0x01, 0x00]

        self.libmetawear.mbl_mw_mag_bmm150_stop(self.board)
        self.assertEqual(self.command, expected)

class TestMagnetometerBmm150Data(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_CPRO_BOARD

        super().setUp()

    def test_b_field_subscribe(self):
        expected= [0x15, 0x05, 0x01]

        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)

    def test_b_field_unsubscribe(self):
        expected= [0x15, 0x05, 0x00]

        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal)

    def test_b_field_data(self):
        expected= CartesianFloat(x = -251.1250, y = 165.1875, z = 71.3125)
        response= create_string_buffer(b'\x15\x05\x4e\xf0\x53\x0a\x75\x04', 8)

        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)
        status= self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))

        self.assertEqual(self.data_cartesian_float, expected)
