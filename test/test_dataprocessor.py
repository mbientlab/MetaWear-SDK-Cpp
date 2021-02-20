from common import TestMetaWearBase, to_string_buffer
from ctypes import *
from cbindings import *
#from mbientlab.metawear.cbindings import *
import threading

class TestDataProcessingChain(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def test_led_controller(self):
        processors = [None, None, None]
        events = [threading.Event(), threading.Event(), threading.Event()]

        def math_created(context, pointer):
            processors[0] = pointer
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(pointer, ComparatorOperation.EQ, 1.0, context, comparator_odd_handler)

        def comp_odd_created(context, pointer):
            processors[1] = pointer
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(processors[0], ComparatorOperation.EQ, 0.0, context, comparator_even_handler)

        def comp_even_created(context, pointer):
            processors[2] = pointer
            events[0].set()

        math_handler = FnVoid_VoidP_VoidP(math_created)
        comparator_even_handler = FnVoid_VoidP_VoidP(comp_even_created)
        comparator_odd_handler = FnVoid_VoidP_VoidP(comp_odd_created)
        counter_handler= FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_math_create(p, MathOperation.MODULUS, 2.0, ctx, math_handler)) 

        switch_signal= self.libmetawear.mbl_mw_switch_get_state_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_counter_create(switch_signal, None, counter_handler)
        events[0].wait()

        event_handler = FnVoid_VoidP_VoidP_Int(lambda ctx, e, s: events[1].set())
        pattern= LedPattern(pulse_duration_ms=1000, high_time_ms=500, high_intensity=16, low_intensity=16, 
                repeat_count=Const.LED_REPEAT_INDEFINITELY)
        self.libmetawear.mbl_mw_event_record_commands(processors[1])
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.BLUE)
        self.libmetawear.mbl_mw_led_play(self.board)
        self.libmetawear.mbl_mw_event_end_record(processors[1], None, event_handler)
        events[1].wait()

        event_handler = FnVoid_VoidP_VoidP_Int(lambda ctx, e, s: events[2].set())
        self.libmetawear.mbl_mw_event_record_commands(processors[2])
        self.libmetawear.mbl_mw_led_stop_and_clear(self.board)
        self.libmetawear.mbl_mw_event_end_record(processors[2], None, event_handler)
        events[2].wait()

        expected= [
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

        self.assertEqual(self.command_history, expected)

    def test_freefall(self):
        processors = []
        e = threading.Event()

        def threshold_processor_created(context, ths):
            processors.append(ths)
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(ths, ComparatorOperation.EQ, -1.0, context, comparator_below_handler)

        rss_handler= FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_average_create(p, 4, ctx, avg_handler))
        avg_handler = FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_threshold_create(p, ThresholdMode.BINARY, 0.5, 0.0, 
                ctx, threshold_handler))
        threshold_handler = FnVoid_VoidP_VoidP(threshold_processor_created)
        comparator_below_handler = FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_comparator_create(processors[0], ComparatorOperation.EQ, 
                1.0, ctx, comparator_above_handler))
        comparator_above_handler = FnVoid_VoidP_VoidP(lambda ctx, p: e.set())

        accel_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(accel_signal, None, rss_handler)
        e.wait()

        expected= [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x07, 0xa5, 0x01],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x03, 0x05, 0x04],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x20, 0x0d, 0x09, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x00, 0x06, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x00, 0x06, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00]
        ]
        self.assertEqual(self.command_history, expected)

    def test_sample_collector(self):
        e = threading.Event()

        sample_handler= FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_passthrough_create(p, PassthroughMode.COUNT, 0, 
                ctx, passthrough_handler))
        passthrough_handler= FnVoid_VoidP_VoidP(lambda ctx, p: e.set())

        gpio_adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0, GpioAnalogReadMode.ADC)
        self.libmetawear.mbl_mw_dataprocessor_sample_create(gpio_adc_signal, 16, None, sample_handler)
        e.wait()

        expected= [
            [0x09, 0x02, 0x05, 0xc7, 0x00, 0x20, 0x0a, 0x01, 0x10],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x01, 0x02, 0x00, 0x00]
        ]
        self.assertEqual(self.command_history, expected)

class TestActivityMonitorRPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        e = threading.Event()
        self.processors = []
        def accum_processor_created(context, acc):
            self.processors.append(acc)
            self.libmetawear.mbl_mw_dataprocessor_buffer_create(acc, context, buffer_handler)

        def time_processor_created(context, time):
            self.processors.append(time)
            self.libmetawear.mbl_mw_dataprocessor_delta_create(time, DeltaMode.DIFFERENTIAL, 180000.0, context, delta_handler)

        rms_handler = FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_accumulator_create_size(p, 4, ctx, accum_handler))
        accum_handler = FnVoid_VoidP_VoidP(accum_processor_created)
        buffer_handler = FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_time_create(self.processors[0], TimeMode.ABSOLUTE, 30000, 
                ctx, time_handler))
        time_handler = FnVoid_VoidP_VoidP(time_processor_created)
        delta_handler = FnVoid_VoidP_VoidP(lambda ctx, p: e.set())

        accel_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rms_create(accel_signal, None, rms_handler)
        e.wait()

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[1], None, self.sensor_data_handler)

    def test_activity_setup(self):
        expected= [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x07, 0xa5, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x02, 0x07],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x0f, 0x03],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x08, 0x03, 0x30, 0x75, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x03, 0x60, 0x0c, 0x0b, 0x00, 0x00, 0xc8, 0xaf],
            [0x09, 0x07, 0x03, 0x01],
            [0x09, 0x03, 0x01]
        ]

        self.assertEqual(self.command_history, expected)

    def test_time_processor_data(self):
        expected= 2003.7236328125
        self.notify_mw_char(create_string_buffer(b'\x09\x03\x03\x4f\xee\xf4\x01', 7))
        self.assertAlmostEqual(self.data_float.value, expected)

    def test_time_processor_unsubscribe(self):
        expected= [0x09, 0x07, 0x03, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.processors[1])
        self.assertEqual(self.command, expected)

