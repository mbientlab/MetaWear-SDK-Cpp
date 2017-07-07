from barometer_bosch_base import BarometerBoschBase
from common import TestMetaWearBase
from mbientlab.metawear.cbindings import *

class TestBarometerBmp280Config(BarometerBoschBase.TestBarometerBoschConfig):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def test_set_standby(self):
        tests= [
            {
                'expected': [0x12, 0x03, 0x2c, 0x00],
                'standby_time': BaroBmp280StandbyTime._0_5ms,
                'time': '0.5ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0x20],
                'standby_time': BaroBmp280StandbyTime._62_5ms,
                'time': '62.5ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0x40],
                'standby_time': BaroBmp280StandbyTime._125ms,
                'time': '125ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0x60],
                'standby_time': BaroBmp280StandbyTime._250ms,
                'time': '250ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0x80],
                'standby_time': BaroBmp280StandbyTime._500ms,
                'time': '500ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0xa0],
                'standby_time': BaroBmp280StandbyTime._1000ms,
                'time': '1000ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0xc0],
                'standby_time': BaroBmp280StandbyTime._2000ms,
                'time': '2000ms'
            },
            {
                'expected': [0x12, 0x03, 0x2c, 0xe0],
                'standby_time': BaroBmp280StandbyTime._4000ms,
                'time': '4000ms'
            }
        ]

        for test in tests:
            with self.subTest(time= test['time']):
                self.libmetawear.mbl_mw_baro_bmp280_set_standby_time(self.board, test['standby_time'])
                self.libmetawear.mbl_mw_baro_bosch_write_config(self.board)
                self.assertEqual(self.command, test['expected'])

    def test_set_all_config(self):
        expected= [0x12, 0x03, 0x28, 0x90]

        self.libmetawear.mbl_mw_baro_bosch_set_oversampling(self.board, BaroBoschOversampling.LOW_POWER)
        self.libmetawear.mbl_mw_baro_bosch_set_iir_filter(self.board, BaroBoschIirFilter.AVG_16)
        self.libmetawear.mbl_mw_baro_bmp280_set_standby_time(self.board, BaroBmp280StandbyTime._500ms)
        self.libmetawear.mbl_mw_baro_bosch_write_config(self.board)
        self.assertListEqual(self.command, expected)

class TestBarometerBmp280PressureData(BarometerBoschBase.TestBarometerBoschPressureData):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

class TestBarometerBmp280AltitudeData(BarometerBoschBase.TestBarometerBoschAltitudeData):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()
