from common import TestMetaWearBase
from cbindings import *
#from mbientlab.metawear.cbindings import *
from ctypes import *

class TestALsLtr329Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

    def test_set_gain(self):
        expected= [0x14, 0x02, 0x18, 0x03]

        self.libmetawear.mbl_mw_als_ltr329_set_gain(self.board, AlsLtr329Gain._48X)
        self.libmetawear.mbl_mw_als_ltr329_write_config(self.board)
        print("TestALsLtr329Config \n")
        self.assertListEqual(self.command, expected)

    def test_set_integration_time(self):
        expected= [0x14, 0x02, 0x00, 0x1b]

        self.libmetawear.mbl_mw_als_ltr329_set_integration_time(self.board, AlsLtr329IntegrationTime._400ms)
        self.libmetawear.mbl_mw_als_ltr329_write_config(self.board)
        print("TestALsLtr329Config \n")
        self.assertListEqual(self.command, expected)

    def test_set_measurement_rate(self):
        expected= [0x14, 0x02, 0x00, 0x05]

        self.libmetawear.mbl_mw_als_ltr329_set_measurement_rate(self.board, AlsLtr329MeasurementRate._2000ms)
        self.libmetawear.mbl_mw_als_ltr329_write_config(self.board)
        print("TestALsLtr329Config \n")
        self.assertListEqual(self.command, expected)

    def test_set_all_config(self):
        expected= [0x14, 0x02, 0x0c, 0x28]

        self.libmetawear.mbl_mw_als_ltr329_set_gain(self.board, AlsLtr329Gain._8X)
        self.libmetawear.mbl_mw_als_ltr329_set_integration_time(self.board, AlsLtr329IntegrationTime._250ms)
        self.libmetawear.mbl_mw_als_ltr329_set_measurement_rate(self.board, AlsLtr329MeasurementRate._50ms)
        self.libmetawear.mbl_mw_als_ltr329_write_config(self.board)
        print("TestALsLtr329Config \n")
        self.assertListEqual(self.command, expected)

class TestAlsLtr329DataHandler(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

        self.ltr329_data_signal= self.libmetawear.mbl_mw_als_ltr329_get_illuminance_data_signal(self.board)

    def test_get_illuminance_data(self):
        response= create_string_buffer(b'\x14\x03\xed\x92\xb0\x00', 6)
        expected= 11571949

        self.libmetawear.mbl_mw_datasignal_subscribe(self.ltr329_data_signal, None, self.sensor_data_handler)
        self.notify_mw_char(response)
        print("TestAlsLtr329DataHandler \n")
        self.assertEqual(self.data_uint32.value, expected)

    def test_stream_illuminance_data(self):
        expected= [0x14, 0x03, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.ltr329_data_signal, None, self.sensor_data_handler)
        print("TestAlsLtr329DataHandler \n")
        self.assertListEqual(self.command, expected)

    def test_end_stream_illuminance_data(self):
        expected= [0x14, 0x03, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.ltr329_data_signal)
        print("TestAlsLtr329DataHandler \n")
        self.assertListEqual(self.command, expected)