class TestTemperatureConversionRPro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        e = threading.Event()

        def f_add_processor_created(context, signal):
            self.fahrenheit_signal= signal
            self.libmetawear.mbl_mw_dataprocessor_math_create(self.temp_signal, MathOperation.ADD, 273.15, context, k_add_handler)

        def k_add_processor_created(context, signal):
            self.kelvin_signal= signal

            self.libmetawear.mbl_mw_datasignal_subscribe(self.temp_signal, None, self.sensor_data_handler)
            self.libmetawear.mbl_mw_datasignal_subscribe(self.fahrenheit_signal, None, self.sensor_data_handler)
            self.libmetawear.mbl_mw_datasignal_subscribe(self.kelvin_signal, None, self.sensor_data_handler)

            e.set()

        f_mult_handler= FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_math_create(p, MathOperation.DIVIDE, 10.0, ctx, f_divide_handler))
        f_divide_handler= FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_math_create(p, MathOperation.ADD, 32.0, ctx, f_add_handler))
        f_add_handler= FnVoid_VoidP_VoidP(f_add_processor_created)
        k_add_handler= FnVoid_VoidP_VoidP(k_add_processor_created)

        self.temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 
                MetaWearRProChannel.ON_DIE)
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.temp_signal, MathOperation.MULTIPLY, 18.0, None, f_mult_handler)
        e.wait()

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
                self.notify_mw_char(resp[0])
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

class TestAccAxisProcessing(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.firmware_revision= create_string_buffer(b'1.2.3', 5)

        super().setUp()

        self.libmetawear.mbl_mw_acc_set_range(self.board, 16.0)
        self.acc_signal = self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)

    def test_acc_z_pulse_setup(self):
        expected= [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0x24, 0x0b, 0x01, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x10, 0x00],
            [0x09, 0x07, 0x00, 0x01],
            [0x09, 0x03, 0x01]
        ]

        def pulse_created(context, pulse):
            self.libmetawear.mbl_mw_datasignal_subscribe(pulse, None, self.sensor_data_handler)
            e.set()

        e = threading.Event()

        pulse_handler = FnVoid_VoidP_VoidP(pulse_created)

        acc_z_signal = self.libmetawear.mbl_mw_datasignal_get_component(self.acc_signal, Const.ACC_ACCEL_Z_AXIS_INDEX)
        self.libmetawear.mbl_mw_dataprocessor_pulse_create(acc_z_signal, PulseOutput.AREA, 1.0, 16, None, pulse_handler)
        e.wait()

        self.assertEqual(self.command_history, expected)

    def test_acc_y_threshold(self):
        expected= [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0x22, 0x0d, 0x0d, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x00, 0x06, 0x01, 0x01],
            [0x09, 0x07, 0x01, 0x01],
            [0x09, 0x03, 0x01]
        ]

        e = threading.Event()
        def comp_created(context, comp):
            self.libmetawear.mbl_mw_datasignal_subscribe(comp, None, self.sensor_data_handler)
            e.set()

        ths_created= FnVoid_VoidP_VoidP(lambda ctx, t: self.libmetawear.mbl_mw_dataprocessor_comparator_create(t, ComparatorOperation.EQ, 1.0, ctx, comp_created))
        comp_created= FnVoid_VoidP_VoidP(comp_created)

        acc_y_signal = self.libmetawear.mbl_mw_datasignal_get_component(self.acc_signal, Const.ACC_ACCEL_Y_AXIS_INDEX)
        self.libmetawear.mbl_mw_dataprocessor_threshold_create(acc_y_signal, ThresholdMode.BINARY, 1.0, 0.0, None, ths_created)
        e.wait()

        self.assertEqual(self.command_history, expected)

class TestGpioAdcPulse(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        e = threading.Event()

        def pulse_processor_created(context, pulse):
            self.pulse_signal= pulse
            self.libmetawear.mbl_mw_datasignal_subscribe(pulse, None, self.sensor_data_handler)

            e.set()

        pulse_handler= FnVoid_VoidP_VoidP(pulse_processor_created)
        gpio_adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0, GpioAnalogReadMode.ADC)
        self.libmetawear.mbl_mw_dataprocessor_pulse_create(gpio_adc_signal, PulseOutput.PEAK, 500.0, 10, None, pulse_handler)

        e.wait()

    def test_pulse_setup(self):
        expected= [
            [0x09, 0x02, 0x05, 0xc7, 0x00, 0x20, 0x0b, 0x01, 0x00, 0x02, 0xf4, 0x01, 0x00, 0x00, 0x0a, 0x00],
            [0x09, 0x07, 0x00, 0x01],
            [0x09, 0x03, 0x01]
        ]

        self.assertEqual(self.command_history, expected)

    def test_pulse_data(self):
        expected= 789

        self.notify_mw_char(create_string_buffer(b'\x09\x03\x00\x15\x03\x00\x00', 7))
        self.assertEqual(self.data_uint32.value, expected)

    def test_pulse_unsubscribe(self):
        expected= [0x09, 0x07, 0x00, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.pulse_signal)
        self.assertEqual(self.command, expected)

