from common import TestMetaWearBase
from ctypes import create_string_buffer
from cbindings import *
#from mbientlab.metawear.cbindings import *

class TestMultiChannelTemperatureMwr(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

    def test_configure_ext_thermistor(self):
        expected= [0x04, 0x02, 0x01, 0x00, 0x01, 0x00]

        self.libmetawear.mbl_mw_multi_chnl_temp_configure_ext_thermistor(self.board, MetaWearRChannel.EXT_THERMISTOR, 0, 1, 0)
        print("TestMultiChannelTemperatureMwr \n")
        self.assertListEqual(self.command, expected)

    def test_read_temperature(self):
        channels= [
            [[0x04, 0x81, 0x00], MetaWearRChannel.ON_DIE],
            [[0x04, 0x81, 0x01], MetaWearRChannel.EXT_THERMISTOR]
        ]

        for chnl in channels:
             with self.subTest(channel=chnl):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, chnl[1])
                self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, None, self.sensor_data_handler)
                self.libmetawear.mbl_mw_datasignal_read(temp_signal)
                print("TestMultiChannelTemperatureMwr \n")
                self.assertListEqual(self.command, chnl[0])

    def test_read_temperature_silent(self):
        channels= [
            [[0x04, 0xc1, 0x00], MetaWearRChannel.ON_DIE],
            [[0x04, 0xc1, 0x01], MetaWearRChannel.EXT_THERMISTOR]
        ]

        for chnl in channels:
             with self.subTest(channel=chnl):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, chnl[1])
                self.libmetawear.mbl_mw_datasignal_read(temp_signal)
                print("TestMultiChannelTemperatureMwr \n")
                self.assertListEqual(self.command, chnl[0])

    def test_get_temperature_data(self):
        responses= [
            [create_string_buffer(b'\x04\x81\x00\x00\x01', 5), 32.0, MetaWearRChannel.ON_DIE],
            [create_string_buffer(b'\x04\x81\x01\xac\x00', 5), 21.5, MetaWearRChannel.EXT_THERMISTOR]
        ]

        for resp in responses:
             with self.subTest(response=resp):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, resp[2])
                self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, None, self.sensor_data_handler)
                self.notify_mw_char(resp[0])
                print("TestMultiChannelTemperatureMwr \n")
                self.assertAlmostEqual(self.data_float.value, resp[1])

    def test_get_num_channels(self):
        expected= 4
        result= self.libmetawear.mbl_mw_multi_chnl_temp_get_num_channels(self.board)

        print("TestMultiChannelTemperatureMwr \n")
        self.assertEqual(result, expected)

    def test_get_source(self):
        channels= [MetaWearRProChannel.ON_DIE, MetaWearRProChannel.BMP280, MetaWearRProChannel.ON_BOARD_THERMISTOR, MetaWearRProChannel.EXT_THERMISTOR]

        for i in range(0, len(channels)):
            with self.subTest(channel=i):
                source= self.libmetawear.mbl_mw_multi_chnl_temp_get_source(self.board, i)
                print("TestMultiChannelTemperatureMwr \n")
                self.assertEqual(source, channels[i])

    #def test_get_invalid_source(self):
    #    channels= [-1, 2]

    #    for chnl in channels:
    #        with self.subTest(channel=chnl):
    #            print("TestMultiChannelTemperatureMwr ", chnl)
    #            print("\n")
    #            self.assertEqual(TemperatureSource.INVALID, self.libmetawear.mbl_mw_multi_chnl_temp_get_source(self.board, chnl))

class TestMultiChannelTemperatureMwrPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD
        super().setUp()

    def test_configure_ext_thermistor(self):
        expected= [0x04, 0x02, 0x02, 0x00, 0x01, 0x00]

        self.libmetawear.mbl_mw_multi_chnl_temp_configure_ext_thermistor(self.board, MetaWearRProChannel.EXT_THERMISTOR, 0, 1, 0)
        print("TestMultiChannelTemperatureMwr \n")
        self.assertListEqual(self.command, expected)

    def test_read_temperature(self):
        channels= [
            [[0x04, 0x81, 0x00], MetaWearRProChannel.ON_DIE],
            [[0x04, 0x81, 0x01], MetaWearRProChannel.ON_BOARD_THERMISTOR],
            [[0x04, 0x81, 0x02], MetaWearRProChannel.EXT_THERMISTOR],
            [[0x04, 0x81, 0x03], MetaWearRProChannel.BMP280]
        ]

        for chnl in channels:
             with self.subTest(channel=chnl):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, chnl[1])
                self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, None, self.sensor_data_handler)
                self.libmetawear.mbl_mw_datasignal_read(temp_signal)
                print("TestMultiChannelTemperatureMwr \n")
                self.assertListEqual(self.command, chnl[0])

    def test_read_temperature_silent(self):
        channels= [
            [[0x04, 0xc1, 0x00], MetaWearRProChannel.ON_DIE],
            [[0x04, 0xc1, 0x01], MetaWearRProChannel.ON_BOARD_THERMISTOR],
            [[0x04, 0xc1, 0x02], MetaWearRProChannel.EXT_THERMISTOR],
            [[0x04, 0xc1, 0x03], MetaWearRProChannel.BMP280]
        ]

        for chnl in channels:
             with self.subTest(channel=chnl):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, chnl[1])
                self.libmetawear.mbl_mw_datasignal_read(temp_signal)
                print("TestMultiChannelTemperatureMwr \n")
                self.assertListEqual(self.command, chnl[0])

    def test_get_temperature_data(self):
        responses= [
            [create_string_buffer(b'\x04\x81\x00\xfe\x00', 5), 31.75, MetaWearRProChannel.ON_DIE],
            [create_string_buffer(b'\x04\x81\x01\xa8\x00', 5), 21.00, MetaWearRProChannel.ON_BOARD_THERMISTOR],
            [create_string_buffer(b'\x04\x81\x02\xac\xff', 5), -10.5, MetaWearRProChannel.EXT_THERMISTOR],
            [create_string_buffer(b'\x04\x81\x03\x00\x00', 5), 0, MetaWearRProChannel.BMP280]
        ]

        for resp in responses:
             with self.subTest(response=resp):
                temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, resp[2])
                self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, None, self.sensor_data_handler)
                self.notify_mw_char(resp[0])
                print("TestMultiChannelTemperatureMwr \n")
                self.assertAlmostEqual(self.data_float.value, resp[1])

    def test_get_num_channels(self):
        expected= 4
        result= self.libmetawear.mbl_mw_multi_chnl_temp_get_num_channels(self.board)

        self.assertEqual(result, expected)

    def test_get_source(self):
        channels= [TemperatureSource.NRF_DIE, TemperatureSource.PRESET_THERM, 
                TemperatureSource.EXT_THERM, TemperatureSource.BMP280]

        for i in range(0, len(channels)):
            with self.subTest(channel=i):
                source= self.libmetawear.mbl_mw_multi_chnl_temp_get_source(self.board, i)
                print("TestMultiChannelTemperatureMwr \n")
                self.assertEqual(source, channels[i])

    def test_get_invalid_source(self):
        channels= [-1, 4]

        for chnl in channels:
             with self.subTest(channel=chnl):
                print("TestMultiChannelTemperatureMwr \n")
                self.assertEqual(TemperatureSource.INVALID, self.libmetawear.mbl_mw_multi_chnl_temp_get_source(self.board, chnl))
