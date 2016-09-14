from common import TestMetaWearBase
from ctypes import *
from mbientlab.metawear.core import Fn_VoidPtr, Fn_VoidPtr_Int, Status
from mbientlab.metawear.peripheral import Led
from mbientlab.metawear.processor import *
from mbientlab.metawear.sensor import Gpio, MultiChannelTemperature
import threading

class TestSwitchLedController(TestMetaWearBase):
    def test_chain_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x01, 0x01, 0xff, 0x00, 0x02, 0x10],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x00, 0x09, 0x03, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x02, 0x02, 0x03, 0x0f],
            [0x0a, 0x03, 0x02, 0x02, 0x10, 0x10, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0xff],
            [0x0a, 0x02, 0x09, 0x03, 0x02, 0x02, 0x01, 0x01],
            [0x0a, 0x03, 0x01],
            [0x0a, 0x02, 0x09, 0x03, 0x03, 0x02, 0x02, 0x01],
            [0x0a, 0x03, 0x01]
        ]

        self.cmds_recorded_handler= Fn_VoidPtr_Int(self.cmds_recorded)
        self.counter_handler= Fn_VoidPtr(self.counter_processor_created) 
        self.math_handler= Fn_VoidPtr(self.math_processor_created)

        switch_signal= self.libmetawear.mbl_mw_switch_get_state_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_counter_create(switch_signal, self.counter_handler)

    def counter_processor_created(self, signal):
        self.libmetawear.mbl_mw_dataprocessor_math_create(signal, Math.OPERATION_MODULUS, 2.0, self.math_handler)

    def math_processor_created(self, signal):
        self.modulus_signal= signal

        self.comparator_odd_handler= Fn_VoidPtr(self.comparator_odd_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.modulus_signal, Comparator.OPERATION_EQ, 
                1.0, self.comparator_odd_handler)

    def comparator_odd_created(self, signal):
        self.comp_odd_signal= signal

        self.comparator_even_handler= Fn_VoidPtr(self.comparator_even_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.modulus_signal, Comparator.OPERATION_EQ, 
                0.0, self.comparator_even_handler)

    def comparator_even_created(self, signal):
        pattern= Led.Pattern(pulse_duration_ms=1000, high_time_ms=500, high_intensity=16, low_intensity=16, 
                repeat_count=Led.REPEAT_INDEFINITELY)
        self.libmetawear.mbl_mw_event_record_commands(self.comp_odd_signal)
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), Led.COLOR_BLUE)
        self.libmetawear.mbl_mw_led_play(self.board)
        self.libmetawear.mbl_mw_event_end_record(self.comp_odd_signal, self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_record_commands(signal)
        self.libmetawear.mbl_mw_led_stop_and_clear(self.board)
        self.libmetawear.mbl_mw_event_end_record(signal, self.cmds_recorded_handler)

    def cmds_recorded(self, event, status):
        self.assertEqual(self.command_history, self.expected_cmds)

class TestFreeFallDetectorRPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def test_freefall_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x07, 0xa5, 0x01],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x03, 0x05, 0x04],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x20, 0x0d, 0x09, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x00, 0x06, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x00, 0x06, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00]
        ]

        self.rss_handler= Fn_VoidPtr(self.rss_processor_created) 
        accel_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(accel_signal, self.rss_handler)

    def rss_processor_created(self, signal):
        self.avg_handler= Fn_VoidPtr(self.average_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_average_create(signal, 4, self.avg_handler)

    def average_processor_created(self, signal):
        self.threshold_handler= Fn_VoidPtr(self.threshold_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_threshold_create(signal, Threshold.MODE_BINARY, 0.5, 0.0,  
                self.threshold_handler)

    def threshold_processor_created(self, signal):
        self.threshold_signal= signal

        self.comparator_below_handler= Fn_VoidPtr(self.comparator_below_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, Comparator.OPERATION_EQ, -1.0, 
                self.comparator_below_handler)

    def comparator_below_created(self, signal):
        self.comparator_above_handler= Fn_VoidPtr(self.comparator_above_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.threshold_signal, Comparator.OPERATION_EQ, 
                1.0, self.comparator_above_handler)

    def comparator_above_created(self, signal):
        self.assertEqual(self.command_history, self.expected_cmds)

class TestActivityMonitorRPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.rms_handler= Fn_VoidPtr(self.rms_processor_created) 
        accel_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rms_create(accel_signal, self.rms_handler)

    def test_activity_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x07, 0xa5, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x02, 0x07],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x0f, 0x03],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x08, 0x03, 0x30, 0x75, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x03, 0x60, 0x0c, 0x0b, 0x00, 0x00, 0xc8, 0xaf],
            [0x09, 0x07, 0x03, 0x01],
            [0x09, 0x03, 0x01]
        ]

        self.assertEqual(self.command_history, self.expected_cmds)

    def test_time_processor_data(self):
        response= create_string_buffer(b'\x09\x03\x03\x4f\xee\xf4\x01', 7)
        expected= 2003.7236328125
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertAlmostEqual(self.data_float.value, expected)

    def test_time_processor_unsubscribe(self):
        expected= [0x09, 0x07, 0x03, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.time_signal)
        self.assertEqual(self.command, expected)

    def rms_processor_created(self, signal):
        self.accum_handler= Fn_VoidPtr(self.accum_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_accumulator_create_size(signal, 4, self.accum_handler)

    def accum_processor_created(self, signal):
        self.accum_signal= signal
        self.buffer_handler= Fn_VoidPtr(self.buffer_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_buffer_create(signal, self.buffer_handler)

    def buffer_processor_created(self, signal):
        self.time_handler= Fn_VoidPtr(self.time_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_time_create(self.accum_signal, Time.MODE_ABSOLUTE, 30000, self.time_handler)

    def time_processor_created(self, signal):
        self.time_signal= signal
        self.delta_handler= Fn_VoidPtr(self.delta_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_delta_create(signal, Delta.MODE_DIFFERENTIAL, 180000.0, self.delta_handler)

    def delta_processor_created(self, signal):
        self.libmetawear.mbl_mw_datasignal_subscribe(self.time_signal, self.sensor_data_handler)

class TestTemperatureConversionRPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.f_mult_handler= Fn_VoidPtr(self.f_mult_processor_created)
        self.temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 
                MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_DIE)
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.temp_signal, Math.OPERATION_MULTIPLY, 18.0, 
                self.f_mult_handler)

    def test_temperature_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x04, 0xc1, 0x00, 0x20, 0x09, 0x17, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x60, 0x09, 0x1f, 0x03, 0x0a, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x09, 0x1f, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x04, 0xc1, 0x00, 0x20, 0x09, 0x17, 0x01, 0x89, 0x08, 0x00, 0x00, 0x00],
            [0x09, 0x07, 0x02, 0x01],
            [0x09, 0x03, 0x01],
            [0x09, 0x07, 0x03, 0x01],
            [0x09, 0x03, 0x01]
        ]

        self.assertEqual(self.command_history, self.expected_cmds)

    def test_temperature_data(self):
        responses= [
            [create_string_buffer(b'\x04\x81\x00\x04\x01', 5), 32.5, "celsius"],
            [create_string_buffer(b'\x09\x03\x02\xd4\x02\x00\x00', 7), 90.5, "fahrenheit"],
            [create_string_buffer(b'\x09\x03\x03\x8d\x09\x00\x00', 7), 305.625, "kelvin"]
        ]

        for resp in responses:
            with self.subTest(response=resp[2]):
                self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, resp[0].raw, len(resp[0]))
                self.assertAlmostEqual(self.data_float.value, resp[1])

    def test_temperature_unsubscribe(self):
        expected_cmds= [
            [0x09, 0x07, 0x02, 0x00],
            [0x09, 0x07, 0x03, 0x00]
        ]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.temp_signal)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.fahrenheit_signal)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.kelvin_signal)

        unsubscribe_cmds= self.command_history[8:11].copy()
        self.assertEqual(unsubscribe_cmds, expected_cmds)

    def f_mult_processor_created(self, signal):
        self.f_divide_handler= Fn_VoidPtr(self.f_divide_processor_created) 
        self.libmetawear.mbl_mw_dataprocessor_math_create(signal, Math.OPERATION_DIVIDE, 10.0, 
                self.f_divide_handler)

    def f_divide_processor_created(self, signal):
        self.f_add_handler= Fn_VoidPtr(self.f_add_processor_created) 
        self.libmetawear.mbl_mw_dataprocessor_math_create(signal, Math.OPERATION_ADD, 32.0, self.f_add_handler)

    def f_add_processor_created(self, signal):
        self.fahrenheit_signal= signal;
        self.k_add_handler= Fn_VoidPtr(self.k_add_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.temp_signal, Math.OPERATION_ADD, 273.15, 
                self.k_add_handler)

    def k_add_processor_created(self, signal):
        self.kelvin_signal= signal

        self.libmetawear.mbl_mw_datasignal_subscribe(self.temp_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.fahrenheit_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.kelvin_signal, self.sensor_data_handler)