class TestGpioFeedbackSetup(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        outer = threading.Event()
        handlers = {}

        def passthrough_processor_created(context, signal):
            self.offset_passthrough= signal
            self.libmetawear.mbl_mw_dataprocessor_math_create(self.gpio_abs_ref_signal, MathOperation.SUBTRACT, 0.0, context, handlers["math"])

        def math_processor_created(context, signal):
            self.abs_ref_offset= signal
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, ComparatorOperation.GT, 0.0, context, handlers["gt_comparator"])

        def gt_comparator_processor_created(context, signal):
            self.gt_comparator= signal
            self.libmetawear.mbl_mw_dataprocessor_counter_create(signal, context, handlers["gt_counter"])

        def gt_counter_processor_created(context, signal):
            self.gt_comparator_counter= signal
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, ComparatorOperation.EQ, 16.0, context, handlers["gt_counter_comparator"])

        def gt_counter_comparator_processor_created(context, signal):
            self.gt_comparator_counter_comparator= signal
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.abs_ref_offset, ComparatorOperation.LTE, 0.0, context, handlers["lte_comparator"])

        def lte_comparator_processor_created(context, signal):
            self.lte_comparator= signal
            self.libmetawear.mbl_mw_dataprocessor_counter_create(signal, context, handlers["lte_counter"])

        def lte_counter_processor_created(context, signal):
            self.lte_comparator_counter= signal
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(signal, ComparatorOperation.EQ, 16.0, context, handlers["lte_counter_comparator"])

        def lte_counter_comparator_processor_created(context, signal):
            e = threading.Event()
            event_handler = FnVoid_VoidP_VoidP_Int(lambda event, ctx, s: e.set())

            self.libmetawear.mbl_mw_event_record_commands(self.offset_passthrough)
            self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.lte_comparator_counter, 0)
            self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.gt_comparator_counter, 0)
            self.libmetawear.mbl_mw_dataprocessor_math_modify_rhs_signal(self.abs_ref_offset, self.offset_passthrough)
            self.libmetawear.mbl_mw_event_end_record(self.offset_passthrough, None, event_handler)
            e.wait()

            e.clear()
            self.libmetawear.mbl_mw_event_record_commands(self.gt_comparator)
            self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.lte_comparator_counter, 0)
            self.libmetawear.mbl_mw_event_end_record(self.gt_comparator, None, event_handler)
            e.wait()

            e.clear()
            self.libmetawear.mbl_mw_event_record_commands(self.gt_comparator_counter_comparator)
            self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(self.offset_passthrough, 1)
            self.libmetawear.mbl_mw_event_end_record(self.gt_comparator_counter_comparator, None, event_handler)
            e.wait()

            e.clear()
            self.libmetawear.mbl_mw_event_record_commands(self.lte_comparator)
            self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.gt_comparator_counter, 0)
            self.libmetawear.mbl_mw_event_end_record(self.lte_comparator, None, event_handler)
            e.wait()

            e.clear()
            self.libmetawear.mbl_mw_event_record_commands(signal)
            self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(self.offset_passthrough, 1)
            self.libmetawear.mbl_mw_event_end_record(signal, None, event_handler)
            e.wait()

            outer.set()

        handlers["passthrough"] = FnVoid_VoidP_VoidP(passthrough_processor_created)
        handlers["math"] = FnVoid_VoidP_VoidP(math_processor_created)
        handlers["gt_comparator"] = FnVoid_VoidP_VoidP(gt_comparator_processor_created)
        handlers["gt_counter"]= FnVoid_VoidP_VoidP(gt_counter_processor_created)
        handlers["gt_counter_comparator"]= FnVoid_VoidP_VoidP(gt_counter_comparator_processor_created)
        handlers["lte_comparator"]= FnVoid_VoidP_VoidP(lte_comparator_processor_created)
        handlers["lte_counter"]= FnVoid_VoidP_VoidP(lte_counter_processor_created)
        handlers["lte_counter_comparator"]= FnVoid_VoidP_VoidP(lte_counter_comparator_processor_created)

        self.gpio_abs_ref_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0, GpioAnalogReadMode.ABS_REF)
        self.libmetawear.mbl_mw_dataprocessor_passthrough_create(self.gpio_abs_ref_signal, PassthroughMode.COUNT, 0, None, handlers["passthrough"])
        outer.wait()

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

        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def set_count(self):
        self.status= self.libmetawear.mbl_mw_dataprocessor_passthrough_set_count(self.processors[0], 20)

    def test_valid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_passthrough_create(self.baro_pa_signal, PassthroughMode.COUNT, 10, None, self.processor_handler)
        self.events["processor"].wait()

        self.set_count()
        self.assertEqual(self.status, Const.STATUS_OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_sample_create(self.baro_pa_signal, 16, None, self.processor_handler)
        self.events["processor"].wait()

        self.set_count()
        self.assertEqual(self.status, Const.STATUS_WARNING_INVALID_PROCESSOR_TYPE)

class TestAccumulatorSetSum(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def set_state(self):
        self.status= self.libmetawear.mbl_mw_dataprocessor_set_accumulator_state(self.processors[0], 101325.0)

    def test_valid_set_state(self):
        self.libmetawear.mbl_mw_dataprocessor_accumulator_create(self.baro_pa_signal, None, self.processor_handler)
        self.events["processor"].wait()

        self.set_state()
        self.assertEqual(self.status, Const.STATUS_OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_time_create(self.baro_pa_signal, TimeMode.DIFFERENTIAL, 30000, None, self.processor_handler)
        self.events["processor"].wait()

        self.set_state()
        self.assertEqual(self.status, Const.STATUS_WARNING_INVALID_PROCESSOR_TYPE)

class TestCounterSetCount(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def set_state(self):
        self.status= self.libmetawear.mbl_mw_dataprocessor_counter_set_state(self.processors[0], 128)

    def test_valid_set_state(self):
        self.libmetawear.mbl_mw_dataprocessor_counter_create(self.baro_pa_signal, None, self.processor_handler)
        self.events["processor"].wait()

        self.set_state()
        self.assertEqual(self.status, Const.STATUS_OK)

    def test_invalid_set_count(self):
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.baro_pa_signal, ComparatorOperation.LT, 101325.0, None, self.processor_handler)
        self.events["processor"].wait()

        self.set_state()
        self.assertEqual(self.status, Const.STATUS_WARNING_INVALID_PROCESSOR_TYPE)

class TestAverageReset(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def reset(self):
        self.status= self.libmetawear.mbl_mw_dataprocessor_average_reset(self.processors[0])

    def test_valid_reset(self):
        self.libmetawear.mbl_mw_dataprocessor_average_create(self.baro_pa_signal, 8, None, self.processor_handler)
        self.events["processor"].wait()

        self.reset()
        self.assertEqual(self.status, Const.STATUS_OK)

    def test_invalid_reset(self):
        self.libmetawear.mbl_mw_dataprocessor_pulse_create(self.baro_pa_signal, PulseOutput.AREA, 101325.0, 64, None, self.processor_handler)
        self.events["processor"].wait()

        self.reset()
        self.assertEqual(self.status, Const.STATUS_WARNING_INVALID_PROCESSOR_TYPE)

class TestDeltaSetPrevious(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.baro_pa_signal= self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)

    def set_reference(self):
        self.status= self.libmetawear.mbl_mw_dataprocessor_delta_set_reference(self.processors[0], 101325.0)

    def test_valid_set_previous(self):
        self.libmetawear.mbl_mw_dataprocessor_delta_create(self.baro_pa_signal, DeltaMode.DIFFERENTIAL, 25331.25, None, self.processor_handler)
        self.events["processor"].wait()

        self.set_reference()
        self.assertEqual(self.status, Const.STATUS_OK)

    def test_invalid_set_previous(self):
        self.libmetawear.mbl_mw_dataprocessor_math_create(self.baro_pa_signal, MathOperation.DIVIDE, 1000.0, None, self.processor_handler)
        self.events["processor"].wait()

        self.set_reference()
        self.assertEqual(self.status, Const.STATUS_WARNING_INVALID_PROCESSOR_TYPE)

class TestThreshold(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD

        super().setUp()

    def test_valid_set_count(self):
        expected= [0x09, 0x02, 0x16, 0xc1, 0xff, 0x60, 0x0d, 0x0b, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x00]

        signal= self.libmetawear.mbl_mw_humidity_bme280_get_percentage_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_threshold_create(signal, ThresholdMode.BINARY, 57.0, 0.0, None, self.processor_handler)
        self.events["processor"].wait()

        self.assertEqual(self.command, expected)

class TestThreeAxisRightShift(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD
        self.processor_created_fn= FnVoid_VoidP_VoidP(self.processor_created)
        self.logger_created_fn= FnVoid_VoidP_VoidP(self.logger_created)

    def test_acc_rshift(self):
        e = threading.Event()
        handlers= {
            "rshift": FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_datasignal_log(p, None, handlers["rshift_logger"])),
            "rshift_logger": FnVoid_VoidP_VoidP(lambda ctx, p: e.set())
        }

        processor_created_fn= FnVoid_VoidP_VoidP(self.processor_created)

        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_math_create(signal, MathOperation.RSHIFT, 8, None, handlers["rshift"])
        e.wait()

        expected= [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x09, 0x14, 0x08, 0x08, 0x00, 0x00, 0x00, 0x02],
            [0x0b, 0x02, 0x09, 0x03, 0x00, 0x40]
        ]
        self.assertEqual(self.command_history, expected)

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

        e = threading.Event()
        rss_created_ptr= FnVoid_VoidP_VoidP(lambda ctx, rms: self.libmetawear.mbl_mw_dataprocessor_comparator_create(rms, ComparatorOperation.GT, 250.0, 
                ctx, comp_created_ptr))
        comp_created_ptr= FnVoid_VoidP_VoidP(lambda ctx, comp: e.set())

        signal= self.libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(signal, None, rss_created_ptr)
        e.wait()

        self.assertEqual(self.command, expected)

class TestDataProcessorTimeout(TestMetaWearBase):
    def dataprocessor_created(self, context, processor):
        self.created_proc= processor
        self.e.set()
        
    def commandLogger(self, context, board, writeType, characteristic, command, length):
        if (command[0] == 0x9 and command[1] == 0x2):
            self.notify_mw_char(create_string_buffer(b'\x0a\x00', 2))
        else:
            super().commandLogger(context, board, writeType, characteristic, command, length)

    def test_timeout_no_state(self):
        dataprocessor_created_fn= FnVoid_VoidP_VoidP(self.dataprocessor_created)
        self.e= threading.Event()
        signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(signal, None, dataprocessor_created_fn)
        self.e.wait()

        self.assertIsNone(self.created_proc)

    def test_timeout_with_state(self):
        dataprocessor_created_fn= FnVoid_VoidP_VoidP(self.dataprocessor_created)
        self.e= threading.Event()
        signal= self.libmetawear.mbl_mw_switch_get_state_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_accumulator_create(signal, None, dataprocessor_created_fn)
        self.e.wait()

        self.assertIsNone(self.created_proc)

class TestMultiComparatorRedirect(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.firmware_revision= create_string_buffer(b'1.2.3', 5)

        super().setUp()

    def test_modify(self):
        expected = [
            [0x09, 0x02, 0x03, 0x04, 0xFF, 0xA0, 0x07, 0xA5, 0x01],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x06, 0x22, 0x00, 0x80],
            [0x09, 0x05, 0x01, 0x06, 0x22, 0x00, 0x20],
            [0x09, 0x05, 0x01, 0x06, 0x22, 0x00, 0x80]
        ]

        acc_signal = self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(acc_signal, None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()
        self.libmetawear.mbl_mw_dataprocessor_comparator_create(self.processors[0], ComparatorOperation.GT, 2.0, None, self.processor_handler)
        self.events["processor"].wait()

        self.libmetawear.mbl_mw_dataprocessor_comparator_modify(self.processors[1], ComparatorOperation.GT, 0.5)
        self.libmetawear.mbl_mw_dataprocessor_comparator_modify(self.processors[1], ComparatorOperation.GT, 2.0)

        self.assertEqual(self.command_history, expected)

class TestMultiComparator(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.firmware_revision= create_string_buffer(b'1.2.3', 5)

        super().setUp()

    def test_reference(self):
        expected= [0x09, 0x02, 0x05, 0xc7, 0x15, 0x20, 0x06, 0x6a, 0x00, 0x04, 0x00, 0x02, 0x00, 0x01, 0x80, 0x00]

        references= (c_float * 4)(1024, 512, 256, 128)
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x15, GpioAnalogReadMode.ADC)

        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(adc_signal, ComparatorOperation.GTE, ComparatorMode.REFERENCE, references, 
                len(references), None, self.processor_handler)
        self.events["processor"].wait()

        self.assertEqual(self.command, expected)

    def test_zone(self):
        expected= [0x09, 0x02, 0x14, 0x03, 0xff, 0x60, 0x06, 0x96, 0x00, 0xc4, 0x02, 0x00, 0xfa, 0x33, 0x5b, 0x00, 0x2a, 0x6d, 0x75, 0x01]

        references= (c_float * 3)(181248, 5977082, 24472874)
        signal= self.libmetawear.mbl_mw_als_ltr329_get_illuminance_data_signal(self.board)

        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(signal, ComparatorOperation.LT, ComparatorMode.ZONE, references, 
                len(references), None, self.processor_handler)
        self.events["processor"].wait()

        self.assertEqual(self.command, expected)

    def test_absolute(self):
        expected= [0x09, 0x02, 0x04, 0xc1, 0x01, 0x20, 0x06, 0x03, 0xc0, 0x00, 0xc8, 0x00, 0xd0, 0x00]

        references= (c_float * 3)(24.0, 25.0, 26.0)
        signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, MetaWearRProChannel.ON_BOARD_THERMISTOR)

        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(signal, ComparatorOperation.EQ, ComparatorMode.ABSOLUTE, references, 
                len(references), None, self.processor_handler)
        self.events["processor"].wait()

        self.assertEqual(self.command, expected)

    def test_modify_config(self):
        references= (c_float * 4)(1024, 512, 256, 128)
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x15, GpioAnalogReadMode.ADC)

        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(adc_signal, ComparatorOperation.GTE, ComparatorMode.REFERENCE, references, 
                len(references), None, self.processor_handler)
        self.events["processor"].wait()

        expected= [0x09, 0x05, 0x00, 0x06, 0x52, 0x80, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x04]
        references= (c_float * 4)(128, 256, 512, 1024)
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_modify(self.processors[0], ComparatorOperation.LT, references, len(references))

        self.assertEqual(self.command, expected)

    def test_modify_config_signal(self):
        expected_cmds= [
            [0x09, 0x02, 0x05, 0xc6, 0x00, 0x20, 0x06, 0x2a, 0x80, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x00, 0x09, 0x05, 0x05, 0x05, 0x03],
            [0x0a, 0x03, 0x00, 0x06, 0x2a, 0x00, 0x00]
        ]

        references= (c_float * 1)(128)
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x0, GpioAnalogReadMode.ABS_REF)

        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(adc_signal, ComparatorOperation.GTE, ComparatorMode.ABSOLUTE, references, 
                len(references), None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()

        self.libmetawear.mbl_mw_event_record_commands(self.processors[0])
        self.libmetawear.mbl_mw_dataprocessor_comparator_modify_signal(self.processors[0], ComparatorOperation.GTE, self.processors[0])
        self.libmetawear.mbl_mw_event_end_record(self.processors[0], None, self.commands_recorded_fn)
        self.events["event"].wait()

        self.assertEqual(self.command_history, expected_cmds)

class TestMathProcessor(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, MetaWearRChannel.ON_DIE)
        self.libmetawear.mbl_mw_dataprocessor_math_create(temp_signal, MathOperation.CONSTANT, 25.0, None, self.processor_handler)
        self.events["processor"].wait()

    def test_constant_op(self):
        expected= [0x09, 0x02, 0x04, 0xc1, 0x00, 0x20, 0x09, 0x17, 0x0b, 0x19, 0x00, 0x00, 0x00, 0x00]

        self.assertEqual(self.command, expected)

    def test_constant_op_data(self):
        expected= 25

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[0], None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x09\x03\x00\x19\x00\x00\x00', 7))
        self.assertEqual(self.data_int32.value, expected)

class TestSensorFusionLimiter(TestMetaWearBase):
    def setUp(self):
        self.metawear_motion_r_services[0x09]= create_string_buffer(b'\x09\x80\x00\x01\x1c', 5)
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

    def test_quaternion(self):
        expected = [
            [0x09, 0x02, 0x19, 0x07, 0xff, 0xe0, 0x08, 0x17, 0x14, 0x00, 0x00, 0x00],
            [0x0b, 0x02, 0x09, 0x03, 0x00, 0x60],
            [0x0b, 0x02, 0x09, 0x03, 0x00, 0x64],
            [0x0b, 0x02, 0x09, 0x03, 0x00, 0x68],
            [0x0b, 0x02, 0x09, 0x03, 0x00, 0x6c]
        ]

        signal = self.libmetawear.mbl_mw_sensor_fusion_get_data_signal(self.board, SensorFusionData.QUATERNION)
        self.libmetawear.mbl_mw_dataprocessor_time_create(signal, TimeMode.ABSOLUTE, 20, None, self.processor_handler)
        self.events["processor"].wait()

        self.libmetawear.mbl_mw_datasignal_log(self.processors[0], None, self.logger_created)
        self.events["log"].wait()

        self.assertEqual(self.command_history, expected)

class TestAccounter(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.notify_mw_char(create_string_buffer(b'\x0b\x84\x04\x33\x0d\x00\x03', 7))

        signal = self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_accounter_create(signal, None, self.processor_handler)
        self.events["processor"].wait()

    def test_create(self):
        expected = [ 0x9, 0x2, 0x3, 0x4, 0xff, 0xa0, 0x11, 0x31, 0x3 ]

        self.assertEqual(self.command, expected)

    def test_data_extraction(self):
        expected = CartesianFloat(x= 0.0118, y= 0.573, z= -0.795)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[0], None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x09\x03\x00\xa6\x33\x0d\x00\xc1\x00\xb1\x24\x19\xcd', 13))

        self.assertEqual(self.data, expected)

    def test_time_offset(self):
        expected = [10, 11, 8, 11, 10]

        offsets = []
        prev_time = []
        def handle_sensor_data(context, data):
            if len(prev_time) == 0:
                prev_time.append(data.contents.epoch)
            else:
                offsets.append(data.contents.epoch - prev_time[0])
                prev_time[0] = data.contents.epoch

        fn_wrapper = FnVoid_VoidP_DataP(handle_sensor_data)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[0], None, fn_wrapper)

        responses = [
            [0x09, 0x03, 0x00, 0xa6, 0x33, 0x0d, 0x00, 0xc1, 0x00, 0xb1, 0x24, 0x19, 0xcd],
            [0x09, 0x03, 0x00, 0xad, 0x33, 0x0d, 0x00, 0xd4, 0x00, 0x18, 0x25, 0xc0, 0xcc],
            [0x09, 0x03, 0x00, 0xb4, 0x33, 0x0d, 0x00, 0xc7, 0x00, 0x09, 0x25, 0xb2, 0xcc],
            [0x09, 0x03, 0x00, 0xba, 0x33, 0x0d, 0x00, 0xc5, 0x00, 0x17, 0x25, 0xbc, 0xcc],
            [0x09, 0x03, 0x00, 0xc1, 0x33, 0x0d, 0x00, 0xd4, 0x00, 0xe9, 0x24, 0xe4, 0xcc],
            [0x09, 0x03, 0x00, 0xc8, 0x33, 0x0d, 0x00, 0xaf, 0x00, 0xf7, 0x24, 0xe3, 0xcc]
        ]
        for r in responses:
            self.notify_mw_char(to_string_buffer(r))

        self.assertEqual(offsets, expected)

    def test_count_and_time(self):
        epochs = []
        fn_wrapper = FnVoid_VoidP_DataP(lambda ctx, p: epochs.append(p.contents.epoch))

        self.events["processor"].clear()
        acc = self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_packer_create(acc, 2, None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()
        self.libmetawear.mbl_mw_dataprocessor_accounter_create_count(self.processors[1], None, self.processor_handler)
        self.events["processor"].wait()

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[2], None, self.sensor_data_handler)

        self.events["processor"].clear()
        barometer = self.libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_accounter_create(barometer, None, self.processor_handler)
        self.events["processor"].wait()

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[3], None, fn_wrapper)

        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x03, 0x72, 0xA4, 0x03, 0x00, 0x77, 0x6C, 0x84, 0x01]))
        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x02, 0x8D, 0x00, 0x00, 0x00, 0x4E, 0xFF, 0x35, 0xFD, 0x79, 0x07, 0x4D, 0xFF, 0x35, 0xFD, 0x7D, 0x07]))
        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x03, 0xA4, 0xA4, 0x03, 0x00, 0x05, 0x65, 0x84, 0x01]))

        self.assertEqual(epochs[1] - epochs[0], 73)

