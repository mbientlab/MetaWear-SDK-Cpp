from mbientlab.metawear.core import * 
from ctypes import *
import copy
import os
import unittest

class TestMetaWearBase(unittest.TestCase):
    METAWEAR_R_BOARD= 0
    METAWEAR_RG_BOARD= 1
    METAWEAR_RPRO_BOARD= 2

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.libmetawear= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])
        self.initialized_fn= FnVoid(self.initialized)
        self.sensor_data_handler= FnDataPtr(self.sensorDataHandler)
        self.timer_signal_ready= FnVoidPtr(self.timerSignalReady)
        self.commands_recorded_fn= FnVoid(self.commandsRecorded)

        self.send_command_fn= FnGattCharPtrByteArray(self.commandLogger)
        self.read_gatt_char_fn= FnGattCharPtr(self.read_gatt_char)
        self.btle_connection= BtleConnection(write_gatt_char = self.send_command_fn, read_gatt_char = self.read_gatt_char_fn)

        self.metawear_rg_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x01\x00', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x00', 4),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1c', 5),
            0x0a: create_string_buffer(b'\x0a\x80\x00\x00\x1c', 5),
            0x0b: create_string_buffer(b'\x0b\x80\x00\x00\x08\x80\x2f', 7),
            0x0c: create_string_buffer(b'\x0c\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0d\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0f\x80\x00\x00', 4),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x00', 4),
            0x12: create_string_buffer(b'\x12\x80', 2),
            0x13: create_string_buffer(b'\x13\x80\x00\x00', 4),
            0x14: create_string_buffer(b'\x14\x80', 2),
            0xfe: create_string_buffer(b'\xfe\x80\x00\x00', 4)
        }
        self.metawear_rg_model= create_string_buffer(b'1', 1)

        self.metawear_r_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x00\x00', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x01', 6),
            0x05: create_string_buffer(b'\x05\x80\x00\x00', 4),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1C', 5),
            0x0a: create_string_buffer(b'\x0A\x80\x00\x00\x1C', 5),
            0x0b: create_string_buffer(b'\x0B\x80\x00\x00\x08\x80\x33', 7),
            0x0c: create_string_buffer(b'\x0C\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0D\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0F\x80\x00\x00', 4),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x00', 4),
            0x12: create_string_buffer(b'\x12\x80', 2),
            0x13: create_string_buffer(b'\x13\x80', 2),
            0x14: create_string_buffer(b'\x14\x80', 2),
            0xfe: create_string_buffer(b'\xFE\x80\x00\x00', 4)
        }
        self.metawear_r_model= create_string_buffer(b'0', 1)

        self.metawear_rpro_services= {
            0x01: create_string_buffer(b'\x01\x80\x00\x00', 4),
            0x02: create_string_buffer(b'\x02\x80\x00\x00', 4),
            0x03: create_string_buffer(b'\x03\x80\x01\x00', 4),
            0x04: create_string_buffer(b'\x04\x80\x01\x00\x00\x03\x01\x02', 8),
            0x05: create_string_buffer(b'\x05\x80\x00\x00', 4),
            0x06: create_string_buffer(b'\x06\x80\x00\x00', 4),
            0x07: create_string_buffer(b'\x07\x80\x00\x00', 4),
            0x08: create_string_buffer(b'\x08\x80\x00\x00', 4),
            0x09: create_string_buffer(b'\x09\x80\x00\x00\x1C', 5),
            0x0a: create_string_buffer(b'\x0A\x80\x00\x00\x1C', 5),
            0x0b: create_string_buffer(b'\x0B\x80\x00\x00\x08\x80\x2F', 7),
            0x0c: create_string_buffer(b'\x0C\x80\x00\x00\x08', 5),
            0x0d: create_string_buffer(b'\x0D\x80\x00\x00', 4),
            0x0f: create_string_buffer(b'\x0F\x80\x00\x00', 4),
            0x10: create_string_buffer(b'\x10\x80', 2),
            0x11: create_string_buffer(b'\x11\x80\x00\x00', 4),
            0x12: create_string_buffer(b'\x12\x80\x00\x00', 4),
            0x13: create_string_buffer(b'\x13\x80\x00\x00', 4),
            0x14: create_string_buffer(b'\x14\x80\x00\x00', 4),
            0xfe: create_string_buffer(b'\xFE\x80\x00\x00', 4)
        }
        self.metawear_rpro_model= create_string_buffer(b'1', 1)

        self.firmware_revision= create_string_buffer(b'1.1.0', 5)

        self.command_history= []
        self.full_history= []

        self.eventId= 0
        self.timerId= 0
        self.dataprocId= 0
        self.timerSignals= []
        self.boardType= TestMetaWearBase.METAWEAR_R_BOARD
        self.initialized= False
        
    def setUp(self):
        self.board= self.libmetawear.mbl_mw_metawearboard_create(byref(self.btle_connection))
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)

    def tearDown(self):
        self.libmetawear.mbl_mw_metawearboard_free(self.board)

    def commandsRecorded(self):
        self.recorded= True

    def timerSignalReady(self, timer_signal):
        self.timerSignals.append(timer_signal)

    def initialized(self):
        self.initialized= True

    def read_gatt_char(self, characteristic):
        if (characteristic.contents.uuid_high == 0x00002a2400001000 and characteristic.contents.uuid_low == 0x800000805f9b34fb):
            if (self.boardType == TestMetaWearBase.METAWEAR_RG_BOARD):
                model_number= self.metawear_rg_model
            elif (self.boardType == TestMetaWearBase.METAWEAR_R_BOARD):
                model_number= self.metawear_r_model
            elif (self.boardType == TestMetaWearBase.METAWEAR_RPRO_BOARD):
                model_number= self.metawear_rpro_model

            self.libmetawear.mbl_mw_connection_char_read(self.board, characteristic, model_number.raw, len(model_number.raw))
        elif (characteristic.contents.uuid_high == 0x00002a2600001000 and characteristic.contents.uuid_low == 0x800000805f9b34fb):
            self.libmetawear.mbl_mw_connection_char_read(self.board, characteristic, self.firmware_revision.raw, len(self.firmware_revision.raw))

    def commandLogger(self, characteristic, command, length):
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

            self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, service_response.raw, len(service_response))
        else:
            # ignore module discovey commands
            self.command_history.append(self.command)
            if (command[0] == 0xc and command[1] == 0x2):
                response= create_string_buffer(b'\x0c\x02', 3)
                response[2]= self.timerId
                self.timerId+= 1
                self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))
            elif (command[0] == 0xa and command[1] == 0x3):
                response= create_string_buffer(b'\x0a\x02', 3)
                response[2]= self.eventId
                self.eventId+= 1
                self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))
            elif (command[0] == 0x9 and command[1] == 0x2):
                response= create_string_buffer(b'\x09\x02', 3)
                response[2]= self.dataprocId
                self.dataprocId+= 1
                self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response.raw))

    def sensorDataHandler(self, data):
        if (data.contents.type_id == DataTypeId.UINT32):
            data_ptr= cast(data.contents.value, POINTER(c_uint))
            self.data_uint32= c_uint()
            self.data_uint32.value= data_ptr.contents.value
        elif (data.contents.type_id == DataTypeId.FLOAT):
            data_ptr= cast(data.contents.value, POINTER(c_float));
            self.data_float= c_float()
            self.data_float.value= data_ptr.contents.value
        elif (data.contents.type_id == DataTypeId.CARTESIAN_FLOAT):
            data_ptr= cast(data.contents.value, POINTER(CartesianFloat));
            self.data_cartesian_float= copy.deepcopy(data_ptr.contents)
        else:
            raise RuntimeException('Unrecognized data type id: ' + data.contents.type_id)
