from common import TestMetaWearBase
from ctypes import byref
from cbindings import *
#from mbientlab.metawear.cbindings import *
import threading

class TestMacro(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def test_led_on_boot(self):
        e = threading.Event()
        callback = FnVoid_VoidP_VoidP_Int(lambda ctx, board, status: e.set())

        pattern= LedPattern(rise_time_ms = 0, pulse_duration_ms = 1000, repeat_count = 5, high_time_ms = 500, high_intensity = 16, low_intensity = 16)

        self.libmetawear.mbl_mw_macro_record(self.board, 1)
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.BLUE)
        self.libmetawear.mbl_mw_led_play(self.board)
        self.libmetawear.mbl_mw_macro_end_record(self.board, None, callback)

        e.wait()

        expected = [
            [0x02, 0x03, 0x02, 0x02, 0x10, 0x10, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x05],
            [0x02, 0x01, 0x01],
            [0x0f, 0x02, 0x01],
            [0x0f, 0x03, 0x02, 0x03, 0x02, 0x02, 0x10, 0x10, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x05],
            [0x0f, 0x03, 0x02, 0x01, 0x01],
            [0x0f, 0x04]
        ]
        self.assertEqual(self.command_history, expected)

    def test_freefall_on_boot(self):
        events = [threading.Event(), threading.Event()]

        def threshold_processor_created(context, pointer):
            threshold_signal= pointer
            self.comparator_below_handler = FnVoid_VoidP_VoidP(lambda ctx, pointer: self.libmetawear.mbl_mw_dataprocessor_comparator_create(threshold_signal, 
                ComparatorOperation.EQ, 1.0, ctx, comparator_above_handler))
            comparator_above_handler = FnVoid_VoidP_VoidP(lambda ctx, pointer: events[1].set())
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(pointer, ComparatorOperation.EQ, -1.0, context, self.comparator_below_handler)

        rss_handler= FnVoid_VoidP_VoidP(lambda ctx, pointer: self.libmetawear.mbl_mw_dataprocessor_average_create(pointer, 4, ctx, avg_handler))
        avg_handler= FnVoid_VoidP_VoidP(lambda ctx, pointer: self.libmetawear.mbl_mw_dataprocessor_threshold_create(pointer, ThresholdMode.BINARY, 0.5, 0.0,  
                ctx, threshold_handler))
        threshold_handler= FnVoid_VoidP_VoidP(threshold_processor_created)
        callback = FnVoid_VoidP_VoidP_Int(lambda ctx, board, status: events[0].set())

        accel_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)

        self.libmetawear.mbl_mw_macro_record(self.board, 1)
        self.libmetawear.mbl_mw_dataprocessor_rss_create(accel_signal, None, rss_handler)
        events[1].wait()

        self.libmetawear.mbl_mw_acc_enable_acceleration_sampling(self.board)
        self.libmetawear.mbl_mw_acc_start(self.board)

        self.libmetawear.mbl_mw_macro_end_record(self.board, None, callback)
        events[0].wait()

        expected = [
            [0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x07, 0xa5, 0x01],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x03, 0x05, 0x04],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x20, 0x0d, 0x09, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x00, 0x06, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff],
            [0x09, 0x02, 0x09, 0x03, 0x02, 0x00, 0x06, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00],
            [0x03, 0x02, 0x01, 0x00],
            [0x03, 0x01, 0x01],
            [0x0f, 0x02, 0x01],
            [0x0f, 0x03, 0x09, 0x02, 0x03, 0x04, 0xff, 0xa0, 0x07, 0xa5, 0x01],
            [0x0f, 0x03, 0x09, 0x02, 0x09, 0x03, 0x00, 0x20, 0x03, 0x05, 0x04],
            [0x0f, 0x03, 0x09, 0x02, 0x09, 0x03, 0x01, 0x20, 0x0d, 0x09, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00],
            [0x0f, 0x03, 0x09, 0x02, 0x09, 0x03, 0x02, 0x00, 0x06, 0x01, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff],
            [0x0f, 0x03, 0x09, 0x02, 0x09, 0x03, 0x02, 0x00, 0x06, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00],
            [0x0f, 0x03, 0x03, 0x02, 0x01, 0x00],
            [0x0f, 0x03, 0x03, 0x01, 0x01],
            [0x0f, 0x04]
        ]
        self.maxDiff = None
        self.assertEqual(self.command_history, expected)

    def test_led_controller(self):
        control_signals = [None, None, None]
        events = [threading.Event(), threading.Event(), threading.Event(), threading.Event()]

        def comp_even_created(context, pointer):
            control_signals[1] = pointer
            events[0].set()

        def comp_odd_created(context, pointer):
            control_signals[0] = pointer
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(control_signals[2], ComparatorOperation.EQ, 0.0, context, comparator_even_handler)

        def math_created(context, pointer):
            control_signals[2] = pointer
            self.libmetawear.mbl_mw_dataprocessor_comparator_create(pointer, ComparatorOperation.EQ, 1.0, context, comparator_odd_handler)

        math_handler = FnVoid_VoidP_VoidP(math_created)
        comparator_even_handler = FnVoid_VoidP_VoidP(comp_even_created)
        comparator_odd_handler = FnVoid_VoidP_VoidP(comp_odd_created)
        counter_handler = FnVoid_VoidP_VoidP(lambda ctx, p: self.libmetawear.mbl_mw_dataprocessor_math_create(p, MathOperation.MODULUS, 2.0, ctx, math_handler))

        switch_signal= self.libmetawear.mbl_mw_switch_get_state_data_signal(self.board)

        self.libmetawear.mbl_mw_macro_record(self.board, 0)
        self.libmetawear.mbl_mw_dataprocessor_counter_create(switch_signal, None, counter_handler)
        events[0].wait()

        event_handler = FnVoid_VoidP_VoidP_Int(lambda ctx, e, s: events[1].set())
        pattern= LedPattern(pulse_duration_ms=1000, high_time_ms=500, high_intensity=16, low_intensity=16, 
                repeat_count=Const.LED_REPEAT_INDEFINITELY)
        self.libmetawear.mbl_mw_event_record_commands(control_signals[0])
        self.libmetawear.mbl_mw_led_write_pattern(self.board, byref(pattern), LedColor.BLUE)
        self.libmetawear.mbl_mw_led_play(self.board)
        self.libmetawear.mbl_mw_event_end_record(control_signals[0], None, event_handler)
        events[1].wait()

        event_handler = FnVoid_VoidP_VoidP_Int(lambda ctx, e, s: events[2].set())
        self.libmetawear.mbl_mw_event_record_commands(control_signals[1])
        self.libmetawear.mbl_mw_led_stop_and_clear(self.board)
        self.libmetawear.mbl_mw_event_end_record(control_signals[1], None, event_handler)
        events[2].wait()

        callback = FnVoid_VoidP_VoidP_Int(lambda ctx, board, status: events[3].set())
        self.libmetawear.mbl_mw_macro_end_record(self.board, None, callback)
        events[3].wait()

        expected = [
            [0x09, 0x02, 0x01, 0x01, 0xff, 0x00, 0x02, 0x10],
            [0x09, 0x02, 0x09, 0x03, 0x00, 0x00, 0x09, 0x03, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00],
            [0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x0a, 0x02, 0x09, 0x03, 0x02, 0x02, 0x03, 0x0f],
            [0x0a, 0x03, 0x02, 0x02, 0x10, 0x10, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0xff],
            [0x0a, 0x02, 0x09, 0x03, 0x02, 0x02, 0x01, 0x01],
            [0x0a, 0x03, 0x01],
            [0x0a, 0x02, 0x09, 0x03, 0x03, 0x02, 0x02, 0x01],
            [0x0a, 0x03, 0x01],
            [0x0f, 0x02, 0x00],
            [0x0f, 0x03, 0x09, 0x02, 0x01, 0x01, 0xff, 0x00, 0x02, 0x10],
            [0x0f, 0x03, 0x09, 0x02, 0x09, 0x03, 0x00, 0x00, 0x09, 0x03, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00],
            [0x0f, 0x03, 0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00],
            [0x0f, 0x03, 0x09, 0x02, 0x09, 0x03, 0x01, 0x60, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
            [0x0f, 0x03, 0x0a, 0x02, 0x09, 0x03, 0x02, 0x02, 0x03, 0x0f],
            [0x0f, 0x03, 0x0a, 0x03, 0x02, 0x02, 0x10, 0x10, 0x00, 0x00, 0xf4, 0x01, 0x00, 0x00, 0xe8, 0x03, 0x00, 0x00, 0xff],
            [0x0f, 0x03, 0x0a, 0x02, 0x09, 0x03, 0x02, 0x02, 0x01, 0x01],
            [0x0f, 0x03, 0x0a, 0x03, 0x01],
            [0x0f, 0x03, 0x0a, 0x02, 0x09, 0x03, 0x03, 0x02, 0x02, 0x01],
            [0x0f, 0x03, 0x0a, 0x03, 0x01],
            [0x0f, 0x04]
        ]
        self.assertEqual(self.command_history, expected)
