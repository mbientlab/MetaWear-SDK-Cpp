from barometer_bosch_base import BarometerBoschBase
from common import TestMetaWearBase
from mbientlab.metawear.sensor import BarometerBme280, BarometerBosch

class TestBarometerBme280Config(BarometerBoschBase.TestBarometerBoschConfig):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

    def test_set_standby(self):
        tests= [
            {
                'expected': [0x12, 0x03, 0x2c, 0x00],
                'standby_time': BarometerBme280.STANDBY_TIME_0_5MS,
                'time': '0.5ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0x20],
                'standby_time': BarometerBme280.STANDBY_TIME_62_5MS,
                'time': '62.5ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0x40],
                'standby_time': BarometerBme280.STANDBY_TIME_125MS,
                'time': '125ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0x60],
                'standby_time': BarometerBme280.STANDBY_TIME_250MS,
                'time': '250ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0x80],
                'standby_time': BarometerBme280.STANDBY_TIME_500MS,
                'time': '500ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0xa0],
                'standby_time': BarometerBme280.STANDBY_TIME_1000MS,
                'time': '1000ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0xc0],
                'standby_time': BarometerBme280.STANDBY_TIME_10MS,
                'time': '2000ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0xe0],
                'standby_time': BarometerBme280.STANDBY_TIME_20MS,
                'time': '4000ms'
            }
        ]

        for test in tests:
            with self.subTest(time= test['time']):
                self.libmetawear.mbl_mw_baro_bme280_set_standby_time(self.board, test['standby_time'])
                self.libmetawear.mbl_mw_baro_bosch_write_config(self.board)
                self.assertEqual(self.command, test['expected'])

class TestBarometerBmp280PressureData(BarometerBoschBase.TestBarometerBoschPressureData):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

class TestBarometerBmp280AltitudeData(BarometerBoschBase.TestBarometerBoschAltitudeData):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()
