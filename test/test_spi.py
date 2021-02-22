from common import TestMetaWearBase
from ctypes import byref, cast, c_ubyte, c_void_p, create_string_buffer
from cbindings import SpiParameters, SpiFrequency, SpiMode
#from mbientlab.metawear.cbindings import SpiParameters, SpiFrequency, SpiMode

class TestSpiRPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RG_BOARD
        self.metawear_rg_services[0xd]= create_string_buffer(b'\x0d\x80\x00\x01', 4)

        super().setUp()

    def test_read_bmi160(self):
        expected= [0x0d, 0xc2, 0x0a, 0x00, 0x0b, 0x07, 0x76, 0xe4, 0xda]

        extra_data= (c_ubyte * 1)(0xda)
        parameters= SpiParameters(mode = SpiMode._3, frequency = SpiFrequency._8MHz, data = extra_data, data_length = len(extra_data), 
                slave_select_pin = 10, clock_pin = 0, mosi_pin = 11, miso_pin = 7, lsb_first = 0, use_nrf_pins = 1)
        signal= self.libmetawear.mbl_mw_spi_get_data_signal(self.board, 5, 0xe)
        self.libmetawear.mbl_mw_datasignal_read_with_parameters(signal, byref(parameters))
        self.assertEqual(self.command, expected)

    def test_bmi160_data(self):
        expected= [0x07, 0x30, 0x81, 0x0b, 0xc0]

        signal= self.libmetawear.mbl_mw_spi_get_data_signal(self.board, 5, 0xc)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x0d\x82\x0c\x07\x30\x81\x0b\xc0', 8))

        self.assertEqual(self.data_byte_array, expected)
