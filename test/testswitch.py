import os
import unittest
from mbientlab.metawear.sensor_defs import *
from ctypes import *

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestSwitchData(unittest.TestCase):
    def setUp(self):
        self.accel_data_source= metawear_lib.mbl_mw_switch_get_data_source
        self.accel_data_source.restype= c_long

    def test_mbl_mw_switch_subscribe(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x01\x01\x01', 3)

        metawear_lib.mbl_mw_sensor_subscribe(command, c_long(self.accel_data_source()))
        self.assertEqual(command.raw, expected.raw)

    def test_mbl_mw_switch_unsubscribe(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x01\x01\x00', 3)

        metawear_lib.mbl_mw_sensor_unsubscribe(command, c_long(self.accel_data_source()))
        self.assertEqual(command.raw, expected.raw)

    def test_mbl_mw_switch_get_data_pushed(self):
        data= create_string_buffer(1);
        response= create_string_buffer(b'\x01\x01\x01', 3)

        status= metawear_lib.mbl_mw_switch_get_data(data, response)
        self.assertEqual(status, 0)
        self.assertEqual(data.raw, b'\x01')

    def test_mbl_mw_switch_get_data_released(self):
        data= create_string_buffer(1);
        response= create_string_buffer(b'\x01\x01\x00', 3)

        status= metawear_lib.mbl_mw_switch_get_data(data, response)
        self.assertEqual(status, 0)
        self.assertEqual(data.raw, b'\x00')

    def test_mbl_mw_switch_get_data_invalid_00(self):
        data= create_string_buffer(1);
        response= create_string_buffer(b'\x01\x02\x01', 3)

        status= metawear_lib.mbl_mw_switch_get_data(response, data)
        self.assertEqual(status, Status.STATUS_INVALID_RESPONSE)

    def test_mbl_mw_switch_get_data_invalid_01(self):
        data= create_string_buffer(1);
        response= create_string_buffer(b'\x02\x01\x01', 3)

        status= metawear_lib.mbl_mw_switch_get_data(response, data)
        self.assertEqual(status, Status.STATUS_INVALID_RESPONSE)
