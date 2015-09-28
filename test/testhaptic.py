import os
import unittest
from ctypes import CDLL, create_string_buffer, c_float

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestHaptic(unittest.TestCase):
    def setUp(self):
        self.command= create_string_buffer(6)

    def test_start_motor(self):
        expected= create_string_buffer(b'\x08\x01\xf8\x88\x13\x00', 6)

        metawear_lib.mbl_mw_haptic_start_motor(self.command, c_float(100.0), 5000)
        self.assertEqual(self.command.raw, expected.raw)

    def test_start_buzzer(self):
        expected= create_string_buffer(b'\x08\x01\x7f\x88\x13\x01', 6)

        metawear_lib.mbl_mw_haptic_start_buzzer(self.command, 5000)
        self.assertEqual(self.command.raw, expected.raw)