class TestDataCollector(TestMetaWearBase):
    def test_collector_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x05, 0xc7, 0x00, 0x20, 0x0a, 0x01, 0x10],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x01, 0x02, 0x00, 0x00]
        ]

        self.sample_handler= Fn_VoidPtr(self.sample_processor_created)
        gpio_adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0, Gpio.ANALOG_READ_MODE_ADC)
        self.libmetawear.mbl_mw_dataprocessor_sample_create(gpio_adc_signal, 16, self.sample_handler)

    def sample_processor_created(self, signal):
        self.passthrough_handler= Fn_VoidPtr(self.passthrough_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_passthrough_create(signal, Passthrough.MODE_COUNT, 0, 
                self.passthrough_handler)

    def passthrough_processor_created(self, signal):
        self.assertEqual(self.command_history, self.expected_cmds)

class TestGpioAdcPulse(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        self.pulse_handler= Fn_VoidPtr(self.pulse_processor_created)
        gpio_adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0, Gpio.ANALOG_READ_MODE_ADC)
        self.libmetawear.mbl_mw_dataprocessor_pulse_create(gpio_adc_signal, Pulse.OUTPUT_PEAK, 500.0, 
                10, self.pulse_handler)

    def test_pulse_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x05, 0xc7, 0x00, 0x20, 0x0b, 0x01, 0x00, 0x02, 0xf4, 0x01, 0x00, 0x00, 0x0a, 0x00],
            [0x09, 0x07, 0x00, 0x01],
            [0x09, 0x03, 0x01]
        ]

        self.assertEqual(self.command_history, self.expected_cmds)

    def test_pulse_data(self):
        expected= 789
        response= create_string_buffer(b'\x09\x03\x00\x15\x03\x00\x00', 7)

        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_uint32.value, expected)

    def test_pulse_unsubscribe(self):
        expected= [0x09, 0x07, 0x00, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.pulse_signal)
        self.assertEqual(self.command, expected)

    def pulse_processor_created(self, signal):
        self.pulse_signal= signal
        self.libmetawear.mbl_mw_datasignal_subscribe(self.pulse_signal, self.sensor_data_handler)

