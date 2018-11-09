from common import TestMetaWearBase, to_string_buffer
from ctypes import create_string_buffer
from mbientlab.metawear.cbindings import *
from threading import Event
import copy

class TestSensorFusion(TestMetaWearBase):
    def setUp(self):
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

    def test_received_data(self):
        tests = [
            {
                'expected' : CorrectedCartesianFloat(x = -3.3799, y = 15.9995, z = -15.9995, accuracy = Const.SENSOR_FUSION_CALIBRATION_ACCURACY_UNRELIABLE),
                'response' : create_string_buffer(b'\x19\x04\x20\x3e\x53\xc5\x0c\xfe\x79\x46\x0c\xfe\x79\xc6\x00', 15),
                'data' : SensorFusionData.CORRECTED_ACC,
                'name': 'corrected acc'
            },
            {
                'expected' : CorrectedCartesianFloat(x = 72.669, y = -56.096, z = 528.820, accuracy = Const.SENSOR_FUSION_CALIBRATION_ACCURACY_UNRELIABLE),
                'response' : create_string_buffer(b'\x19\x05\x7a\x56\x91\x42\xb4\x62\x60\xc2\x73\x34\x04\x44\x00', 15),
                'data' : SensorFusionData.CORRECTED_GYRO,
                'name': 'corrected gyro'
            },
            {
                'expected' : CorrectedCartesianFloat(x = 32.500, y = -14.800, z = 29.700, accuracy = Const.SENSOR_FUSION_CALIBRATION_ACCURACY_HIGH),
                'response' : create_string_buffer(b'\x19\x06\x00\x00\x02\x42\xcd\xcc\x6c\xc1\x9a\x99\xed\x41\x03', 15),
                'data' : SensorFusionData.CORRECTED_MAG,
                'name': 'corrected mag'
            },
            {
                'expected' : Quaternion(w = 0.940, x = -0.050, y = -0.154, z = -0.301),
                'response' : create_string_buffer(b'\x19\x07\x1b\x9b\x70\x3f\x8c\x5e\x4d\xbd\x07\x7f\x1d\xbe\x78\x02\x9a\xbe', 18),
                'data' : SensorFusionData.QUATERNION,
                'name': 'quaternion'
            },
            {
                'expected' : EulerAngles(heading = 24.747, pitch = -120.862, yaw = 24.747, roll = -33.046),
                'response' : create_string_buffer(b'\x19\x08\xb1\xf9\xc5\x41\x44\xb9\xf1\xc2\x1a\x2f\x04\xc2\xb1\xf9\xc5\x41', 18),
                'data' : SensorFusionData.EULER_ANGLE,
                'name': 'euler angle'
            },
            {
                'expected' : CartesianFloat(x = 0.042, y = 0.826, z = -0.562),
                'response' : create_string_buffer(b'\x19\x09\xee\x20\xd3\x3e\xb2\x93\x01\x41\x04\x59\xb0\xc0', 14),
                'data' : SensorFusionData.GRAVITY_VECTOR,
                'name': 'gravity'
            },
            {
                'expected' : CartesianFloat(x = 0.296, y = 1.439, z = -0.380),
                'response' : create_string_buffer(b'\x19\x0a\x2f\xca\x39\x40\x86\xd4\x61\x41\x80\x4c\x6e\xc0', 14),
                'data' : SensorFusionData.LINEAR_ACC,
                'name': 'linear acc'
            }
        ]

        for test in tests:
            with self.subTest(data = test['name']):
                signal = self.libmetawear.mbl_mw_sensor_fusion_get_data_signal(self.board, test['data'])
                self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)

                expected_value = test['expected']
                self.notify_mw_char(test['response'])

                self.assertEqual(self.data, expected_value)

    def test_sensor_control(self):
        data_sources = [
            SensorFusionData.CORRECTED_ACC, SensorFusionData.CORRECTED_GYRO, SensorFusionData.CORRECTED_MAG,
            SensorFusionData.QUATERNION, SensorFusionData.EULER_ANGLE,
            SensorFusionData.GRAVITY_VECTOR, SensorFusionData.LINEAR_ACC
        ]
        test_bases = [
            {
                'enable': 6,
                'mode': SensorFusionMode.NDOF,
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
                'mode': SensorFusionMode.IMU_PLUS,
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
                'mode': SensorFusionMode.COMPASS,
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
                'mode': SensorFusionMode.M4G,
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

    def test_read_calibration(self):
        signal = self.libmetawear.mbl_mw_sensor_fusion_calibration_state_data_signal(self.board)

        self.assertIsNone(signal)

class TestSensorFusionRev1(TestMetaWearBase):
    def setUp(self):
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD
        self.metawear_motion_r_services[0x19]= create_string_buffer(b'\x19\x80\x00\x01\x03\x00\x06\x00\x02\x00\x01\x00', 12)

        super().setUp()

    def test_read_calibration(self):
        signal = self.libmetawear.mbl_mw_sensor_fusion_calibration_state_data_signal(self.board)

        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_read(signal)
        self.assertEqual([0x19, 0x8b], self.command)


        expected_state = CalibrationState(
                accelerometer = Const.SENSOR_FUSION_CALIBRATION_ACCURACY_UNRELIABLE, 
                gyroscope = Const.SENSOR_FUSION_CALIBRATION_ACCURACY_LOW, 
                magnetometer = Const.SENSOR_FUSION_CALIBRATION_ACCURACY_MEDIUM
        )
        self.notify_mw_char(to_string_buffer([0x19, 0x8b, 0x00, 0x01, 0x02]))
        self.assertEqual(expected_state, self.data)

    def test_read_calibration_data(self):
        e = Event()
        actual = []
        def received(board, ctx, ptr):
            actual.append(ptr)
            e.set()

        fn_wrapper = FnVoid_VoidP_VoidP_CalibrationDataP(received)
        self.libmetawear.mbl_mw_sensor_fusion_read_calibration_data(self.board, None, fn_wrapper)
        e.wait()

        # unsupported for rev1
        self.assertFalse(bool(actual[0]))

    def test_write_calibration_data(self):
        data = CalibrationData(
            acc = (c_ubyte * 10)(),
            gyro = (c_ubyte * 10)(),
            mag = (c_ubyte * 10)()
        )

        self.libmetawear.mbl_mw_sensor_fusion_write_calibration_data(self.board, byref(data))

        # unsupported in rev 1, no commands
        self.assertEqual([], self.command_history)

class TestSensorFusionRev2(TestMetaWearBase):
    def setUp(self):
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD
        self.metawear_motion_r_services[0x19]= create_string_buffer(b'\x19\x80\x00\x02\x03\x00\x06\x00\x02\x00\x01\x00', 12)

        super().setUp()

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        TestMetaWearBase.commandLogger(self, context, board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            response = None
            if (command[0] == 0x19 and command[1] == 0x8c):
                response = to_string_buffer([0x19, 0x8c, 0xf6, 0xff, 0x00, 0x00, 0x0a, 0x00, 0xe8, 0x03, 0x03, 0x00])
            elif (command[0] == 0x19 and command[1] == 0x8d):
                response = to_string_buffer([0x19, 0x8d, 0x04, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00])
            elif(command[0] == 0x19 and command[1] == 0x8e):
                response = to_string_buffer([0x19, 0x8e, 0x66, 0x00, 0x17, 0xfd, 0x8a, 0xfc, 0x7f, 0x03, 0x01, 0x00])

            if (response != None):
                self.schedule_response(response)

    def test_read_calibration_data(self):
        expected = [
            [0xf6, 0xff, 0x00, 0x00, 0x0a, 0x00, 0xe8, 0x03, 0x03, 0x00],
            [0x04, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00],
            [0x66, 0x00, 0x17, 0xfd, 0x8a, 0xfc, 0x7f, 0x03, 0x01, 0x00]
        ]

        e = Event()
        actual = []
        def received(board, ctx, ptr):
            value = cast(ptr, POINTER(CalibrationData)).contents
            actual.append([value.acc[i] for i in range(0, 10)])
            actual.append([value.gyro[i] for i in range(0, 10)])
            actual.append([value.mag[i] for i in range(0, 10)])
            
            self.libmetawear.mbl_mw_memory_free(ptr)
            e.set()

        fn_wrapper = FnVoid_VoidP_VoidP_CalibrationDataP(received)
        self.libmetawear.mbl_mw_sensor_fusion_read_calibration_data(self.board, None, fn_wrapper)
        e.wait()

        self.assertEqual(expected, actual)

    def test_write_calibration_data(self):
        data = CalibrationData(
            acc = (c_ubyte * 10)(0xf6, 0xff, 0x00, 0x00, 0x0a, 0x00, 0xe8, 0x03, 0x03, 0x00),
            gyro = (c_ubyte * 10)(0x04, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00),
            mag = (c_ubyte * 10)(0x66, 0x00, 0x17, 0xfd, 0x8a, 0xfc, 0x7f, 0x03, 0x01, 0x00)
        )
        
        acc_cmd = [0x19, 0x0c, 0xf6, 0xff, 0x00, 0x00, 0x0a, 0x00, 0xe8, 0x03, 0x03, 0x00]
        gyro_cmd = [0x19, 0x0d, 0x04, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00]
        mag_cmd = [0x19, 0x0e, 0x66, 0x00, 0x17, 0xfd, 0x8a, 0xfc, 0x7f, 0x03, 0x01, 0x00]
        

        tests = [
            {
                'mode': SensorFusionMode.NDOF,
                'name': 'ndof',
                'expected': [
                    acc_cmd,
                    gyro_cmd,
                    mag_cmd
                ]
            },
            {
                'mode': SensorFusionMode.IMU_PLUS,
                'name': 'imu_plus',
                'expected': [
                    acc_cmd,
                    gyro_cmd
                ]
            },
            {
                'mode': SensorFusionMode.COMPASS,
                'name': 'compass',
                'expected': [
                    acc_cmd,
                    mag_cmd
                ]
            },
            {
                'mode': SensorFusionMode.M4G,
                'name': 'm4g',
                'expected': [
                    acc_cmd,
                    mag_cmd,
                ]
            }
        ]
        for test in tests:
            with self.subTest(mode = test['name']):
                self.command_history = []

                self.libmetawear.mbl_mw_sensor_fusion_set_mode(self.board, test['mode'])
                self.libmetawear.mbl_mw_sensor_fusion_write_calibration_data(self.board, byref(data))

                self.assertEqual(test['expected'], self.command_history)