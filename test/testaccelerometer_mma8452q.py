import os
import unittest
from mbientlab.metawear.accelerometer_mma8452q import *
from mbientlab.metawear.types import *
from ctypes import *

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestMma8452qConfiguration(unittest.TestCase):
    def setUp(self):
        self.config= metawear_lib.mbl_mw_acc_mma8452q_create_config()

    def tearDown(self):
        metawear_lib.mbl_mw_acc_mma8452q_free_config(self.config)

    def test_set_odr(self):
        command= create_string_buffer(7)
        expected= create_string_buffer(b'\x03\x03\x00\x00\x20\x00\x00', 7)

        metawear_lib.mbl_mw_acc_mma8452q_set_odr(self.config, AccMma8452qOdr.ODR_50HZ)
        metawear_lib.mbl_mw_acc_mma8452q_write_acceleration_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_set_range(self):
        command= create_string_buffer(7)
        expected= create_string_buffer(b'\x03\x03\x02\x00\x18\x00\x00', 7)

        metawear_lib.mbl_mw_acc_mma8452q_set_range(self.config, AccMma8452qRange.FSR_8G)
        metawear_lib.mbl_mw_acc_mma8452q_write_acceleration_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_set_odr_and_range(self):
        command= create_string_buffer(7)
        expected= create_string_buffer(b'\x03\x03\x01\x00\x10\x00\x00', 7)

        metawear_lib.mbl_mw_acc_mma8452q_set_odr(self.config, AccMma8452qOdr.ODR_200HZ)
        metawear_lib.mbl_mw_acc_mma8452q_set_range(self.config, AccMma8452qRange.FSR_4G)
        metawear_lib.mbl_mw_acc_mma8452q_write_acceleration_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_enable_acceleration_sampling(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x03\x02\x01', 3)

        metawear_lib.mbl_mw_acc_mma8452q_enable_acceleration_sampling(command)
        self.assertEqual(command.raw, expected.raw)

    def test_disable_acceleration_sampling(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x03\x02\x00', 3)

        metawear_lib.mbl_mw_acc_mma8452q_disable_acceleration_sampling(command)
        self.assertEqual(command.raw, expected.raw)

class TestAccMma8452qAccelerationData(unittest.TestCase):
    def setUp(self):
        self.accel_data_source= metawear_lib.mbl_mw_acc_mma8452q_get_acceleration_data_source
        self.accel_data_source.restype= c_long

    def test_subscribe_acceleration_data(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x03\x04\x01', 3)

        metawear_lib.mbl_mw_sensor_subscribe(command, c_long(self.accel_data_source()))
        self.assertEqual(command.raw, expected.raw)

    def test_unsubscribe_acceleration_data(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x03\x04\x00', 3)

        metawear_lib.mbl_mw_sensor_unsubscribe(command, c_long(self.accel_data_source()))
        self.assertEqual(command.raw, expected.raw)

    def test_get_acceleration_data_mg(self):
        response_bytes= create_string_buffer(b'\x03\x04\xb3\xff\x39\xfb\x62\x06', 8)
        response_cs= CartesianShort() 
        expected= CartesianShort(x= -77, y= -1223, z= 1634)

        metawear_lib.mbl_mw_acc_mma8452q_get_acceleration_data_mg(byref(response_cs), response_bytes)
        self.assertEqual(response_cs, expected)

    def test_get_acceleration_data_g(self):
        response_bytes= create_string_buffer(b'\x03\x04\x56\xfa\x05\xf6\x18\x03', 8)
        response_cf= CartesianFloat() 
        expected= CartesianFloat(x= -1.450, y= -2.555, z= 0.792)

        metawear_lib.mbl_mw_acc_mma8452q_get_acceleration_data_g(byref(response_cf), response_bytes)
        self.assertEqual(response_cf, expected)
