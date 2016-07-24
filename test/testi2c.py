from common import TestMetaWearBase
from ctypes import byref, create_string_buffer
from mbientlab.metawear.sensor import I2C

class TestI2cR(TestMetaWearBase):
    def test_read_who_am_i(self):
        expected= [0x0d, 0xc1, 0x1c, 0x0d, 0x0a, 0x01]

        parameters= I2C.ReadParameters(device_addr= 0x1c, register_addr= 0xd)
        signal= self.libmetawear.mbl_mw_i2c_get_data_signal(self.board, 1, 0xa)
        self.libmetawear.mbl_mw_datasignal_read_with_parameters(signal, byref(parameters))
        self.assertEqual(self.command, expected)

    def test_who_am_i_data(self):
        expected= [0x2a]
        response= create_string_buffer(b'\x0d\x81\x0a\x2a', 4)

        signal= self.libmetawear.mbl_mw_i2c_get_data_signal(self.board, 1, 0xa)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))

        self.assertEqual(self.data_byte_array, expected)
