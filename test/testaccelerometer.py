from common import TestMetaWearBase
from ctypes import c_float

class TestGenericAccelerometerBmi160(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

    def test_acc_bmi160_set_odr(self):
        expected= [0x03, 0x03, 0x27, 0x03]

        self.libmetawear.mbl_mw_acc_set_odr(self.board, c_float(55.))
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_set_range(self):
        expected= [0x03, 0x03, 0x28, 0x0c]

        self.libmetawear.mbl_mw_acc_set_range(self.board, c_float(14.75))
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_acc_enable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_acc_disable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_module_null(self):
        result= self.libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal(self.board)
        self.assertEqual(result, False)


    def test_acc_mma8452q_module_valid(self):
        result= self.libmetawear.mbl_mw_acc_bmi160_get_acceleration_data_signal(self.board)
        self.assertNotEqual(result, False)

class TestGenericAccelerometerMma8452q(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_R_BOARD
        super().setUp()

    def test_acc_mma8452q_set_odr(self):
        expected= [0x03, 0x03, 0x00, 0x00, 0x20, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_set_odr(self.board, c_float(35.25))
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_set_range(self):
        expected= [0x03, 0x03, 0x02, 0x00, 0x18, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_set_range(self.board, c_float(7.3333))
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01]

        self.libmetawear.mbl_mw_acc_enable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00]

        self.libmetawear.mbl_mw_acc_disable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_mma8452q_module_null(self):
        result= self.libmetawear.mbl_mw_acc_bmi160_get_acceleration_data_signal(self.board)
        self.assertEqual(result, False)

    def test_acc_bmi160_module_valid(self):
        result= self.libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal(self.board)
        self.assertNotEqual(result, False)
