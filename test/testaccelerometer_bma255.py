from common import TestMetaWearBase
from mbientlab.metawear.core import CartesianFloat
from mbientlab.metawear.sensor import AccelerometerBma255, AccelerometerBosch
from ctypes import create_string_buffer

class TestAccBma255Config(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_DETECT_BOARD

        super().setUp()

    def test_set_range(self):
        tests= [
            {
                'expected': [0x03, 0x03, 0x0a, 0x03],
                'range': AccelerometerBosch.FSR_2G,
                'range_name': '2g'
            },
            {
                'expected': [0x03, 0x03, 0x0a, 0x05],
                'range': AccelerometerBosch.FSR_4G,
                'range_name': '4g'
            },
            {
                'expected': [0x03, 0x03, 0x0a, 0x08],
                'range': AccelerometerBosch.FSR_8G,
                'range_name': '8g'
            },
            {
                'expected': [0x03, 0x03, 0x0a, 0x0c],
                'range': AccelerometerBosch.FSR_16G,
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
                'odr': AccelerometerBma255.ODR_15_62HZ,
                'odr_name': '15.62Hz'
            },
            {
                'expected': [0x03, 0x03, 0x09, 0x03],
                'odr': AccelerometerBma255.ODR_31_26HZ,
                'odr_name': '31.26Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0a, 0x03],
                'odr': AccelerometerBma255.ODR_62_5HZ,
                'odr_name': '62.5Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0b, 0x03],
                'odr': AccelerometerBma255.ODR_125HZ,
                'odr_name': '125Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0c, 0x03],
                'odr': AccelerometerBma255.ODR_250HZ,
                'odr_name': '250Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0d, 0x03],
                'odr': AccelerometerBma255.ODR_500HZ,
                'odr_name': '500Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0e, 0x03],
                'odr': AccelerometerBma255.ODR_1000HZ,
                'odr_name': '1000Hz'
            },
            {
                'expected': [0x03, 0x03, 0x0f, 0x03],
                'odr': AccelerometerBma255.ODR_2000HZ,
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

        self.libmetawear.mbl_mw_acc_bma255_set_odr(self.board, AccelerometerBma255.ODR_250HZ)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccelerometerBosch.FSR_4G)
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

        self.libmetawear.mbl_mw_datasignal_subscribe(self.signal, self.sensor_data_handler)
        self.assertListEqual(self.command, expected)

    def test_unsubscribe_data(self):
        expected= [0x03, 0x04, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.signal)
        self.assertListEqual(self.command, expected)

    def test_handle_data(self):
        response= create_string_buffer(b'\x03\x04\xe1\xb3\xa1\x24\xb1\x2e', 8)
        expected= CartesianFloat(x= -4.7576, y= 2.2893, z= 2.9182)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccelerometerBosch.FSR_8G)
        status= self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_cartesian_float, expected)
