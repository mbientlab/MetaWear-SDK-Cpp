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
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_cartesian_float, expected)

    def test_subscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe_acceleration_data(self):
        expected= [0x03, 0x04, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)

class TestBmi160HighFreqAccData(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

        self.accel_data_signal= self.libmetawear.mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal(self.board)

    def sensorDataHandler(self, data):
        super().sensorDataHandler(data)

        self.cartesian_float_values.append(self.data_cartesian_float)

    def test_get_acceleration_data_g(self):
        response= create_string_buffer(b'\x03\x1c\x62\xb7\x53\x0d\xe9\xfd\x16\xd0\x4d\x0e\x57\x02\x8a\xff\xa1\x05\x0a\x01', 20)
        expected_values= [CartesianFloat(x= -4.539, y= 0.833, z= -0.131), CartesianFloat(x= -2.995, y= 0.894, z= 0.146), CartesianFloat(x= -0.029, y= 0.352, z= 0.065)]

        self.cartesian_float_values= []
        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccelerometerBosch.FSR_8G)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.cartesian_float_values, expected_values)

    def test_subscribe(self):
        expected= [0x03, 0x1c, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.accel_data_signal, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe(self):
        expected= [0x03, 0x1c, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.accel_data_signal)
        self.assertListEqual(self.command, expected)

class TestBmi160StepCounterSetup(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

    def test_set_mode(self):
        tests= [
            {
                'expected': [0x03, 0x18, 0x15, 0x0b],
                'mode': AccelerometerBmi160.STEP_COUNTER_MODE_NORMAL,
                'mode_name': 'normal'
            },
            {
                'expected': [0x03, 0x18, 0x2d, 0x08],
                'mode': AccelerometerBmi160.STEP_COUNTER_MODE_SENSITIVE,
                'mode_name': 'sensitive'
            },
            {
                'expected': [0x03, 0x18, 0x1d, 0x0f],
                'mode': AccelerometerBmi160.STEP_COUNTER_MODE_ROBUST,
                'mode_name': 'robust'
            }
        ]

        for test in tests:
            with self.subTest(mode= test['mode_name']):
                self.libmetawear.mbl_mw_acc_bmi160_set_step_counter_mode(self.board, test['mode'])
                self.libmetawear.mbl_mw_acc_bmi160_enable_step_counter(self.board)
                self.libmetawear.mbl_mw_acc_bmi160_write_step_counter_config(self.board)
                self.assertListEqual(self.command, test['expected'])

class TestBmi160StepCounterData(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

        self.step_counter_signal= self.libmetawear.mbl_mw_acc_bmi160_get_step_counter_data_signal(self.board)

    def test_get_step_count_value(self):
        response= create_string_buffer(b'\x03\x9a\x2b\x00', 4)
        expected= 43

        self.libmetawear.mbl_mw_datasignal_subscribe(self.step_counter_signal, self.sensor_data_handler)
        status= self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_uint32.value, expected)

    def test_read_step_counter(self):
        expected= [0x03, 0x9a]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.step_counter_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_read(self.step_counter_signal)
        self.assertListEqual(self.command, expected)

    def test_read_step_counter_silent(self):
        expected= [0x03, 0xda]

        self.libmetawear.mbl_mw_datasignal_read(self.step_counter_signal)
        self.assertListEqual(self.command, expected)

class TestBmi160StepDetectorData(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD

        super().setUp()

        self.step_detector_signal= self.libmetawear.mbl_mw_acc_bmi160_get_step_detector_data_signal(self.board)

    def test_subscribe_detector(self):
        expected= [0x3, 0x19, 0x1]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.step_detector_signal, self.sensor_data_handler)
        self.assertEqual(self.command, expected)

    def test_unsubscribe_detector(self):
        expected= [0x3, 0x19, 0x0]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.step_detector_signal, self.sensor_data_handler)
        self.assertEqual(self.command, expected)

    def test_enable_detector(self):
        expected= [0x03, 0x17, 0x01, 0x00]

        self.libmetawear.mbl_mw_acc_bmi160_enable_step_detector(self.board)
        self.assertEqual(self.command, expected)

    def test_disable_detector(self):
        expected= [0x03, 0x17, 0x00, 0x01]

        self.libmetawear.mbl_mw_acc_bmi160_disable_step_detector(self.board)
        self.assertEqual(self.command, expected)

    def test_get_detection(self):
        response= create_string_buffer(b'\x03\x19\x01')
        expected= 1

        self.libmetawear.mbl_mw_datasignal_subscribe(self.step_detector_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_uint32.value, expected)
