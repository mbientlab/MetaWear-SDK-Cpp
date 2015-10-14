from common import TestMetaWearBase
from mbientlab.metawear import AccelerometerBmi160, CartesianFloat
from ctypes import create_string_buffer

class TestAccBmi160Config(TestMetaWearBase):
    def test_set_odr(self):
        expected= [0x03, 0x03, 0x27, 0x03]

        self.libmetawear.mbl_mw_acc_bmi160_set_odr(self.board, AccelerometerBmi160.ODR_50HZ)
        self.libmetawear.mbl_mw_acc_bmi160_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_range(self):
        expected= [0x03, 0x03, 0x28, 0x0c]

        self.libmetawear.mbl_mw_acc_bmi160_set_range(self.board, AccelerometerBmi160.FSR_16G)
        self.libmetawear.mbl_mw_acc_bmi160_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_odr_and_range(self):
        expected= [0x03, 0x03, 0x29, 0x08]

        self.libmetawear.mbl_mw_acc_bmi160_set_odr(self.board, AccelerometerBmi160.ODR_200HZ)
        self.libmetawear.mbl_mw_acc_bmi160_set_range(self.board, AccelerometerBmi160.FSR_8G)
        self.libmetawear.mbl_mw_acc_bmi160_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_acc_bmi160_enable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_acc_bmi160_disable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

class TestBmi160AccelerationData(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        self.accel_data_signal= self.libmetawear.mbl_mw_acc_bmi160_get_acceleration_data_signal(self.board)

#    def test_get_acceleration_data_mg(self):
#        response_bytes= create_string_buffer(b'\x03\x04\xe7\x72\x64\xbe\x28\xda')
#        response_cs= CartesianShort() 
#        expected= CartesianShort(x= 7181, y= -4100, z= -2365)
#
#        libmetawear.mbl_mw_acc_bmi160_set_range(self.board, AccBmi160Range.FSR_8G)
#        libmetawear.mbl_mw_acc_bmi160_get_acceleration_data_mg(byref(response_cs), self.board, response_bytes)
#        self.assertListEqual(response_cs, expected)

    def test_get_acceleration_data_g(self):
        response= create_string_buffer(b'\x03\x04\x16\xc4\x94\xa2\x2a\xd0')
        expected= CartesianFloat(x= -1.872, y= -2.919, z= -1.495)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal)
        self.libmetawear.mbl_mw_acc_bmi160_set_range(self.board, AccelerometerBmi160.FSR_4G)
        status= self.libmetawear.mbl_mw_metawearboard_handle_response(self.board, response.raw, len(response))
        self.assertEqual(self.data_cartesian_float, expected)

    def test_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)
