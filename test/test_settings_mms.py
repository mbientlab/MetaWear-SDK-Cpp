from common import TestMetaWearBase, to_string_buffer
from ctypes import *
from cbindings import BatteryState, FnVoid_VoidP_Int, FnVoid_VoidP_VoidP_Int, LedColor, LedPattern, Const
#from mbientlab.metawear.cbindings import BatteryState, FnVoid_VoidP_Int, FnVoid_VoidP_VoidP_Int, LedColor, LedPattern, Const
from threading import Event

class TestSettings(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_S_BOARD
        super().setUp()

    def test_turn_on_3v_regulator(self):
        expected= [0x11, 0x1c, 0x01]
        
        self.libmetawear.mbl_mw_settings_enable_3V_regulator(self.board, 1)
        self.assertEqual(self.command, expected)

   
