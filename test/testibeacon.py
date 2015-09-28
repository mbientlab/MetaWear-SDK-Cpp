import os
import unittest
import uuid
from ctypes import CDLL, create_string_buffer

metawear_lib= CDLL(os.environ["METAWEAR_LIB_SO_NAME"])

class TestIBeacon(unittest.TestCase):
    def test_set_major(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x07\x03\x4e\x00', 4)

        metawear_lib.mbl_mw_ibeacon_set_major(command, 78);
        self.assertEqual(command.raw, expected.raw)

    def test_set_minor(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x07\x04\x1d\x1d', 4)

        metawear_lib.mbl_mw_ibeacon_set_minor(command, 7453);
        self.assertEqual(command.raw, expected.raw)

    def test_set_period(self):
        command= create_string_buffer(4)
        expected= create_string_buffer(b'\x07\x07\xb3\x3a', 4)

        metawear_lib.mbl_mw_ibeacon_set_period(command, 15027);
        self.assertEqual(command.raw, expected.raw)

    def test_set_rx_power(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x07\x05\xc9', 3)

        metawear_lib.mbl_mw_ibeacon_set_rx_power(command, -55);
        self.assertEqual(command.raw, expected.raw)

    def test_set_tx_power(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x07\x06\xf4', 3)

        metawear_lib.mbl_mw_ibeacon_set_tx_power(command, -12);
        self.assertEqual(command.raw, expected.raw)

    def test_set_uuid(self):
        command= create_string_buffer(18)
        expected= create_string_buffer(b'\x07\x02\x5a\xe7\xba\xfb\x4c\x46\xdd\xd9\x95\x91\xcb\x85\x06\x90\x6a\x32', 18)

        ad_uuid= uuid.UUID('{326a9006-85cb-9195-d9dd-464cfbbae75a}')
        metawear_lib.mbl_mw_ibeacon_set_uuid(command, ad_uuid.bytes[::-1]);
        self.assertEqual(command.raw, expected.raw)

    def test_enable(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x07\x01\x01', 3)

        metawear_lib.mbl_mw_ibeacon_enable(command);
        self.assertEqual(command.raw, expected.raw)

    def test_ibeacon_disable(self):
        command= create_string_buffer(3)
        expected= create_string_buffer(b'\x07\x01\x00', 3)

        metawear_lib.mbl_mw_ibeacon_disable(command);
        self.assertEqual(command.raw, expected.raw)
