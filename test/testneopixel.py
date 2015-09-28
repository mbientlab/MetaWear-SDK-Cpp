import os
import unittest
from mbientlab.metawear.neopixel import *
from ctypes import CDLL, create_string_buffer

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestNeoPixelInit(unittest.TestCase):
    def test_init_slow_strand(self):
        command= create_string_buffer(6)
        expected= create_string_buffer(b'\x06\x01\x01\x03\x00\x1e', 6)

        metawear_lib.mbl_mw_neopixel_init_slow_strand(command, 1, 0, 30, ColorOrdering.WS2811_GBR)
        self.assertEqual(command.raw, expected.raw)

    def test_init_fast_strand(self):
        command= create_string_buffer(6)
        expected= create_string_buffer(b'\x06\x01\x02\x05\x01\x3c', 6)

        metawear_lib.mbl_mw_neopixel_init_fast_strand(command, 2, 1, 60, ColorOrdering.WS2811_RBG)
        self.assertEqual(command.raw, expected.raw)

    def test_free_strand(self):
        command= create_string_buffer(3);
        expected= create_string_buffer(b'\x06\x06\x02', 3)

        metawear_lib.mbl_mw_neopixel_free_strand(command, 2)
        self.assertEqual(command.raw, expected.raw)

class TestNeoPixelRotate(unittest.TestCase):
    def setUp(self):
        self.command= create_string_buffer(7)

    def test_rotate(self):
        expected= create_string_buffer(b'\x06\x05\x00\x01\x4b\xE8\x03', 7)

        metawear_lib.mbl_mw_neopixel_rotate(self.command, 0, 75, 1000, RotationDirection.AWAY)
        self.assertEqual(self.command.raw, expected.raw)

    def test_rotate_away(self):
        expected= create_string_buffer(b'\x06\x05\x00\x01\x4b\xE8\x03', 7)

        metawear_lib.mbl_mw_neopixel_rotate(self.command, 0, 75, 1000, 143)
        self.assertEqual(self.command.raw, expected.raw)

    def test_rotate_indefinitely(self):
        expected= create_string_buffer(b'\x06\x05\x01\x00\xff\xfa\x00', 7)

        metawear_lib.mbl_mw_neopixel_rotate_indefinitely(self.command, 1, 250, RotationDirection.TOWARDS)
        self.assertEqual(self.command.raw, expected.raw)

    def test_stop_rotatation(self):
        expected= create_string_buffer(b'\x06\x05\x02\x00\x00\x00\x00', 7)

        metawear_lib.mbl_mw_neopixel_stop_rotation(self.command, 2)
        self.assertEqual(self.command.raw, expected.raw)

class TestNeoPixelColor(unittest.TestCase):
    def test_clear(self):
        command= create_string_buffer(5)
        expected= create_string_buffer(b'\x06\x03\x00\x0a\x2d', 5)

        metawear_lib.mbl_mw_neopixel_clear(command, 0, 10, 45)
        self.assertEqual(command.raw, expected.raw)

    def test_set_color(self):
        command= create_string_buffer(7)
        expected= create_string_buffer(b'\x06\x04\x01\x18\xd5\x55\x6b', 7)

        metawear_lib.mbl_mw_neopixel_set_color(command, 1, 24, 213, 85, 107)
        self.assertEqual(command.raw, expected.raw)

    def test_enable_hold(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x06\x02\x02\x01', 4)

        metawear_lib.mbl_mw_neopixel_enable_hold(command, 2)
        self.assertEqual(command.raw, expected.raw)

    def test_enable_hold(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x06\x02\x00\x00', 4)

        metawear_lib.mbl_mw_neopixel_disable_hold(command, 0)
        self.assertEqual(command.raw, expected.raw)