class TestAccounterCount(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        signal = self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_accounter_create_count(signal, None, self.processor_handler)
        self.events["processor"].wait()

    def test_create(self):
        expected = [ 0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x11, 0x30, 0x03 ]

        self.assertEqual(self.command, expected)

    def test_count_extraction(self):
        expected = 492
        actual = []

        def data_handler(context, data):
            actual.append(cast(data.contents.extra, POINTER(c_uint)).contents.value)
        data_handler_fn = FnVoid_VoidP_DataP(data_handler)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[0], None, data_handler_fn)
        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x00, 0xec, 0x01, 0x00, 0x00, 0x01, 0x0b, 0x9a, 0x07, 0x40, 0x40]))

        self.assertEqual(actual[0], expected)

class TestPacker(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        signal = self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, MetaWearRProChannel.ON_BOARD_THERMISTOR);
        self.libmetawear.mbl_mw_dataprocessor_packer_create(signal, 4, None, self.processor_handler)
        self.events["processor"].wait()

    def test_create(self):
        expected = [0x9, 0x2, 0x4, 0xc1, 0x1, 0x20, 0x10, 0x1, 0x3]

        self.assertEqual(self.command, expected)

    def test_data_extraction(self):
        expected = [30.625, 30.125, 30.25, 30.25]

        values = []
        def handle_sensor_data(context, data):
            data_ptr= cast(data.contents.value, POINTER(c_float))
            values.append(data_ptr.contents.value)

        fn_wrapper = FnVoid_VoidP_DataP(handle_sensor_data)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[0], None, fn_wrapper)
        self.notify_mw_char(create_string_buffer(b'\x09\x03\x00\xf5\x00\xf1\x00\xf2\x00\xf2\x00', 11))

        self.assertEqual(values, expected)

