from common import TestMetaWearBase, to_string_buffer
from mbientlab.metawear.cbindings import *
from ctypes import create_string_buffer

class TestMma8452qConfiguration(TestMetaWearBase):
    def test_set_odr(self):
        expected= [0x03, 0x03, 0x00, 0x00, 0x20, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_mma8452q_set_odr(self.board, AccMma8452qOdr._50Hz)
        self.libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_range(self):
        expected= [0x03, 0x03, 0x02, 0x00, 0x18, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_mma8452q_set_range(self.board, AccMma8452qRange._8G)
        self.libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config(self.board)
        self.assertListEqual(self.command, expected)

    def test_set_odr_and_range(self):
        expected= [0x03, 0x03, 0x01, 0x00, 0x10, 0x00, 0x00]

        self.libmetawear.mbl_mw_acc_mma8452q_set_odr(self.board, AccMma8452qOdr._200Hz)
        self.libmetawear.mbl_mw_acc_mma8452q_set_range(self.board, AccMma8452qRange._4G)
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

    def test_get_acceleration_data_g(self):
        response= create_string_buffer(b'\x03\x04\x56\xfa\x05\xf6\x18\x03', 8)
        expected= CartesianFloat(x= -1.450, y= -2.555, z= 0.792)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, None, self.sensor_data_handler)
        self.notify_mw_char(response)

        self.assertEqual(self.data_cartesian_float, expected)

    def test_handle_data_component(self):
        response= create_string_buffer(b'\x03\x04\x56\xfa\x05\xf6\x18\x03', 8)
        tests= [
            {
                'expected': -1.450,
                'index': Const.ACC_ACCEL_X_AXIS_INDEX,
                'name': 'x-axis'
            },
            {
                'expected': -2.555,
                'index': Const.ACC_ACCEL_Y_AXIS_INDEX,
                'name': 'y-axis'
            },
            {
                'expected': 0.792,
                'index': Const.ACC_ACCEL_Z_AXIS_INDEX,
                'name': 'z-axis'
            }
        ]

        for test in tests:
            with self.subTest(odr= test['name']):
                acc_component = self.libmetawear.mbl_mw_datasignal_get_component(self.accel_data_signal, test['index'])
                self.libmetawear.mbl_mw_datasignal_subscribe(acc_component, None, self.sensor_data_handler)
                self.notify_mw_char(response)
                
                self.assertAlmostEqual(self.data_float.value, test['expected'], delta = 0.001)

    def test_stream_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_end_stream_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)

