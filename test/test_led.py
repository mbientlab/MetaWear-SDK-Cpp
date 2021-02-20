from ctypes import byref, create_string_buffer
from common import TestMetaWearBase
from cbindings import *
#from mbientlab.metawear.cbindings import *

class TestLedControl(TestMetaWearBase):
    def test_play(self):
        expected= [0x02, 0x01, 0x01]

        self.libmetawear.mbl_mw_led_play(self.board)
        self.assertEqual(self.command, expected)

    def test_autoplay(self):
        expected= [0x02, 0x01, 0x02]

        self.libmetawear.mbl_mw_led_autoplay(self.board)
        self.assertEqual(self.command, expected)

    def test_pause(self):
        expected= [0x02, 0x01, 0x00]

        self.libmetawear.mbl_mw_led_pause(self.board)
        self.assertEqual(self.command, expected)

    def test_stop_clear(self):
        expected= [0x02, 0x02, 0x01]

        self.libmetawear.mbl_mw_led_stop_and_clear(self.board)
        self.assertEqual(self.command, expected)

    def test_stop_no_clear(self):
        expected= [0x02, 0x02, 0x00]

        self.libmetawear.mbl_mw_led_stop(self.board, 0)
        self.assertEqual(self.command, expected)

class TestLedPattern(TestMetaWearBase):
    def test_blink_pattern(self):
        expected= [0x02, 0x03, 0x00, 0x02, 0x1f, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0x0a]
        pattern= LedPattern(delay_time_ms= 5000, repeat_count= 10)

        self.libmetawear.mbl_mw_led_load_preset_pattern(byref(pattern), LedPreset.BLINK)
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.GREEN)

        self.assertEqual(self.command, expected)

    def test_solid_pattern(self):
        expected= [0x02, 0x03, 0x01, 0x02, 0x1f, 0x1f, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xE8, 0x03, 0x00, 0x00, 0x14]
        pattern= LedPattern(delay_time_ms= 10000, repeat_count= 20)

        self.libmetawear.mbl_mw_led_load_preset_pattern(byref(pattern), LedPreset.SOLID)
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.RED)

        self.assertEqual(self.command, expected)

    def test_pulse_pattern(self):
        expected= [0x02, 0x03, 0x02, 0x02, 0x1f, 0x00, 0xd5, 0x02, 0xf4, 0x01, 0xd5, 0x02, 0xd0, 0x07, 0x00, 0x00, 0x28]
        pattern= LedPattern(delay_time_ms= 12345, repeat_count= 40)

        self.libmetawear.mbl_mw_led_load_preset_pattern(byref(pattern), LedPreset.PULSE)
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.BLUE)

        self.assertEqual(self.command, expected)

class TestLedDelayedPattern(TestMetaWearBase):
    def setUp(self):
        self.metawear_r_services[0x2]= create_string_buffer(b'\x02\x80\x00\x01\x03\x00', 6)
        super().setUp()

    def test_blink_pattern(self):
        expected= [0x02, 0x03, 0x00, 0x02, 0x1f, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x88, 0x13, 0x0a]
        pattern= LedPattern(delay_time_ms= 5000, repeat_count= 10)

        self.libmetawear.mbl_mw_led_load_preset_pattern(byref(pattern), LedPreset.BLINK)
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.GREEN)

        self.assertEqual(self.command, expected)

    def test_solid_pattern(self):
        expected= [0x02, 0x03, 0x01, 0x02, 0x1f, 0x1f, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xE8, 0x03, 0x10, 0x27, 0x14]
        pattern= LedPattern(delay_time_ms= 10000, repeat_count= 20)

        self.libmetawear.mbl_mw_led_load_preset_pattern(byref(pattern), LedPreset.SOLID)
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.RED)

        self.assertEqual(self.command, expected)

    def test_pulse_pattern(self):
        expected= [0x02, 0x03, 0x02, 0x02, 0x1f, 0x00, 0xd5, 0x02, 0xf4, 0x01, 0xd5, 0x02, 0xd0, 0x07, 0x39, 0x30, 0x28]
        pattern= LedPattern(delay_time_ms= 12345, repeat_count= 40)

        self.libmetawear.mbl_mw_led_load_preset_pattern(byref(pattern), LedPreset.PULSE)
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.BLUE)

        self.assertEqual(self.command, expected)
