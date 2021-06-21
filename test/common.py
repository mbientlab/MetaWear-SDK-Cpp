from ctypes import *
from cbindings import *
#from mbientlab.metawear.cbindings import *
from threading import Timer, Event
import copy
import os
import queue
import unittest

class TestMetaWearBase(unittest.TestCase):
    CUSTOM_BOARD= -1
    METAWEAR_R_BOARD= 0
    METAWEAR_RG_BOARD= 1
    METAWEAR_RPRO_BOARD= 2
    METAWEAR_CPRO_BOARD= 3
    METAWEAR_ENV_BOARD= 4
    METAWEAR_DETECT_BOARD= 5
    METAWEAR_MOTION_R_BOARD= 6
    METAWEAR_MOTION_RL_BOARD= 7
    METAWEAR_MOTION_S_BOARD= 8

    @classmethod
    def setUpClass(cls):
        cls.libmetawear= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])
        init_libmetawear(cls.libmetawear)

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.events = {"processor" : Event(), "event" : Event(), "log" : Event(), "timer" : Event() }

        self.initialized_fn= FnVoid_VoidP_VoidP_Int(self.initialized)
        self.sensor_data_handler= FnVoid_VoidP_DataP(self.sensorDataHandler)
        self.timer_signal_ready= FnVoid_VoidP_VoidP(self.timer_created)
        self.processor_handler = FnVoid_VoidP_VoidP(self.processor_created)
        self.logger_created = FnVoid_VoidP_VoidP(self.logger_ready)
        self.commands_recorded_fn= FnVoid_VoidP_VoidP_Int(self.commandsRecorded)

        self.send_command_fn= FnVoid_VoidP_VoidP_GattCharWriteType_GattCharP_UByteP_UByte(self.commandLogger)
        self.read_gatt_char_fn= FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray(self.read_gatt_char)
        self.enable_gatt_notify_fn = FnVoid_VoidP_VoidP_GattCharP_FnIntVoidPtrArray_FnVoidVoidPtrInt(self.enable_gatt_notify)
        self.on_disconnect_fn = FnVoid_VoidP_VoidP_FnVoidVoidPtrInt(self.on_disconnect)
        self.btle_connection= BtleConnection(context = None, write_gatt_char = self.send_command_fn, read_gatt_char = self.read_gatt_char_fn, 
                enable_notifications = self.enable_gatt_notify_fn, on_disconnect = self.on_disconnect_fn)

        self.metawear_rg_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x01\x01', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x00', 4),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1c', 5),
            0x0a: create_string_buffer(b'\x0a\x80\x00\x00\x1c', 5),
            0x0b: create_string_buffer(b'\x0b\x80\x00\x02\x08\x80\x2D\x00\x00', 9),
            0x0c: create_string_buffer(b'\x0c\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0d\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0f\x80\x00\x00', 4),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x00', 4),
            0x12: create_string_buffer(b'\x12\x80', 2),
            0x13: create_string_buffer(b'\x13\x80\x00\x01', 4),
            0x14: create_string_buffer(b'\x14\x80', 2),
            0x15: create_string_buffer(b'\x15\x80', 2),
            0x16: create_string_buffer(b'\x16\x80', 2),
            0x17: create_string_buffer(b'\x17\x80', 2),
            0x18: create_string_buffer(b'\x18\x80', 2),
            0x19: create_string_buffer(b'\x19\x80', 2),
            0xfe: create_string_buffer(b'\xfe\x80\x00\x02', 4)
        }

        self.metawear_r_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x00\x01', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x01', 6),
            0x05: create_string_buffer(b'\x05\x80\x00\x00', 4),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1C', 5),
            0x0a: create_string_buffer(b'\x0A\x80\x00\x00\x1C', 5),
            0x0b: create_string_buffer(b'\x0B\x80\x00\x02\x08\x80\x31\x00\x00', 9),
            0x0c: create_string_buffer(b'\x0C\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0D\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0F\x80\x00\x00', 4),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x00', 4),
            0x12: create_string_buffer(b'\x12\x80', 2),
            0x13: create_string_buffer(b'\x13\x80', 2),
            0x14: create_string_buffer(b'\x14\x80', 2),
            0x15: create_string_buffer(b'\x15\x80', 2),
            0x16: create_string_buffer(b'\x16\x80', 2),
            0x17: create_string_buffer(b'\x17\x80', 2),
            0x18: create_string_buffer(b'\x18\x80', 2),
            0x19: create_string_buffer(b'\x19\x80', 2),
            0xfe: create_string_buffer(b'\xFE\x80\x00\x02', 4)
        }

        self.metawear_rpro_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x01\x01', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x00', 4),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1C', 5),
            0x0a: create_string_buffer(b'\x0A\x80\x00\x00\x1C', 5),
            0x0b: create_string_buffer(b'\x0B\x80\x00\x02\x08\x80\x2D\x00\x00', 9),
            0x0c: create_string_buffer(b'\x0C\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0D\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0F\x80\x00\x00', 4),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x00', 4),
            0x12: create_string_buffer(b'\x12\x80\x00\x00', 4),
            0x13: create_string_buffer(b'\x13\x80\x00\x01', 4),
            0x14: create_string_buffer(b'\x14\x80\x00\x00', 4),
            0x15: create_string_buffer(b'\x15\x80', 2),
            0x16: create_string_buffer(b'\x16\x80', 2),
            0x17: create_string_buffer(b'\x17\x80', 2),
            0x18: create_string_buffer(b'\x18\x80', 2),
            0x19: create_string_buffer(b'\x19\x80', 2),
            0xfe: create_string_buffer(b'\xFE\x80\x00\x02', 4)
        }

        self.metawear_cpro_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x01\x01', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x00', 4),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1C', 5),
            0x0a: create_string_buffer(b'\x0A\x80\x00\x00\x1C', 5),
            0x0b: create_string_buffer(b'\x0B\x80\x00\x02\x08\x80\x2B\x00\x00', 9),
            0x0c: create_string_buffer(b'\x0C\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0D\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0F\x80\x00\x00', 4),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x00', 4),
            0x12: create_string_buffer(b'\x12\x80\x00\x00', 4),
            0x13: create_string_buffer(b'\x13\x80\x00\x01', 4),
            0x14: create_string_buffer(b'\x14\x80\x00\x00', 4),
            0x15: create_string_buffer(b'\x15\x80\x00\x00', 4),
            0x16: create_string_buffer(b'\x16\x80', 2),
            0x17: create_string_buffer(b'\x17\x80', 2),
            0x18: create_string_buffer(b'\x18\x80', 2),
            0x19: create_string_buffer(b'\x19\x80', 2),
            0xfe: create_string_buffer(b'\xFE\x80\x00\x02', 4)
        }

        self.metawear_detector_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x03\x01', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x01\x03\x03\x03\x03\x01', 9),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1c', 5),
            0x0a: create_string_buffer(b'\x0a\x80\x00\x00\x1c', 5),
            0x0b: create_string_buffer(b'\x0b\x80\x00\x02\x08\x80\x2b\x00\x00', 9),
            0x0c: create_string_buffer(b'\x0c\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0d\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0f\x80\x00\x01\x08', 5),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x03', 4),
            0x12: create_string_buffer(b'\x12\x80', 2),
            0x13: create_string_buffer(b'\x13\x80', 2),
            0x14: create_string_buffer(b'\x14\x80\x00\x00', 4),
            0x15: create_string_buffer(b'\x15\x80', 2),
            0x16: create_string_buffer(b'\x16\x80', 2),
            0x17: create_string_buffer(b'\x17\x80', 2),
            0x18: create_string_buffer(b'\x18\x80\x00\x00', 4),
            0x19: create_string_buffer(b'\x19\x80', 2),
            0xfe: create_string_buffer(b'\xfe\x80\x00\x02', 4),
        }

        self.metawear_environment_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x03\x01', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x01\x03\x03\x03\x03\x01', 9),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1c', 5),
            0x0a: create_string_buffer(b'\x0a\x80\x00\x00\x1c', 5),
            0x0b: create_string_buffer(b'\x0b\x80\x00\x02\x08\x80\x2b\x00\x00', 9),
            0x0c: create_string_buffer(b'\x0c\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0d\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0f\x80\x00\x01\x08', 5),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x03', 4),
            0x12: create_string_buffer(b'\x12\x80\x01\x00', 4),
            0x13: create_string_buffer(b'\x13\x80', 2),
            0x14: create_string_buffer(b'\x14\x80', 2),
            0x15: create_string_buffer(b'\x15\x80', 2),
            0x16: create_string_buffer(b'\x16\x80\x00\x00', 4),
            0x17: create_string_buffer(b'\x17\x80\x00\x00', 4),
            0x18: create_string_buffer(b'\x18\x80', 2),
            0x19: create_string_buffer(b'\x19\x80', 2),
            0xfe: create_string_buffer(b'\xfe\x80\x00\x02', 4),
        }

        self.metawear_motion_r_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x01\x01', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x01\x03\x03\x03\x03\x01', 9),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1c', 5),
            0x0a: create_string_buffer(b'\x0a\x80\x00\x00\x1c', 5),
            0x0b: create_string_buffer(b'\x0b\x80\x00\x02\x08\x80\x2b\x00\x00', 9),
            0x0c: create_string_buffer(b'\x0c\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0d\x80\x00\x01', 4),
            0x0f: create_string_buffer(b'\x0f\x80\x00\x01\x08', 5),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x03', 4),
            0x12: create_string_buffer(b'\x12\x80\x00\x00', 4),
            0x13: create_string_buffer(b'\x13\x80\x00\x01', 4),
            0x14: create_string_buffer(b'\x14\x80\x00\x00', 4),
            0x15: create_string_buffer(b'\x15\x80\x00\x01', 4),
            0x16: create_string_buffer(b'\x16\x80', 2),
            0x17: create_string_buffer(b'\x17\x80', 2),
            0x18: create_string_buffer(b'\x18\x80', 2),
            0x19: create_string_buffer(b'\x19\x80\x00\x00\x03\x00\x06\x00\x02\x00\x01\x00', 12),
            0xfe: create_string_buffer(b'\xfe\x80\x00\x02', 4),
        }

        self.metawear_motion_rl_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x01\x02', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x01\x03\x03\x03\x03\x01', 9),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1c', 5),
            0x0a: create_string_buffer(b'\x0a\x80\x00\x00\x1c', 5),
            0x0b: create_string_buffer(b'\x0b\x80\x00\x02\x08\x80\x2b\x00\x00', 9),
            0x0c: create_string_buffer(b'\x0c\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0d\x80\x00\x01', 4),
            0x0f: create_string_buffer(b'\x0f\x80\x00\x01\x08', 5),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x03', 4),
            0x12: create_string_buffer(b'\x12\x80', 2),
            0x13: create_string_buffer(b'\x13\x80\x00\x01', 4),
            0x14: create_string_buffer(b'\x14\x80', 2),
            0x15: create_string_buffer(b'\x15\x80\x00\x01', 4),
            0x16: create_string_buffer(b'\x16\x80', 2),
            0x17: create_string_buffer(b'\x17\x80', 2),
            0x18: create_string_buffer(b'\x18\x80', 2),
            0x19: create_string_buffer(b'\x19\x80\x00\x00\x03\x00\x06\x00\x02\x00\x01\x00', 12),
            0xfe: create_string_buffer(b'\xfe\x80\x00\x02', 4),
        }

        self.metawear_motion_s_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x01\x03\x00', 6),
            0x03: create_string_buffer(b'\x03\x80\x04\x00', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x02\x03\x03\x03\x03\x03\x03\x01', 11),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1c', 5),
            0x0a: create_string_buffer(b'\x0a\x80\x00\x00\x1c', 5),
            0x0b: create_string_buffer(b'\x0b\x80\x00\x03\x08\x00\x00\xfb\x03\x00\x02', 11),
            0x0c: create_string_buffer(b'\x0c\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0d\x80\x00\x01', 4),
            0x0e: create_string_buffer(b'\x0e\x80', 2),
            0x0f: create_string_buffer(b'\x0f\x80\x00\x02\x08\x00', 6),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x09\x07\x00', 6),
            0x12: create_string_buffer(b'\x12\x80\x00\x00', 4),
            0x13: create_string_buffer(b'\x13\x80\x01\x00', 4),
            0x14: create_string_buffer(b'\x14\x80\x00\x00', 4),
            0x15: create_string_buffer(b'\x15\x80\x00\x02', 4),
            0x16: create_string_buffer(b'\x16\x80', 2),
            0x17: create_string_buffer(b'\x17\x80', 2),
            0x18: create_string_buffer(b'\x18\x80', 2),
            0x19: create_string_buffer(b'\x19\x80\x00\x03\x03\x00\x06\x00\x02\x00\x01\x00', 12),
            0xfe: create_string_buffer(b'\xfe\x80\x00\x05\x03', 5),
        }

        self.firmware_revision= create_string_buffer(b'1.1.3', 5)

        self.command_history= []
        self.full_history= []
        self.pending_responses = queue.Queue()

        self.eventId= 0
        self.timerId= 0
        self.dataprocId= 0
        self.loggerId= 0
        self.macroId = 0
        self.timerSignals= []
        self.processors= []
        self.loggers= []
        self.event_status = []
        self.boardType= TestMetaWearBase.METAWEAR_R_BOARD
        
    def setUp(self):
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, None, self.initialized_fn)

    def tearDown(self):
        #Need to separate the btle code so we can cleanly free the memory.
        #Will look into it later
        #self.libmetawear.mbl_mw_metawearboard_free(self.board)
        #self.board = None
        pass

    def commandsRecorded(self, context, event, status):
        self.event_status.append(status)
        self.events["event"].set()
        self.recorded= True

    def processor_created(self, context, pointer):
        self.processors.append(pointer)
        self.events["processor"].set()

    def logger_ready(self, context, pointer):
        self.loggers.append(pointer)
        self.events["log"].set()

    def timer_created(self, context, timer_signal):
        self.timerSignals.append(timer_signal)
        self.events["timer"].set()

    def initialized(self, context, board, status):
        self.init_status= status

    def on_disconnect(self, context, board, handler):
        self.dc_handler = handler

    def enable_gatt_notify(self, context, board, characteristic, handler, ready):
        self.notify_handler = handler
        ready(board, 0)

    def read_gatt_char(self, context, board, characteristic, handler):
        if (characteristic.contents.uuid_high == 0x00002a2400001000 and characteristic.contents.uuid_low == 0x800000805f9b34fb):
            if (self.boardType == TestMetaWearBase.METAWEAR_RG_BOARD):
                model_number= create_string_buffer(b'1', 1)
            elif (self.boardType == TestMetaWearBase.METAWEAR_R_BOARD):
                model_number= create_string_buffer(b'0', 1)
            elif (self.boardType == TestMetaWearBase.METAWEAR_RPRO_BOARD):
                model_number= create_string_buffer(b'1', 1)
            elif (self.boardType == TestMetaWearBase.METAWEAR_CPRO_BOARD or self.boardType == TestMetaWearBase.METAWEAR_DETECT_BOARD or self.boardType == TestMetaWearBase.METAWEAR_ENV_BOARD):
                model_number= create_string_buffer(b'2', 1)
            elif (self.boardType == TestMetaWearBase.METAWEAR_MOTION_R_BOARD or self.boardType == TestMetaWearBase.METAWEAR_MOTION_RL_BOARD):
                model_number= create_string_buffer(b'5', 1)
            elif (self.boardType == TestMetaWearBase.METAWEAR_MOTION_S_BOARD):
                model_number= create_string_buffer(b'8', 1)
            elif (self.boardType == TestMetaWearBase.CUSTOM_BOARD):
                model_number= create_string_buffer(self.custom_module_number, 1)

            bytes = cast(model_number.raw, POINTER(c_ubyte))
            handler(board, bytes, len(model_number.raw))
        elif (characteristic.contents.uuid_high == 0x00002a2600001000 and characteristic.contents.uuid_low == 0x800000805f9b34fb):
            bytes = cast(self.firmware_revision.raw, POINTER(c_ubyte))
            handler(board, bytes, len(self.firmware_revision.raw))
        elif (characteristic.contents.uuid_high == 0x00002a2700001000 and characteristic.contents.uuid_low == 0x800000805f9b34fb):
            hw = create_string_buffer(b'0.1', 3)
            bytes = cast(hw.raw, POINTER(c_ubyte))
            handler(board, bytes, len(hw.raw))
        elif (characteristic.contents.uuid_high == 0x00002a2900001000 and characteristic.contents.uuid_low == 0x800000805f9b34fb):
            mft = create_string_buffer(b'deadbeef', 8)
            bytes = cast(mft.raw, POINTER(c_ubyte))
            handler(board, bytes, len(mft.raw))
        elif (characteristic.contents.uuid_high == 0x00002a2500001000 and characteristic.contents.uuid_low == 0x800000805f9b34fb):
            serial = create_string_buffer(b'cafebabe', 8)
            bytes = cast(serial.raw, POINTER(c_ubyte))
            handler(board, bytes, len(serial.raw))

    def commandLogger(self, context, board, writeType, characteristic, command, length):
        self.command= []
        for i in range(0, length):
            self.command.append(command[i])

        self.full_history.append(self.command)
        if (command[1] == 0x80):
            if (self.boardType == TestMetaWearBase.METAWEAR_RG_BOARD and command[0] in self.metawear_rg_services):
                service_response= self.metawear_rg_services[command[0]]
            elif (self.boardType == TestMetaWearBase.METAWEAR_R_BOARD and command[0] in self.metawear_r_services):
                service_response= self.metawear_r_services[command[0]]
            elif (self.boardType == TestMetaWearBase.METAWEAR_RPRO_BOARD and command[0] in self.metawear_rpro_services):
                service_response= self.metawear_rpro_services[command[0]]
            elif (self.boardType == TestMetaWearBase.METAWEAR_CPRO_BOARD and command[0] in self.metawear_cpro_services):
                service_response= self.metawear_cpro_services[command[0]]
            elif (self.boardType == TestMetaWearBase.METAWEAR_DETECT_BOARD and command[0] in self.metawear_detector_services):
                service_response= self.metawear_detector_services[command[0]]
            elif (self.boardType == TestMetaWearBase.METAWEAR_ENV_BOARD and command[0] in self.metawear_environment_services):
                service_response= self.metawear_environment_services[command[0]]
            elif (self.boardType == TestMetaWearBase.METAWEAR_MOTION_R_BOARD and command[0] in self.metawear_motion_r_services):
                service_response= self.metawear_motion_r_services[command[0]]
            elif (self.boardType == TestMetaWearBase.METAWEAR_MOTION_RL_BOARD and command[0] in self.metawear_motion_rl_services):
                service_response= self.metawear_motion_rl_services[command[0]]
            elif (self.boardType == TestMetaWearBase.METAWEAR_MOTION_S_BOARD and command[0] in self.metawear_motion_s_services):
                service_response= self.metawear_motion_s_services[command[0]]
            elif (self.boardType == TestMetaWearBase.CUSTOM_BOARD):
                service_response= self.lookup_module_response(command[0])
            else:
                raise RuntimeError('Unrecognized module or board: ' + command[0])

            self.notify_mw_char(service_response)
        elif (command[0] == 0xb and command[1] == 0x84):
            self.notify_mw_char(create_string_buffer(b'\x0b\x84\x15\x04\x00\x00\x05', 7))
        else:
            # ignore module discovey commands
            self.command_history.append(self.command)
            response = None
            if (command[0] == 0xc and command[1] == 0x2):
                response= create_string_buffer(b'\x0c\x02', 3)
                response[2]= self.timerId
                self.timerId+= 1
            elif (command[0] == 0xa and command[1] == 0x3):
                response= create_string_buffer(b'\x0a\x02', 3)
                response[2]= self.eventId
                self.eventId+= 1
            elif (command[0] == 0x9 and command[1] == 0x2):
                response= create_string_buffer(b'\x09\x02', 3)
                response[2]= self.dataprocId
                self.dataprocId+= 1
            elif (command[0] == 0xf and command[1] == 0x2):
                response= create_string_buffer(b'\x0f\x02', 3)
                response[2]= self.macroId
                self.macroId+= 1
            elif (command[0] == 0xb and command[1] == 0x2):
                response= create_string_buffer(b'\x0b\x02', 3)
                response[2]= self.loggerId
                self.loggerId+= 1
            elif (command[0] == 0xb and command[1] == 0x85):
                self.notify_mw_char(create_string_buffer(b'\x0b\x85\x9e\x01\x00\x00', 6))
                response= None
            elif (command[0] == 0x11 and command[1] == 0x91):
                response = create_string_buffer(b'\x11\x91\x01', 3)
            elif (command[0] == 0x11 and command[1] == 0x92):
                response = create_string_buffer(b'\x11\x92\x00', 3)
            if (response != None):
                self.schedule_response(response)

    def sensorDataHandler(self, context, data):
        if (data.contents.type_id == DataTypeId.UINT32):
            data_ptr= cast(data.contents.value, POINTER(c_uint))
            self.data_uint32= c_uint()
            self.data_uint32.value= data_ptr.contents.value
            self.data = self.data_uint32
        elif (data.contents.type_id == DataTypeId.INT32 or data.contents.type_id == DataTypeId.SENSOR_ORIENTATION):
            data_ptr= cast(data.contents.value, POINTER(c_int))
            self.data_int32= c_int()
            self.data_int32.value= data_ptr.contents.value
            self.data = self.data_int32
        elif (data.contents.type_id == DataTypeId.FLOAT):
            data_ptr= cast(data.contents.value, POINTER(c_float))
            self.data_float= c_float()
            self.data_float.value= data_ptr.contents.value
            self.data = self.data_float
        elif (data.contents.type_id == DataTypeId.CARTESIAN_FLOAT):
            data_ptr= cast(data.contents.value, POINTER(CartesianFloat))
            self.data_cartesian_float= copy.deepcopy(data_ptr.contents)
            self.data = self.data_cartesian_float
        elif (data.contents.type_id == DataTypeId.BATTERY_STATE):
            data_ptr= cast(data.contents.value, POINTER(BatteryState))
            self.data_battery_state= copy.deepcopy(data_ptr.contents)
            self.data = self.data_battery_state
        elif (data.contents.type_id == DataTypeId.BYTE_ARRAY):
            data_ptr= cast(data.contents.value, POINTER(c_ubyte * data.contents.length))

            self.data_byte_array= []
            for i in range(0, data.contents.length):
                self.data_byte_array.append(data_ptr.contents[i])

            self.data = self.data_byte_array
        elif (data.contents.type_id == DataTypeId.TCS34725_ADC):
            data_ptr= cast(data.contents.value, POINTER(Tcs34725ColorAdc))
            self.data_tcs34725_adc= copy.deepcopy(data_ptr.contents)
            self.data = self.data_tcs34725_adc
        elif (data.contents.type_id == DataTypeId.EULER_ANGLE):
            data_ptr= cast(data.contents.value, POINTER(EulerAngles))
            self.data= copy.deepcopy(data_ptr.contents)
        elif (data.contents.type_id == DataTypeId.QUATERNION):
            data_ptr= cast(data.contents.value, POINTER(Quaternion))
            self.data= copy.deepcopy(data_ptr.contents)
        elif (data.contents.type_id == DataTypeId.CORRECTED_CARTESIAN_FLOAT):
            data_ptr= cast(data.contents.value, POINTER(CorrectedCartesianFloat))
            self.data= copy.deepcopy(data_ptr.contents)
        elif (data.contents.type_id == DataTypeId.OVERFLOW_STATE):
            data_ptr= cast(data.contents.value, POINTER(OverflowState))
            self.data= copy.deepcopy(data_ptr.contents)
        elif (data.contents.type_id == DataTypeId.STRING):
            data_ptr= cast(data.contents.value, c_char_p)
            self.data = data_ptr.value.decode("ascii")
        elif (data.contents.type_id == DataTypeId.BOSCH_ANY_MOTION):
            data_ptr = cast(data.contents.value, POINTER(BoschAnyMotion))
            self.data = copy.deepcopy(data_ptr.contents)
        elif (data.contents.type_id == DataTypeId.CALIBRATION_STATE):
            data_ptr = cast(data.contents.value, POINTER(CalibrationState))
            self.data = copy.deepcopy(data_ptr.contents)
        elif(data.contents.type_id == DataTypeId.BOSCH_TAP):
            data_ptr = cast(data.contents.value, POINTER(BoschTap))
            self.data = copy.deepcopy(data_ptr.contents)
        else:
            raise RuntimeError('Unrecognized data type id: ' + str(data.contents.type_id))

    def notify_mw_char(self, buffer):
        bytes = cast(buffer, POINTER(c_ubyte))
        if (self.board != None):
            return self.notify_handler(self.board, bytes, len(buffer.raw))
        else:
            return 0

    def schedule_response(self, response):
        def send_response():
            if (not self.pending_responses.empty()):
                self.notify_mw_char(self.pending_responses.get())

        self.pending_responses.put(response)
        Timer(0.020, send_response).start()

def to_string_buffer(bytes):
    buffer= create_string_buffer(len(bytes))
    i= 0
    for b in bytes:
        buffer[i]= b & 0xff
        i= i + 1

    return buffer

def to_ubyte_pointer(bytes):
    arr = (c_ubyte * len(bytes))()
    i = 0
    for b in bytes:
        arr[i] = b
        i = i + 1

    return arr
