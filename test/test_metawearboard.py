from common import TestMetaWearBase
from ctypes import byref, cast, create_string_buffer, c_ubyte, c_uint, POINTER
from mbientlab.metawear.core import *
from mbientlab.metawear.processor import *
from mbientlab.metawear.sensor import Gpio
from test_dataprocessor import TestGpioFeedbackSetup
from test_logging import TestAccelerometerLoggingBase
import serializedstate
import threading

class TestMetaWearBoard(TestMetaWearBase):
    def test_no_response_handler(self):
        tests= [
            {
                'response': create_string_buffer(b'\x03\x04\x16\xc4\x94\xa2\x2a\xd0', 8),
                'status': Status.WARNING_UNEXPECTED_SENSOR_DATA,
                'name': 'acc'
            },
            {
                'response': create_string_buffer(b'\x14\x03\xed\x92\xb0\x00', 6),
                'status': Status.WARNING_INVALID_RESPONSE,
                'name': 'ambient_light'
            },
            {
                'response': create_string_buffer(b'\x12\x01\xd3\x35\x8b\x01', 6),
                'status': Status.WARNING_INVALID_RESPONSE,
                'name': 'baro_pressure'
            },
            {
                'response': create_string_buffer(b'\x12\x02\x1e\x1f\xfe\xff', 6),
                'status': Status.WARNING_INVALID_RESPONSE,
                'name': 'baro_altitude'
            },
            {
                'response': create_string_buffer(b'\x13\x05\x3e\x43\xff\x7f\x00\x80', 8),
                'status': Status.WARNING_INVALID_RESPONSE,
                'name': 'gyro'
            },
            {
                'response': create_string_buffer(b'\x01\x01\x01', 3),
                'status': Status.WARNING_UNEXPECTED_SENSOR_DATA,
                'name': 'switch'
            }
        ]

        for test in tests:
            with self.subTest(response=test['name']):
                status= self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, test['response'].raw, len(test['response'].raw))
                self.assertEqual(status, test['status'])

    def test_service_discovery(self):
        self.assertEqual(self.init_status, Status.OK)

class TestMetaWearBoardInitialize(TestMetaWearBase):
    def test_reinitialize(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80],
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80],
            [0xfe, 0x80], [0x0b, 0x84]
        ]

        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)
        self.assertEqual(self.full_history, expected_cmds)

    def test_reinitialize_new_firmware(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80], 
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80],
            [0xfe, 0x80], [0x0b, 0x84],
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80], 
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80],
            [0xfe, 0x80], [0x0b, 0x84]
        ]

        self.firmware_revision= create_string_buffer(b'1.1.4', 5)
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)
        self.assertEqual(self.full_history, expected_cmds)

class TestMetaWearBoardInitialize(TestMetaWearBase):
    def setUp(self):
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def initialized(self, board, status):
        super().initialized(board, status)

        self.e.set()

    def commandLogger(self, board, characteristic, command, length):
        if (command[1] == 0x80):
            response= create_string_buffer(b'\x00\x00', 2)
            response[0]= command[0]
            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))
        else:
            super().commandLogger(board, characteristic, command, length)

    def test_timeout(self):
        self.e= threading.Event()
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)
        self.e.wait()

        self.assertEqual(self.init_status, Status.ERROR_TIMEOUT)

class TestTearDown(TestGpioFeedbackSetup):
    def test_dataproc_event(self):
        expected_cmds= [
            [0x09, 0x08],
            [0x0a, 0x05],
            [0x0b, 0x0a]
        ]

        self.libmetawear.mbl_mw_metawearboard_tear_down(self.board)
        tear_down_cmds= self.command_history[22:].copy()

        self.assertEqual(tear_down_cmds, expected_cmds)

