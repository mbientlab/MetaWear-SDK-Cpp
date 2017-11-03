from common import TestMetaWearBase, to_string_buffer
from mbientlab.metawear.cbindings import *
from threading import Event

class AnonymousSignalBase(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

        self.result = self.sync_loggers()

    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            if (command[0] == 0x03 and command[1] == 0x83):
                response = to_string_buffer([0x03, 0x83, 40, 8])
            elif (command[0] == 0x13 and command[1] == 0x83):
                response = to_string_buffer([0x13, 0x83, 40, 3])
            elif(command[0] == 0x19 and command[1] == 0x82):
                response = to_string_buffer([0x19, 0x82, 0x1, 0xf])
            else:
                response = None

            if (response != None):
                self.schedule_response(response)

    def sync_loggers(self):
        event = Event()
        result = {}
        def created(board, signals, len):
            result['length'] = len
            result['signals'] = cast(signals, POINTER(c_void_p * len)) if signals is not None else None
            event.set()

        self.created_fn = FnVoid_VoidP_VoidP_UInt(created)
        self.libmetawear.mbl_mw_metawearboard_create_anonymous_datasignals(self.board, self.created_fn)
        event.wait()

        return result

class TestAcceleration(AnonymousSignalBase):
    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            if (command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x03, 0x04, 0xff, 0x60])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x0b, 0x82, 0x03, 0x04, 0xff, 0x24])
                else:
                    response = to_string_buffer([0x0b, 0x82])

                if (response != None):
                    self.schedule_response(response)

    def test_sync_loggers(self):
        self.assertEqual(self.result['length'], 1)

    def test_handle_download(self):
        expected= CartesianFloat(x= 0.060, y= 0.077, z= 0.991)

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([11,7,-96,-26,66,0,0,-11,0,61,1,-95,-26,66,0,0,-35,15,0,0]))
        self.assertEqual(self.data_cartesian_float, expected)

class TestGyroY(AnonymousSignalBase):
    def sensorDataHandler(self, data):
        self.actual.append(cast(data.contents.value, POINTER(c_float)).contents.value)

    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            if (command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x13, 0x05, 0xff, 0x22])
                else:
                    response = to_string_buffer([0x0b, 0x82])

                if (response != None):
                    self.schedule_response(response)

    def test_sync_loggers(self):
        self.assertEqual(self.result['length'], 1)

    def test_handle_download(self):
        expected= [-0.053, -0.015]
        self.actual = []

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([11, 7, 64, 34, 223, 4, 0, 249, 255, 0, 0, 64, 61, 223, 4, 0, 254, 255, 0, 0]))

        for a, b in zip(self.actual, expected):
            self.assertAlmostEqual(a, b, delta = 0.001)

    def test_identifier(self):
        raw = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[0])
        actual = cast(raw, c_char_p).value.decode("ascii")
        self.libmetawear.mbl_mw_memory_free(raw)

        self.assertEqual(actual, "angular-velocity[1]")

class TestSplitImu(AnonymousSignalBase):
    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            if (command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x03, 0x04, 0xff, 0x60])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x0b, 0x82, 0x13, 0x05, 0xff, 0x60])
                elif (command[2] == 0x02):
                    response = to_string_buffer([0x0b, 0x82, 0x03, 0x04, 0xff, 0x24])
                elif (command[2] == 0x03):
                    response = to_string_buffer([0x0b, 0x82, 0x13, 0x05, 0xff, 0x24])
                else:
                    response = to_string_buffer([0x0b, 0x82])

                if (response != None):
                    self.schedule_response(response)

    def test_sync_loggers(self):
        self.assertEqual(self.result['length'], 2)

    def test_handle_download(self):
        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], self.sensor_data_handler)
        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[1], self.sensor_data_handler)

        expected = CartesianFloat(x = 0.060, y = 0.077, z = 0.991)
        self.notify_mw_char(to_string_buffer([11,7,0x60,-26,66,0,0,-11,0,61,1,0x62,-26,66,0,0,-35,15,0,0]))
        self.assertEqual(self.data_cartesian_float, expected)

        
        expected= CartesianFloat(x = 224.192, y = 170.823, z = 147.012)
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0x61, 0x38, 0xc2, 0x01, 0x00, 0xe6, 0x72, 0x8c, 0x57, 0x63, 0x38, 0xc2, 0x01, 0x00, 0x58, 0x4b, 0x00, 0x00]))
        self.assertEqual(self.data_cartesian_float, expected)

