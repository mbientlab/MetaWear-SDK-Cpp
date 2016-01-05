import copy
from common import TestMetaWearBase
from mbientlab.metawear import AccelerometerMma8452q, CartesianFloat
from ctypes import create_string_buffer

class TestMma8452qConfiguration(TestMetaWearBase):
    def test_set_odr(self):
        expected= [0x03, 0x03, 0x00, 0x00, 0x20, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_mma8452q_set_odr(self.board, AccelerometerMma8452q.ODR_50HZ)
        self.libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_range(self):
        expected= [0x03, 0x03, 0x02, 0x00, 0x18, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_mma8452q_set_range(self.board, AccelerometerMma8452q.FSR_8G)
        self.libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_odr_and_range(self):
        expected= [0x03, 0x03, 0x01, 0x00, 0x10, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_mma8452q_set_odr(self.board, AccelerometerMma8452q.ODR_200HZ)
        self.libmetawear.mbl_mw_acc_mma8452q_set_range(self.board, AccelerometerMma8452q.FSR_4G)
        self.libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01]

        self.libmetawear.mbl_mw_acc_mma8452q_enable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00]

        self.libmetawear.mbl_mw_acc_mma8452q_disable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

class TestAccMma8452qAccelerationData(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        self.accel_data_signal= self.libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal(self.board)

#    def test_get_acceleration_data_mg(self):
#        response_bytes= create_string_buffer(b'\x03\x04\xb3\xff\x39\xfb\x62\x06')
#        response_cs= CartesianShort() 
#        expected= CartesianShort(x= -77, y= -1223, z= 1634)
#
#        libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_mg(byref(response_cs), response_bytes)
#        self.assertEqual(response_cs, expected)

    def test_get_acceleration_data_g(self):
        response= create_string_buffer(b'\x03\x04\x56\xfa\x05\xf6\x18\x03', 8)
        expected= CartesianFloat(x= -1.450, y= -2.555, z= 0.792)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_handle_response(self.board, response.raw, len(response))

        self.assertEqual(self.data_cartesian_float, expected)

    def test_stream_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_end_stream_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)
