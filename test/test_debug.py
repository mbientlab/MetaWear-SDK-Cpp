from common import TestMetaWearBase

class TestDebug(TestMetaWearBase):
    def test_mbl_mw_debug_reset(self):
        expected= [0xfe, 0x01]

        self.libmetawear.mbl_mw_debug_reset(self.board)
        self.assertListEqual(self.command, expected)

    def test_mbl_mw_debug_jump_to_bootloader(self):
        expected= [0xfe, 0x02]

        self.libmetawear.mbl_mw_debug_jump_to_bootloader(self.board)
        self.assertEqual(self.command, expected)

    def test_mbl_mw_debug_disconnect(self):
        expected= [0xfe, 0x06]

        self.libmetawear.mbl_mw_debug_disconnect(self.board)
        self.assertEqual(self.command, expected)

    def bml_mw_debug_reset_after_gc(self):
        expected= [0xfe, 0x05]

        self.libmetawear.mbl_mw_debug_reset_after_gc(self.board)
        self.assertEqual(self.command, expected)

