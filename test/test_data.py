from common import TestMetaWearBase
from ctypes import create_string_buffer
from cbindings import *
#from mbientlab.metawear.cbindings import *

class TestDataLength(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_MOTION_S_BOARD
        self.metawear_motion_s_services[0x11]= create_string_buffer(b'\x11\x80\x00\x03', 4)
        super().setUp()

    def sensorDataHandler(self, context, data):
        self.data_length= data.contents.length

    def test_uint32(self):
        expected= 4

        pin_monitor_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 1, GpioAnalogReadMode.ADC);
        self.libmetawear.mbl_mw_datasignal_subscribe(pin_monitor_signal, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x05\x87\x01\x72\x03', 5))

        print("TestDataLength \n")
        self.assertEqual(self.data_length, expected)

    def test_float(self):
        expected= 4

        temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 
                MetaWearRProChannel.EXT_THERMISTOR)
        self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x04\x81\x02\xac\xff', 5))

        print("TestDataLength \n")
        self.assertEqual(self.data_length, expected)

    def test_cartesian_float(self):
        expected= 12

        accel_data_signal= self.libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(accel_data_signal, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x03\x04\xe1\xb3\xa1\x24\xb1\x2e', 8))

        print("TestDataLength \n")
        self.assertEqual(self.data_length, expected)

    def test_byte_array(self):
        expected= 1

        signal= self.libmetawear.mbl_mw_i2c_get_data_signal(self.board, 1, 0xa)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x0d\x81\x0a\x2a', 4))

        print("TestDataLength \n")
        self.assertEqual(self.data_length, expected)

    def test_battery_state(self):
        expected= 4

        signal= self.libmetawear.mbl_mw_settings_get_battery_state_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, None, self.sensor_data_handler)
        self.notify_mw_char(create_string_buffer(b'\x11\x8c\x63\x34\x10', 5))

        print("TestDataLength \n")
        self.assertEqual(self.data_length, expected)
