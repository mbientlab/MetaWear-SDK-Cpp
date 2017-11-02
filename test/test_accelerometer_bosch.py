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
                    self.libmetawear.mbl_mw_datasignal_subscribe(orientation, self.sensor_data_handler)
                    
                    self.notify_mw_char(to_string_buffer(test['response']))
                    self.assertEqual(self.data_int32.value, test['expected'])

class TestAccelerometerBmi160Orientation(AccelerometerBoschBase.TestOrientation):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

class TestAccelerometerBma255rientation(AccelerometerBoschBase.TestOrientation):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()