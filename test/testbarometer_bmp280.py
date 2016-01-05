from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear import BarometerBmp280

class TestBaroBmp280Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def test_set_oversampling(self):
        expected= [0x12, 0x03, 0x14, 0x00]

        self.libmetawear.mbl_mw_baro_bmp280_set_oversampling(self.board, BarometerBmp280.OVERSAMPLING_ULTRA_HIGH)
        self.libmetawear.mbl_mw_baro_bmp280_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_filter(self):
        expected= [0x12, 0x03, 0x0c, 0x08]

        self.libmetawear.mbl_mw_baro_bmp280_set_iir_filter(self.board, BarometerBmp280.IIR_FILTER_AVG_4)
        self.libmetawear.mbl_mw_baro_bmp280_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_standby(self):
        expected= [0x12, 0x03, 0x0c, 0x60]

        self.libmetawear.mbl_mw_baro_bmp280_set_standby_time(self.board, BarometerBmp280.STANDBY_TIME_250MS)
        self.libmetawear.mbl_mw_baro_bmp280_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_all_config(self):
        expected= [0x12, 0x03, 0x08, 0x90]

        self.libmetawear.mbl_mw_baro_bmp280_set_oversampling(self.board, BarometerBmp280.OVERSAMPLING_LOW_POWER)
        self.libmetawear.mbl_mw_baro_bmp280_set_iir_filter(self.board, BarometerBmp280.IIR_FILTER_AVG_16)
        self.libmetawear.mbl_mw_baro_bmp280_set_standby_time(self.board, BarometerBmp280.STANDBY_TIME_500MS)
        self.libmetawear.mbl_mw_baro_bmp280_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_start(self):
        expected= [0x12, 0x04, 0x01, 0x01]

        self.libmetawear.mbl_mw_baro_bmp280_start(self.board);
        self.assertEqual(self.command, expected)

    def test_stop(self):
        expected= [0x12, 0x04, 0x00, 0x00]

        self.libmetawear.mbl_mw_baro_bmp280_stop(self.board);
        self.assertEqual(self.command, expected)

class TestBaroBmp280PressureData(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.pa_data_signal= self.libmetawear.mbl_mw_baro_bmp280_get_pressure_data_signal(self.board)

    def test_get_pressure_data(self):
        response= create_string_buffer(b'\x12\x01\xd3\x35\x8b\x01', 6)
        expected= 101173.828125

        self.libmetawear.mbl_mw_datasignal_subscribe(self.pa_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_handle_response(self.board, response.raw, len(response))
        self.assertAlmostEqual(self.data_float.value, expected)

    def test_pressure_subscribe(self):
        expected= [0x12, 0x1, 0x1]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.pa_data_signal, self.sensor_data_handler);
        self.assertListEqual(self.command, expected)

    def test_pressure_unsubscribe(self):
        expected= [0x12, 0x1, 0x0]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.pa_data_signal);
        self.assertListEqual(self.command, expected)


class TestBaroBmp280AltitudeData(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.m_data_signal= self.libmetawear.mbl_mw_baro_bmp280_get_altitude_data_signal(self.board)

    def test_get_altitude_data(self):
        response= create_string_buffer(b'\x12\x02\x1e\x1f\xfe\xff', 6)
        expected= -480.8828125

        self.libmetawear.mbl_mw_datasignal_subscribe(self.m_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_handle_response(self.board, response.raw, len(response))
        self.assertAlmostEqual(self.data_float.value, expected)

    def test_altitude_subscribe(self):
        expected= [0x12, 0x2, 0x1]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.m_data_signal, self.sensor_data_handler);
        self.assertListEqual(self.command, expected)

    def test_altitude_unsubscribe(self):
        expected= [0x12, 0x2, 0x0]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.m_data_signal);
        self.assertListEqual(self.command, expected)
