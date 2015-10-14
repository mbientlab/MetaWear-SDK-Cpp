from common import TestMetaWearBase
import uuid

class TestIBeacon(TestMetaWearBase):
    def test_set_major(self):
        expected= [0x07, 0x03, 0x4e, 0x00]

        self.libmetawear.mbl_mw_ibeacon_set_major(self.board, 78);
        self.assertEqual(self.command, expected)

    def test_set_minor(self):
        expected= [0x07, 0x04, 0x1d, 0x1d]

        self.libmetawear.mbl_mw_ibeacon_set_minor(self.board, 7453);
        self.assertEqual(self.command, expected)

    def test_set_period(self):
        expected= [0x07, 0x07, 0xb3, 0x3a]

        self.libmetawear.mbl_mw_ibeacon_set_period(self.board, 15027);
        self.assertEqual(self.command, expected)

    def test_set_rx_power(self):
        expected= [0x07, 0x05, 0xc9]

        self.libmetawear.mbl_mw_ibeacon_set_rx_power(self.board, -55);
        self.assertEqual(self.command, expected)

    def test_set_tx_power(self):
        expected= [0x07, 0x06, 0xf4]

        self.libmetawear.mbl_mw_ibeacon_set_tx_power(self.board, -12);
        self.assertEqual(self.command, expected)

    def test_set_uuid(self):
        expected= [0x07, 0x02, 0x5a, 0xe7, 0xba, 0xfb, 0x4c, 0x46, 0xdd, 0xd9, 0x95, 0x91, 0xcb, 0x85, 0x06, 0x90, 0x6a, 0x32]

        ad_uuid= uuid.UUID('{326a9006-85cb-9195-d9dd-464cfbbae75a}')
        self.libmetawear.mbl_mw_ibeacon_set_uuid(self.board, ad_uuid.bytes[::-1]);
        self.assertEqual(self.command, expected)

    def test_enable(self):
        expected= [0x07, 0x01, 0x01]

        self.libmetawear.mbl_mw_ibeacon_enable(self.board);
        self.assertEqual(self.command, expected)

    def test_ibeacon_disable(self):
        expected= [0x07, 0x01, 0x00]

        self.libmetawear.mbl_mw_ibeacon_disable(self.board);
        self.assertEqual(self.command, expected)
