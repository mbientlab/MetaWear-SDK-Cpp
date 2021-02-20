from common import TestMetaWearBase
from cbindings import *
#from mbientlab.metawear.cbindings import *

class TestNeoPixelInit(TestMetaWearBase):
    def test_init_slow_strand(self):
        expected= [0x06, 0x01, 0x01, 0x03, 0x00, 0x1e]

        self.libmetawear.mbl_mw_neopixel_init_slow_strand(self.board, 1, 0, 30, NeoPixelColorOrdering.WS2811_GBR)
        self.assertEqual(self.command, expected)

    def test_init_fast_strand(self):
        expected= [0x06, 0x01, 0x02, 0x05, 0x01, 0x3c]

        self.libmetawear.mbl_mw_neopixel_init_fast_strand(self.board, 2, 1, 60, NeoPixelColorOrdering.WS2811_RBG)
        self.assertEqual(self.command, expected)

    def test_free_strand(self):
        expected= [0x06, 0x06, 0x02]

        self.libmetawear.mbl_mw_neopixel_free_strand(self.board, 2)
        self.assertEqual(self.command, expected)

class TestNeoPixelRotate(TestMetaWearBase):
    def test_rotate(self):
        expected= [0x06, 0x05, 0x00, 0x01, 0x4b, 0xE8, 0x03]

        self.libmetawear.mbl_mw_neopixel_rotate(self.board, 0, 75, 1000, NeoPixelRotDirection.AWAY)
        self.assertEqual(self.command, expected)

    def test_rotate_away(self):
        expected= [0x06, 0x05, 0x00, 0x01, 0x4b, 0xE8, 0x03]

        self.libmetawear.mbl_mw_neopixel_rotate(self.board, 0, 75, 1000, 143)
        self.assertEqual(self.command, expected)

    def test_rotate_indefinitely(self):
        expected= [0x06, 0x05, 0x01, 0x00, 0xff, 0xfa, 0x00]

        self.libmetawear.mbl_mw_neopixel_rotate_indefinitely(self.board, 1, 250, NeoPixelRotDirection.TOWARDS)
        self.assertEqual(self.command, expected)

    def test_stop_rotatation(self):
        expected= [0x06, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00]

        self.libmetawear.mbl_mw_neopixel_stop_rotation(self.board, 2)
        self.assertEqual(self.command, expected)

class TestNeoPixelColor(TestMetaWearBase):
    def test_clear(self):
        expected= [0x06, 0x03, 0x00, 0x0a, 0x2d]

        self.libmetawear.mbl_mw_neopixel_clear(self.board, 0, 10, 45)
        self.assertEqual(self.command, expected)

    def test_set_color(self):
        expected= [0x06, 0x04, 0x01, 0x18, 0xd5, 0x55, 0x6b]

        self.libmetawear.mbl_mw_neopixel_set_color(self.board, 1, 24, 213, 85, 107)
        self.assertEqual(self.command, expected)

    def test_enable_hold(self):
        expected= [0x06, 0x02, 0x02, 0x01]

        self.libmetawear.mbl_mw_neopixel_enable_hold(self.board, 2)
        self.assertEqual(self.command, expected)

    def test_enable_hold(self):
        expected= [0x06, 0x02, 0x00, 0x00]

        self.libmetawear.mbl_mw_neopixel_disable_hold(self.board, 0)
        self.assertEqual(self.command, expected)
