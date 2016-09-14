from ctypes import *
from common import TestMetaWearBase

class TestSwitch(TestMetaWearBase):
    def setUp(self):
        super().setUp()

        self.switch_data_signal= self.libmetawear.mbl_mw_switch_get_state_data_signal(self.board)

    def test_mbl_mw_switch_subscribe(self):
        expected= [0x01, 0x01, 0x01]

        self.libmetawear.mbl_mw_datasignal_subscribe(self.switch_data_signal, self.sensor_data_handler)
        self.assertEqual(self.command, expected)

    def test_mbl_mw_switch_unsubscribe(self):
        expected= [0x01, 0x01, 0x00]

        self.libmetawear.mbl_mw_datasignal_unsubscribe(self.switch_data_signal)
        self.assertEqual(self.command, expected)

    def test_mbl_mw_switch_get_data_pushed(self):
        response= create_string_buffer(b'\x01\x01\x01', 3)
        expected= 1
        
        self.libmetawear.mbl_mw_datasignal_subscribe(self.switch_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_uint32.value, expected)

    def test_mbl_mw_switch_get_data_released(self):
        response= create_string_buffer(b'\x01\x01\x00', 3)
        expected= 0

        self.libmetawear.mbl_mw_datasignal_subscribe(self.switch_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_connection_notify_char_changed(self.board, response.raw, len(response))
        self.assertEqual(self.data_uint32.value, expected)