class TestAccMma8452qHighFreqAccData(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        self.accel_data_signal= self.libmetawear.mbl_mw_acc_mma8452q_get_packed_acceleration_data_signal(self.board)

    def sensorDataHandler(self, context, data):
        super().sensorDataHandler(context, data)

        self.cartesian_float_values.append(self.data_cartesian_float)

    def test_get_acceleration_data_g(self):
        response= create_string_buffer(b'\x03\x12\xdf\xed\x2b\x16\x15\xff\x7d\xfa\xed\x04\xc5\x0a\x31\xfb\xdb\xf8\x7b\xf2', 20)
        expected_values= [CartesianFloat(x= -4.641, y= 5.675, z= -0.235), CartesianFloat(x= -1.411, y= 1.261, z= 2.757), CartesianFloat(x= -1.231, y= -1.829, z= -3.461)]

        self.cartesian_float_values= []
        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, None, self.sensor_data_handler)
        self.notify_mw_char(response)

        self.assertEqual(self.cartesian_float_values, expected_values)

    def test_stream_acceleration_data(self):
        expected= [0x03, 0x12, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, None, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_end_stream_acceleration_data(self):
        expected= [0x03, 0x12, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)

class TestOrientation(TestMetaWearBase):
    def test_enable(self):
        expected= [
            [0x03, 0x09, 0x00, 0xc0, 0x0a, 0x44, 0x84],
            [0x03, 0x08, 0x01]
        ]

        self.libmetawear.mbl_mw_acc_mma8452q_enable_orientation_detection(self.board)
        self.assertEqual(self.command_history, expected)

    def test_disable(self):
        expected= [
            [0x03, 0x08, 0x00],
            [0x03, 0x09, 0x00, 0x80, 0x00, 0x44, 0x84]
        ]

        self.libmetawear.mbl_mw_acc_mma8452q_disable_orientation_detection(self.board)
        self.assertEqual(self.command_history, expected)

    def test_handle_response(self):
        tests= [
            {
                'expected': SensorOrientation.FACE_UP_LANDSCAPE_RIGHT,
                'response': [0x03, 0x0a, 0x84],
                'name': 'fulr'
            },
            {
                'expected': SensorOrientation.FACE_UP_PORTRAIT_UPRIGHT,
                'response': [0x03, 0x0a, 0x80],
                'name': 'fupu'
            },
            {
                'expected': SensorOrientation.FACE_UP_PORTRAIT_UPSIDE_DOWN,
                'response': [0x03, 0x0a, 0x82],
                'name': 'fupd'
            },
            {
                'expected': SensorOrientation.FACE_UP_LANDSCAPE_LEFT,
                'response': [0x03, 0x0a, 0x86],
                'name': 'full'
            },
            {
                'expected': SensorOrientation.FACE_DOWN_LANDSCAPE_RIGHT,
                'response': [0x03, 0x0a, 0x85],
                'name': 'fdlr'
            },
            {
                'expected': SensorOrientation.FACE_DOWN_LANDSCAPE_LEFT,
                'response': [0x03, 0x0a, 0x87],
                'name': 'fdll'
            },
            {
                'expected': SensorOrientation.FACE_DOWN_PORTRAIT_UPRIGHT,
                'response': [0x03, 0x0a, 0x81],
                'name': 'fdpu'
            },
            {
                'expected': SensorOrientation.FACE_DOWN_PORTRAIT_UPSIDE_DOWN,
                'response': [0x03, 0x0a, 0x03],
                'name': 'fdpd'
            }
        ]

        for test in tests:
            with self.subTest(odr= test['name']):
                orientation = self.libmetawear.mbl_mw_acc_mma8452q_get_orientation_detection_data_signal(self.board)
                self.libmetawear.mbl_mw_datasignal_subscribe(orientation, None, self.sensor_data_handler)
                
                self.notify_mw_char(to_string_buffer(test['response']))
                self.assertEqual(self.data_int32.value, test['expected'])

class TestHighPassFilter(TestMetaWearBase):
    ODR = [
        AccMma8452qOdr._800Hz,
        AccMma8452qOdr._400Hz,
        AccMma8452qOdr._200Hz,
        AccMma8452qOdr._100Hz,
        AccMma8452qOdr._50Hz,
        AccMma8452qOdr._12_5Hz,
        AccMma8452qOdr._6_25Hz,
        AccMma8452qOdr._1_56Hz
    ]
    ODR_MASKS = [
        0x0, 0x8, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38
    ]
    
    def test_configure_normal(self):
        params = [
            [[18.0, 15.9, 12.0, 3.125, 1.625, 2.125, 2.0, 1.5625], 0 ],
            [[10.0, 7.9, 5.75, 2.125, 1.375, 1.25, 0.9, 0.75], 1],
            [[5.0, 3.9, 1.75, 1.125, 0.375, 0.6, 0.74, 0.4], 2 ],
            [[2.75, 1.9, 0.75, 0.125, 0.374, 0.3, 0.20, 0.1], 3]
        ]
        tests = []
        for dr in TestHighPassFilter.ODR:
            for p in params:
                tests.append({
                    'odr': dr,
                    'cutoffs': p[0],
                    'mask': p[1]
                })

        for test in tests:
            with self.subTest(odr= test['odr']):
                self.libmetawear.mbl_mw_acc_mma8452q_set_odr(self.board, test['odr'])
                self.libmetawear.mbl_mw_acc_mma8452q_set_high_pass_cutoff(self.board, test['cutoffs'][test['odr']])
                self.libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config(self.board)

                self.assertEqual(self.command, [0x03, 0x03, 0x10, test['mask'], TestHighPassFilter.ODR_MASKS[test['odr']], 0x00, 0x00])