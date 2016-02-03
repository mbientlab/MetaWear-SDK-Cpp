from common import TestMetaWearBase
from mbientlab.metawear.sensor import AmbientLightLtr329
from ctypes import *

class TestALsLtr329Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def test_set_gain(self):
        expected= [0x14, 0x02, 0x18, 0x03]

        self.libmetawear.mbl_mw_als_ltr329_set_gain(self.board, AmbientLightLtr329.GAIN_48X)
        self.libmetawear.mbl_mw_als_ltr329_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_integration_time(self):
        expected= [0x14, 0x02, 0x00, 0x1b]

        self.libmetawear.mbl_mw_als_ltr329_set_integration_time(self.board, AmbientLightLtr329.INTEGRATION_TIME_400MS)
        self.libmetawear.mbl_mw_als_ltr329_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_measurement_rate(self):
        expected= [0x14, 0x02, 0x00, 0x05]

        self.libmetawear.mbl_mw_als_ltr329_set_measurement_rate(self.board, AmbientLightLtr329.MEASUREMENT_RATE_2000MS)
        self.libmetawear.mbl_mw_als_ltr329_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_all_config(self):
        expected= [0x14, 0x02, 0x0c, 0x28]

        self.libmetawear.mbl_mw_als_ltr329_set_gain(self.board, AmbientLightLtr329.GAIN_8X)
        self.libmetawear.mbl_mw_als_ltr329_set_integration_time(self.board, AmbientLightLtr329.INTEGRATION_TIME_250MS)
        self.libmetawear.mbl_mw_als_ltr329_set_measurement_rate(self.board, AmbientLightLtr329.MEASUREMENT_RATE_50MS)
        self.libmetawear.mbl_mw_als_ltr329_write_config(self.board)
        self.assertListEqual(self.command, expected)

class TestAlsLtr329DataHandler(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.ltr329_data_signal= self.libmetawear.mbl_mw_als_ltr329_get_illuminance_data_signal(self.board)

    def test_get_illuminance_data(self):
        response= create_string_buffer(b'\x14\x03\xed\x92\xb0\x00', 6)
        expected= 11571949

        self.libmetawear.mbl_mw_datasignal_subscribe(self.ltr329_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_uint32.value, expected)

    def test_stream_illuminance_data(self):
        expected= [0x14, 0x03, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.ltr329_data_signal, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_end_stream_illuminance_data(self):
        expected= [0x14, 0x03, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.ltr329_data_signal)
        self.assertListEqual(self.command, expected)
