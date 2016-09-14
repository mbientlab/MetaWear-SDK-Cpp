import copy
from common import TestMetaWearBase
from ctypes import byref
#from datetime import datetime
from logdata import Bmi160Accelerometer
from mbientlab.metawear.core import *
from mbientlab.metawear.sensor import AccelerometerBosch
#from time import mktime
import threading

class TestLoggingModule(TestMetaWearBase):
    def test_start_overwrite(self):
        expected_cmds= [
            [0x0b, 0x0b, 0x01],
            [0x0b, 0x01, 0x01]
        ]

        self.libmetawear.mbl_mw_logging_start(self.board, 1)
        self.assertEqual(self.command_history, expected_cmds)

    def test_start_no_overwrite(self):
        expected_cmds= [
            [0x0b, 0x0b, 0x00],
            [0x0b, 0x01, 0x01]
        ]

        self.libmetawear.mbl_mw_logging_start(self.board, 0)
        self.assertEqual(self.command_history, expected_cmds)
        
    def test_stop(self):
        expected= [0x0b, 0x01, 0x00]

        self.libmetawear.mbl_mw_logging_stop(self.board)
        self.assertEqual(self.command, expected)

    def test_clear_entries(self):
        expected= [0x0b, 0x09, 0xff, 0xff, 0xff, 0xff]

        self.libmetawear.mbl_mw_logging_clear_entries(self.board)
        self.assertEqual(self.command, expected)

class TestLogDownload(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

        self.progress_update= Fn_Uint_Uint(self.progress_update_handler)
        self.unknown_entry= Fn_Ubyte_LongLong_ByteArray(self.unknown_entry_handler)
        self.download_handler= LogDownloadHandler(received_progress_update = self.progress_update, received_unknown_entry = self.unknown_entry)
        self.updates= []

    def progress_update_handler(self, entries_left, total_entries):
        self.updates.append(entries_left)
        if (entries_left == 0):
            self.assertEqual(self.updates, self.expected_updates)

    def unknown_entry_handler(self, id, epoch, data, length):
#        now= datetime.datetime.now()
#        now_ms= (mktime(now.timetuple()) + now.microsecond/1000000.0) * 1000
        data_ptr= cast(data, POINTER(c_uint))
        self.assertTrue(id == self.expected_entry[0] and data_ptr.contents.value == self.expected_entry[1])

    def test_readout_page_confirm(self):
        expected= [0x0b, 0x0e]
        page_completed= create_string_buffer(b'\x0b\x0d', 2)

        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, page_completed.raw, len(page_completed.raw))
        self.assertEqual(self.command, expected)

    def test_readout_progress(self):
        self.expected_updates= [
            0x0271, 0x0251, 0x0231, 0x0211, 0x01f1, 
            0x01d1, 0x01b1, 0x0191, 0x0171, 0x0151, 
            0x0131, 0x0111, 0x00f1, 0x00d1, 0x00b1, 
            0x0091, 0x0071, 0x0051, 0x0031, 0x0011, 0x0000
        ]

        progress_responses= [
            create_string_buffer(b'\x0b\x08\x71\x02\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x51\x02\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x31\x02\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x11\x02\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\xf1\x01\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\xd1\x01\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\xb1\x01\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x91\x01\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x71\x01\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x51\x01\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x31\x01\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x11\x01\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\xf1\x00\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\xd1\x00\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\xb1\x00\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x91\x00\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x71\x00\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x51\x00\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x31\x00\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x11\x00\x00\x00', 6),
            create_string_buffer(b'\x0b\x08\x00\x00\x00\x00', 6)
        ]

        self.libmetawear.mbl_mw_logging_download(self.board, 20, byref(self.download_handler))
        for buffer in progress_responses:
            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, buffer.raw, len(buffer.raw))

    def test_download(self):
        expected_cmds= [
            [0x0b, 0x0d, 0x01],
            [0x0b, 0x07, 0x01],
            [0x0b, 0x08, 0x01],
            [0x0b, 0x85],
            [0x0b, 0x06, 0x9e, 0x01, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00]
        ]

        self.libmetawear.mbl_mw_logging_download(self.board, 20, self.download_handler)
        self.assertEqual(self.command_history, expected_cmds)

    def test_unknown_entry(self):
        self.expected_entry= [0x1, 0x016c]
        log_response= create_string_buffer(b'\x0b\x07\xa1\xcc\x4d\x00\x00\x6c\x01\x00\x00', 11)

        self.libmetawear.mbl_mw_logging_download(self.board, 20, byref(self.download_handler))
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, log_response.raw, len(log_response.raw))

