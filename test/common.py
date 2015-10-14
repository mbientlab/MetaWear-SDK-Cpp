from mbientlab.metawear import CartesianFloat, DataTypeId, MetaWearConnection, ReceivedSensorData, SendCommand 
from ctypes import byref, cast, c_float, c_ubyte, c_uint, CDLL, POINTER
import copy
import os
import unittest

class TestMetaWearBase(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.libmetawear= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])
        self.conn= MetaWearConnection(send_command= SendCommand(self.commandLogger), 
                received_sensor_data= ReceivedSensorData(self.sensorDataHandler))
        
    def setUp(self):
        self.board= self.libmetawear.mbl_mw_create_metawear_board()
        self.libmetawear.mbl_mw_connection_init(byref(self.conn))

    def tearDown(self):
        self.libmetawear.mbl_mw_free_metawear_board(self.board)

    def commandLogger(self, board, command, length):
        self.command= []
        for i in range(0, length):
            self.command.append(command[i])

    def sensorDataHandler(self, signal, data):
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
