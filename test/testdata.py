from common import TestMetaWearBase
from ctypes import create_string_buffer
from mbientlab.metawear.sensor import *

class TestDataLength(TestMetaWearBase):
    def setUp(self):
        self.boardType= TestMetaWearBase.METAWEAR_ENV_BOARD
        self.metawear_environment_services[0x11]= create_string_buffer(b'\x11\x80\x00\x03', 4)
        super().setUp()

    def sensorDataHandler(self, data):
        self.data_length= data.contents.length

    def test_uint32(self):
        expected= 4
        response= create_string_buffer(b'\x05\x87\x01\x72\x03', 5)
    
        pin_monitor_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 1, Gpio.ANALOG_READ_MODE_ADC);
        self.libmetawear.mbl_mw_datasignal_subscribe(pin_monitor_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_notify_char_changed(self.board, response.raw, len(response))

        self.assertEqual(self.data_length, expected)

    def test_float(self):
        expected= 4
        response= create_string_buffer(b'\x04\x81\x02\xac\xff', 5)

        temp_signal= self.libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal(self.board, 
                MultiChannelTemperature.METAWEAR_RPRO_CHANNEL_EXT_THERMISTOR)
        self.libmetawear.mbl_mw_datasignal_subscribe(temp_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_notify_char_changed(self.board, response.raw, len(response))

        self.assertEqual(self.data_length, expected)

    def test_cartesian_float(self):
        expected= 12
        response= create_string_buffer(b'\x03\x04\xe1\xb3\xa1\x24\xb1\x2e', 8)

        accel_data_signal= self.libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(accel_data_signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_notify_char_changed(self.board, response.raw, len(response))

        self.assertEqual(self.data_length, expected)

    def test_byte_array(self):
        expected= 1
        response= create_string_buffer(b'\x0d\x81\x0a\x2a', 4)

        signal= self.libmetawear.mbl_mw_i2c_get_data_signal(self.board, 1, 0xa)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_notify_char_changed(self.board, response.raw, len(response.raw))

        self.assertEqual(self.data_length, expected)

    def test_battery_state(self):
        expected= 4
        response= create_string_buffer(b'\x11\x8c\x63\x34\x10', 5)

        signal= self.libmetawear.mbl_mw_settings_get_battery_state_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_notify_char_changed(self.board, response.raw, len(response.raw))

        self.assertEqual(self.data_length, expected)


    def test_tcs34725(self):
        expected= 8
        response= create_string_buffer(b'\x17\x81\xa2\x01\x7b\x00\x9a\x00\x7c\x00', 10)

        signal= self.libmetawear.mbl_mw_cd_tcs34725_get_adc_data_signal(self.board)
        self.libmetawear.mbl_mw_datasignal_subscribe(signal, self.sensor_data_handler)
        self.libmetawear.mbl_mw_metawearboard_notify_char_changed(self.board, response.raw, len(response.raw))

        self.assertEqual(self.data_length, expected)
