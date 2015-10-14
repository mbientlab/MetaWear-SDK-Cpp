from common import TestMetaWearBase
from ctypes import c_float

class TestHaptic(TestMetaWearBase):
    def test_start_motor(self):
        expected= [0x08, 0x01, 0xf8, 0x88, 0x13, 0x00]

        self.libmetawear.mbl_mw_haptic_start_motor(self.board, c_float(100.0), 5000)
        self.assertListEqual(self.command, expected)

    def test_start_buzzer(self):
        expected= [0x08, 0x01, 0x7f, 0x4c, 0x1d, 0x01]

        self.libmetawear.mbl_mw_haptic_start_buzzer(self.board, 7500)
        self.assertListEqual(self.command, expected)