class TestAccounterPackerChain(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        signal = self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, MetaWearRProChannel.ON_BOARD_THERMISTOR);
        self.libmetawear.mbl_mw_dataprocessor_accounter_create(signal, None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()
        self.libmetawear.mbl_mw_dataprocessor_packer_create(self.processors[0], 2, None, self.processor_handler)
        self.events["processor"].wait()

    def test_create(self):
        expected = [
            [0x9, 0x2, 0x4, 0xc1, 0x1, 0x20, 0x11, 0x31, 0x3],
            [0x9, 0x2, 0x9, 0x3, 0x0, 0xa0, 0x10, 0x5, 0x1]
        ]

        self.assertEqual(self.command_history, expected)

    def test_data_extraction(self):
        expected = [29.5, 29.375, 29.875, 29.625]

        values = []
        def handle_sensor_data(context, data):
            data_ptr= cast(data.contents.value, POINTER(c_float))
            values.append(data_ptr.contents.value)

        fn_wrapper = FnVoid_VoidP_DataP(handle_sensor_data)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[1], None, fn_wrapper)
        self.notify_mw_char(create_string_buffer(b'\x09\x03\x01\x7b\x64\x02\x00\xec\x00\x92\x64\x02\x00\xeb\x00', 15))
        self.notify_mw_char(create_string_buffer(b'\x09\x03\x01\xa8\x64\x02\x00\xef\x00\xbf\x64\x02\x00\xed\x00', 15))

        self.assertEqual(values, expected)

    def test_time_offset(self):
        expected = [34, 32, 34]

        offsets = []
        prev_time = []
        def handle_sensor_data(context, data):
            if len(prev_time) == 0:
                prev_time.append(data.contents.epoch)
            else:
                offsets.append(data.contents.epoch - prev_time[0])
                prev_time[0] = data.contents.epoch

        fn_wrapper = FnVoid_VoidP_DataP(handle_sensor_data)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[1], None, fn_wrapper)

        responses = [
            [0x0b, 0x84, 0xf5, 0x62, 0x02, 0x00, 0x00],
            [0x09, 0x03, 0x01, 0x7b, 0x64, 0x02, 0x00, 0xec, 0x00, 0x92, 0x64, 0x02, 0x00, 0xeb, 0x00],
            [0x09, 0x03, 0x01, 0xa8, 0x64, 0x02, 0x00, 0xef, 0x00, 0xbf, 0x64, 0x02, 0x00, 0xed, 0x00]
        ]
        for r in responses:
            self.notify_mw_char(to_string_buffer(r))

        self.assertEqual(offsets, expected)

