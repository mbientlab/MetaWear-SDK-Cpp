from common import TestMetaWearBase
from ctypes import create_string_buffer

class TestTimer(TestMetaWearBase):
    def test_create_timer(self):
        expected= [0x0c, 0x02, 0x45, 0x0c, 0x00, 0x00, 0x3B, 0x0, 0x0]

        self.libmetawear.mbl_mw_timer_create(self.board, 3141, 59, 1, None, self.timer_signal_ready)
        self.events["timer"].wait()

        self.assertEqual(self.command, expected)

    def test_create_timer_indefinite(self):
        expected= [0x0c, 0x02, 0xe8, 0x03, 0x00, 0x00, 0xff, 0xff, 0x01]

        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 1000, 0, None, self.timer_signal_ready)
        self.events["timer"].wait()

        self.assertEqual(self.command, expected)

    def test_start(self):
        expected= [0x0c, 0x03, 0x0]

        self.libmetawear.mbl_mw_timer_create(self.board, 1000, 10, 0, None, self.timer_signal_ready)
        self.events["timer"].wait()
        self.libmetawear.mbl_mw_timer_start(self.timerSignals[0])

        self.assertEqual(self.command, expected)

    def test_stop(self):
        expected= [0x0c, 0x04, 0x5]

        self.timerId= 5
        self.libmetawear.mbl_mw_timer_create(self.board, 1000, -1, 0, None, self.timer_signal_ready)
        self.events["timer"].wait()

        self.libmetawear.mbl_mw_timer_stop(self.timerSignals[0])

        self.assertEqual(self.command, expected)

    def test_remove(self):
        expected_cmds= [
            [0x0c, 0x05, 0x7],
            [0x0a, 0x04, 0x0],
            [0x0a, 0x04, 0x1]
        ]

        di_signal= self.libmetawear.mbl_mw_gpio_get_digital_input_data_signal(self.board, 3)
        temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 1)

        self.timerId= 7
        self.libmetawear.mbl_mw_timer_create(self.board, 667408, -1, 0, None, self.timer_signal_ready)
        self.events["timer"].wait()

        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[0])
        self.libmetawear.mbl_mw_datasignal_read(di_signal)
        self.libmetawear.mbl_mw_datasignal_read(temp_signal)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[0], None, self.commands_recorded_fn)
        self.events["event"].wait()

        self.libmetawear.mbl_mw_timer_remove(self.timerSignals[0])

        # Ignore the add timer and events commands
        del self.command_history[0:5]
        self.assertEqual(self.command_history, expected_cmds)

class TestTimerTimeout(TestMetaWearBase):
    def commandLogger(self, context, board, writeType, characteristic, command, length):
        if (command[0] == 0xc and command[1] == 0x2):
            self.notify_mw_char(create_string_buffer(b'\x0c\x00', 2))
        else:
            super().commandLogger(context, board, writeType, characteristic, command, length)

    def test_timeout(self):
        self.libmetawear.mbl_mw_timer_create(self.board, 667408, -1, 0, None, self.timer_signal_ready)
        self.events["timer"].wait()

        self.assertIsNone(self.timerSignals[0])
