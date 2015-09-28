import os
import unittest
from mbientlab.metawear.barometer_bmp280 import *
from ctypes import CDLL, c_float, create_string_buffer, byref

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestBaroBmp280Config(unittest.TestCase):
    def setUp(self):
        self.config= metawear_lib.mbl_mw_baro_bmp280_create_config()

    def tearDown(self):
        metawear_lib.mbl_mw_baro_bmp280_free_config(self.config)

    def test_set_oversampling(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x12\x03\x14\x00', 4)

        metawear_lib.mbl_mw_baro_bmp280_set_oversampling(self.config, BaroBmp280Oversampling.ULTRA_HIGH)
        metawear_lib.mbl_mw_baro_bmp280_write_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_set_filter(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x12\x03\x0c\x08', 4)

        metawear_lib.mbl_mw_baro_bmp280_set_iir_filter(self.config, BaroBmp280IirFilter.AVG_4)
        metawear_lib.mbl_mw_baro_bmp280_write_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_set_standby(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x12\x03\x0c\x60', 4)

        metawear_lib.mbl_mw_baro_bmp280_set_standby_time(self.config, BaroBmp280Standby.STANDBY_250MS)
        metawear_lib.mbl_mw_baro_bmp280_write_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_set_all_config(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x12\x03\x08\x90', 4)

        metawear_lib.mbl_mw_baro_bmp280_set_oversampling(self.config, BaroBmp280Oversampling.LOW_POWER)
        metawear_lib.mbl_mw_baro_bmp280_set_iir_filter(self.config, BaroBmp280IirFilter.AVG_16)
        metawear_lib.mbl_mw_baro_bmp280_set_standby_time(self.config, BaroBmp280Standby.STANDBY_500MS)
        metawear_lib.mbl_mw_baro_bmp280_write_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)



class TestBaroBmp280(unittest.TestCase):
    def test_start(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x12\x04\x01\x01', 4)

        metawear_lib.mbl_mw_baro_bmp280_start(command);
        self.assertEqual(command.raw, expected.raw)

    def test_stop(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x12\x04\x00\x00', 4)

        metawear_lib.mbl_mw_baro_bmp280_stop(command);
        self.assertEqual(command.raw, expected.raw)

    def test_get_pressure_data(self):
        response_bytes= create_string_buffer(b'\x12\x01\xd3\x35\x8b\x01', 6)
        response_float= c_float()
        expected= 101173.828125

        metawear_lib.mbl_mw_baro_bmp280_get_pressure_data(byref(response_float), response_bytes)
        self.assertAlmostEqual(response_float.value, expected)

    def test_get_altitude_data(self):
        response_bytes= create_string_buffer(b'\x12\x02\x1e\x1f\xfe\xff', 6)
        response_float= c_float()
        expected= -480.8828125

        metawear_lib.mbl_mw_baro_bmp280_get_altitude_data(byref(response_float), response_bytes)
        self.assertAlmostEqual(response_float.value, expected)
