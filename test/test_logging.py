import copy
from common import TestMetaWearBase, to_string_buffer
from ctypes import byref
#from datetime import datetime
from logdata import *
from cbindings import *
#from mbientlab.metawear.cbindings import *
import time
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

        self.progress_update= FnVoid_VoidP_UInt_UInt(self.progress_update_handler)
        self.unknown_entry= FnVoid_VoidP_UByte_Long_UByteP_UByte(self.unknown_entry_handler)
        self.download_handler= LogDownloadHandler(received_progress_update = self.progress_update, received_unknown_entry = self.unknown_entry)
        self.updates= []

    def progress_update_handler(self, context, entries_left, total_entries):
        self.updates.append(entries_left)
        if (entries_left == 0):
            self.assertEqual(self.updates, self.expected_updates)

    def unknown_entry_handler(self, context, id, epoch, data, length):
#        now= datetime.datetime.now()
#        now_ms= (mktime(now.timetuple()) + now.microsecond/1000000.0) * 1000
        data_ptr= cast(data, POINTER(c_uint))
        self.assertTrue(id == self.expected_entry[0] and data_ptr.contents.value == self.expected_entry[1])

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
            self.notify_mw_char(buffer)

    def test_download(self):
        expected_cmds= [
            [0x0b, 0x08, 0x01],
            [0x0b, 0x0d, 0x01],
            [0x0b, 0x07, 0x01],
            [0x0b, 0x85],
            [0x0b, 0x06, 0x9e, 0x01, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00],
            [0x0b, 0x0e]
        ]

        self.libmetawear.mbl_mw_logging_download(self.board, 20, self.download_handler)
        self.notify_mw_char(create_string_buffer(b'\x0b\x0d', 2))

        self.assertEqual(self.command_history, expected_cmds)

    def test_unknown_entry(self):
        self.expected_entry= [0x1, 0x016c]

        self.libmetawear.mbl_mw_logging_download(self.board, 20, byref(self.download_handler))
        self.notify_mw_char(create_string_buffer(b'\x0b\x07\xa1\xcc\x4d\x00\x00\x6c\x01\x00\x00', 11))

class TestAccelerometerLoggingBase(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.logged_data= []
        self.data_time_offsets= []
        self.prev_time= -1

        self.download_handler= LogDownloadHandler(context = None, received_progress_update = cast(None, FnVoid_VoidP_UInt_UInt), \
                received_unknown_entry = cast(None, FnVoid_VoidP_UByte_Long_UByteP_UByte), received_unhandled_entry = cast(None, FnVoid_VoidP_DataP))
        self.responses = Bmi160Accelerometer.log_responses

    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def cartesian_float_data_handler(self, context, data):
        if (self.prev_time == -1):
            self.prev_time= data.contents.epoch
        else:
            self.data_time_offsets.append(data.contents.epoch - self.prev_time)
            self.prev_time= data.contents.epoch

        contents= copy.deepcopy(cast(data.contents.value, POINTER(CartesianFloat)).contents)
        self.logged_data.append(contents)

    def logger_ready(self, context, logger):
        cartesian_float_data= FnVoid_VoidP_DataP(self.cartesian_float_data_handler)
        self.libmetawear.mbl_mw_logger_subscribe(logger, None, cartesian_float_data)
        self.libmetawear.mbl_mw_logging_download(self.board, 20, byref(self.download_handler))
        for buffer in self.responses:
            self.notify_mw_char(buffer)

        self.events["log"].set()

class TestAccelerometerLogging(TestAccelerometerLoggingBase):
    def test_acc_data(self):
        acc_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccBoschRange._8G)
        self.libmetawear.mbl_mw_datasignal_log(acc_signal, None, self.logger_created)
        self.events["log"].wait()

        self.assertEqual(self.logged_data, Bmi160Accelerometer.expected_values)

    def test_epoch_calc(self):
        acc_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_acc_bosch_set_range(self.board, AccBoschRange._8G)
        self.libmetawear.mbl_mw_datasignal_log(acc_signal, None, self.logger_created)
        self.events["log"].wait()

        self.maxDiff = None
        self.assertEqual(self.data_time_offsets, Bmi160Accelerometer.expected_offsets)