class TestPackerAccounterChain(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        signal = self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, MetaWearRProChannel.ON_BOARD_THERMISTOR);
        self.libmetawear.mbl_mw_dataprocessor_packer_create(signal, 4, None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()
        self.libmetawear.mbl_mw_dataprocessor_accounter_create(self.processors[0], None, self.processor_handler)
        self.events["processor"].wait()

    def test_create(self):
        expected = [
            [0x09, 0x02, 0x04, 0xc1, 0x01, 0x20, 0x10, 0x01, 0x03],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x11, 0x31, 0x03]
        ]

        self.assertEqual(self.command_history, expected)

    def test_data_extraction(self):
        expected = [24.5, 24.625, 24.5, 24.375, 24.25, 24.375, 24.5, 24.25]

        values = []
        def handle_sensor_data(context, data):
            data_ptr= cast(data.contents.value, POINTER(c_float))
            values.append(data_ptr.contents.value)

        fn_wrapper = FnVoid_VoidP_DataP(handle_sensor_data)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[1], None, fn_wrapper)

        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x01, 0x04, 0x85, 0xa0, 0x00, 0xc4, 0x00, 0xc5, 0x00, 0xc4, 0x00, 0xc3, 0x00]))
        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x01, 0x5e, 0x85, 0xa0, 0x00, 0xc2, 0x00, 0xc3, 0x00, 0xc4, 0x00, 0xc2, 0x00]))

        self.assertEqual(values, expected)

    def test_time_offset(self):
        expected = [0, 0, 0, 132, 0, 0, 0, 132, 0, 0, 0, 133, 0, 0, 0]

        offsets = []
        prev_time = []
        def handle_sensor_data(context, data):
            if len(prev_time) == 0:
                prev_time.append(data.contents.epoch)
            else:
                offsets.append(data.contents.epoch - prev_time[0])
                prev_time[0] = data.contents.epoch

        fn_wrapper = FnVoid_VoidP_DataP(handle_sensor_data)
        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[1], None, fn_wrapper)

        responses = [
            [0x0b, 0x84, 0x1c, 0x84, 0xa0, 0x00, 0x01],
            [0x09, 0x03, 0x01, 0x04, 0x85, 0xa0, 0x00, 0xc4, 0x00, 0xc5, 0x00, 0xc4, 0x00, 0xc3, 0x00],
            [0x09, 0x03, 0x01, 0x5e, 0x85, 0xa0, 0x00, 0xc2, 0x00, 0xc3, 0x00, 0xc4, 0x00, 0xc2, 0x00],
            [0x09, 0x03, 0x01, 0xb8, 0x85, 0xa0, 0x00, 0xc3, 0x00, 0xc4, 0x00, 0xc3, 0x00, 0xc3, 0x00],
            [0x09, 0x03, 0x01, 0x13, 0x86, 0xa0, 0x00, 0xc5, 0x00, 0xc3, 0x00, 0xc5, 0x00, 0xc2, 0x00]
        ]
        for r in responses:
            self.notify_mw_char(to_string_buffer(r))

        self.assertEqual(offsets, expected)

