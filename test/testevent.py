from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.core import Fn_VoidPtr_Int, Status
from mbientlab.metawear.sensor import Gpio
import threading

class TestEvent(TestMetaWearBase):
    def test_schedule_read_temp(self):
        expected_cmds= [
            [0x0c, 0x02, 0x64, 0xb0, 0x05, 0xa2, 0x50, 0xb3, 0x1],
            [0x0a, 0x02, 0x0c, 0x06, 0x00, 0x04, 0xc1, 0x01],
            [0x0a, 0x03, 0x01]
        ]

        temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 1)
        self.libmetawear.mbl_mw_timer_create(self.board, 2718281828, 45904, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[0])
        self.libmetawear.mbl_mw_datasignal_read(temp_signal)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[0], self.commands_recorded_fn)

        self.assertEqual(self.command_history, expected_cmds)

    def test_schedule_read_gpio_adc(self):
        expected_cmds= [
            [0x0c, 0x02, 0x10, 0x2f, 0x0a, 0x00, 0xff, 0xff, 0x0],
            [0x0a, 0x02, 0x0c, 0x06, 0x01, 0x05, 0xc7, 0x01],
            [0x0a, 0x03, 0x02]
        ]
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 2, Gpio.ANALOG_READ_MODE_ADC)

        self.timerId= 1
        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 667408, 1, self.timer_signal_ready)
        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[0])
        self.libmetawear.mbl_mw_datasignal_read(adc_signal)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[0], self.commands_recorded_fn)

        self.assertEqual(self.command_history, expected_cmds)

    def test_schedule_read_temp_gpio(self):
        expected_cmds= [
            [0x0c, 0x02, 0x96, 0x22, 0x0, 0x0, 0x5e, 0x49, 0x0],
            [0x0c, 0x02, 0xda, 0x79, 0x7d, 0x07, 0xff, 0xff, 0x1],
            [0x0a, 0x02, 0x0c, 0x06, 0x0, 0x05, 0xc6, 0x01],
            [0x0a, 0x03, 0x03],
            [0x0a, 0x02, 0x0c, 0x06, 0x01, 0x04, 0xc1, 0x01],
            [0x0a, 0x03, 0x0]
        ]

        self.libmetawear.mbl_mw_timer_create(self.board, 8854, 18782, 1, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 125663706, 0, self.timer_signal_ready)

        abs_ref_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 3, Gpio.ANALOG_READ_MODE_ABS_REF)
        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[0])
        self.libmetawear.mbl_mw_datasignal_read(abs_ref_signal)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[0], self.commands_recorded_fn)

        temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 0)
        self.libmetawear.mbl_mw_event_record_commands(self.timerSignals[1])
        self.libmetawear.mbl_mw_datasignal_read(temp_signal)
        self.libmetawear.mbl_mw_event_end_record(self.timerSignals[1], self.commands_recorded_fn)

        self.assertEqual(self.command_history, expected_cmds)

class TestEventTimeout(TestMetaWearBase):
    def commands_recorded(self, event, status):
        self.received_status= status
        self.e.set()
        
    def commandLogger(self, board, characteristic, command, length):
        if (command[0] == 0xa and command[1] == 0x3):
            response= create_string_buffer(b'\x0a\x00', 2)
            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))
        else:
            super().commandLogger(board, characteristic, command, length)

    def test_timeout(self):
        self.e= threading.Event()

        commands_recorded_handler= Fn_VoidPtr_Int(self.commands_recorded)
        signal= self.libmetawear.mbl_mw_switch_get_state_data_signal(self.board)

        abs_ref_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 3, Gpio.ANALOG_READ_MODE_ABS_REF)
        self.libmetawear.mbl_mw_event_record_commands(signal)
        self.libmetawear.mbl_mw_datasignal_read(abs_ref_signal)
        self.libmetawear.mbl_mw_event_end_record(signal, commands_recorded_handler)
        self.e.wait()

        self.assertEqual(self.received_status, Status.ERROR_TIMEOUT)
