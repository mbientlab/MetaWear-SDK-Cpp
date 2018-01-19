from common import TestMetaWearBase

class TestGenericAccelerometerBmi160(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

    def test_acc_bmi160_set_odr_cmd(self):
        expected= [0x03, 0x03, 0x27, 0x03]

        self.libmetawear.mbl_mw_acc_set_odr(self.board, 55.0)
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_set_odr_value(self):
        expected= 50.0

        actual_odr= self.libmetawear.mbl_mw_acc_set_odr(self.board, 55.0)
        self.assertAlmostEqual(actual_odr, expected)

    def test_acc_bmi160_set_range_cmd(self):
        expected= [0x03, 0x03, 0x28, 0x0c]
 
        self.libmetawear.mbl_mw_acc_set_range(self.board, 14.75)
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_set_range_value(self):
        expected= 16.0 

        actual_range= self.libmetawear.mbl_mw_acc_set_range(self.board, 14.75)
        self.assertAlmostEqual(actual_range, expected)

    def test_acc_bmi160_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal, self.sensor_data_handler)
        self.assertAlmostEqual(self.command, expected)

    def test_acc_bmi160_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_acc_enable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bmi160_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_acc_disable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_mma8452q_module_null(self):
        result= self.libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal(self.board)
        self.assertEqual(result, None)

    def test_acc_bosch_module_valid(self):
        result= self.libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal(self.board)
        self.assertNotEqual(result, None)

class TestGenericAccelerometerMma8452q(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_R_BOARD
        super().setUp()

    def test_acc_mma8452q_set_odr_cmd(self):
        expected= [0x03, 0x03, 0x00, 0x00, 0x20, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_set_odr(self.board, 35.25)
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_mma8452q_set_odr_value(self):
        expected= 50.0 

        actual_odr= self.libmetawear.mbl_mw_acc_set_odr(self.board, 35.25)
        self.assertAlmostEqual(actual_odr, expected)

    def test_acc_mma8452q_set_range_cmd(self):
        expected= [0x03, 0x03, 0x02, 0x00, 0x18, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_set_range(self.board, 7.3333)
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_mma8452q_set_range_value(self):
        expected= 8.0

        actual_range= self.libmetawear.mbl_mw_acc_set_range(self.board, 7.3333)
        self.assertAlmostEqual(actual_range, expected)

    def test_acc_mma8452q_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_acc_mma8452q_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_acc_mma8452q_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01]

        self.libmetawear.mbl_mw_acc_enable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_mma8452q_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00]

        self.libmetawear.mbl_mw_acc_disable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bosch_module_null(self):
        result= self.libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal(self.board)
        self.assertEqual(result, None)

    def test_acc_mma8452q_module_valid(self):
        result= self.libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal(self.board)
        self.assertNotEqual(result, None)

class TestGenericAccelerometerBma255(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

    def test_acc_bma255_set_odr_cmd(self):
        expected= [0x03, 0x03, 0x0a, 0x03]

        self.libmetawear.mbl_mw_acc_set_odr(self.board, 50.0)
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bma255_set_odr_value(self):
        expected= 62.5

        actual_odr= self.libmetawear.mbl_mw_acc_set_odr(self.board, 50.0)
        self.assertAlmostEqual(actual_odr, expected)

    def test_acc_bma255_set_range_cmd(self):
        expected= [0x03, 0x03, 0xa, 0x0c]
 
        self.libmetawear.mbl_mw_acc_set_range(self.board, 20.0)
        self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bma255_set_range_value(self):
        expected= 16.0

        actual_range= self.libmetawear.mbl_mw_acc_set_range(self.board, 20.0)
        self.assertAlmostEqual(actual_range, expected)

    def test_acc_bma255_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_acc_bma255_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal, self.sensor_data_handler)
        self.assertAlmostEqual(self.command, expected)

    def test_acc_bma255_enable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_acc_enable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_bma255_disable_acceleration_sampling(self):
        expected= [0x03, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_acc_disable_acceleration_sampling(self.board)
        self.assertListEqual(self.command, expected)

    def test_acc_mma8452q_module_null(self):
        result= self.libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal(self.board)
        self.assertEqual(result, None)


    def test_acc_bosch_module_valid(self):
        result= self.libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal(self.board)
        self.assertNotEqual(result, None)