class TestActivity(AnonymousSignalBase):
    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            response = None
            if (command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x02, 0x60])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0xc4, 0x03, 0x60])
                else:
                    response = to_string_buffer([0x0b, 0x82])
            elif (command[0] == 0x9 and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x09, 0x82, 0x03, 0x04, 0xff, 0xa0, 0x07, 0xa5, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x09, 0x82, 0x09, 0x03, 0x00, 0x20, 0x02, 0x07, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
                elif (command[2] == 0x02):
                    response = to_string_buffer([0x09, 0x82, 0x09, 0x03, 0x01, 0x60, 0x08, 0x13, 0x30, 0x75, 0x00, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
                elif (command[2] == 0x03):
                    response = to_string_buffer([0x09, 0x82, 0x09, 0x03, 0x01, 0x60, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
                else:
                    response = to_string_buffer([0x0b, 0x82])

            if (response != None):
                self.schedule_response(response)

    def test_sync_loggers(self):
        self.assertEqual(self.result['length'], 2)

    def test_check_scheme(self):        
        raw = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[0])
        actual = cast(raw, c_char_p).value.decode("ascii")
        self.libmetawear.mbl_mw_memory_free(raw)
        self.assertEqual("acceleration:rms?id=0:accumulate?id=1:time?id=2", actual)

        raw = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[1])
        actual = cast(raw, c_char_p).value.decode("ascii")
        self.libmetawear.mbl_mw_memory_free(raw)
        self.assertEqual("acceleration:rms?id=0:accumulate?id=1:buffer-state?id=3", actual)

    def test_handle_download(self):
        expected= [1.16088868, 1793.6878, 3545.5054]
        self.actual = []

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0x00, 0x3c, 0xe2, 0x01, 0x00, 0x93, 0x12, 0x00, 0x00, 0x00, 0x48, 0x32, 0x02, 0x00, 0x01, 0x1b, 0x70, 0x00]))
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0x00, 0x53, 0x82, 0x02, 0x00, 0x16, 0x98, 0xdd, 0x00]))

        for a, b in zip(self.actual, expected):
            self.assertAlmostEqual(a, b, delta = 0.001)

    def test_handle_state_download(self):
        expected = 3521.868

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[1], self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xc1, 0xe9, 0x06, 0x02, 0x00, 0xe3, 0x1d, 0xdc, 0x00]))

        self.assertAlmostEqual(expected, self.data_float.value, delta = 0.001)

class TestQuaternionLimiter(AnonymousSignalBase):
    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            response = None
            if (command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x00, 0x60])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x00, 0x64])
                elif (command[2] == 0x02):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x00, 0x68])
                elif (command[2] == 0x03):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x00, 0x6c])
                else:
                    response = to_string_buffer([0x0b, 0x82])
            elif (command[0] == 0x9 and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x09, 0x82, 0x19, 0x07, 0xff, 0xe0, 0x08, 0x17, 0x14, 0x00, 0x00, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
                else:
                    response = to_string_buffer([0x0b, 0x82])

            if (response != None):
                self.schedule_response(response)

    def test_sync_loggers(self):
        self.assertEqual(self.result['length'], 1)

    def test_check_scheme(self):
        raw = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[0])
        actual = cast(raw, c_char_p).value.decode("ascii")
        self.libmetawear.mbl_mw_memory_free(raw)
        self.assertEqual("quaternion:time?id=0", actual)

