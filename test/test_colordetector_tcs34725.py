from common import TestMetaWearBase
from ctypes import c_float, create_string_buffer
from mbientlab.metawear.core import Tcs34725ColorAdc
from mbientlab.metawear.sensor import ColorDetectorTcs34725

class TestColorDetectorTcs34725Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

    def test_gain(self):
        tests= [
            {
                'expected': [0x17, 0x02, 0xff, 0x00, 0x00],
                'gain': ColorDetectorTcs34725.GAIN_1X,
                'gain_name': '1x'
            },
            {
                'expected': [0x17, 0x02, 0xff, 0x01, 0x00],
                'gain': ColorDetectorTcs34725.GAIN_4X,
                'gain_name': '4x'
            },
            {
                'expected': [0x17, 0x02, 0xff, 0x02, 0x00],
                'gain': ColorDetectorTcs34725.GAIN_16X,
                'gain_name': '16x'
            },
            {
                'expected': [0x17, 0x02, 0xff, 0x03, 0x00],
                'gain': ColorDetectorTcs34725.GAIN_60X,
                'gain_name': '60x'
            },
        ]

        for test in tests:
            with self.subTest(channel= test['gain_name']):
                self.libmetawear.mbl_mw_cd_tcs34725_set_gain(self.board, test['gain'])
                self.libmetawear.mbl_mw_cd_tcs34725_write_config(self.board)
                self.assertEqual(self.command, test['expected'])

    def test_integration_time(self):
        tests= [
            {
                'expected': [0x17, 0x02, 0x01, 0x00, 0x00],
                'integration_time': c_float(612),
                'time': '4.8ms'
            },
            {
                'expected': [0x17, 0x02, 0xfe, 0x00, 0x00],
                'integration_time': c_float(4.8),
                'time': '612ms'
            }
        ]

        for test in tests:
            with self.subTest(channel= test['time']):
                self.libmetawear.mbl_mw_cd_tcs34725_set_integration_time(self.board, test['integration_time'])
                self.libmetawear.mbl_mw_cd_tcs34725_write_config(self.board)
                self.assertEqual(self.command, test['expected'])

    def test_illuminator_enable(self):
        expected= [0x17, 0x02, 0xff, 0x00, 0x01]

        self.libmetawear.mbl_mw_cd_tcs34725_enable_illuminator_led(self.board)
        self.libmetawear.mbl_mw_cd_tcs34725_write_config(self.board)
        self.assertEqual(self.command, expected)

    def test_illuminator_enable(self):
        expected= [0x17, 0x02, 0xff, 0x00, 0x00]

        self.libmetawear.mbl_mw_cd_tcs34725_disable_illuminator_led(self.board)
        self.libmetawear.mbl_mw_cd_tcs34725_write_config(self.board)
        self.assertEqual(self.command, expected)

class TestColorDetectorTcs34725Data(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

        self.adc_signal= self.libmetawear.mbl_mw_cd_tcs34725_get_adc_data_signal(self.board)

    def test_read_adc(self):
        expected= [0x17, 0x81]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.adc_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_read(self.adc_signal)
        self.assertEqual(self.command, expected)

    def test_read_adc_silent(self):
        expected= [0x17, 0xc1]

        self.libmetawear.mbl_mw_datasignal_read(self.adc_signal)
        self.assertEqual(self.command, expected)

    def test_color_adc_data(self):
        expected= Tcs34725ColorAdc(clear= 418, red= 123, green= 154, blue= 124)
        response= create_string_buffer(b'\x17\x81\xa2\x01\x7b\x00\x9a\x00\x7c\x00', 10)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.adc_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))

        self.assertEqual(self.data_tcs34725_adc, expected)
