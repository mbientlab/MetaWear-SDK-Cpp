import os
import unittest
from ctypes import CDLL, create_string_buffer, c_long

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestSwitchData(unittest.TestCase):
    def setUp(self):
        self.accel_data_source= metawear_lib.mbl_mw_switch_get_data_source
        self.accel_data_source.restype= c_long

    def test_switch_accumulator(self):
        command= create_string_buffer(8);
        expected= create_string_buffer(b'\x09\x02\x01\x01\xff\x00\x02\x00', 8);

        metawear_lib.mbl_mw_switch_get_data_source.restype= c_long
        next_data_attr= metawear_lib.mbl_mw_data_processor_accumulator(command, c_long(self.accel_data_source()))
        metawear_lib.mbl_mw_sensor_free_data_source(next_data_attr)
        self.assertEqual(command.raw, expected.raw)

    def test_switch_math(self):
        command= create_string_buffer(13);
        expected= create_string_buffer(b'\x09\x02\x01\x01\xff\x00\x09\x03\x04\x02\x00\x00\x00', 13);

        metawear_lib.mbl_mw_switch_get_data_source.restype= c_long
        next_data_attr= metawear_lib.mbl_mw_data_processor_math(command, c_long(self.accel_data_source()), 0, 4, 2)
        metawear_lib.mbl_mw_sensor_free_data_source(next_data_attr)
        self.assertEqual(command.raw, expected.raw)

    def test_switch_comparison(self):
        command= create_string_buffer(14);
        expected= create_string_buffer(b'\x09\x02\x01\x01\xff\x00\x06\x00\x00\x00\x01\x00\x00\x00', 14);

        metawear_lib.mbl_mw_switch_get_data_source.restype= c_long
        next_data_attr= metawear_lib.mbl_mw_data_processor_comparison(command, c_long(self.accel_data_source()), 0, 0, 1)
        metawear_lib.mbl_mw_sensor_free_data_source(next_data_attr)
        self.assertEqual(command.raw, expected.raw)

