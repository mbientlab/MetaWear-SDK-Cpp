from common import TestMetaWearBase

class TestTimer(TestMetaWearBase):
    def test_create_timer(self):
        expected= [0x0c, 0x02, 0x45, 0x0c, 0x00, 0x00, 0x3B, 0x0, 0x0]

        self.libmetawear.mbl_mw_timer_create(self.board, 3141, 59, 1, self.timer_signal_ready)
        self.assertEqual(self.command, expected)

    def test_create_timer_indefinite(self):
        expected= [0x0c, 0x02, 0xe8, 0x03, 0x00, 0x00, 0xff, 0xff, 0x01]

        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 1000, 0, self.timer_signal_ready)
        self.assertEqual(self.command, expected)

    def test_start(self):
        expected= [0x0c, 0x03, 0x0]

        self.libmetawear.mbl_mw_timer_create(self.board, 1000, 10, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_start(self.timerSignals[0])
        self.assertEqual(self.command, expected)

    def test_stop(self):
        expected= [0x0c, 0x04, 0x5]

        self.timerId= 5
        self.libmetawear.mbl_mw_timer_create(self.board, 1000, -1, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_stop(self.timerSignals[0])
        self.assertEqual(self.command, expected)

    def test_remove(self):
        expected_cmds= [
            [0x0c, 0x04, 0x7],
            [0x0c, 0x05, 0x7]
        ]

        self.timerId= 7
        self.libmetawear.mbl_mw_timer_create(self.board, 667408, -1, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_remove(self.timerSignals[0])

        # Ignore the add timer command
        self.command_history.pop(0)
        self.assertEqual(self.command_history, expected_cmds)