class TestTimerTearDown(TestMetaWearBase):
    def test_timer(self):
        expected_cmds= [
            [0x0c, 0x05, 0x00],
            [0x0c, 0x05, 0x01],
            [0x0c, 0x05, 0x02],
            [0x0c, 0x05, 0x03],
            [0x09, 0x08],
            [0x0a, 0x05],
            [0x0b, 0x0a]
        ]

        self.libmetawear.mbl_mw_timer_create(self.board, 667408, -1, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create(self.board, 1000, -1, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create(self.board, 1000, 10, 0, self.timer_signal_ready)
        self.libmetawear.mbl_mw_timer_create_indefinite(self.board, 1000, 0, self.timer_signal_ready)

        self.libmetawear.mbl_mw_metawearboard_tear_down(self.board)
        self.assertEqual(self.command_history[4:], expected_cmds)

class TestMetaWearBoardSerialize(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.expected_state= [
            0x00, 
            0x03, 0x01, 0x01, 
            0x01, 0x31, 
            0x18, 
            0x01, 0xff, 0xff,
            0x02, 0xff, 0xff,
            0x03, 0x01, 0x00, 0x00, 
            0x04, 0xff, 0xff,
            0x05, 0xff, 0xff,
            0x06, 0xff, 0xff,
            0x07, 0xff, 0xff,
            0x08, 0xff, 0xff,
            0x09, 0xff, 0xff,
            0x0a, 0xff, 0xff,
            0x0b, 0xff, 0xff,
            0x0c, 0xff, 0xff,
            0x0d, 0xff, 0xff,
            0x0f, 0xff, 0xff,
            0x10, 0xff, 0xff,
            0x11, 0x00, 0x03, 0x00,
            0x12, 0xff, 0xff,
            0x13, 0xff, 0xff,
            0x14, 0xff, 0xff,
            0x15, 0xff, 0xff,
            0x16, 0xff, 0xff,
            0x17, 0xff, 0xff,
            0x18, 0xff, 0xff,
            0xfe, 0xff, 0xff,
            0x05, 
            0x03, 0x04, 0xff, 0x00, 0x07, 0x02, 0x03, 0x02, 0x01, 0x00, 
            0x03, 0x19, 0xff, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00,
            0x03, 0x9a, 0xff, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x00,
            0x11, 0x0a, 0xff, 0x00, 
            0x11, 0x8c, 0xff, 0x00, 0x0e, 0x00, 0x01, 0x03, 0x00, 0x00,
            0x01,
            0x03, 0x28, 0x03, 0x07, 0x30, 0x81, 0x0b, 0xc0, 0x00, 0x14, 0x14, 0x14, 0x40, 0x0a, 0x18, 0x48, 0x08, 0x11, 0x00, 0x00, 
            0x01, 
            0xf6, 0x0b, 0x99, 0x36, 0x55, 0x01, 0x00, 0x00, 0x15, 0x04, 0x00, 0x00, 0x05, 
            0x00
        ]

    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))
        self.logger_ready= Fn_VoidPtr(self.logger_ready_handler)

    def logger_ready_handler(self, logger):
        self.libmetawear.mbl_mw_metawearboard_tear_down(self.board)

        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        python_array= []
        for i in range(0, state_array_size.value):
            python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.maxDiff= None
        self.assertEqual(python_array[0:148], self.expected_state[0:148])

    def test_teardown_serialize(self):
        state_buffer= self.to_string_buffer(self.expected_state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, state_buffer.raw, len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)

        acc_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_log(acc_signal, self.logger_ready)

class TestMetaWearBoardDeserialize(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.state= self.to_string_buffer(serializedstate.activity_with_buffer)
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def test_deserialize_same_firmware(self):
        # only thing to do is re-read the logging tick
        expected= [0xb, 0x84]

        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, self.state.raw, len(self.state.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)

        self.assertEqual(self.command, expected)

    def test_deserialize_diff_firmware(self):
        expected_cmds= [
            [0x01, 0x80], [0x02, 0x80], [0x03, 0x80], [0x04, 0x80],
            [0x05, 0x80], [0x06, 0x80], [0x07, 0x80], [0x08, 0x80],
            [0x09, 0x80], [0x0a, 0x80], [0x0b, 0x80], [0x0c, 0x80],
            [0x0d, 0x80], [0x0f, 0x80], [0x10, 0x80], [0x11, 0x80],
            [0x12, 0x80], [0x13, 0x80], [0x14, 0x80], [0x15, 0x80],
            [0x16, 0x80], [0x17, 0x80], [0x18, 0x80],
            [0xfe, 0x80], [0x0b, 0x84]
        ]

        self.firmware_revision= create_string_buffer(b'1.0.4', 5)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, self.state.raw, len(self.state.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)

        self.assertEqual(self.full_history, expected_cmds)

class TestDeserializeTimer(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= self.to_string_buffer(serializedstate.timer_two_events)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, state_buffer, len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)

        self.serialized_timer= self.libmetawear.mbl_mw_timer_lookup_id(self.board, 7);

    def test_start(self):
        expected= [0x0c, 0x03, 0x7]

        self.libmetawear.mbl_mw_timer_start(self.serialized_timer)
        self.assertEqual(self.command, expected)

    def test_stop(self):
        expected= [0x0c, 0x04, 0x7]

        self.libmetawear.mbl_mw_timer_stop(self.serialized_timer)
        self.assertEqual(self.command, expected)

    def test_remove(self):
        expected_cmds= [
            [0x0c, 0x05, 0x7],
            [0x0a, 0x04, 0x0],
            [0x0a, 0x04, 0x1]
        ]

        self.libmetawear.mbl_mw_timer_remove(self.serialized_timer)
        self.assertEqual(self.command_history, expected_cmds)

    def test_null_timer(self):
        invalid_ids= [0, 1, 2, 3, 4, 5, 6]

        for id in invalid_ids:
            with self.subTest(timer_id=id):
                self.assertIsNone(self.libmetawear.mbl_mw_timer_lookup_id(self.board, id))

class TestDeserializeAccelerometerLog(TestAccelerometerLoggingBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= self.to_string_buffer(serializedstate.accelerometer_log)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, state_buffer, len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)
        self.acc_logger= self.libmetawear.mbl_mw_logger_lookup_id(self.board, 0)

    def test_acc_data(self):
        progress_update= Fn_Uint_Uint(self.acc_data_progress_update_handler)
        self.download_handler= LogDownloadHandler(received_progress_update = progress_update, received_unknown_entry = cast(None, Fn_Ubyte_LongLong_ByteArray))

        self.logger_ready_handler(self.acc_logger)

    def test_epoch_calc(self):
        progress_update= Fn_Uint_Uint(self.data_epoch_progress_update_handler)
        self.download_handler= LogDownloadHandler(received_progress_update = progress_update, received_unknown_entry = cast(None, Fn_Ubyte_LongLong_ByteArray))

        self.logger_ready_handler(self.acc_logger)

    def test_remove_logger(self):
        expected_cmds= [
            [0x0b, 0x03, 0x00],
            [0x0b, 0x03, 0x01]
        ]

        self.libmetawear.mbl_mw_logger_remove(self.acc_logger)
        self.assertEqual(self.command_history, expected_cmds)

class TestDeserializeDataProcessor(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= self.to_string_buffer(serializedstate.activity_with_buffer)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, state_buffer, len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)

    def test_read_state(self):
        ids= [1, 2]

        for id in ids:
            with self.subTest(processor_id=id):
                processor= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, id)
                state_signal= self.libmetawear.mbl_mw_dataprocessor_get_state_data_signal(processor)
                self.libmetawear.mbl_mw_datasignal_read(state_signal)

                expected= [0x9, 0xc4, id]
                self.assertEqual(self.command, expected)

    def test_remove_processor_chain(self):
        expected_cmds= [
            [0x9, 0x6, 0x2],
            [0x9, 0x6, 0x3],
            [0x9, 0x6, 0x1]
        ]

        processor= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, 1)
        self.libmetawear.mbl_mw_dataprocessor_remove(processor)
        self.assertEqual(self.command_history, expected_cmds)

    def test_buffer_read(self):
        response= create_string_buffer(b'\x09\x84\x02\xcd\x20\x41\x00', 7)
        expected= 260.5125

        processor= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, 2)
        processor_state= self.libmetawear.mbl_mw_dataprocessor_get_state_data_signal(processor)
        self.libmetawear.mbl_mw_datasignal_subscribe(processor_state, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))

        self.assertAlmostEqual(self.data_float.value, expected, places= 4)

    def test_timer_data(self):
        response= create_string_buffer(b'\x09\x03\x03\xfe\x3f\xb7\x01', 7)
        expected= 1756.9999

        processor= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, 3)
        self.libmetawear.mbl_mw_datasignal_subscribe(processor, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))

        self.assertAlmostEqual(self.data_float.value, expected, places= 4)

