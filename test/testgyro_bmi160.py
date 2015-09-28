import os
import unittest
from mbientlab.metawear.gyro_bmi160 import *
from mbientlab.metawear.types import CartesianFloat
from ctypes import *

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestGyroBmi160Config(unittest.TestCase):
    def setUp(self):
        self.config= metawear_lib.mbl_mw_gyro_bmi160_create_config()

    def tearDown(self):
        metawear_lib.mbl_mw_gyro_bmi160_free_config(self.config)

    def test_mbl_mw_gyro_bmi160_set_odr(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x13\x03\x29\x00')

        metawear_lib.mbl_mw_gyro_bmi160_set_odr(self.config, GyroBmi160Odr.ODR_200HZ)
        metawear_lib.mbl_mw_gyro_bmi160_write_config(command, self.config)
        self.assertEqual(command.value, expected.value)

    def test_mbl_mw_gyro_bmi160_set_fsr(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x13\x03\x28\x03')

        metawear_lib.mbl_mw_gyro_bmi160_set_range(self.config, GyroBmi160Range.FSR_250DPS)
        metawear_lib.mbl_mw_gyro_bmi160_write_config(command, self.config)
        self.assertEqual(command.value, expected.value)

    def test_mbl_mw_gyro_bmi160_set_all_config(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x13\x03\x27\x04')

        metawear_lib.mbl_mw_gyro_bmi160_set_odr(self.config, GyroBmi160Odr.ODR_50HZ)
        metawear_lib.mbl_mw_gyro_bmi160_set_range(self.config, GyroBmi160Range.FSR_125DPS)
        metawear_lib.mbl_mw_gyro_bmi160_write_config(command, self.config)
        self.assertEqual(command.value, expected.value)

class TestGyroBmi160RotationData(unittest.TestCase):
    def setUp(self):
        self.gyro_rot_data_source= metawear_lib.mbl_mw_gyro_bmi160_get_rotation_data_source
        self.gyro_rot_data_source.restype= c_long

    def test_mbl_mw_gyro_bmi160_start(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x13\x01\x01')

        metawear_lib.mbl_mw_gyro_bmi160_start(command)
        self.assertEqual(command.value, expected.value)

    def test_mbl_mw_gyro_bmi160_stop(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x13\x01\x00')

        metawear_lib.mbl_mw_gyro_bmi160_stop(command)
        self.assertEqual(command.value, expected.value)

    def test_mbl_mw_gyro_bmi160_enable_rotation_sampling(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x13\x02\x01\x00')

        metawear_lib.mbl_mw_gyro_bmi160_enable_rotation_sampling(command)
        self.assertEqual(command.value, expected.value)

    def test_mbl_mw_gyro_bmi160_disable_rotation_sampling(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x13\x02\x00\x01')

        metawear_lib.mbl_mw_gyro_bmi160_disable_rotation_sampling(command)
        self.assertEqual(command.value, expected.value)

    def test_mbl_mw_gyro_bmi160_subscribe_rotation_data(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x13\x05\x01')

        metawear_lib.mbl_mw_sensor_subscribe(command, c_long(self.gyro_rot_data_source()))
        self.assertEqual(command.value, expected.value)

    def test_mbl_mw_gyro_bmi160_unsubscribe_rotation_data(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x13\x05\x00')

        metawear_lib.mbl_mw_sensor_unsubscribe(command, c_long(self.gyro_rot_data_source()))
        self.assertEqual(command.value, expected.value)

    def test_mbl_mw_gyro_bmi160_get_rotation_data(self):
        response_bytes= create_string_buffer(b'\x13\x05\x3e\x43\xff\x7f\x00\x80')
        response_cf= CartesianFloat()
        expected= CartesianFloat(x= 262.409, y= 499.497, z= -499.512)

        config= metawear_lib.mbl_mw_gyro_bmi160_create_config()
        metawear_lib.mbl_mw_gyro_bmi160_set_range(config, GyroBmi160Range.FSR_500DPS)
        metawear_lib.mbl_mw_gyro_bmi160_get_rotation_data(byref(response_cf), config, response_bytes)
        metawear_lib.mbl_mw_gyro_bmi160_free_config(config)

        self.assertEqual(response_cf, expected)

