from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.cbindings import *

class TestMagnetometerBmm150(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_CPRO_BOARD

        super().setUp()

    def test_preset(self):
        tests= [
            {
                'expected': [[0x15, 0x04, 0x01, 0x02], [0x15, 0x03, 0x0]] ,
                'preset': MagBmm150Preset.LOW_POWER,
                'preset_name': 'low power',
                'offset': 0
            },
            {
                'expected': [[0x15, 0x04, 0x04, 0x0e], [0x15, 0x03, 0x00]], 
                'preset': MagBmm150Preset.REGULAR,
                'preset_name': 'regular',
                'offset': 2
            },
            {
                'expected': [[0x15, 0x04, 0x07, 0x1a], [0x15, 0x03, 0x00]], 
                'preset': MagBmm150Preset.ENHANCED_REGULAR,
                'preset_name': 'enhanced regular',
                'offset': 4
            },
            {
                'expected': [[0x15, 0x04, 0x17, 0x52], [0x15, 0x03, 0x05]], 
                'preset': MagBmm150Preset.HIGH_ACCURACY,
                'preset_name': 'high accuracy',
                'offset': 6
            },
        ]

        for test in tests:
            with self.subTest(preset= test['preset_name']):
                offset= test['offset']
                self.libmetawear.mbl_mw_mag_bmm150_set_preset(self.board, test['preset'])
                self.assertEqual(self.command_history[offset:offset + 2], test['expected'])

    def test_enable_b_field_sampling(self):
        expected= [0x15, 0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_mag_bmm150_enable_b_field_sampling(self.board)
        self.assertEqual(self.command, expected)

    def test_disable_b_field_sampling(self):
        expected= [0x15, 0x02, 0x00, 0x01]

        self.libmetawear.mbl_mw_mag_bmm150_disable_b_field_sampling(self.board)
        self.assertEqual(self.command, expected)

    def test_start(self):
        expected= [0x15, 0x01, 0x01]

        self.libmetawear.mbl_mw_mag_bmm150_start(self.board)
        self.assertEqual(self.command, expected)

    def test_stop(self):
        expected= [0x15, 0x01, 0x00]

        self.libmetawear.mbl_mw_mag_bmm150_stop(self.board)
        self.assertEqual(self.command, expected)

    def test_suspend(self):
        # last command should be read time register
        expected = [0x0b, 0x84]

        self.libmetawear.mbl_mw_mag_bmm150_suspend(self.board)
        self.assertEqual(self.command, expected)

class TestMagnetometerBmm150Rev2(TestMagnetometerBmm150):
    def setUp(self):
        self.metawear_cpro_services[0x15] = create_string_buffer(b'\x15\x80\x00\x02', 4)

        super().setUp()

    def test_preset(self):
        expected = [
            [0x15, 0x01, 0x00],
            [0x15, 0x04, 0x04, 0x0e], 
            [0x15, 0x03, 0x00]
        ]
        self.libmetawear.mbl_mw_mag_bmm150_set_preset(self.board, MagBmm150Preset.REGULAR)
        self.assertEqual(self.command_history, expected)

    def test_suspend(self):
        expected = [0x15, 0x01, 0x02]

        self.libmetawear.mbl_mw_mag_bmm150_suspend(self.board)
        self.assertEqual(self.command, expected)

class TestMagnetometerBmm150Data(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_CPRO_BOARD

        super().setUp()

    def test_b_field_subscribe(self):
        expected= [0x15, 0x05, 0x01]

        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)

    def test_b_field_component_subscribe(self):
        # expect only 1 subscribe & unsubscribe commands to be issued
        expected_cmds= [
            [0x15, 0x05, 0x01],
            [0x15, 0x05, 0x00]
        ]

        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)

        indices= [Const.MAG_BFIELD_X_AXIS_INDEX, Const.MAG_BFIELD_Y_AXIS_INDEX, Const.MAG_BFIELD_Z_AXIS_INDEX]
        for i in indices:
            component= self.libmetawear.mbl_mw_datasignal_get_component(signal, i)
            self.libmetawear.mbl_mw_datasignal_subscribe(component, None, self.sensor_data_handler)

        for i in indices:
            component= self.libmetawear.mbl_mw_datasignal_get_component(signal, i)
            self.libmetawear.mbl_mw_datasignal_unsubscribe(component)

        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal)

        self.assertEqual(self.command_history, expected_cmds)

    def test_b_field_unsubscribe(self):
        expected= [0x15, 0x05, 0x00]

        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(signal)

    def test_b_field_data(self):
        expected= CartesianFloat(x = -251.1250, y = 165.1875, z = 71.3125)
        response= create_string_buffer(b'\x15\x05\x4e\xf0\x53\x0a\x75\x04', 8)

        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.notify_mw_char(response)

        self.assertEqual(self.data_cartesian_float, expected)

    def test_b_field_component_data(self):
        response= create_string_buffer(b'\x15\x05\x4e\xf0\x53\x0a\x75\x04', 8)
        tests= [
            {
                'expected': -251.1250,
                'index': Const.MAG_BFIELD_X_AXIS_INDEX,
                'name': 'x-axis'
            },
            {
                'expected': 165.1875,
                'index': Const.MAG_BFIELD_Y_AXIS_INDEX,
                'name': 'y-axis'
            },
            {
                'expected': 71.3125,
                'index': Const.MAG_BFIELD_Z_AXIS_INDEX,
                'name': 'z-axis'
            }
        ]

        for test in tests:
            with self.subTest(odr= test['name']):
                signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
                signal_component = self.libmetawear.mbl_mw_datasignal_get_component(signal, test['index'])
                self.libmetawear.mbl_mw_datasignal_subscribe(signal_component, None, self.sensor_data_handler)
                self.notify_mw_char(response)

                self.assertAlmostEqual(self.data_float.value, test['expected'], delta = 0.001)

class TestMagnetometerPackedData(TestMetaWearBase):
    def setUp(self):
        self.metawear_cpro_services[0x15] = create_string_buffer(b'\x15\x80\x00\x01', 4)
        self.boardType = TestMetaWearBase.METAWEAR_CPRO_BOARD

        super().setUp()

        self.packed_bfield_data_signal = self.libmetawear.mbl_mw_mag_bmm150_get_packed_b_field_data_signal(self.board)

    def sensorDataHandler(self, context, data):
        super().sensorDataHandler(context, data)

        self.cartesian_float_values.append(self.data_cartesian_float)

    def test_subscribe_packed_bfield_data(self):
        expected = [0x15, 0x09, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.packed_bfield_data_signal, None, self.sensor_data_handler)
        self.assertEqual(self.command, expected)

    def test_unsubscribe_packed_bfield_data(self):
        expected = [0x15, 0x09, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.packed_bfield_data_signal)
        self.assertEqual(self.command, expected)

    def test_packed_bfield_data(self):
        response = create_string_buffer(b'\x15\x09\xb6\x0c\x72\xf7\x89\xee\xb6\x0b\x5a\xf8\x32\xee\xe6\x0a\xa2\xf7\x25\xef', 20);
        expected = [CartesianFloat(x = 203.375, y = -136.874, z = -279.437), 
                CartesianFloat(x = 187.375, y = -122.375, z = -284.874), 
                CartesianFloat(x = 174.375, y = -133.875, z = -269.687)]

        self.cartesian_float_values= []
        self.libmetawear.mbl_mw_datasignal_subscribe(self.packed_bfield_data_signal, None, self.sensor_data_handler)
        self.notify_mw_char(response)

        self.assertEqual(self.cartesian_float_values, expected)
