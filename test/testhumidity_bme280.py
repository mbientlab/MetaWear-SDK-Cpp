from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.sensor import HumidityBme280

class TestHumidityBme280Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

    def test_oversampling(self):
        tests= [
            {
                'expected': [0x16, 0x02, 0x01],
                'oversampling': HumidityBme280.OVERSAMPLING_1X,
                'oversampling_name': '1x'
            },
            {
                'expected': [0x16, 0x02, 0x02],
                'oversampling': HumidityBme280.OVERSAMPLING_2X,
                'oversampling_name': '2x'
            },
            {
                'expected': [0x16, 0x02, 0x03],
                'oversampling': HumidityBme280.OVERSAMPLING_4X,
                'oversampling_name': '4x'
            },
            {
                'expected': [0x16, 0x02, 0x04],
                'oversampling': HumidityBme280.OVERSAMPLING_8X,
                'oversampling_name': '8x'
            },
            {
                'expected': [0x16, 0x02, 0x05],
                'oversampling': HumidityBme280.OVERSAMPLING_16X,
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
        response= create_string_buffer(b'\x16\x81\xc7\xfc\x00\x00')

        signal= self.libmetawear.mbl_mw_humidity_bme280_get_percentage_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))

        self.assertAlmostEqual(self.data_float.value, expected)
