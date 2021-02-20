from common import TestMetaWearBase
from ctypes import byref, create_string_buffer
from cbindings import *
#from mbientlab.metawear.cbindings import *

class TestI2cR(TestMetaWearBase):
    def test_read_who_am_i(self):
        expected= [0x0d, 0xc1, 0x1c, 0x0d, 0x0a, 0x01]

        parameters= I2cReadParameters(device_addr= 0x1c, register_addr= 0xd)
        signal= self.libmetawear.mbl_mw_i2c_get_data_signal(self.board, 1, 0xa)
        self.libmetawear.mbl_mw_datasignal_read_with_parameters(signal, byref(parameters))
        self.assertEqual(self.command, expected)

    def test_who_am_i_data(self):
        expected= [0x2a]

        signal= self.libmetawear.mbl_mw_i2c_get_data_signal(self.board, 1, 0xa)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x0d\x81\x0a\x2a', 4))

        self.assertEqual(self.data_byte_array, expected)
