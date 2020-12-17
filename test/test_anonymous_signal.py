from common import TestMetaWearBase, to_string_buffer
from mbientlab.metawear.cbindings import *
from threading import Event

class AnonymousSignalBase(TestMetaWearBase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.acc_range = 0x08
        self.gyr_range = 0x03

    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

        self.result = self.sync_loggers()
        self.libmetawear.mbl_mw_logging_download(self.board, 0, cast(None, POINTER(LogDownloadHandler)))

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            if (command[0] == 0x03 and command[1] == 0x83):
                response = to_string_buffer([0x03, 0x83, 40, self.acc_range])
            elif (command[0] == 0x13 and command[1] == 0x83):
                response = to_string_buffer([0x13, 0x83, 40, self.gyr_range])
            elif(command[0] == 0x19 and command[1] == 0x82):
                response = to_string_buffer([0x19, 0x82, 0x1, 0xf])
            else:
                response = None

            if (response != None):
                self.schedule_response(response)

    def sync_loggers(self):
        event = Event()
        result = {}
        def created(context, board, signals, len):
            result['length'] = len
            result['signals'] = cast(signals, POINTER(c_void_p * len)) if signals is not None else None
            event.set()

        self.created_fn = FnVoid_VoidP_VoidP_VoidP_UInt(created)
        self.libmetawear.mbl_mw_metawearboard_create_anonymous_datasignals(self.board, None, self.created_fn)
        event.wait()

        return result

class TestAcceleration(AnonymousSignalBase):
    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
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

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([11,7,-96,-26,66,0,0,-11,0,61,1,-95,-26,66,0,0,-35,15,0,0]))
        self.assertEqual(self.data_cartesian_float, expected)

class TestGyroY(AnonymousSignalBase):
    def sensorDataHandler(self, context, data):
        self.actual.append(cast(data.contents.value, POINTER(c_float)).contents.value)

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
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

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([11, 7, 64, 34, 223, 4, 0, 249, 255, 0, 0, 64, 61, 223, 4, 0, 254, 255, 0, 0]))

        for a, b in zip(self.actual, expected):
            self.assertAlmostEqual(a, b, delta = 0.001)

    def test_identifier(self):
        actual = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[0])
        self.assertEqual(actual.decode('ascii'), "angular-velocity[1]")

class TestSplitImu(AnonymousSignalBase):
    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
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
        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], None, self.sensor_data_handler)
        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[1], None, self.sensor_data_handler)

        expected = CartesianFloat(x = 0.060, y = 0.077, z = 0.991)
        self.notify_mw_char(to_string_buffer([11,7,0x60,-26,66,0,0,-11,0,61,1,0x62,-26,66,0,0,-35,15,0,0]))
        self.assertEqual(self.data_cartesian_float, expected)

        
        expected= CartesianFloat(x = 224.192, y = 170.823, z = 147.012)
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0x61, 0x38, 0xc2, 0x01, 0x00, 0xe6, 0x72, 0x8c, 0x57, 0x63, 0x38, 0xc2, 0x01, 0x00, 0x58, 0x4b, 0x00, 0x00]))
        self.assertEqual(self.data_cartesian_float, expected)

class TestActivity(AnonymousSignalBase):
    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
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
        actual = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[0])
        self.assertEqual("acceleration:rms?id=0:accumulate?id=1:time?id=2", actual.decode('ascii'))

        actual = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[1])
        self.assertEqual("acceleration:rms?id=0:accumulate?id=1:buffer-state?id=3", actual.decode('ascii'))

    def test_handle_download(self):
        expected= [1.16088868, 1793.6878, 3545.5054]
        self.actual = []

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0x00, 0x3c, 0xe2, 0x01, 0x00, 0x93, 0x12, 0x00, 0x00, 0x00, 0x48, 0x32, 0x02, 0x00, 0x01, 0x1b, 0x70, 0x00]))
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0x00, 0x53, 0x82, 0x02, 0x00, 0x16, 0x98, 0xdd, 0x00]))

        for a, b in zip(self.actual, expected):
            self.assertAlmostEqual(a, b, delta = 0.001)

    def test_handle_state_download(self):
        expected = 3521.868

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[1], None, self.sensor_data_handler)
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xc1, 0xe9, 0x06, 0x02, 0x00, 0xe3, 0x1d, 0xdc, 0x00]))

        self.assertAlmostEqual(expected, self.data_float.value, delta = 0.001)

class TestQuaternionLimiter(AnonymousSignalBase):
    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
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
        actual = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[0])
        self.assertEqual("quaternion:time?id=0", actual.decode('ascii'))

