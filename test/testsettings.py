from common import TestMetaWearBase
from ctypes import create_string_buffer, c_float

class TestSettings(TestMetaWearBase):
    def test_set_name(self):
        expected= [0x11, 0x01, 0x41, 0x6e, 0x74, 0x69, 0x57, 0x61, 0x72, 0x65]

        device_name= create_string_buffer(b'AntiWare', 8)
        self.libmetawear.mbl_mw_settings_set_device_name(self.board, device_name.raw, len(device_name.raw))

        self.assertEqual(self.command, expected)

    def test_set_tx_power(self):
        expected= [0x11, 0x03, 0xec]

        self.libmetawear.mbl_mw_settings_set_tx_power(self.board, -20)
        self.assertEqual(self.command, expected)

    def test_set_scan_response(self):
        expected_cmds= [
            [0x11, 0x08, 0x03, 0x03, 0xd8, 0xfe, 0x10, 0x16, 0xd8, 0xfe, 0x00, 0x12, 0x00, 0x6d, 0x62],
            [0x11, 0x07, 0x69, 0x65, 0x6e, 0x74, 0x6c, 0x61, 0x62, 0x00]
        ]

        scan_response= create_string_buffer(b'\x03\x03\xD8\xfe\x10\x16\xd8\xfe\x00\x12\x00\x6d\x62\x69\x65\x6e\x74\x6c\x61\x62\x00', 21)
        self.libmetawear.mbl_mw_settings_set_scan_response(self.board, scan_response.raw, len(scan_response.raw))
        self.assertEqual(self.command_history, expected_cmds)

    def test_start_advertising(self):
        expected= [0x11, 0x5]

        self.libmetawear.mbl_mw_settings_start_advertising(self.board)
        self.assertEqual(self.command, expected)

    def test_set_conn_params(self):
        expected= [0x11, 0x09, 0x58, 0x02, 0x20, 0x03, 0x80, 0x00, 0x66, 0x06]

        self.libmetawear.mbl_mw_settings_set_connection_parameters(self.board, c_float(750.), c_float(1000.), 128, 16384)
        self.assertEqual(self.command, expected)

    def test_set_ad_interval(self):
        expected= [0x11, 0x02, 0xa1, 0x01, 0x0]
        self.libmetawear.mbl_mw_settings_set_ad_interval(self.board, 417, 0)
        self.assertEqual(self.command, expected)

class TestSettingsRevision1(TestMetaWearBase):
    def setUp(self):
        self.metawear_r_services[0x11]= create_string_buffer(b'\x11\x80\x00\x01', 4)
        super().setUp()

    def test_set_ad_interval(self):
        self.libmetawear.mbl_mw_metawearboard_initialize(self.board, self.initialized_fn)

        expected= [0x11, 0x02, 0x9b, 0x02, 0xb4]
        self.libmetawear.mbl_mw_settings_set_ad_interval(self.board, 417, 180)
        self.assertEqual(self.command, expected)