class TestHpf(TestMetaWearBase):
    def setUp(self):
        self.metawear_rpro_services[0x09] = create_string_buffer(b'\x09\x80\x00\x02\x1C', 5)
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        signal = self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_dataprocessor_highpass_create(signal, 4, None, self.processor_handler)
        self.events["processor"].wait()        

    def test_create_acc(self):
        expected = [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x03, 0x25, 0x04, 0x02]
        ]

        self.assertEqual(self.command_history, expected)

    def test_acc_hpf_data(self):
        expected = CartesianFloat(x= -0.00104, y= 0.0025, z= 0.0013)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[0], None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x00, 0xef, 0xff, 0x29, 0x00, 0x16, 0x00]))

        self.assertEqual(self.data_cartesian_float, expected)

class TestFuser(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.metawear_rpro_services[0x09] = create_string_buffer(b'\x09\x80\x00\x03\x1C', 5)

        super().setUp()

        self.acc = self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.gyro = self.libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal(self.board)

        signals = (c_void_p * 1)()
        signals[0] = self.gyro
        self.libmetawear.mbl_mw_dataprocessor_fuser_create(self.acc, signals, 1, None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()
        self.libmetawear.mbl_mw_dataprocessor_time_create(self.processors[0], TimeMode.ABSOLUTE, 20, None, self.processor_handler)
        self.events["processor"].wait()

    def test_data_handling(self):
        parsed_values = {}
        def fused_data_handler(context, data):
            values = cast(data.contents.value, POINTER(POINTER(Data) * 2))

            self.sensorDataHandler(context, values.contents[0])
            parsed_values['acc'] = self.data

            self.sensorDataHandler(context, values.contents[1])
            parsed_values['gyro'] = self.data
        fn_wrapper = FnVoid_VoidP_DataP(fused_data_handler)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[1], None, fn_wrapper)
        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x02, 0xf4, 0x0d, 0x3c, 0x39, 0x99, 0x11, 0x01, 0x80, 0xd6, 0x91, 0xd3, 0x67]))

        self.libmetawear.mbl_mw_datasignal_subscribe(self.acc, None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x03, 0x04, 0xf4, 0x0d, 0x3c, 0x39, 0x99, 0x11]))
        self.assertEqual(parsed_values['acc'], self.data)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.gyro, None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x13, 0x05, 0x01, 0x80, 0xd6, 0x91, 0xd3, 0x67]))
        self.assertEqual(parsed_values['gyro'], self.data)

    def test_commands(self):
        expected= [
            [0x09, 0x02, 0x13, 0x05, 0xff, 0xa0, 0x0f, 0x05],
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x1b, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x08, 0x13, 0x14, 0x00, 0x00, 0x00]
        ]

        self.assertEqual(self.command_history, expected)

