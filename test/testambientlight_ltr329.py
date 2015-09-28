import os
import unittest
from mbientlab.metawear.ambientlight_ltr329 import *
from ctypes import *

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestALsLtr329Config(unittest.TestCase):
    def setUp(self):
        self.config= metawear_lib.mbl_mw_als_ltr329_create_config()

    def tearDown(self):
        metawear_lib.mbl_mw_als_ltr329_free_config(self.config)

    def test_set_gain(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x14\x02\x18\x03')

        metawear_lib.mbl_mw_als_ltr329_set_gain(self.config, Ltr329Gain.GAIN_48X)
        metawear_lib.mbl_mw_als_ltr329_write_config(command, self.config)
        self.assertEqual(command.value, expected.value)

    def test_set_integration_time(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x14\x02\x00\x1b')

        metawear_lib.mbl_mw_als_ltr329_set_integration_time(self.config, Ltr329IntegrationTime.TIME_400MS)
        metawear_lib.mbl_mw_als_ltr329_write_config(command, self.config)
        self.assertEqual(command.value, expected.value)

    def test_set_measurement_rate(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x14\x02\x00\x05')

        metawear_lib.mbl_mw_als_ltr329_set_measurement_rate(self.config, Ltr329MeasurementRate.RATE_2000MS)
        metawear_lib.mbl_mw_als_ltr329_write_config(command, self.config)
        self.assertEqual(command.value, expected.value)

    def test_set_all_config(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x14\x02\x0c\x28')

        metawear_lib.mbl_mw_als_ltr329_set_gain(self.config, Ltr329Gain.GAIN_8X)
        metawear_lib.mbl_mw_als_ltr329_set_integration_time(self.config, Ltr329IntegrationTime.TIME_250MS)
        metawear_lib.mbl_mw_als_ltr329_set_measurement_rate(self.config, Ltr329MeasurementRate.RATE_50MS)
        metawear_lib.mbl_mw_als_ltr329_write_config(command, self.config)
        self.assertEqual(command.value, expected.value)

class TestAlsLtr329IllumunanceData(unittest.TestCase):
    def setUp(self):
        self.illuminance_data_source= metawear_lib.mbl_mw_als_ltr329_get_data_source
        self.illuminance_data_source.restype= c_long

    def test_subscribe_illuminance_data(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x14\x03\x01')

        metawear_lib.mbl_mw_sensor_subscribe(command, c_long(self.illuminance_data_source()))
        self.assertEqual(command.value, expected.value)

    def test_unsubscribe_illuminance_data(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x14\x03\x00')

        metawear_lib.mbl_mw_sensor_unsubscribe(command, c_long(self.illuminance_data_source()))
        self.assertEqual(command.value, expected.value)

    def test_get_illuminance_data(self):
        response_bytes= create_string_buffer(b'\x14\x03\xed\x92\xb0\x00')
        response_uint32= c_int()
        expected= 11571949

        metawear_lib.mbl_mw_als_ltr329_get_illuminance_data(byref(response_uint32), response_bytes)
        self.assertEqual(response_uint32.value, expected)
