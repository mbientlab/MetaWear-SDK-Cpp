from common import TestMetaWearBase
from mbientlab.metawear.core import CartesianFloat
from mbientlab.metawear.sensor import AccelerometerBmi160, AccelerometerBosch
from ctypes import create_string_buffer

class TestAccBmi160Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

    def test_set_odr(self):
        tests= [
            {
                'expected': [0x03, 0x03, 0x81, 0x03],
                'odr': AccelerometerBmi160.ODR_0_78125HZ,
                'odr_name': '0.78125Hz'
            },
            {
                'expected': [0x03, 0x03, 0x82, 0x03],
                'odr': AccelerometerBmi160.ODR_1_5625HZ,
                'odr_name': '1.5625Hz'
            },
            {
                'expected': [0x03, 0x03, 0x83, 0x03],
                'odr': AccelerometerBmi160.ODR_3_125HZ,
                'odr_name': '3.125Hz'
            },
            {
                'expected': [0x03, 0x03, 0x84, 0x03],
                'odr': AccelerometerBmi160.ODR_6_25HZ,
                'odr_name': '6.25Hz'
            },
            {
                'expected': [0x03, 0x03, 0x25, 0x03],
                'odr': AccelerometerBmi160.ODR_12_5HZ,
                'odr_name': '12.5Hz'
            },
            {
                'expected': [0x03, 0x03, 0x26, 0x03],
                'odr': AccelerometerBmi160.ODR_25HZ,
                'odr_name': '25Hz'
            },
            {
                'expected': [0x03, 0x03, 0x27, 0x03],
                'odr': AccelerometerBmi160.ODR_50HZ,
                'odr_name': '50Hz'
            },
            {
                'expected': [0x03, 0x03, 0x28, 0x03],
                'odr': AccelerometerBmi160.ODR_100HZ,
                'odr_name': '100Hz'
            },
            {
                'expected': [0x03, 0x03, 0x29, 0x03],
                'odr': AccelerometerBmi160.ODR_200HZ,
                'odr_name': '200Hz'
            },
            {
                'expected': [0x03, 0x03, 0x2a, 0x03],
                'odr': AccelerometerBmi160.ODR_400HZ,
                'odr_name': '400Hz'
            },
            {
                'expected': [0x03, 0x03, 0x2b, 0x03],
                'odr': AccelerometerBmi160.ODR_800HZ,
                'odr_name': '800Hz'
            },
            {
                'expected': [0x03, 0x03, 0x2c, 0x03],
                'odr': AccelerometerBmi160.ODR_1600HZ,
                'odr_name': '1600Hz'
            }
        ]

        for test in tests:
            with self.subTest(odr= test['odr_name']):
                self.libmetawear.mbl_mw_acc_bmi160_set_odr(self.board, test['odr'])
                self.libmetawear.mbl_mw_acc_bosch_write_acceleration_config(self.board)
                self.assertListEqual(self.command, test['expected'])

    def test_set_range(self):
        expected= [0x03, 0x03, 0x28, 0x0c]

        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccelerometerBosch.FSR_16G)
        self.libmetawear.mbl_mw_acc_bosch_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_odr_and_range(self):
        expected= [0x03, 0x03, 0x29, 0x08]

        self.libmetawear.mbl_mw_acc_bmi160_set_odr(self.board, AccelerometerBmi160.ODR_200HZ)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccelerometerBosch.FSR_8G)
        self.libmetawear.mbl_mw_acc_bosch_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_acc_bosch_enable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_acc_bosch_disable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

class TestBmi160AccelerationData(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

        self.accel_data_signal= self.libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal(self.board)

    def test_get_acceleration_data_g(self):
        response= create_string_buffer(b'\x03\x04\x16\xc4\x94\xa2\x2a\xd0')
        expected= CartesianFloat(x= -1.872, y= -2.919, z= -1.495)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccelerometerBosch.FSR_4G)
        status= self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_cartesian_float, expected)

    def test_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)
