from common import TestMetaWearBase
from ctypes import *
from mbientlab.metawear.core import FnVoid, FnVoidPtr, Status
from mbientlab.metawear.peripheral import Led
from mbientlab.metawear.processor import *
from mbientlab.metawear.sensor import Gpio, MultiChannelTemperature

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

        self.cmds_recorded_handler= FnVoid(self.cmds_recorded)
        self.counter_handler= FnVoidPtr(self.counter_processor_created) 
        self.math_handler= FnVoidPtr(self.math_processor_created)

        switch_signal= self.libmetawear.mbl_mw_switch_get_state_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_counter_create(switch_signal, self.counter_handler)

    def counter_processor_created(self, signal):
        self.libmetawear.mbl_mw_dataprocessor_math_create(signal, Math.OPERATION_MODULUS, c_float(2), self.math_handler)

    def math_processor_created(self, signal):
        self.modulus_signal= signal

        self.comparator_odd_handler= FnVoidPtr(self.comparator_odd_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.modulus_signal, Comparator.OPERATION_EQ, 
                c_float(1), self.comparator_odd_handler)

    def comparator_odd_created(self, signal):
        self.comp_odd_signal= signal

        self.comparator_even_handler= FnVoidPtr(self.comparator_even_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.modulus_signal, Comparator.OPERATION_EQ, 
                c_float(0), self.comparator_even_handler)

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

    def cmds_recorded(self):
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

        self.rss_handler= FnVoidPtr(self.rss_processor_created) 
        accel_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(accel_signal, self.rss_handler)

    def rss_processor_created(self, signal):
        self.avg_handler= FnVoidPtr(self.average_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_average_create(signal, 4, self.avg_handler)

    def average_processor_created(self, signal):
        self.threshold_handler= FnVoidPtr(self.threshold_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_threshold_create(signal, Threshold.MODE_BINARY, c_float(0.5), c_float(0), 
                self.threshold_handler)

    def threshold_processor_created(self, signal):
        self.threshold_signal= signal

        self.comparator_below_handler= FnVoidPtr(self.comparator_below_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, Comparator.OPERATION_EQ, c_float(-1), 
                self.comparator_below_handler)

    def comparator_below_created(self, signal):
        self.comparator_above_handler= FnVoidPtr(self.comparator_above_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.threshold_signal, Comparator.OPERATION_EQ, 
                c_float(1), self.comparator_above_handler)

    def comparator_above_created(self, signal):
        self.assertEqual(self.command_history, self.expected_cmds)

class TestActivityMonitorRPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.rms_handler= FnVoidPtr(self.rms_processor_created) 
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
        self.accum_handler= FnVoidPtr(self.accum_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_accumulator_create_size(signal, c_ubyte(4), self.accum_handler)

    def accum_processor_created(self, signal):
        self.accum_signal= signal
        self.buffer_handler= FnVoidPtr(self.buffer_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_buffer_create(signal, self.buffer_handler)

    def buffer_processor_created(self, signal):
        self.time_handler= FnVoidPtr(self.time_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_time_create(self.accum_signal, Time.MODE_ABSOLUTE, 30000, self.time_handler)

    def time_processor_created(self, signal):
        self.time_signal= signal
        self.delta_handler= FnVoidPtr(self.delta_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_delta_create(signal, Delta.MODE_DIFFERENTIAL, c_float(180000), 
                self.delta_handler)

    def delta_processor_created(self, signal):
        self.libmetawear.mbl_mw_datasignal_subscribe(self.time_signal, self.sensor_data_handler)

class TestTemperatureConversionRPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.f_mult_handler= FnVoidPtr(self.f_mult_processor_created)
        self.temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 
                c_ubyte(MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_ON_DIE))
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.temp_signal, Math.OPERATION_MULTIPLY, c_float(18), 
                self.f_mult_handler)

    def test_temperature_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x04, 0x81, 0x00, 0x20, 0x09, 0x17, 0x02, 0x12, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x60, 0x09, 0x1f, 0x03, 0x0a, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x09, 0x1f, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x04, 0x81, 0x00, 0x20, 0x09, 0x17, 0x01, 0x89, 0x08, 0x00, 0x00, 0x00],
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
        self.f_divide_handler= FnVoidPtr(self.f_divide_processor_created) 
        self.libmetawear.mbl_mw_dataprocessor_math_create(signal, Math.OPERATION_DIVIDE, c_float(10), 
                self.f_divide_handler)

    def f_divide_processor_created(self, signal):
        self.f_add_handler= FnVoidPtr(self.f_add_processor_created) 
        self.libmetawear.mbl_mw_dataprocessor_math_create(signal, Math.OPERATION_ADD, c_float(32), self.f_add_handler)

    def f_add_processor_created(self, signal):
        self.fahrenheit_signal= signal;
        self.k_add_handler= FnVoidPtr(self.k_add_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.temp_signal, Math.OPERATION_ADD, c_float(273.15), 
                self.k_add_handler)

    def k_add_processor_created(self, signal):
        self.kelvin_signal= signal

        self.libmetawear.mbl_mw_datasignal_subscribe(self.temp_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.fahrenheit_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.kelvin_signal, self.sensor_data_handler)

class TestDataCollector(TestMetaWearBase):
    def test_collector_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x05, 0x87, 0x00, 0x20, 0x0a, 0x01, 0x10],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x01, 0x02, 0x00, 0x00]
        ]

        self.sample_handler= FnVoidPtr(self.sample_processor_created)
        gpio_adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, c_ubyte(0), 
                Gpio.ANALOG_READ_MODE_ADC)
        self.libmetawear.mbl_mw_dataprocessor_sample_create(gpio_adc_signal, c_ubyte(16), self.sample_handler)

    def sample_processor_created(self, signal):
        self.passthrough_handler= FnVoidPtr(self.passthrough_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_passthrough_create(signal, Passthrough.MODE_COUNT, c_ushort(0), 
                self.passthrough_handler)

    def passthrough_processor_created(self, signal):
        self.assertEqual(self.command_history, self.expected_cmds)

class TestGpioAdcPulse(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        self.pulse_handler= FnVoidPtr(self.pulse_processor_created)
        gpio_adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, c_ubyte(0), 
                Gpio.ANALOG_READ_MODE_ADC)
        self.libmetawear.mbl_mw_dataprocessor_pulse_create(gpio_adc_signal, Pulse.OUTPUT_PEAK, c_float(500), 
                c_ushort(10), self.pulse_handler)

    def test_pulse_setup(self):
        self.expected_cmds= [
            [0x09, 0x02, 0x05, 0x87, 0x00, 0x20, 0x0b, 0x01, 0x00, 0x02, 0xf4, 0x01, 0x00, 0x00, 0x0a, 0x00],
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

        self.passthrough_handler= FnVoidPtr(self.passthrough_processor_created)
        self.gpio_abs_ref_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, c_ubyte(0), 
                Gpio.ANALOG_READ_MODE_ABS_REF)
        self.libmetawear.mbl_mw_dataprocessor_passthrough_create(self.gpio_abs_ref_signal, Passthrough.MODE_COUNT, 
                c_ushort(0), self.passthrough_handler)

    def passthrough_processor_created(self, signal):
        self.offset_passthrough= signal
        self.math_handler= FnVoidPtr(self.math_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.gpio_abs_ref_signal, Math.OPERATION_SUBTRACT, c_float(0), self.math_handler)

    def math_processor_created(self, signal):
        self.abs_ref_offset= signal
        self.gt_comparator_handler= FnVoidPtr(self.gt_comparator_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, Comparator.OPERATION_GT, c_float(0), 
                self.gt_comparator_handler)

    def gt_comparator_processor_created(self, signal):
        self.gt_comparator= signal
        self.gt_counter_handler= FnVoidPtr(self.gt_counter_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_counter_create(signal, self.gt_counter_handler)

    def gt_counter_processor_created(self, signal):
        self.gt_comparator_counter= signal
        self.gt_counter_comparator_handler= FnVoidPtr(self.gt_counter_comparator_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, Comparator.OPERATION_EQ, c_float(16), 
                self.gt_counter_comparator_handler)

    def gt_counter_comparator_processor_created(self, signal):
        self.gt_comparator_counter_comparator= signal
        self.lte_comparator_handler= FnVoidPtr(self.lte_comparator_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.abs_ref_offset, Comparator.OPERATION_LTE, 
                c_float(0), self.lte_comparator_handler)

    def lte_comparator_processor_created(self, signal):
        self.lte_comparator= signal
        self.lte_counter_handler= FnVoidPtr(self.lte_counter_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_counter_create(signal, self.lte_counter_handler)

    def lte_counter_processor_created(self, signal):
        self.lte_comparator_counter= signal
        self.lte_counter_comparator_handler= FnVoidPtr(self.lte_counter_comparator_processor_created)
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, Comparator.OPERATION_EQ, c_float(16), 
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
        self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(self.offset_passthrough, c_ushort(1))
        self.libmetawear.mbl_mw_event_end_record(self.gt_comparator_counter_comparator, self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_record_commands(self.lte_comparator)
        self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.gt_comparator_counter, 0)
        self.libmetawear.mbl_mw_event_end_record(self.lte_comparator, self.commands_recorded_fn)

        self.libmetawear.mbl_mw_event_record_commands(signal)
        self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(self.offset_passthrough, c_ushort(1))
        self.libmetawear.mbl_mw_event_end_record(signal, self.commands_recorded_fn)

class TestGpioFeedback(TestGpioFeedbackSetup):
    def test_feedback_setup(self):
        expected_cmds= [
            [0x09, 0x02, 0x05, 0x86, 0x00, 0x20, 0x01, 0x02, 0x00, 0x00],
            [0x09, 0x02, 0x05, 0x86, 0x00, 0x20, 0x09, 0x07, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00],
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

        self.processor_handler= FnVoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bmp280_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(signal, c_ushort(20))

    def test_valid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_passthrough_create(self.baro_pa_signal, Passthrough.MODE_COUNT, 
                c_ushort(10), self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_sample_create(self.baro_pa_signal, c_ubyte(16), self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestAccumulatorSetSum(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= FnVoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bmp280_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_set_accumulator_state(signal, c_float(101325))

    def test_valid_set_state(self):
        self.libmetawear.mbl_mw_dataprocessor_accumulator_create(self.baro_pa_signal, self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_time_create(self.baro_pa_signal, Time.MODE_DIFFERENTIAL, 30000, 
                self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestCounterSetCount(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= FnVoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bmp280_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_counter_set_state(signal, 128)

    def test_valid_set_state(self):
        self.libmetawear.mbl_mw_dataprocessor_counter_create(self.baro_pa_signal, self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.baro_pa_signal, Comparator.OPERATION_LT, 
                c_float(101325), self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestAverageReset(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= FnVoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bmp280_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_average_reset(signal)

    def test_valid_reset(self):
        self.libmetawear.mbl_mw_dataprocessor_average_create(self.baro_pa_signal, c_ubyte(8), self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_reset(self):
        self.libmetawear.mbl_mw_dataprocessor_pulse_create(self.baro_pa_signal, Pulse.OUTPUT_AREA, 
                c_float(101325), c_ushort(64), self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)

class TestDeltaSetPrevious(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.processor_handler= FnVoidPtr(self.processor_created)
        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bmp280_get_pressure_data_signal(self.board)

    def processor_created(self, signal):
        self.status= self.libmetawear.mbl_mw_dataprocessor_delta_set_reference(signal, c_float(101325))

    def test_valid_set_previous(self):
        self.libmetawear.mbl_mw_dataprocessor_delta_create(self.baro_pa_signal, Delta.MODE_DIFFERENTIAL, 
                c_float(25331.25), self.processor_handler)
        self.assertEqual(self.status, Status.OK)

    def test_invalid_set_previous(self):
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.baro_pa_signal, Math.OPERATION_DIVIDE, c_float(1000), 
                self.processor_handler)
        self.assertEqual(self.status, Status.WARNING_INVALID_PROCESSOR_TYPE)