class TestFuserAccounter(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.metawear_rpro_services[0x09] = create_string_buffer(b'\x09\x80\x00\x03\x1C', 5)

        super().setUp()

        self.acc = self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.gyro = self.libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal(self.board)

        signals = (c_void_p * 1)()
        signals[0] = self.gyro
        self.libmetawear.mbl_mw_dataprocessor_fuser_create(self.acc, signals, 1, None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()
        self.libmetawear.mbl_mw_dataprocessor_time_create(self.processors[0], TimeMode.ABSOLUTE, 20, None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()
        self.libmetawear.mbl_mw_dataprocessor_accounter_create_count(self.processors[1], None, self.processor_handler)
        self.events["processor"].wait()

        self.events["processor"].clear()
        self.libmetawear.mbl_mw_dataprocessor_accounter_create(self.processors[1], None, self.processor_handler)
        self.events["processor"].wait()

    def test_data_handling(self):
        parsed_values = {}
        def fused_data_handler(context, data):
            counter = cast(data.contents.extra, POINTER(c_uint)).contents.value
            self.assertEqual(542, counter)

            values = cast(data.contents.value, POINTER(POINTER(Data) * 2))

            self.sensorDataHandler(context, values.contents[0])
            parsed_values['acc'] = self.data

            self.sensorDataHandler(context, values.contents[1])
            parsed_values['gyro'] = self.data

        fn_wrapper = FnVoid_VoidP_DataP(fused_data_handler)

        parsed_values_no_counter = {}
        def fused_data_handler_no_counter(context, data):
            values = cast(data.contents.value, POINTER(POINTER(Data) * 2))

            self.sensorDataHandler(context, values.contents[0])
            parsed_values_no_counter['acc'] = self.data

            self.sensorDataHandler(context, values.contents[1])
            parsed_values_no_counter['gyro'] = self.data

        fn_wrapper_no_counter = FnVoid_VoidP_DataP(fused_data_handler_no_counter)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[3], None, fn_wrapper_no_counter)
        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x04, 0x1e, 0x02, 0x00, 0x00, 0xf4, 0x0d, 0x3c, 0x39, 0x99, 0x11, 0x01, 0x80, 0xd6, 0x91, 0xd3, 0x67]))

        self.libmetawear.mbl_mw_datasignal_subscribe(self.acc, None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x03, 0x04, 0xf4, 0x0d, 0x3c, 0x39, 0x99, 0x11]))
        self.assertEqual(parsed_values_no_counter['acc'], self.data)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.gyro, None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x13, 0x05, 0x01, 0x80, 0xd6, 0x91, 0xd3, 0x67]))
        self.assertEqual(parsed_values_no_counter['gyro'], self.data)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.processors[2], None, fn_wrapper)
        self.notify_mw_char(to_string_buffer([0x09, 0x03, 0x03, 0x1e, 0x02, 0x00, 0x00, 0xf4, 0x0d, 0x3c, 0x39, 0x99, 0x11, 0x01, 0x80, 0xd6, 0x91, 0xd3, 0x67]))

        self.libmetawear.mbl_mw_datasignal_subscribe(self.acc, None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x03, 0x04, 0xf4, 0x0d, 0x3c, 0x39, 0x99, 0x11]))
        self.assertEqual(parsed_values['acc'], self.data)

        self.libmetawear.mbl_mw_datasignal_subscribe(self.gyro, None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x13, 0x05, 0x01, 0x80, 0xd6, 0x91, 0xd3, 0x67]))
        self.assertEqual(parsed_values['gyro'], self.data)

    def test_commands(self):
        expected= [
            [0x09, 0x02, 0x13, 0x05, 0xff, 0xa0, 0x0f, 0x05],
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x1b, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x08, 0x13, 0x14, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x60, 0x11, 0x30, 0x03],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x60, 0x11, 0x31, 0x03]
        ]

        self.assertEqual(self.command_history, expected)
