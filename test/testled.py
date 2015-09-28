import os
import unittest
from mbientlab.metawear.led import *
from ctypes import CDLL, create_string_buffer

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestLedControl(unittest.TestCase):
    def setUp(self):
        self.command= create_string_buffer(3)

    def test_play(self):
        expected= create_string_buffer(b'\x02\x01\x01', 3)

        metawear_lib.mbl_mw_led_play(self.command)
        self.assertEqual(self.command.raw, expected.raw)

    def test_autoplay(self):
        expected= create_string_buffer(b'\x02\x01\x02', 3)

        metawear_lib.mbl_mw_led_autoplay(self.command)
        self.assertEqual(self.command.raw, expected.raw)

    def test_pause(self):
        expected= create_string_buffer(b'\x02\x01\x00', 3)

        metawear_lib.mbl_mw_led_pause(self.command)
        self.assertEqual(self.command.raw, expected.raw)

    def test_stop_clear(self):
        expected= create_string_buffer(b'\x02\x02\x01', 3)

        metawear_lib.mbl_mw_led_stop(self.command, 1)
        self.assertEqual(self.command.raw, expected.raw)

    def test_stop_no_clear(self):
        expected= create_string_buffer(b'\x02\x02\x00', 3)

        metawear_lib.mbl_mw_led_stop(self.command, 0)
        self.assertEqual(self.command.raw, expected.raw)

class TestLedPattern(unittest.TestCase):
    def setUp(self):
        self.command= create_string_buffer(17)

    def tearDown(self):
        metawear_lib.mbl_mw_led_free_pattern(self.config)

    def test_blink_pattern(self):
        expected= create_string_buffer(b'\x02\x03\x00\x02\x1f\x00\x00\x00\x32\x00\x00\x00\xf4\x01\x00\x00\x0a', 17)

        self.config= metawear_lib.mbl_mw_led_create_preset_pattern(Preset.BLINK)
        metawear_lib.mbl_mw_led_set_repeat_count(self.config, 10)
        metawear_lib.mbl_mw_led_write_pattern(self.command, self.config, Color.GREEN)

        self.assertEqual(self.command.raw, expected.raw)

    def test_solid_pattern(self):
        expected= create_string_buffer(b'\x02\x03\x01\x02\x1f\x1f\x00\x00\xf4\x01\x00\x00\xE8\x03\x00\x00\x14', 17)

        self.config= metawear_lib.mbl_mw_led_create_preset_pattern(Preset.SOLID)
        metawear_lib.mbl_mw_led_set_repeat_count(self.config, 20)
        metawear_lib.mbl_mw_led_write_pattern(self.command, self.config, Color.RED)

        self.assertEqual(self.command.raw, expected.raw)

    def test_pulse_pattern(self):
        expected= create_string_buffer(b'\x02\x03\x02\x02\x1f\x00\xd5\x02\xf4\x01\xd5\x02\xd0\x07\x00\x00\x28', 17)

        self.config= metawear_lib.mbl_mw_led_create_preset_pattern(Preset.PULSE)
        metawear_lib.mbl_mw_led_set_repeat_count(self.config, 40)
        metawear_lib.mbl_mw_led_write_pattern(self.command, self.config, Color.BLUE)

        self.assertEqual(self.command.raw, expected.raw)
