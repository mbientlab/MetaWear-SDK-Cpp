from common import TestMetaWearBase
from mbientlab.metawear import Gpio

class TestEvent(TestMetaWearBase):
    def test_schedule_read_temp(self):
        expected_cmds= [
            [0x0c, 0x02, 0x64, 0xb0, 0x05, 0xa2, 0x50, 0xb3, 0x1],
            [0x0a, 0x02, 0x0c, 0x06, 0x00, 0x04, 0x81, 0x01],
            [0x0a, 0x03, 0x01]
        ]

        self.libmetawear.mbl_mw_timer_create(self.board, 2718281828, 45904, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[0])
        self.libmetawear.mbl_mw_multi_chnl_temp_read_temperature(self.board, 1)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[0], self.commands_recorded_fn)

        self.assertEqual(self.command_history, expected_cmds)

    def test_schedule_read_gpio_adc(self):
        expected_cmds= [
            [0x0c, 0x02, 0x10, 0x2f, 0x0a, 0x00, 0xff, 0xff, 0x0],
            [0x0a, 0x02, 0x0c, 0x06, 0x01, 0x05, 0x87, 0x01],
            [0x0a, 0x03, 0x02]
        ]

        self.timerId= 1
        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 667408, 1, self.timer_signal_ready)
        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[0])
        self.libmetawear.mbl_mw_gpio_read_analog_input(self.board, 2, Gpio.ANALOG_READ_MODE_ADC)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[0], self.commands_recorded_fn)

        self.assertEqual(self.command_history, expected_cmds)

    def test_schedule_read_temp_gpio(self):
        expected_cmds= [
            [0x0c, 0x02, 0x96, 0x22, 0x0, 0x0, 0x5e, 0x49, 0x0],
            [0x0c, 0x02, 0xda, 0x79, 0x7d, 0x07, 0xff, 0xff, 0x1],
            [0x0a, 0x02, 0x0c, 0x06, 0x0, 0x05, 0x86, 0x01],
            [0x0a, 0x03, 0x03],
            [0x0a, 0x02, 0x0c, 0x06, 0x01, 0x04, 0x81, 0x01],
            [0x0a, 0x03, 0x0]
        ]

        self.libmetawear.mbl_mw_timer_create(self.board, 8854, 18782, 1, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 125663706, 0, self.timer_signal_ready)

        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[0])
        self.libmetawear.mbl_mw_gpio_read_analog_input(self.board, 3, Gpio.ANALOG_READ_MODE_ABS_REF)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[0], self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[1])
        self.libmetawear.mbl_mw_multi_chnl_temp_read_temperature(self.board, 0)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[1], self.commands_recorded_fn)

        self.assertEqual(self.command_history, expected_cmds)



class TestEventRemove(TestMetaWearBase):
    def test_remove_commands(self):
        expected_cmds= [
            [0x0a, 0x04, 0x0],
            [0x0a, 0x04, 0x1]
        ]

        self.libmetawear.mbl_mw_timer_create(self.board, 89875517, 8736, 0, self.timer_signal_ready)

        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[0])
        self.libmetawear.mbl_mw_gpio_read_digital_input(self.board, 3)
        self.libmetawear.mbl_mw_multi_chnl_temp_read_temperature(self.board, 2)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[0], self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_remove_commands(self.timerSignals[0])

        del self.command_history[0:5]
        self.assertEqual(self.command_history, expected_cmds)
