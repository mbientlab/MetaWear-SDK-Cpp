from common import TestMetaWearBase

class TestGenericAccelerometerBmi160(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

    def test_acc_bmi160_set_odr_cmd(self):
        expected= [0x03, 0x03, 0x27, 0x03]

        self.libmetawear.mbl_mw_acc_set_odr(self.board, 55.0)
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_set_odr_value(self):
        expected= 50.0

        actual_odr= self.libmetawear.mbl_mw_acc_set_odr(self.board, 55.0)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertAlmostEqual(actual_odr, expected)

    def test_acc_bmi160_set_range_cmd(self):
        expected= [0x03, 0x03, 0x28, 0x0c]
 
        self.libmetawear.mbl_mw_acc_set_range(self.board, 14.75)
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_set_range_value(self):
        expected= 16.0 

        actual_range= self.libmetawear.mbl_mw_acc_set_range(self.board, 14.75)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertAlmostEqual(actual_range, expected)

    def test_acc_bmi160_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal, self.sensor_data_handler)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertAlmostEqual(self.command, expected)

    def test_acc_bmi160_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_acc_enable_acceleration_sampling(self.board)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_acc_disable_acceleration_sampling(self.board)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertListEqual(self.command, expected)

    def test_acc_bosch_module_valid(self):
        result= self.libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal(self.board)
        print("TestGenericAccelerometerBmi160 \n")
        self.assertNotEqual(result, None)