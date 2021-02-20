import copy
from common import TestMetaWearBase
from ctypes import create_string_buffer
#from mbientlab.metawear.cbindings import *
from cbindings import *

class TestGyroBmi160Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

    def test_mbl_mw_gyro_bmi160_set_odr(self):
        expected= [0x13, 0x3, 0x29, 0x0]

        self.libmetawear.mbl_mw_gyro_bmi160_set_odr(self.board, GyroBoschOdr._200Hz)
        self.libmetawear.mbl_mw_gyro_bmi160_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_mbl_mw_gyro_bmi160_set_fsr(self):
        expected= [0x13, 0x03, 0x28, 0x03]

        self.libmetawear.mbl_mw_gyro_bmi160_set_range(self.board, GyroBoschRange._250dps)
        self.libmetawear.mbl_mw_gyro_bmi160_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_mbl_mw_gyro_bmi160_set_all_config(self):
        expected= [0x13, 0x03, 0x27, 0x04]

        self.libmetawear.mbl_mw_gyro_bmi160_set_odr(self.board, GyroBoschOdr._50Hz)
        self.libmetawear.mbl_mw_gyro_bmi160_set_range(self.board, GyroBoschRange._125dps)
        self.libmetawear.mbl_mw_gyro_bmi160_write_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_gyro_active(self):
        expected= [0x13, 0x01, 0x01]

        self.libmetawear.mbl_mw_gyro_bmi160_start(self.board)
        self.assertListEqual(self.command, expected)

    def test_gyro_standby(self):
        expected= [0x13, 0x01, 0x00]

        self.libmetawear.mbl_mw_gyro_bmi160_stop(self.board)
        self.assertListEqual(self.command, expected)

    def test_enable_rotation_sampling(self):
        expected= [0x13, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_gyro_bmi160_enable_rotation_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_disable_rotation_sampling(self):
        expected= [0x13, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_gyro_bmi160_disable_rotation_sampling(self.board)
        self.assertListEqual(self.command, expected)

class TestGyroBmi160DataHandler(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

        self.gyro_rot_data_signal= self.libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal(self.board)

    def test_subscribe_rotation_data(self):
        expected= [0x13, 0x05, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.gyro_rot_data_signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe_rotation_data(self):
        expected= [0x13, 0x05, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.gyro_rot_data_signal)
        self.assertListEqual(self.command, expected)

    def test_rotation_data_handler(self):
        expected= CartesianFloat(x= 262.409, y= 499.497, z= -499.512)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.gyro_rot_data_signal, None, self.sensor_data_handler)
        self.libmetawear.mbl_mw_gyro_bmi160_set_range(self.board, GyroBoschRange._500dps)
        self.notify_mw_char(create_string_buffer(b'\x13\x05\x3e\x43\xff\x7f\x00\x80', 8))

        self.assertEqual(self.data_cartesian_float, expected)

    def test_rotation_data_component_handler(self):
        response= create_string_buffer(b'\x13\x05\x3e\x43\xff\x7f\x00\x80', 8)
        tests= [
            {
                'expected': 262.409,
                'index': Const.GYRO_ROTATION_X_AXIS_INDEX,
                'name': 'x-axis'
            },
            {
                'expected': 499.497,
                'index': Const.GYRO_ROTATION_Y_AXIS_INDEX,
                'name': 'y-axis'
            },
            {
                'expected': -499.512,
                'index': Const.GYRO_ROTATION_Z_AXIS_INDEX,
                'name': 'z-axis'
            }
        ]

        for test in tests:
            with self.subTest(odr= test['name']):
                rot_component = self.libmetawear.mbl_mw_datasignal_get_component(self.gyro_rot_data_signal, test['index'])
                self.libmetawear.mbl_mw_datasignal_subscribe(rot_component, None, self.sensor_data_handler)
                self.libmetawear.mbl_mw_gyro_bmi160_set_range(self.board, GyroBoschRange._500dps)
                self.notify_mw_char(response)
                
                self.assertAlmostEqual(self.data_float.value, test['expected'], delta = 0.001)

class TestGyroBmi160HighFreqDataHandler(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

        self.gyro_rot_data_signal= self.libmetawear.mbl_mw_gyro_bmi160_get_packed_rotation_data_signal(self.board)

    def sensorDataHandler(self, context, data):
        super().sensorDataHandler(context, data)

        self.cartesian_float_values.append(self.data_cartesian_float)

    def test_subscribe_rotation_data(self):
        expected= [0x13, 0x07, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.gyro_rot_data_signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe_rotation_data(self):
        expected= [0x13, 0x07, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.gyro_rot_data_signal)
        self.assertListEqual(self.command, expected)

    def test_rotation_data_handler(self):
        expected_values= [CartesianFloat(x= 164.177, y= 301.860, z= -265.122), CartesianFloat(x= 207.134, y= 296.982, z= -215.457), CartesianFloat(x= 254.055, y= 309.634, z= -152.591)]

        self.cartesian_float_values= []
        self.libmetawear.mbl_mw_datasignal_subscribe(self.gyro_rot_data_signal, None, self.sensor_data_handler)
        self.libmetawear.mbl_mw_gyro_bmi160_set_range(self.board, GyroBoschRange._1000dps)
        self.notify_mw_char(create_string_buffer(b'\x13\x07\x09\x15\xad\x26\x08\xde\x8a\x1a\x0d\x26\x65\xe4\x8d\x20\xac\x27\x73\xec', 20))

        self.assertEqual(self.cartesian_float_values, expected_values)





