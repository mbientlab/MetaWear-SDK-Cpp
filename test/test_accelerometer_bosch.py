from common import TestMetaWearBase, to_string_buffer
from mbientlab.metawear.cbindings import *
from ctypes import create_string_buffer

class AccelerometerBoschBase:
    class TestOrientation(TestMetaWearBase):
        def test_enable(self):
            self.libmetawear.mbl_mw_acc_bosch_enable_orientation_detection(self.board)
            self.assertEqual(self.command, [0x03, 0x0f, 0x01, 0x00])

        def test_disable(self):
            self.libmetawear.mbl_mw_acc_bosch_disable_orientation_detection(self.board)
            self.assertEqual(self.command, [0x03, 0x0f, 0x00, 0x01])

        def test_handle_response(self):
            tests= [
                {
                    'expected': SensorOrientation.FACE_UP_LANDSCAPE_RIGHT,
                    'response': [0x03, 0x11, 0x07],
                    'name': 'fulr'
                },
                {
                    'expected': SensorOrientation.FACE_UP_PORTRAIT_UPRIGHT,
                    'response': [0x03, 0x11, 0x01],
                    'name': 'fupu'
                },
                {
                    'expected': SensorOrientation.FACE_UP_PORTRAIT_UPSIDE_DOWN,
                    'response': [0x03, 0x11, 0x03],
                    'name': 'fupd'
                },
                {
                    'expected': SensorOrientation.FACE_UP_LANDSCAPE_LEFT,
                    'response': [0x03, 0x11, 0x05],
                    'name': 'full'
                },
                {
                    'expected': SensorOrientation.FACE_DOWN_LANDSCAPE_RIGHT,
                    'response': [0x03, 0x11, 0x0f],
                    'name': 'fdlr'
                },
                {
                    'expected': SensorOrientation.FACE_DOWN_LANDSCAPE_LEFT,
                    'response': [0x03, 0x11, 0x0d],
                    'name': 'fdll'
                },
                {
                    'expected': SensorOrientation.FACE_DOWN_PORTRAIT_UPRIGHT,
                    'response': [0x03, 0x11, 0x09],
                    'name': 'fdpu'
                },
                {
                    'expected': SensorOrientation.FACE_DOWN_PORTRAIT_UPSIDE_DOWN,
                    'response': [0x03, 0x11, 0x0b],
                    'name': 'fdpd'
                }
            ]

            for test in tests:
                with self.subTest(odr= test['name']):
                    orientation = self.libmetawear.mbl_mw_acc_bosch_get_orientation_detection_data_signal(self.board)
                    self.libmetawear.mbl_mw_datasignal_subscribe(orientation, None, self.sensor_data_handler)
                    
                    self.notify_mw_char(to_string_buffer(test['response']))
                    self.assertEqual(self.data_int32.value, test['expected'])

    class TestAnyMotion(TestMetaWearBase):
        def test_enable(self):
            expected= [
                [0x03, 0x0a, 0x03, 0x2f, 0x14, 0x14] if self.boardType == TestMetaWearBase.METAWEAR_RPRO_BOARD else [0x03, 0x0a, 0x03, 0x2f, 0x14],
                [0x03, 0x09, 0x07, 0x00]
            ]

            self.libmetawear.mbl_mw_acc_set_range(self.board, 8.0)
            self.libmetawear.mbl_mw_acc_bosch_set_any_motion_count(self.board, 4)
            self.libmetawear.mbl_mw_acc_bosch_set_any_motion_threshold(self.board, 0.75)
            self.libmetawear.mbl_mw_acc_bosch_write_motion_config(self.board)
            
            self.libmetawear.mbl_mw_acc_bosch_enable_motion_detection(self.board)

            self.assertEqual(self.command_history, expected)

        def test_disable(self):
            self.libmetawear.mbl_mw_acc_bosch_disable_motion_detection(self.board)
            self.assertEqual(self.command, [0x03, 0x09, 0x00, 0x7f])

        def test_handle_response(self):
            tests= [
                {
                    'expected': BoschAnyMotion(sign = 1, x_axis_active = 0, y_axis_active = 0, z_axis_active = 1),
                    'response': [0x03, 0x0b, 0x22],
                    'name': '+z'
                },
                {
                    'expected': BoschAnyMotion(sign = 0, x_axis_active = 0, y_axis_active = 0, z_axis_active = 1),
                    'response': [0x03, 0x0b, 0x62],
                    'name': '-z'
                },
                {
                    'expected': BoschAnyMotion(sign = 0, x_axis_active = 0, y_axis_active = 1, z_axis_active = 0),
                    'response': [0x03, 0x0b, 0x52],
                    'name': '-y'
                },
                {
                    'expected': BoschAnyMotion(sign = 1, x_axis_active = 0, y_axis_active = 1, z_axis_active = 0),
                    'response': [0x03, 0x0b, 0x12],
                    'name': '+y'
                },
                {
                    'expected': BoschAnyMotion(sign = 1, x_axis_active = 1, y_axis_active = 0, z_axis_active = 0),
                    'response': [0x03, 0x0b, 0x0a],
                    'name': '+x'
                },
                {
                    'expected': BoschAnyMotion(sign = 0, x_axis_active = 1, y_axis_active = 0, z_axis_active = 0),
                    'response': [0x03, 0x0b, 0x4a],
                    'name': '-x'
                }
            ]

            for test in tests:
                with self.subTest(odr= test['name']):
                    orientation = self.libmetawear.mbl_mw_acc_bosch_get_motion_data_signal(self.board)
                    self.libmetawear.mbl_mw_datasignal_subscribe(orientation, None, self.sensor_data_handler)
                    
                    self.notify_mw_char(to_string_buffer(test['response']))
                    self.assertEqual(self.data, test['expected'])

    class TestTapDetector(TestMetaWearBase):
        def test_configure_single(self):
            expected = [0x03, 0x0d, 0x04, 0x04]

            self.libmetawear.mbl_mw_acc_set_range(self.board, 16.0)
            self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)

            self.libmetawear.mbl_mw_acc_bosch_set_threshold(self.board, 2.0)
            self.libmetawear.mbl_mw_acc_bosch_set_shock_time(self.board, AccBoschTapShockTime._50ms)
            self.libmetawear.mbl_mw_acc_bosch_write_tap_config(self.board)

            self.assertEqual(self.command, expected)

        def test_start_single(self):
            expected = [0x03, 0x0c, 0x02, 0x00]

            self.libmetawear.mbl_mw_acc_bosch_enable_tap_detection(self.board, 1, 0)
            self.assertEqual(self.command, expected)

        def test_stop_tap(self):
            expected = [0x03, 0x0c, 0x00, 0x03]

            self.libmetawear.mbl_mw_acc_bosch_disable_tap_detection(self.board)
            self.assertEqual(self.command, expected)

        def test_single_response(self):
            tap = self.libmetawear.mbl_mw_acc_bosch_get_tap_data_signal(self.board)
            self.libmetawear.mbl_mw_datasignal_subscribe(tap, None, self.sensor_data_handler)

            self.notify_mw_char(to_string_buffer([0x03, 0x0e, 0x12]))
            self.assertEqual(self.data, BoschTap(type = 0x2, sign = 0x0))

            self.notify_mw_char(to_string_buffer([0x03, 0x0e, 0x32]))
            self.assertEqual(self.data, BoschTap(type = 0x2, sign = 0x1))

        def test_configure_double(self):
            expected = [0x03, 0x0d, 0xc0, 0x04]

            self.libmetawear.mbl_mw_acc_set_range(self.board, 8.0)
            self.libmetawear.mbl_mw_acc_write_acceleration_config(self.board)

            self.libmetawear.mbl_mw_acc_bosch_set_threshold(self.board, 1.0)
            self.libmetawear.mbl_mw_acc_bosch_set_double_tap_window(self.board, AccBoschDoubleTapWindow._50ms)
            self.libmetawear.mbl_mw_acc_bosch_set_quiet_time(self.board, AccBoschTapQuietTime._20ms)
            self.libmetawear.mbl_mw_acc_bosch_set_shock_time(self.board, AccBoschTapShockTime._75ms)
            self.libmetawear.mbl_mw_acc_bosch_write_tap_config(self.board)

            self.assertEqual(self.command, expected)

        def test_start_double(self):
            expected = [0x03, 0x0c, 0x01, 0x00]

            self.libmetawear.mbl_mw_acc_bosch_enable_tap_detection(self.board, 0, 1)
            self.assertEqual(self.command, expected)

        def test_double_response(self):
            tap = self.libmetawear.mbl_mw_acc_bosch_get_tap_data_signal(self.board)
            self.libmetawear.mbl_mw_datasignal_subscribe(tap, None, self.sensor_data_handler)

            self.notify_mw_char(to_string_buffer([0x03, 0x0e, 0x11]))
            self.assertEqual(self.data, BoschTap(type = 0x1, sign = 0x0))

            self.notify_mw_char(to_string_buffer([0x03, 0x0e, 0x31]))
            self.assertEqual(self.data, BoschTap(type = 0x1, sign = 0x1))

class TestAccelerometerBmi160Orientation(AccelerometerBoschBase.TestOrientation):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

class TestAccelerometerBmi160AnyMotion(AccelerometerBoschBase.TestAnyMotion):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

class TestAccelerometerBmi160AnyMotion(AccelerometerBoschBase.TestTapDetector):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

class TestAccelerometerBma255Orientation(AccelerometerBoschBase.TestOrientation):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

class TestAccelerometerBma255AnyMotion(AccelerometerBoschBase.TestAnyMotion):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

class TestAccelerometerBma255Tap(AccelerometerBoschBase.TestTapDetector):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()