class TestMultipleLoggers(AnonymousSignalBase):
    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
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
    def sensorDataHandler(self, context, data):
        self.actual.append(cast(data.contents.value, POINTER(c_float)).contents.value)

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
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
            self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[x], None, self.sensor_data_handler)

        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xa0, 0xbd, 0x25, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0xa3, 0xbd, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]))
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xa1, 0xbd, 0x25, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0xa2, 0xbd, 0x25, 0x00, 0x00, 0x38, 0xff, 0x00, 0x00]))

        for a, b in zip(self.actual, expected):
            self.assertAlmostEqual(a, b, delta = 0.001)

    def test_identifier(self):
        for x in range(0, 4):
            with self.subTest(source=str(x)):
                actual = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[x])
                self.assertEqual("temperature[%d]" % (x), actual.decode('ascii'))

class TestTimeout(AnonymousSignalBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        self.sync_logger = True
        self.sync_dataproc = True
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        TestMetaWearBase.commandLogger(self, context, board, writeType, characteristic, command, length)
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

        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        result = self.sync_loggers()
        self.assertEqual(result['length'], Const.STATUS_ERROR_TIMEOUT)
        self.assertIsNone(result['signals'])

    def test_dataprocessor_sync_timeout(self):
        self.sync_dataproc = False

        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)
        result = self.sync_loggers()
        self.assertEqual(result['length'], Const.STATUS_ERROR_TIMEOUT)
        self.assertIsNone(result['signals'])

class TestFuser(AnonymousSignalBase):
    def setUp(self):
        self.acc_range = 0x03
        self.gyr_range = 0x04
        
        super().setUp()

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        prev = len(self.full_history)
        super().commandLogger(context, board, writeType, characteristic, command, length)
        curr = len(self.full_history)

        if (prev != curr):
            response = None
            if (command[0] == 0xb and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x01, 0x60])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x01, 0x64])
                elif (command[2] == 0x02):
                    response = to_string_buffer([0x0b, 0x82, 0x09, 0x03, 0x01, 0x68])
                else:
                    response = to_string_buffer([0x0b, 0x82])
            elif (command[0] == 0x9 and command[1] == 0x82):
                if (command[2] == 0x00):
                    response = to_string_buffer([0x09, 0x82, 0x13, 0x05, 0xff, 0xa0, 0x0f, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe9, 0xff])
                elif (command[2] == 0x01):
                    response = to_string_buffer([0x09, 0x82, 0x03, 0x04, 0xff, 0xa0, 0x1b, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe9, 0xff])
                else:
                    response = to_string_buffer([0x0b, 0x82])

            if (response != None):
                self.schedule_response(response)

    def test_sync_loggers(self):
        self.assertEqual(self.result['length'], 1)

    def test_check_scheme(self):        
        actual = self.libmetawear.mbl_mw_anonymous_datasignal_get_identifier(self.result['signals'].contents[0])
        self.assertEqual("acceleration:fuser?id=1", actual.decode('ascii'))

    def test_handle_download(self):
        results = []
        def fused_data_handler(context, data):
            values = cast(data.contents.value, POINTER(POINTER(Data) * 2))

            self.sensorDataHandler(context, values.contents[0])
            results.append(self.data)

            self.sensorDataHandler(context, values.contents[1])
            results.append(self.data)

        fn_wrapper = FnVoid_VoidP_DataP(fused_data_handler)

        expected= [
            CartesianFloat(x=-0.02307129, y= 0.02008057, z= 1.022278),
            CartesianFloat(x=0.2591463, y= 0.4611281, z= 0.04573171),
            CartesianFloat(x=-0.0123291, y= 0.01885986, z= 1.031372),
            CartesianFloat(x=0.3048781, y= 0.4954268, z= 0.1105183)
        ]

        self.libmetawear.mbl_mw_anonymous_datasignal_subscribe(self.result['signals'].contents[0], None, fn_wrapper)
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xc0, 0xac, 0x1b, 0x00, 0x00, 0x86, 0xfe, 0x49, 0x01, 0xc1, 0xac, 0x1b, 0x00, 0x00, 0x6d, 0x41, 0x44, 0x00]))
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xc2, 0xac, 0x1b, 0x00, 0x00, 0x79, 0x00, 0x0c, 0x00, 0xc0, 0xc8, 0x1b, 0x00, 0x00, 0x36, 0xff, 0x35, 0x01]))
        self.notify_mw_char(to_string_buffer([0x0b, 0x07, 0xc1, 0xc8, 0x1b, 0x00, 0x00, 0x02, 0x42, 0x50, 0x00, 0xc2, 0xc8, 0x1b, 0x00, 0x00, 0x82, 0x00, 0x1d, 0x00]))

        self.assertEqual(results, expected)