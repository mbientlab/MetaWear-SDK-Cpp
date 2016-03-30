from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.sensor import BarometerBosch

class BarometerBoschBase:
    class TestBarometerBoschConfig(TestMetaWearBase):
        def test_set_oversampling(self):
            tests= [
                {
                    'expected': [0x12, 0x03, 0x24, 0x00],
                    'oversampling': BarometerBosch.OVERSAMPLING_ULTRA_LOW_POWER,
                    'oversampling_name': 'ultra low power'
                },
                {
                    'expected': [0x12, 0x03, 0x28, 0x00],
                    'oversampling': BarometerBosch.OVERSAMPLING_LOW_POWER,
                    'oversampling_name': 'low power'
                },
                {
                    'expected': [0x12, 0x03, 0x2c, 0x00],
                    'oversampling': BarometerBosch.OVERSAMPLING_STANDARD,
                    'oversampling_name': 'standard'
                },
                {
                    'expected': [0x12, 0x03, 0x30, 0x00],
                    'oversampling': BarometerBosch.OVERSAMPLING_HIGH,
                    'oversampling_name': 'high'
                },
                {
                    'expected': [0x12, 0x03, 0x54, 0x00],
                    'oversampling': BarometerBosch.OVERSAMPLING_ULTRA_HIGH,
                    'oversampling_name': 'ultra high'
                }
            ]

            for test in tests:
                with self.subTest(oversampling= test['oversampling_name']):
                    self.libmetawear.mbl_mw_baro_bosch_set_oversampling(self.board, test['oversampling'])
                    self.libmetawear.mbl_mw_baro_bosch_write_config(self.board)
                    self.assertEqual(self.command, test['expected'])

        def test_set_filter(self):
            tests= [
                {
                    'expected': [0x12, 0x03, 0x2c, 0x00],
                    'filter': BarometerBosch.IIR_FILTER_OFF,
                    'iir_filter': 'off'
                },
                {
                    'expected': [0x12, 0x03, 0x2c, 0x04],
                    'filter': BarometerBosch.IIR_FILTER_AVG_2,
                    'iir_filter': 'avg 2'
                },
                {
                    'expected': [0x12, 0x03, 0x2c, 0x08],
                    'filter': BarometerBosch.IIR_FILTER_AVG_4,
                    'iir_filter': 'avg 4'
                },
                {
                    'expected': [0x12, 0x03, 0x2c, 0x0c],
                    'filter': BarometerBosch.IIR_FILTER_AVG_8,
                    'iir_filter': 'avg 8'
                },
                {
                    'expected': [0x12, 0x03, 0x2c, 0x10],
                    'filter': BarometerBosch.IIR_FILTER_AVG_16,
                    'iir_filter': 'avg 16'
                }
            ]

            for test in tests:
                with self.subTest(iir_filter= test['iir_filter']):
                    self.libmetawear.mbl_mw_baro_bosch_set_iir_filter(self.board, test['filter'])
                    self.libmetawear.mbl_mw_baro_bosch_write_config(self.board)
                    self.assertEqual(self.command, test['expected'])

        def test_start(self):
            expected= [0x12, 0x04, 0x01, 0x01]

            self.libmetawear.mbl_mw_baro_bosch_start(self.board);
            self.assertEqual(self.command, expected)

        def test_stop(self):
            expected= [0x12, 0x04, 0x00, 0x00]

            self.libmetawear.mbl_mw_baro_bosch_stop(self.board);
            self.assertEqual(self.command, expected)

    class TestBarometerBoschPressureData(TestMetaWearBase):
        def setUp(self):
            super().setUp()

            self.pa_data_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

        def test_get_pressure_data(self):
            response= create_string_buffer(b'\x12\x01\xd3\x35\x8b\x01', 6)
            expected= 101173.828125

            self.libmetawear.mbl_mw_datasignal_subscribe(self.pa_data_signal, self.sensor_data_handler)
            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
            self.assertAlmostEqual(self.data_float.value, expected)

        def test_pressure_subscribe(self):
            expected= [0x12, 0x1, 0x1]

            self.libmetawear.mbl_mw_datasignal_subscribe(self.pa_data_signal, self.sensor_data_handler);
            self.assertListEqual(self.command, expected)

        def test_pressure_unsubscribe(self):
            expected= [0x12, 0x1, 0x0]

            self.libmetawear.mbl_mw_datasignal_unsubscribe(self.pa_data_signal);
            self.assertListEqual(self.command, expected)

    class TestBarometerBoschAltitudeData(TestMetaWearBase):
        def setUp(self):
            super().setUp()

            self.m_data_signal= self.libmetawear.mbl_mw_baro_bosch_get_altitude_data_signal(self.board)

        def test_get_altitude_data(self):
            response= create_string_buffer(b'\x12\x02\x1e\x1f\xfe\xff', 6)
            expected= -480.8828125

            self.libmetawear.mbl_mw_datasignal_subscribe(self.m_data_signal, self.sensor_data_handler)
            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
            self.assertAlmostEqual(self.data_float.value, expected)

        def test_altitude_subscribe(self):
            expected= [0x12, 0x2, 0x1]

            self.libmetawear.mbl_mw_datasignal_subscribe(self.m_data_signal, self.sensor_data_handler);
            self.assertListEqual(self.command, expected)

        def test_altitude_unsubscribe(self):
            expected= [0x12, 0x2, 0x0]

            self.libmetawear.mbl_mw_datasignal_unsubscribe(self.m_data_signal);
            self.assertListEqual(self.command, expected)
