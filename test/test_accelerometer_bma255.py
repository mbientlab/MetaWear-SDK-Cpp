from common import TestMetaWearBase
from mbientlab.metawear.cbindings import *
from ctypes import create_string_buffer

class TestAccBma255Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_DETECT_BOARD

        super().setUp()

    def test_set_range(self):
        tests= [
            {
                'expected': [0x03, 0x03, 0x0a, 0x03],
                'range': AccBoschRange._2G,
                'range_name': '2g'
            },
            {
                'expected': [0x03, 0x03, 0x0a, 0x05],
                'range': AccBoschRange._4G,
                'range_name': '4g'
            },
            {
                'expected': [0x03, 0x03, 0x0a, 0x08],
                'range': AccBoschRange._8G,
                'range_name': '8g'
            },
            {
                'expected': [0x03, 0x03, 0x0a, 0x0c],
                'range': AccBoschRange._16G,
                'range_name': '16g'
            }
        ]

        for test in tests:
            with self.subTest(range= test['range_name']):
                self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, test['range'])
                self.libmetawear.mbl_mw_acc_bosch_write_acceleration_config(self.board)
                self.assertEqual(self.command, test['expected'])

    def test_set_odr(self):
        tests= [
            {
                'expected': [0x03, 0x03, 0x08, 0x03],
                'odr': AccBma255Odr._15_62Hz,
                'odr_name': '15.62Hz'
            },
            {
                'expected': [0x03, 0x03, 0x09, 0x03],
                'odr': AccBma255Odr._31_26Hz,
                'odr_name': '31.26Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0a, 0x03],
                'odr': AccBma255Odr._62_5Hz,
                'odr_name': '62.5Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0b, 0x03],
                'odr': AccBma255Odr._125Hz,
                'odr_name': '125Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0c, 0x03],
                'odr': AccBma255Odr._250Hz,
                'odr_name': '250Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0d, 0x03],
                'odr': AccBma255Odr._500Hz,
                'odr_name': '500Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0e, 0x03],
                'odr': AccBma255Odr._1000Hz,
                'odr_name': '1000Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0f, 0x03],
                'odr': AccBma255Odr._2000Hz,
                'odr_name': '2000Hz'
            }
        ]

        for test in tests:
            with self.subTest(odr= test['odr_name']):
                self.libmetawear.mbl_mw_acc_bma255_set_odr(self.board, test['odr'])
                self.libmetawear.mbl_mw_acc_bosch_write_acceleration_config(self.board)
                self.assertEqual(self.command, test['expected'])

    def test_set_odr_and_range(self):
        expected= [0x03, 0x03, 0x0c, 0x05]

        self.libmetawear.mbl_mw_acc_bma255_set_odr(self.board, AccBma255Odr._250Hz)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccBoschRange._4G)
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

class TestAccBma255Data(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_DETECT_BOARD

        super().setUp()

        self.signal= self.libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal(self.board)

    def test_subscribe_data(self):
        expected= [0x03, 0x04, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe_data(self):
        expected= [0x03, 0x04, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.signal)
        self.assertListEqual(self.command, expected)

    def test_handle_data(self):
        response= create_string_buffer(b'\x03\x04\xe1\xb3\xa1\x24\xb1\x2e', 8)
        expected= CartesianFloat(x= -4.7576, y= 2.2893, z= 2.9182)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.signal, None, self.sensor_data_handler)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccBoschRange._8G)
        self.notify_mw_char(response)
        self.assertEqual(self.data_cartesian_float, expected)

    def test_handle_data_component(self):
        response= create_string_buffer(b'\x03\x04\xe1\xb3\xa1\x24\xb1\x2e', 8)
        tests= [
            {
                'expected': -4.7576,
                'index': Const.ACC_ACCEL_X_AXIS_INDEX,
                'name': 'x-axis'
            },
            {
                'expected': 2.2893,
                'index': Const.ACC_ACCEL_Y_AXIS_INDEX,
                'name': 'y-axis'
            },
            {
                'expected': 2.9182,
                'index': Const.ACC_ACCEL_Z_AXIS_INDEX,
                'name': 'z-axis'
            }
        ]

        for test in tests:
            with self.subTest(odr= test['name']):
                acc_component = self.libmetawear.mbl_mw_datasignal_get_component(self.signal, test['index'])
                self.libmetawear.mbl_mw_datasignal_subscribe(acc_component, None, self.sensor_data_handler)
                self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccBoschRange._8G)
                self.notify_mw_char(response)

                self.assertAlmostEqual(self.data_float.value, test['expected'], delta = 0.0001)

class TestBma255HighFreqAccdata(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

        self.accel_data_signal= self.libmetawear.mbl_mw_acc_bosch_get_packed_acceleration_data_signal(self.board)
        
    def sensorDataHandler(self, context, data):
        super().sensorDataHandler(context, data)

        self.cartesian_float_values.append(self.data_cartesian_float)

    def test_handle_packed_data(self):
        response = create_string_buffer(b'\x03\x1c\x31\x26\x55\xf9\x65\x77\x39\x29\x89\xdb\xfd\x7f\x95\x3d\x61\xc1\xf1\x7f', 20)
        expected_values = [CartesianFloat(x = 1.1935, y = -0.2084, z = 3.7311), CartesianFloat(x = 1.2882, y = -1.1395, z = 3.9996), CartesianFloat(x = 1.9244, y = -1.9569, z = 3.9982)]

        self.cartesian_float_values= []
        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, None, self.sensor_data_handler)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccBoschRange._4G)
        self.notify_mw_char(response)
        self.assertEqual(self.cartesian_float_values, expected_values)

    def test_subscribe(self):
        expected= [0x03, 0x1c, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe(self):
        expected= [0x03, 0x1c, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)