class TestSerializeMultiComparator(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.firmware_revision= create_string_buffer(b'1.2.3', 5)

        super().setUp()

    def test_initial_state(self):
        references= (c_float * 4)(1024, 512, 256, 128)
        adc_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x15, Gpio.ANALOG_READ_MODE_ADC)

        comp_created_ptr= Fn_VoidPtr(self.comp_created_cmd_check)
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_create(adc_signal, Comparator.OPERATION_GTE, Comparator.MODE_REFERENCE, references, 
                len(references), comp_created_ptr)

    def comp_created_cmd_check(self, comparator):
        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        python_array= []
        for i in range(0, state_array_size.value):
            python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.assertEqual(python_array[211:234], serializedstate.multi_comparator_state[211:234])

class TestDeserializeMultiComparator(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD
        self.firmware_revision= create_string_buffer(b'1.2.3', 5)
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

        state_buffer= self.to_string_buffer(serializedstate.multi_comparator_state)
        self.libmetawear.mbl_mw_metawearboard_deserialize(self.board, state_buffer, len(state_buffer.raw))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)

    def test_modify_config_state(self):
        references= (c_float * 2)(128, 256)

        comparator= self.libmetawear.mbl_mw_dataprocessor_lookup_id(self.board, 0)
        self.libmetawear.mbl_mw_dataprocessor_multi_comparator_modify(comparator, Comparator.OPERATION_LT, references, len(references))

        state_array_size= c_uint(0)
        state_ptr= cast(self.libmetawear.mbl_mw_metawearboard_serialize(self.board, byref(state_array_size)), POINTER(c_ubyte * state_array_size.value))

        python_array= []
        for i in range(0, state_array_size.value):
            python_array.append(state_ptr.contents[i])
        self.libmetawear.mbl_mw_memory_free(state_ptr)

        self.assertEqual(python_array[0:321], serializedstate.multi_comparator_modified_state[0:321])
