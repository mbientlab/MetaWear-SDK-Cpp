from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear import Status

class TestMetaWearBoard(TestMetaWearBase):
    def test_no_response_handler(self):
        responses= [
            create_string_buffer(b'\x03\x04\x16\xc4\x94\xa2\x2a\xd0', 8),
            create_string_buffer(b'\x03\x04\x56\xfa\x05\xf6\x18\x03', 8),
            create_string_buffer(b'\x14\x03\xed\x92\xb0\x00', 6),
            create_string_buffer(b'\x12\x01\xd3\x35\x8b\x01', 6),
            create_string_buffer(b'\x12\x02\x1e\x1f\xfe\xff', 6),
            create_string_buffer(b'\x13\x05\x3e\x43\xff\x7f\x00\x80', 8),
            create_string_buffer(b'\x01\x01\x01', 3)
        ]

        for resp in responses:
            with self.subTest(response=resp):
                status= self.libmetawear.mbl_mw_metawearboard_handle_response(self.board, resp.raw, len(resp))
                self.assertEqual(status, Status.WARNING_UNEXPECTED_SENSOR_DATA)