class TestGyroYAxisLoggingBase(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.logged_data= []
        self.download_handler= LogDownloadHandler(context = None, received_progress_update = cast(None, FnVoid_VoidP_UInt_UInt), \
                received_unknown_entry = cast(None, FnVoid_VoidP_UByte_Long_UByteP_UByte), received_unhandled_entry = cast(None, FnVoid_VoidP_DataP))

    def setUp(self):        
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def float_data_handler(self, context, data):
        self.logged_data.append(cast(data.contents.value, POINTER(c_float)).contents.value)

    def logger_ready(self, context, logger):
        cartesian_float_data= FnVoid_VoidP_DataP(self.float_data_handler)
        self.libmetawear.mbl_mw_logger_subscribe(logger, None, cartesian_float_data)
        self.libmetawear.mbl_mw_logging_download(self.board, 20, byref(self.download_handler))

        for line in Bmi160GyroYAxis.log_responses:
            self.notify_mw_char(to_string_buffer(line))

        self.events["log"].set()

class TestGyroYAxisLogging(TestGyroYAxisLoggingBase):
    def test_gyro_data(self):
        rot_signal= self.libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal(self.board)
        roy_y_signal = self.libmetawear.mbl_mw_datasignal_get_component(rot_signal, Const.GYRO_ROTATION_Y_AXIS_INDEX)
        self.libmetawear.mbl_mw_gyro_bmi160_set_range(self.board, GyroBoschRange._250dps)

        self.libmetawear.mbl_mw_datasignal_log(roy_y_signal, None, self.logger_created)
        self.events["log"].wait()

        # why doesn't unittest come with an assertAlmostEqual for list of floats?
        self.assertEqual(len(self.logged_data), len(Bmi160GyroYAxis.expected_values))
        for a, b in zip(self.logged_data, Bmi160GyroYAxis.expected_values):
            self.assertAlmostEqual(a, b, delta = 0.001)

class TestLogIdentifiers(TestMetaWearBase):
    def setUp(self):        
        self.boardType= TestMetaWearBase.METAWEAR_RPRO_BOARD

        super().setUp()

    def test_gyro_z_identifier(self):
        rot_signal= self.libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal(self.board)
        roy_y_signal = self.libmetawear.mbl_mw_datasignal_get_component(rot_signal, Const.GYRO_ROTATION_Z_AXIS_INDEX)

        self.libmetawear.mbl_mw_datasignal_log(roy_y_signal, None, self.logger_created)
        self.events["log"].wait()

        actual = self.libmetawear.mbl_mw_logger_generate_identifier(self.loggers[0])
        self.assertEqual(actual.decode('ascii'), "angular-velocity[2]")

class TestSensorFusionLogging(TestMetaWearBase):
    def setUp(self):
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

    def test_quaternion_setup(self):
        expected_cmds= [
            [0x0b, 0x02, 0x19, 0x07, 0xff, 0x60],
            [0x0b, 0x02, 0x19, 0x07, 0xff, 0x64],
            [0x0b, 0x02, 0x19, 0x07, 0xff, 0x68],
            [0x0b, 0x02, 0x19, 0x07, 0xff, 0x6c]
        ]

        signal = self.libmetawear.mbl_mw_sensor_fusion_get_data_signal(self.board, SensorFusionData.QUATERNION)
        self.libmetawear.mbl_mw_datasignal_log(signal, None, self.logger_created)
        self.events["log"].wait()

        self.assertEqual(self.command_history, expected_cmds)

    def test_corrected_mag_setup(self):
        expected_cmds= [
            [0x0b, 0x02, 0x19, 0x06, 0xff, 0x60],
            [0x0b, 0x02, 0x19, 0x06, 0xff, 0x64],
            [0x0b, 0x02, 0x19, 0x06, 0xff, 0x68],
            [0x0b, 0x02, 0x19, 0x06, 0xff, 0x0c]
        ]

        signal = self.libmetawear.mbl_mw_sensor_fusion_get_data_signal(self.board, SensorFusionData.CORRECTED_MAG)
        self.libmetawear.mbl_mw_datasignal_log(signal, None, self.logger_created)
        self.events["log"].wait()

        self.assertEqual(self.command_history, expected_cmds)

class TestLoggerSetup(TestMetaWearBase):
    def test_accelerometer(self):
        self.expected_cmds= [
            [0x0b, 0x02, 0x03, 0x04, 0xff, 0x60],
            [0x0b, 0x02, 0x03, 0x04, 0xff, 0x24],
            [0x0b, 0x03, 0x00],
            [0x0b, 0x03, 0x01]
        ]

        self.test_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_log(self.test_signal, None, self.logger_created)
        self.events["log"].wait()

        self.libmetawear.mbl_mw_logger_remove(self.loggers[0])

        self.assertEqual(self.command_history, self.expected_cmds)

class TestLoggerTimeout(TestMetaWearBase):
    def commandLogger(self, context, board, writeType, characteristic, command, length):
        if (command[0] == 0xb and command[1] == 0x2):
            self.dc_handler(self.board, 0)
        else:
            super().commandLogger(context, board, writeType, characteristic, command, length)

    def test_timeout(self):
        self.libmetawear.mbl_mw_metawearboard_set_time_for_response(self.board, 2000)

        self.test_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_log(self.test_signal, None, self.logger_created)

        self.events["log"].wait()
        self.assertIsNone(self.loggers[0])

class TestLogTimestamp(TestMetaWearBase):
    def initialized(self, context, board, status):
        super().initialized(context, board, status)
        self.now = int(time.time() * 1000)

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        if (command[0] == 0xb and command[1] == 0x84):
            self.schedule_response(to_string_buffer([0x0b, 0x84, 0xa9, 0x72, 0x04, 0x00, 0x01]))
        else:
            super().commandLogger(context, board, writeType, characteristic, command, length)

    def test_past(self):
        acc_signal= self.libmetawear.mbl_mw_acc_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_log(acc_signal, None, self.logger_created)
        self.events["log"].wait()

        epoch = []
        handler_ptr = FnVoid_VoidP_DataP(lambda ctx, data: epoch.append(data.contents.epoch))
        self.libmetawear.mbl_mw_logger_subscribe(self.loggers[0], None, handler_ptr)

        self.libmetawear.mbl_mw_logging_download(self.board, 0, cast(None, POINTER(LogDownloadHandler)))
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0x20, 0x75, 0x1b, 0x04, 0x00, 0x3e, 0x01, 0xcd, 0x01, 0x21, 0x76, 0x1b, 0x04, 0x00, 0xc0, 0x07, 0x00, 0x00]))

        # epoch should be within 32701ms
        self.assertTrue(abs(epoch[0] - self.now) <= 32701)