class TestAccelerometerLoggingBase(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.logged_data= []
        self.data_time_offsets= []
        self.prev_time= -1

    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def acc_data_progress_update_handler(self, entries_left, total_entries):
        if (entries_left == 0):
            self.assertEqual(self.logged_data, Bmi160Accelerometer.expected_values)

    def data_epoch_progress_update_handler(self, entries_left, total_entries):
        if (entries_left == 0):
            self.assertEqual(self.data_time_offsets, Bmi160Accelerometer.expected_offsets)

    def cartesian_float_data_handler(self, data):
        if (self.prev_time == -1):
            self.prev_time= data.contents.epoch
        else:
            self.data_time_offsets.append(data.contents.epoch - self.prev_time)
            self.prev_time= data.contents.epoch

        contents= copy.deepcopy(cast(data.contents.value, POINTER(CartesianFloat)).contents)
        self.logged_data.append(contents)

    def logger_ready_handler(self, logger):
        cartesian_float_data= Fn_DataPtr(self.cartesian_float_data_handler)
        self.libmetawear.mbl_mw_logger_subscribe(logger, cartesian_float_data)
        self.libmetawear.mbl_mw_logging_download(self.board, 20, byref(self.download_handler))
        for buffer in Bmi160Accelerometer.log_responses:
            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, buffer.raw, len(buffer.raw))

class TestAccelerometerLogging(TestAccelerometerLoggingBase):
    def test_acc_data(self):
        progress_update= Fn_Uint_Uint(self.acc_data_progress_update_handler)
        self.download_handler= LogDownloadHandler(received_progress_update = progress_update, \
                received_unknown_entry = cast(None, Fn_Ubyte_LongLong_ByteArray), received_unhandled_entry = cast(None, Fn_DataPtr))

        logger_ready= Fn_VoidPtr(self.logger_ready_handler)

        acc_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccelerometerBosch.FSR_8G)
        self.libmetawear.mbl_mw_datasignal_log(acc_signal, logger_ready)

    def test_epoch_calc(self):
        progress_update= Fn_Uint_Uint(self.data_epoch_progress_update_handler)
        self.download_handler= LogDownloadHandler(received_progress_update = progress_update, \
                received_unknown_entry = cast(None, Fn_Ubyte_LongLong_ByteArray), received_unhandled_entry = cast(None, Fn_DataPtr))

        logger_ready= Fn_VoidPtr(self.logger_ready_handler)

        acc_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccelerometerBosch.FSR_8G)
        self.libmetawear.mbl_mw_datasignal_log(acc_signal, logger_ready)

class TestLoggerSetup(TestMetaWearBase):
    def logger_ready_handler(self, logger):
        self.libmetawear.mbl_mw_logger_remove(logger)
        self.assertEqual(self.command_history, self.expected_cmds)

    def test_accelerometer(self):
        self.expected_cmds= [
            [0x0b, 0x02, 0x03, 0x04, 0xff, 0x60],
            [0x0b, 0x02, 0x03, 0x04, 0xff, 0x24],
            [0x0b, 0x03, 0x00],
            [0x0b, 0x03, 0x01]
        ]

        logger_ready= Fn_VoidPtr(self.logger_ready_handler)
        self.test_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_log(self.test_signal, logger_ready)

class TestLoggerTimeout(TestMetaWearBase):
    def logger_ready(self, logger):
        self.created_logger= logger

    def logger_ready_snd(self, logger):
        self.e.set()

    def commandLogger(self, board, characteristic, command, length):
        if (command[0] == 0xb and command[1] == 0x2):
            response= create_string_buffer(b'\x0b\x00', 2)
            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))
        else:
            super().commandLogger(board, characteristic, command, length)

    def test_timeout(self):
        self.e= threading.Event()

        logger_ready_fn= Fn_VoidPtr(self.logger_ready)
        logger_ready_snd_fn= Fn_VoidPtr(self.logger_ready_snd)
        self.test_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_log(self.test_signal, logger_ready_fn)
        self.libmetawear.mbl_mw_datasignal_log(self.test_signal, logger_ready_fn)
        self.libmetawear.mbl_mw_datasignal_log(self.test_signal, logger_ready_snd_fn)

        self.e.wait()
        self.assertIsNone(self.created_logger)
