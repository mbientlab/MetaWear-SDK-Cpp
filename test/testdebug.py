import os
import unittest
from ctypes import CDLL, create_string_buffer

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestDebug(unittest.TestCase):
    def test_mbl_mw_debug_reset(self):
        command= create_string_buffer(2)
        expected= create_string_buffer(b'\xfe\x01', 2)

        metawear_lib.mbl_mw_debug_reset(command)
        self.assertEqual(command.raw, expected.raw)

    def test_mbl_mw_debug_jump_to_bootloader(self):
        command= create_string_buffer(2)
        expected= create_string_buffer(b'\xfe\x02', 2)

        metawear_lib.mbl_mw_debug_jump_to_bootloader(command)
        self.assertEqual(command.raw, expected.raw)

    def test_mbl_mw_debug_disconnect(self):
        command= create_string_buffer(2)
        expected= create_string_buffer(b'\xfe\x06', 2)

        metawear_lib.mbl_mw_debug_disconnect(command)
        self.assertEqual(command.raw, expected.raw)

    def bml_mw_debug_reset_after_gc(self):
        command= create_string_buffer(2)
        expected= create_string_buffer(b'\xfe\x05', 2)

        metawear_lib.mbl_mw_debug_reset_after_gc(command)
        self.assertEqual(command.raw, expected.raw)