class TestGpioFeedbackSetup(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        self.passthrough_handler= Fn_VoidPtr(self.passthrough_processor_created)
        self.gpio_abs_ref_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0, 
                Gpio.ANALOG_READ_MODE_ABS_REF)
        self.libmetawear.mbl_mw_dataprocessor_passthrough_create(self.gpio_abs_ref_signal, Passthrough.MODE_COUNT, 
                0, self.passthrough_handler)

    def passthrough_processor_created(self, signal):
        self.offset_passthrough= signal
        self.math_handler= Fn_VoidPtr(self.math_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.gpio_abs_ref_signal, Math.OPERATION_SUBTRACT, 0.0, self.math_handler)

    def math_processor_created(self, signal):
        self.abs_ref_offset= signal
        self.gt_comparator_handler= Fn_VoidPtr(self.gt_comparator_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, Comparator.OPERATION_GT, 0.0, 
                self.gt_comparator_handler)

    def gt_comparator_processor_created(self, signal):
        self.gt_comparator= signal
        self.gt_counter_handler= Fn_VoidPtr(self.gt_counter_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_counter_create(signal, self.gt_counter_handler)

    def gt_counter_processor_created(self, signal):
        self.gt_comparator_counter= signal
        self.gt_counter_comparator_handler= Fn_VoidPtr(self.gt_counter_comparator_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, Comparator.OPERATION_EQ, 16.0, 
                self.gt_counter_comparator_handler)

    def gt_counter_comparator_processor_created(self, signal):
        self.gt_comparator_counter_comparator= signal
        self.lte_comparator_handler= Fn_VoidPtr(self.lte_comparator_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.abs_ref_offset, Comparator.OPERATION_LTE, 
                0.0, self.lte_comparator_handler)

    def lte_comparator_processor_created(self, signal):
        self.lte_comparator= signal
        self.lte_counter_handler= Fn_VoidPtr(self.lte_counter_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_counter_create(signal, self.lte_counter_handler)

    def lte_counter_processor_created(self, signal):
        self.lte_comparator_counter= signal
        self.lte_counter_comparator_handler= Fn_VoidPtr(self.lte_counter_comparator_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, Comparator.OPERATION_EQ, 16.0, 
                self.lte_counter_comparator_handler)

    def lte_counter_comparator_processor_created(self, signal):
        self.libmetawear.mbl_mw_event_record_commands(self.offset_passthrough)
        self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.lte_comparator_counter, 0)
        self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.gt_comparator_counter, 0)
        self.libmetawear.mbl_mw_dataprocessor_math_modify_rhs_signal(self.abs_ref_offset, self.offset_passthrough)
        self.libmetawear.mbl_mw_event_end_record(self.offset_passthrough, self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_record_commands(self.gt_comparator)
        self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.lte_comparator_counter, 0)
        self.libmetawear.mbl_mw_event_end_record(self.gt_comparator, self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_record_commands(self.gt_comparator_counter_comparator)
        self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(self.offset_passthrough, 1)
        self.libmetawear.mbl_mw_event_end_record(self.gt_comparator_counter_comparator, self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_record_commands(self.lte_comparator)
        self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.gt_comparator_counter, 0)
        self.libmetawear.mbl_mw_event_end_record(self.lte_comparator, self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_record_commands(signal)
        self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(self.offset_passthrough, 1)
        self.libmetawear.mbl_mw_event_end_record(signal, self.commands_recorded_fn)

class TestGpioFeedback(TestGpioFeedbackSetup):
    def test_feedback_setup(self):
        expected_cmds= [
            [0x09, 0x02, 0x05, 0xc6, 0x00, 0x20, 0x01, 0x02, 0x00, 0x00],
            [0x09, 0x02, 0x05, 0xc6, 0x00, 0x20, 0x09, 0x07, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x06, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x60, 0x02, 0x1c],
            [0x09, 0x02, 0x09, 0x03, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x06, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x05, 0x60, 0x02, 0x1c],
            [0x09, 0x02, 0x09, 0x03, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x00, 0x09, 0x04, 0x05],
            [0x0a, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x00, 0x09, 0x04, 0x05],
            [0x0a, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x00, 0x09, 0x05, 0x09, 0x05, 0x04],
            [0x0a, 0x03, 0x01, 0x09, 0x07, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x02, 0x09, 0x04, 0x05],
            [0x0a, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x04, 0x09, 0x04, 0x03],
            [0x0a, 0x03, 0x00, 0x01, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x05, 0x09, 0x04, 0x05],
            [0x0a, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x07, 0x09, 0x04, 0x03],
            [0x0a, 0x03, 0x00, 0x01, 0x00]
        ]

        self.assertEqual(self.command_history, expected_cmds)

    def test_remove_out_of_order(self):
        expected_cmds= [
            [0x09, 0x06, 0x07],
            [0x0a, 0x04, 0x06],
            [0x09, 0x06, 0x06],
            [0x09, 0x06, 0x05],
            [0x0a, 0x04, 0x05],
            [0x09, 0x06, 0x04],
            [0x0a, 0x04, 0x04],
            [0x09, 0x06, 0x03],
            [0x09, 0x06, 0x02],
            [0x0a, 0x04, 0x03],
            [0x09, 0x06, 0x01]
        ]

        self.libmetawear.mbl_mw_dataprocessor_remove(self.lte_comparator)
        self.libmetawear.mbl_mw_dataprocessor_remove(self.abs_ref_offset)
        remove_cmds= self.command_history[22:].copy()

        self.assertEqual(remove_cmds, expected_cmds)

    def test_remove_passthrough(self):
        expected_cmds= [
            [0x09, 0x06, 0x00],
            [0x0a, 0x04, 0x00],
            [0x0a, 0x04, 0x01],
            [0x0a, 0x04, 0x02]
        ]

        self.libmetawear.mbl_mw_dataprocessor_remove(self.offset_passthrough)
        remove_cmds= self.command_history[22:].copy()

        self.assertEqual(remove_cmds, expected_cmds)

    def test_remove_math(self):
        expected_cmds= [
            [0x09, 0x06, 0x04],
            [0x0a, 0x04, 0x04],
            [0x09, 0x06, 0x03],
            [0x09, 0x06, 0x02],
            [0x0a, 0x04, 0x03],
            [0x09, 0x06, 0x07],
            [0x0a, 0x04, 0x06],
            [0x09, 0x06, 0x06],
            [0x09, 0x06, 0x05],
            [0x0a, 0x04, 0x05],
            [0x09, 0x06, 0x01]
        ]

        self.libmetawear.mbl_mw_dataprocessor_remove(self.abs_ref_offset)
        remove_cmds= self.command_history[22:].copy()

        self.assertEqual(remove_cmds, expected_cmds)

class TestPassthroughSetCount(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= Fn_VoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(signal, 20)

    def test_valid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_passthrough_create(self.baro_pa_signal, Passthrough.MODE_COUNT, 
                10, self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_sample_create(self.baro_pa_signal, 16, self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestAccumulatorSetSum(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= Fn_VoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_set_accumulator_state(signal, 101325.0)

    def test_valid_set_state(self):
        self.libmetawear.mbl_mw_dataprocessor_accumulator_create(self.baro_pa_signal, self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_time_create(self.baro_pa_signal, Time.MODE_DIFFERENTIAL, 30000, self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestCounterSetCount(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= Fn_VoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_counter_set_state(signal, 128)

    def test_valid_set_state(self):
        self.libmetawear.mbl_mw_dataprocessor_counter_create(self.baro_pa_signal, self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.baro_pa_signal, Comparator.OPERATION_LT, 
                101325.0, self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestAverageReset(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= Fn_VoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_average_reset(signal)

    def test_valid_reset(self):
        self.libmetawear.mbl_mw_dataprocessor_average_create(self.baro_pa_signal, 8, self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_reset(self):
        self.libmetawear.mbl_mw_dataprocessor_pulse_create(self.baro_pa_signal, Pulse.OUTPUT_AREA, 
                101325.0, 64, self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestDeltaSetPrevious(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= Fn_VoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_delta_set_reference(signal, 101325.0)

    def test_valid_set_previous(self):
        self.libmetawear.mbl_mw_dataprocessor_delta_create(self.baro_pa_signal, Delta.MODE_DIFFERENTIAL, 
                25331.25, self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_set_previous(self):
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.baro_pa_signal, Math.OPERATION_DIVIDE, 1000.0, 
                self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestThreshold(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

    def test_valid_set_count(self):
        expected= [0x09, 0x02, 0x16, 0xc1, 0xff, 0x60, 0x0d, 0x0b, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x00]

        signal= self.libmetawear.mbl_mw_humidity_bme280_get_percentage_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_threshold_create(signal, Threshold.MODE_BINARY, 57.0, 0.0, 
                Fn_VoidPtr(lambda p: self.assertEqual(self.command, expected)))

class TestThreeAxisRightShift(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD
        self.processor_created_fn= Fn_VoidPtr(self.processor_created)
        self.logger_created_fn= Fn_VoidPtr(self.logger_created)

    def test_acc_rshift(self):
        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_math_create(signal, Math.OPERATION_RSHIFT, 8, self.processor_created_fn)

    def processor_created(self, processor):
        self.libmetawear.mbl_mw_datasignal_log(processor, self.logger_created_fn)

    def logger_created(self, logger):
        expected_cmds= [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x09, 0x14, 0x08, 0x08, 0x00, 0x00, 0x00, 0x02],
            [0x0b, 0x02, 0x09, 0x03, 0x00, 0x40]
        ]
        self.assertEqual(self.command_history, expected_cmds)

# Future test data, figure out how to use this
#Shifted: (1.969, 0.812, 0.984)
#[126, 52, 63]
#Raw: [46, 126, 85, 52, 119, 63]
#Raw: (1.972, 0.818, 0.992)

class TestBmm150DataProcessor(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.boardType= TestMetaWearBase.METAWEAR_CPRO_BOARD

    def test_number_to_firmware(self):
        expected= [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x06, 0x00, 0x04, 0x00, 0xa0, 0x0f, 0x00, 0x00]

        comp_created_ptr= Fn_VoidPtr(lambda comp: self.assertEqual(self.command, expected))
        rss_created_ptr= Fn_VoidPtr(lambda rms: self.libmetawear.mbl_mw_dataprocessor_comparator_create(rms, Comparator.OPERATION_GT, 250.0, 
                comp_created_ptr))
        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(signal, rss_created_ptr)

class TestDataProcessorTimeout(TestMetaWearBase):
    def dataprocessor_created(self, processor):
        self.created_proc= processor
        self.e.set()
        
    def commandLogger(self, board, characteristic, command, length):
        if (command[0] == 0x9 and command[1] == 0x2):
            response= create_string_buffer(b'\x0a\x00', 2)
            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))
        else:
            super().commandLogger(board, characteristic, command, length)

    def test_timeout_no_state(self):
        dataprocessor_created_fn= Fn_VoidPtr(self.dataprocessor_created)
        self.e= threading.Event()
        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(signal, dataprocessor_created_fn)
        self.e.wait()

        self.assertIsNone(self.created_proc)

    def test_timeout_with_state(self):
        dataprocessor_created_fn= Fn_VoidPtr(self.dataprocessor_created)
        self.e= threading.Event()
        signal= self.libmetawear.mbl_mw_switch_get_state_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_accumulator_create(signal, dataprocessor_created_fn)
        self.e.wait()

        self.assertIsNone(self.created_proc)

class TestMultiComparator(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.firmware_revision= create_string_buffer(b'1.2.3', 5)

        super().setUp()

    def test_reference(self):
        expected= [0x09, 0x02, 0x05, 0xc7, 0x15, 0x20, 0x06, 0x6a, 0x00, 0x04, 0x00, 0x02, 0x00, 0x01, 0x80, 0x00]

        references= (c_float * 4)(1024, 512, 256, 128)
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x15, Gpio.ANALOG_READ_MODE_ADC)

        comp_created_ptr= Fn_VoidPtr(lambda comp: self.assertEqual(self.command, expected))
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(adc_signal, Comparator.OPERATION_GTE, Comparator.MODE_REFERENCE, references, 
                len(references), comp_created_ptr)

    def test_zone(self):
        expected= [0x09, 0x02, 0x14, 0x03, 0xff, 0x60, 0x06, 0x96, 0x00, 0xc4, 0x02, 0x00, 0xfa, 0x33, 0x5b, 0x00, 0x2a, 0x6d, 0x75, 0x01]

        references= (c_float * 3)(181248, 5977082, 24472874)
        signal= self.libmetawear.mbl_mw_als_ltr329_get_illuminance_data_signal(self.board)

        comp_created_ptr= Fn_VoidPtr(lambda comp: self.assertEqual(self.command, expected))
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(signal, Comparator.OPERATION_LT, Comparator.MODE_ZONE, references, 
                len(references), comp_created_ptr)

    def test_absolute(self):
        expected= [0x09, 0x02, 0x04, 0xc1, 0x01, 0x20, 0x06, 0x03, 0xc0, 0x00, 0xc8, 0x00, 0xd0, 0x00]

        references= (c_float * 3)(24.0, 25.0, 26.0)
        signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR)

        comp_created_ptr= Fn_VoidPtr(lambda comp: self.assertEqual(self.command, expected))
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(signal, Comparator.OPERATION_EQ, Comparator.MODE_ABSOLUTE, references, 
                len(references), comp_created_ptr)

    def test_modify_config(self):
        references= (c_float * 4)(1024, 512, 256, 128)
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x15, Gpio.ANALOG_READ_MODE_ADC)

        comp_created_ptr= Fn_VoidPtr(self.comp_created_cmd_check)
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(adc_signal, Comparator.OPERATION_GTE, Comparator.MODE_REFERENCE, references, 
                len(references), comp_created_ptr)

    def comp_created_cmd_check(self, comparator):
        expected= [0x09, 0x05, 0x00, 0x06, 0x52, 0x80, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x04]
        references= (c_float * 4)(128, 256, 512, 1024)

        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_modify(comparator, Comparator.OPERATION_LT, references, len(references))
        self.assertEqual(self.command, expected)

    def test_modify_config_signal(self):
        references= (c_float * 1)(128)
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x0, Gpio.ANALOG_READ_MODE_ABS_REF)

        comp_created_ptr= Fn_VoidPtr(self.comp_created_2)
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(adc_signal, Comparator.OPERATION_GTE, Comparator.MODE_ABSOLUTE, references, 
                len(references), comp_created_ptr)

    def comp_created_2(self, comparator):
        expected_cmds= [
            [0x09, 0x02, 0x05, 0xc6, 0x00, 0x20, 0x06, 0x2a, 0x80, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x00, 0x09, 0x05, 0x05, 0x05, 0x03],
            [0x0a, 0x03, 0x00, 0x06, 0x2a, 0x00, 0x00]
        ]

        self.libmetawear.mbl_mw_event_record_commands(comparator)
        self.libmetawear.mbl_mw_dataprocessor_comparator_modify_signal(comparator, Comparator.OPERATION_GTE, comparator)
        self.libmetawear.mbl_mw_event_end_record(comparator, 
                Fn_VoidPtr_Int(lambda event, status: self.assertEqual(self.command_history, expected_cmds)))

class TestMathProcessor(TestMetaWearBase):
    def test_constant_op(self):
        expected= [0x09, 0x02, 0x04, 0xc1, 0x00, 0x20, 0x09, 0x17, 0x0b, 0xc8, 0x00, 0x00, 0x00, 0x00]

        temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, MultiChannelTemperature.METAWEAR_R_CHANNEL_ON_DIE)
        self.libmetawear.mbl_mw_dataprocessor_math_create(temp_signal, Math.OPERATION_CONSTANT, 25.0, 
                Fn_VoidPtr(lambda math: self.assertEqual(self.command, expected)))

    def test_constant_op_data(self):
        const_created_ptr= Fn_VoidPtr(self.const_created)
        temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, MultiChannelTemperature.METAWEAR_R_CHANNEL_ON_DIE)
        self.libmetawear.mbl_mw_dataprocessor_math_create(temp_signal, Math.OPERATION_CONSTANT, 25.0, const_created_ptr)

    def const_created(self, math):
        expected= 3.125
        response= create_string_buffer(b'\x09\x03\x00\x19\x00\x00\x00', 7)

        self.libmetawear.mbl_mw_datasignal_subscribe(math, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_float.value, expected)
