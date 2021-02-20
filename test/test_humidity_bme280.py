from common import TestMetaWearBase
from ctypes import create_string_buffer
from cbindings import *
#from mbientlab.metawear.cbindings import *

class TestHumidityBme280Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

    def test_oversampling(self):
        tests= [
            {
                'expected': [0x16, 0x02, 0x01],
                'oversampling': HumidityBme280Oversampling._1X,
                'oversampling_name': '1x'
            },
            {
                'expected': [0x16, 0x02, 0x02],
                'oversampling': HumidityBme280Oversampling._2X,
                'oversampling_name': '2x'
            },
            {
                'expected': [0x16, 0x02, 0x03],
                'oversampling': HumidityBme280Oversampling._4X,
                'oversampling_name': '4x'
            },
            {
                'expected': [0x16, 0x02, 0x04],
                'oversampling': HumidityBme280Oversampling._8X,
                'oversampling_name': '8x'
            },
            {
                'expected': [0x16, 0x02, 0x05],
                'oversampling': HumidityBme280Oversampling._16X,
                'oversampling_name': '16x'
            },
        ]

        for test in tests:
            with self.subTest(channel= test['oversampling_name']):
                self.libmetawear.mbl_mw_humidity_bme280_set_oversampling(self.board, test['oversampling'])
                self.assertEqual(self.command, test['expected'])

class TestHumidityBme280Data(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

    def test_humidity_data(self):
        expected= 63.1943359375

        signal= self.libmetawear.mbl_mw_humidity_bme280_get_percentage_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x16\x81\xc7\xfc\x00\x00'))

        self.assertAlmostEqual(self.data_float.value, expected)
