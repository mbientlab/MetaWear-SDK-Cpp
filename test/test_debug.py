from common import TestMetaWearBase, to_string_buffer
from mbientlab.metawear.cbindings import OverflowState

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

    def test_mbl_mw_debug_reset_after_gc(self):
        expected= [0xfe, 0x05]

        self.libmetawear.mbl_mw_debug_reset_after_gc(self.board)
        self.assertEqual(self.command, expected)

    def test_mbl_mw_debug_enable_power_save(self):
        expected= [0xfe, 0x07]

        self.libmetawear.mbl_mw_debug_enable_power_save(self.board)
        self.assertEqual(self.command, expected)

    def test_stack_overflow_disable(self):
        expected = [0xfe, 0x09, 0x00]

        self.libmetawear.mbl_mw_debug_set_stack_overflow_assertion(self.board, 0)
        self.assertEqual(self.command, expected)

    def test_stack_overflow(self):
        states= [0, 1]

        for s in states:
             with self.subTest(state=s):
                expected = [0xfe, 0x09, s]

                self.libmetawear.mbl_mw_debug_set_stack_overflow_assertion(self.board, s)
                self.assertEqual(self.command, expected)

    def test_handle_overflow_state(self):
        expected = OverflowState(length = 0x0c94, assert_en = 0)

        self.libmetawear.mbl_mw_debug_read_stack_overflow_state(self.board, None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0xfe, 0x89, 0x00, 0x94, 0x0c]))

        self.assertEqual(self.data, expected)

    def test_read_overflow_state(self):
        expected = [0xfe, 0x89]

        self.libmetawear.mbl_mw_debug_read_stack_overflow_state(self.board, None, self.sensor_data_handler)
        self.assertEqual(self.command, expected)

    def test_handle_schedule_queue_state(self):
        expected = [0x03, 0x02, 0x01, 0x00, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x1e]

        self.libmetawear.mbl_mw_debug_read_schedule_queue_usage(self.board, None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0xfe, 0x8a, 0x03, 0x02, 0x01, 0x00, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x1e]))

        self.assertEqual(self.data, expected)

    def test_read_schedule_queue_state(self):
        expected = [0xfe, 0x8a]

        self.libmetawear.mbl_mw_debug_read_schedule_queue_usage(self.board, None, self.sensor_data_handler)
        self.assertEqual(self.command, expected)