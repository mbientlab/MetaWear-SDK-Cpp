import os
import unittest
from mbientlab.metawear.accelerometer_bmi160 import *
from mbientlab.metawear.types import *
from ctypes import *

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestAccBmi160Config(unittest.TestCase):
    def setUp(self):
        self.config= metawear_lib.mbl_mw_acc_bmi160_create_config()

    def tearDown(self):
        metawear_lib.mbl_mw_acc_bmi160_free_config(self.config)

    def test_set_odr(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x03\x03\x27\x03', 4)

        metawear_lib.mbl_mw_acc_bmi160_set_odr(self.config, AccBmi160Odr.ODR_50HZ)
        metawear_lib.mbl_mw_acc_bmi160_write_acceleration_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_set_range(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x03\x03\x28\x0c', 4)

        metawear_lib.mbl_mw_acc_bmi160_set_range(self.config, AccBmi160Range.FSR_16G)
        metawear_lib.mbl_mw_acc_bmi160_write_acceleration_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_set_odr_and_range(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x03\x03\x29\x08', 4)

        metawear_lib.mbl_mw_acc_bmi160_set_odr(self.config, AccBmi160Odr.ODR_200HZ)
        metawear_lib.mbl_mw_acc_bmi160_set_range(self.config, AccBmi160Range.FSR_8G)
        metawear_lib.mbl_mw_acc_bmi160_write_acceleration_config(command, self.config)
        self.assertEqual(command.raw, expected.raw)

    def test_enable_acceleration_sampling(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x03\x02\x01\x00', 4)

        metawear_lib.mbl_mw_acc_bmi160_enable_acceleration_sampling(command)
        self.assertEqual(command.raw, expected.raw)

    def test_disable_acceleration_sampling(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x03\x02\x00\x01', 4)

        metawear_lib.mbl_mw_acc_bmi160_disable_acceleration_sampling(command)
        self.assertEqual(command.raw, expected.raw)

class TestBmi160AccelerationData(unittest.TestCase):
    def setUp(self):
        self.config= metawear_lib.mbl_mw_acc_bmi160_create_config()
        self.accel_data_source= metawear_lib.mbl_mw_acc_bmi160_get_acceleration_data_source
        self.accel_data_source.restype= c_long

    def tearDown(self):
        metawear_lib.mbl_mw_acc_bmi160_free_config(self.config)

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
        response_bytes= create_string_buffer(b'\x03\x04\xe7\x72\x64\xbe\x28\xda', 8)
        response_cs= CartesianShort() 
        expected= CartesianShort(x= 7181, y= -4100, z= -2365)

        metawear_lib.mbl_mw_acc_bmi160_set_range(self.config, AccBmi160Range.FSR_8G)
        metawear_lib.mbl_mw_acc_bmi160_get_acceleration_data_mg(byref(response_cs), self.config, response_bytes)
        self.assertEqual(response_cs, expected)

    def test_get_acceleration_data_g(self):
        response_bytes= create_string_buffer(b'\x03\x04\x16\xc4\x94\xa2\x2a\xd0', 8)
        response_cf= CartesianFloat() 
        expected= CartesianFloat(x= -1.872, y= -2.919, z= -1.495)

        metawear_lib.mbl_mw_acc_bmi160_set_range(self.config, AccBmi160Range.FSR_4G)
        metawear_lib.mbl_mw_acc_bmi160_get_acceleration_data_g(byref(response_cf), self.config, response_bytes)
        self.assertEqual(response_cf, expected)
