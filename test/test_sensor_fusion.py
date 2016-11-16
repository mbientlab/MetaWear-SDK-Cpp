from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.core import CartesianFloat, CorrectedCartesianFloat, EulerAngle, Quaternion
from mbientlab.metawear.sensor import SensorFusion
import copy

class TestSensorFusion(TestMetaWearBase):
    def setUp(self):
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

    def test_received_data(self):
        tests = [
            {
                'expected' : CorrectedCartesianFloat(x = -3.3799, y = 15.9995, z = -15.9995, accuracy = SensorFusion.CALIBRATION_ACCURACY_UNRELIABLE),
                'response' : create_string_buffer(b'\x19\x04\x20\x3e\x53\xc5\x0c\xfe\x79\x46\x0c\xfe\x79\xc6\x00', 15),
                'data' : SensorFusion.DATA_CORRECTED_ACC,
                'name': 'corrected acc'
            },
            {
                'expected' : CorrectedCartesianFloat(x = 72.669, y = -56.096, z = 528.820, accuracy = SensorFusion.CALIBRATION_ACCURACY_UNRELIABLE),
                'response' : create_string_buffer(b'\x19\x05\x7a\x56\x91\x42\xb4\x62\x60\xc2\x73\x34\x04\x44\x00', 15),
                'data' : SensorFusion.DATA_CORRECTED_GYRO,
                'name': 'corrected gyro'
            },
            {
                'expected' : CorrectedCartesianFloat(x = 32.500, y = -14.800, z = 29.700, accuracy = SensorFusion.CALIBRATION_ACCURACY_HIGH),
                'response' : create_string_buffer(b'\x19\x06\x00\x00\x02\x42\xcd\xcc\x6c\xc1\x9a\x99\xed\x41\x03', 15),
                'data' : SensorFusion.DATA_CORRECTED_MAG,
                'name': 'corrected mag'
            },
            {
                'expected' : Quaternion(w = 0.940, x = -0.050, y = -0.154, z = -0.301),
                'response' : create_string_buffer(b'\x19\x07\x1b\x9b\x70\x3f\x8c\x5e\x4d\xbd\x07\x7f\x1d\xbe\x78\x02\x9a\xbe', 18),
                'data' : SensorFusion.DATA_QUATERION,
                'name': 'quaternion'
            },
            {
                'expected' : EulerAngle(heading = 24.747, pitch = -120.862, yaw = 24.747, roll = -33.046),
                'response' : create_string_buffer(b'\x19\x08\xb1\xf9\xc5\x41\x44\xb9\xf1\xc2\x1a\x2f\x04\xc2\xb1\xf9\xc5\x41', 18),
                'data' : SensorFusion.DATA_EULER_ANGLE,
                'name': 'euler angle'
            },
            {
                'expected' : CartesianFloat(x = 0.042, y = 0.826, z = -0.562),
                'response' : create_string_buffer(b'\x19\x09\xee\x20\xd3\x3e\xb2\x93\x01\x41\x04\x59\xb0\xc0', 14),
                'data' : SensorFusion.DATA_GRAVITY_VECTOR,
                'name': 'gravity'
            },
            {
                'expected' : CartesianFloat(x = 0.296, y = 1.439, z = -0.380),
                'response' : create_string_buffer(b'\x19\x0a\x2f\xca\x39\x40\x86\xd4\x61\x41\x80\x4c\x6e\xc0', 14),
                'data' : SensorFusion.DATA_LINEAR_ACC,
                'name': 'linear acc'
            }
        ]

        for test in tests:
            with self.subTest(data = test['name']):
                signal = self.libmetawear.mbl_mw_sensor_fusion_get_data_signal(self.board, test['data'])
                self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)

                response = test['response']
                expected_value = test['expected']
                self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))

                self.assertEqual(self.data, expected_value)

    def test_sensor_control(self):
        data_sources = [
            SensorFusion.DATA_CORRECTED_ACC, SensorFusion.DATA_CORRECTED_GYRO, SensorFusion.DATA_CORRECTED_MAG,
            SensorFusion.DATA_QUATERION, SensorFusion.DATA_EULER_ANGLE,
            SensorFusion.DATA_GRAVITY_VECTOR, SensorFusion.DATA_LINEAR_ACC
        ]
        test_bases = [
            {
                'enable': 6,
                'mode': SensorFusion.MODE_NDOF,
                'name': 'ndof',
                'expected': [
                    [0x03, 0x02, 0x01, 0x00],
                    [0x13, 0x02, 0x01, 0x00],
                    [0x15, 0x02, 0x01, 0x00],
                    [0x03, 0x01, 0x01],
                    [0x13, 0x01, 0x01],
                    [0x15, 0x01, 0x01],
                    [0x19, 0x03, 0x00, 0x00],
                    [0x19, 0x01, 0x01],
                    [0x19, 0x01, 0x00],
                    [0x19, 0x03, 0x00, 0x7f],
                    [0x03, 0x01, 0x00],
                    [0x13, 0x01, 0x00],
                    [0x15, 0x01, 0x00],
                    [0x03, 0x02, 0x00, 0x01],
                    [0x13, 0x02, 0x00, 0x01],
                    [0x15, 0x02, 0x00, 0x01]
                ]
            },
            {
                'enable': 4,
                'mode': SensorFusion.MODE_IMU_PLUS,
                'name': 'imu_plus',
                'expected': [
                    [0x03, 0x02, 0x01, 0x00],
                    [0x13, 0x02, 0x01, 0x00],
                    [0x03, 0x01, 0x01],
                    [0x13, 0x01, 0x01],
                    [0x19, 0x03, 0x00, 0x00],
                    [0x19, 0x01, 0x01],
                    [0x19, 0x01, 0x00],
                    [0x19, 0x03, 0x00, 0x7f],
                    [0x03, 0x01, 0x00],
                    [0x13, 0x01, 0x00],
                    [0x03, 0x02, 0x00, 0x01],
                    [0x13, 0x02, 0x00, 0x01]
                ]
            },
            {
                'enable': 4,
                'mode': SensorFusion.MODE_COMPASS,
                'name': 'compass',
                'expected': [
                    [0x03, 0x02, 0x01, 0x00],
                    [0x15, 0x02, 0x01, 0x00],
                    [0x03, 0x01, 0x01],
                    [0x15, 0x01, 0x01],
                    [0x19, 0x03, 0x00, 0x00],
                    [0x19, 0x01, 0x01],
                    [0x19, 0x01, 0x00],
                    [0x19, 0x03, 0x00, 0x7f],
                    [0x03, 0x01, 0x00],
                    [0x15, 0x01, 0x00],
                    [0x03, 0x02, 0x00, 0x01],
                    [0x15, 0x02, 0x00, 0x01]
                ]
            },
            {
                'enable': 4,
                'mode': SensorFusion.MODE_M4G,
                'name': 'm4g',
                'expected': [
                    [0x03, 0x02, 0x01, 0x00],
                    [0x15, 0x02, 0x01, 0x00],
                    [0x03, 0x01, 0x01],
                    [0x15, 0x01, 0x01],
                    [0x19, 0x03, 0x00, 0x00],
                    [0x19, 0x01, 0x01],
                    [0x19, 0x01, 0x00],
                    [0x19, 0x03, 0x00, 0x7f],
                    [0x03, 0x01, 0x00],
                    [0x15, 0x01, 0x00],
                    [0x03, 0x02, 0x00, 0x01],
                    [0x15, 0x02, 0x00, 0x01]
                ]
            }
        ]

        tests= []
        for base in test_bases:
            for source in data_sources:
                test = copy.deepcopy(base)
                test['expected'][test['enable']][2] |= (0x1 << source)
                test['source'] = source
                tests.append(test)

        for test in tests:
            with self.subTest(mode = test['name']):
                self.command_history = []
                self.libmetawear.mbl_mw_sensor_fusion_clear_enabled_mask(self.board)

                self.libmetawear.mbl_mw_sensor_fusion_set_mode(self.board, test['mode'])
                self.libmetawear.mbl_mw_sensor_fusion_enable_data(self.board, test['source'])
                self.libmetawear.mbl_mw_sensor_fusion_start(self.board)
                self.libmetawear.mbl_mw_sensor_fusion_stop(self.board)

                self.assertEqual(test['expected'], self.command_history)
