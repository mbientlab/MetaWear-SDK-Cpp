from common import TestMetaWearBase
from ctypes import create_string_buffer

class TestI2cR(TestMetaWearBase):
    def test_read_who_am_i(self):
        expected= [0x0d, 0x81, 0x1c, 0x0d, 0x0a, 0x01]

        self.libmetawear.mbl_mw_i2c_read(self.board, 0x1c, 0xd, 1, 0xa)
        self.assertEqual(self.command, expected)

    def test_who_am_i_data(self):
        expected= [0x2a]
        response= create_string_buffer(b'\x0d\x81\x0a\x2a', 4)

        signal= self.libmetawear.mbl_mw_i2c_get_data_signal(self.board, 1, 0xa)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))

        self.assertEqual(self.data_byte_array, expected)