class TestMultipleLoggers(AnonymousSignalBase):
    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            if (command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x13, 0x05, 0xff, 0x60])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x0b, 0x82, 0x13, 0x05, 0xff, 0x24])
                elif (command[2] == 0x02):
                    response = to_string_buffer([0x0b, 0x82, 0x13, 0x05, 0xff, 0x22])
                else:
                    response = to_string_buffer([0x0b, 0x82])

                if (response != None):
                    self.schedule_response(response)

    def test_sync_loggers(self):
        self.assertEqual(self.result['length'], 2)

class TestTemperature(AnonymousSignalBase):
    def sensorDataHandler(self, data):
        self.actual.append(cast(data.contents.value, POINTER(c_float)).contents.value)

    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            if (command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x04, 0xc1, 0x00, 0x20])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x0b, 0x82, 0x04, 0xc1, 0x01, 0x20])
                elif (command[2] == 0x02):
                    response = to_string_buffer([0x0b, 0x82, 0x04, 0xc1, 0x02, 0x20])
                elif (command[2] == 0x03):
                    response = to_string_buffer([0x0b, 0x82, 0x04, 0xc1, 0x03, 0x20])
                else:
                    response = to_string_buffer([0x0b, 0x82])

                if (response != None):
                    self.schedule_response(response)

    def test_sync_loggers(self):
        self.assertEqual(self.result['length'], 4)

    def test_handle_download(self):
        expected= [32.25, 0, 31.625, -25.0]
        self.actual = []

        for x in range(0, 4):
            self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[x], self.sensor_data_handler)

        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xa0, 0xbd, 0x25, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0xa3, 0xbd, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]))
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xa1, 0xbd, 0x25, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0xa2, 0xbd, 0x25, 0x00, 0x00, 0x38, 0xff, 0x00, 0x00]))

        for a, b in zip(self.actual, expected):
            self.assertAlmostEqual(a, b, delta = 0.001)

    def test_identifier(self):
        for x in range(0, 4):
            with self.subTest(source=str(x)):
                raw = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[x])
                actual = cast(raw, c_char_p).value.decode("ascii")
                self.libmetawear.mbl_mw_memory_free(raw)
                self.assertEqual("temperature[" + str(x) + "]", actual)

class TestTimeout(AnonymousSignalBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        self.sync_logger = True
        self.sync_dataproc = True
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def commandLogger(self, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        TestMetaWearBase.commandLogger(self, board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            response = None
            if (command[0] == 0x03 and command[1] == 0x83):
                response = to_string_buffer([0x03, 0x83, 40, 8])
            elif (command[0] == 0x13 and command[1] == 0x83):
                response = to_string_buffer([0x13, 0x83, 40, 3])
            elif(command[0] == 0x19 and command[1] == 0x82):
                response = to_string_buffer([0x19, 0x82, 0x1, 0xf])
            elif (self.sync_logger and command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x00, 0x60])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x00, 0x64])
                elif (command[2] == 0x02):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x00, 0x68])
                elif (command[2] == 0x03):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x00, 0x6c])
                else:
                    response = to_string_buffer([0x0b, 0x82])
            elif (self.sync_dataproc and command[0] == 0x9 and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x09, 0x82, 0x19, 0x07, 0xff, 0xe0, 0x08, 0x17, 0x14, 0x00, 0x00, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
                else:
                    response = to_string_buffer([0x09, 0x82])

            if (response != None):
                self.schedule_response(response)

    def test_log_sync_timeout(self):
        self.sync_logger = False

        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)
        result = self.sync_loggers()
        self.assertEqual(result['length'], Const.STATUS_ERROR_TIMEOUT)
        self.assertIsNone(result['signals'])

    def test_dataprocessor_sync_timeout(self):
        self.sync_dataproc = False

        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)
        result = self.sync_loggers()
        self.assertEqual(result['length'], Const.STATUS_ERROR_TIMEOUT)
        self.assertIsNone(result['signals'])
