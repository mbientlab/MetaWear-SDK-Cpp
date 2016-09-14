from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.sensor import MultiChannelTemperature

class TestMultiChannelTemperatureMwr(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_R_BOARD

        super().setUp()

    def test_configure_ext_thermistor(self):
        expected= [0x04, 0x02, 0x01, 0x00, 0x01, 0x00]

        self.libmetawear.mbl_mw_multi_chnl_temp_configure_ext_thermistor(self.board, MultiChannelTemperature.METAWEAR_R_CHANNEL_EXT_THERMISTOR, 0, 1, 0)
        self.assertListEqual(self.command, expected)

    def test_read_temperature(self):
        channels= [
            [[0x04, 0x81, 0x00], MultiChannelTemperature.METAWEAR_R_CHANNEL_ON_DIE],
            [[0x04, 0x81, 0x01], MultiChannelTemperature.METAWEAR_R_CHANNEL_EXT_THERMISTOR]
        ]

        for chnl in channels:
             with self.subTest(channel=chnl):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, chnl[1])
                self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, self.sensor_data_handler)
                self.libmetawear.mbl_mw_datasignal_read(temp_signal)
                self.assertListEqual(self.command, chnl[0])

    def test_read_temperature_silent(self):
        channels= [
            [[0x04, 0xc1, 0x00], MultiChannelTemperature.METAWEAR_R_CHANNEL_ON_DIE],
            [[0x04, 0xc1, 0x01], MultiChannelTemperature.METAWEAR_R_CHANNEL_EXT_THERMISTOR]
        ]

        for chnl in channels:
             with self.subTest(channel=chnl):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, chnl[1])
                self.libmetawear.mbl_mw_datasignal_read(temp_signal)
                self.assertListEqual(self.command, chnl[0])

    def test_get_temperature_data(self):
        responses= [
            [create_string_buffer(b'\x04\x81\x00\x00\x01', 5), 32.0, MultiChannelTemperature.METAWEAR_R_CHANNEL_ON_DIE],
            [create_string_buffer(b'\x04\x81\x01\xac\x00', 5), 21.5, MultiChannelTemperature.METAWEAR_R_CHANNEL_EXT_THERMISTOR]
        ]

        for resp in responses:
             with self.subTest(response=resp):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, resp[2])
                self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, self.sensor_data_handler)
                self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, resp[0].raw, len(resp[0]))
                self.assertAlmostEqual(self.data_float.value, resp[1])

    def test_get_num_channels(self):
        expected= 2
        result= self.libmetawear.mbl_mw_multi_chnl_temp_get_num_channels(self.board)

        self.assertEqual(result, expected)

    def test_get_source(self):
        channels= [MultiChannelTemperature.SOURCE_NRF_DIE, MultiChannelTemperature.METAWEAR_R_CHANNEL_EXT_THERMISTOR]

        for i in range(0, len(channels)):
            with self.subTest(channel=i):
                source= self.libmetawear.mbl_mw_multi_chnl_temp_get_source(self.board, i)
                self.assertEqual(source, channels[i])

    def test_get_invalid_source(self):
        channels= [-1, 2]

        for chnl in channels:
             with self.subTest(channel=chnl):
                self.assertEqual(MultiChannelTemperature.SOURCE_INVALID, self.libmetawear.mbl_mw_multi_chnl_temp_get_source(self.board, chnl))

class TestMultiChannelTemperatureMwrPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        super().setUp()

    def test_configure_ext_thermistor(self):
        expected= [0x04, 0x02, 0x02, 0x00, 0x01, 0x00]

        self.libmetawear.mbl_mw_multi_chnl_temp_configure_ext_thermistor(self.board, 
                MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_EXT_THERMISTOR, 0, 1, 0)
        self.assertListEqual(self.command, expected)

    def test_read_temperature(self):
        channels= [
            [[0x04, 0x81, 0x00], MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_DIE],
            [[0x04, 0x81, 0x01], MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR],
            [[0x04, 0x81, 0x02], MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_EXT_THERMISTOR],
            [[0x04, 0x81, 0x03], MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_BMP280]
        ]

        for chnl in channels:
             with self.subTest(channel=chnl):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, chnl[1])
                self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, self.sensor_data_handler)
                self.libmetawear.mbl_mw_datasignal_read(temp_signal)
                self.assertListEqual(self.command, chnl[0])

    def test_read_temperature_silent(self):
        channels= [
            [[0x04, 0xc1, 0x00], MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_DIE],
            [[0x04, 0xc1, 0x01], MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR],
            [[0x04, 0xc1, 0x02], MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_EXT_THERMISTOR],
            [[0x04, 0xc1, 0x03], MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_BMP280]
        ]

        for chnl in channels:
             with self.subTest(channel=chnl):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, chnl[1])
                self.libmetawear.mbl_mw_datasignal_read(temp_signal)
                self.assertListEqual(self.command, chnl[0])

    def test_get_temperature_data(self):
        responses= [
            [create_string_buffer(b'\x04\x81\x00\xfe\x00', 5), 31.75, MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_DIE],
            [create_string_buffer(b'\x04\x81\x01\xa8\x00', 5), 21.00, MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR],
            [create_string_buffer(b'\x04\x81\x02\xac\xff', 5), -10.5, MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_EXT_THERMISTOR],
            [create_string_buffer(b'\x04\x81\x03\x00\x00', 5), 0, MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_BMP280]
        ]

        for resp in responses:
             with self.subTest(response=resp):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, resp[2])
                self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, self.sensor_data_handler)
                self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, resp[0].raw, len(resp[0]))
                self.assertAlmostEqual(self.data_float.value, resp[1])

    def test_get_num_channels(self):
        expected= 4
        result= self.libmetawear.mbl_mw_multi_chnl_temp_get_num_channels(self.board)

        self.assertEqual(result, expected)

    def test_get_source(self):
        channels= [MultiChannelTemperature.SOURCE_NRF_DIE, MultiChannelTemperature.SOURCE_PRESET_THERM, 
                MultiChannelTemperature.SOURCE_EXT_THERM, MultiChannelTemperature.SOURCE_BMP280]

        for i in range(0, len(channels)):
            with self.subTest(channel=i):
                source= self.libmetawear.mbl_mw_multi_chnl_temp_get_source(self.board, i)
                self.assertEqual(source, channels[i])

    def test_get_invalid_source(self):
        channels= [-1, 4]

        for chnl in channels:
             with self.subTest(channel=chnl):
                self.assertEqual(MultiChannelTemperature.SOURCE_INVALID, self.libmetawear.mbl_mw_multi_chnl_temp_get_source(self.board, chnl))
