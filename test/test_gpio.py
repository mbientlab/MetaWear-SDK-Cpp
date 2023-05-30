from common import TestMetaWearBase
from ctypes import byref, create_string_buffer
from cbindings import *
#from mbientlab.metawear.cbindings import *

class TestGpioDigitalConfig(TestMetaWearBase):
    def test_set_digital_out(self):
        expected= [0x05, 0x01, 0x00]

        self.libmetawear.mbl_mw_gpio_set_digital_output(self.board, 0)
        print("TestGpioDigitalConfig \n")
        self.assertEqual(self.command, expected)

    def test_clear_digital_out(self):
        expected= [0x05, 0x02, 0x01]

        self.libmetawear.mbl_mw_gpio_clear_digital_output(self.board, 1)
        print("TestGpioDigitalConfig \n")
        self.assertEqual(self.command, expected)

    def test_set_pull_mode(self):
        tests= [
            {'expected': [0x05, 0x05, 0x02], 'mode': GpioPullMode.NONE, 'pin': 2},
            {'expected': [0x05, 0x04, 0x03], 'mode': GpioPullMode.DOWN, 'pin': 3},
            {'expected': [0x05, 0x03, 0x04], 'mode': GpioPullMode.UP, 'pin': 4}
        ]

        for test in tests:
            with self.subTest(pullmode=test['mode']):
                self.libmetawear.mbl_mw_gpio_set_pull_mode(self.board, test['pin'], test['mode'])
                print("TestGpioDigitalConfig \n")
                self.assertEqual(self.command, test['expected'])

    def test_set_pin_change_type(self):
        tests= [
            {'expected': [0x05, 0x09, 0x05, 0x03], 'mode': GpioPinChangeType.ANY, 'pin': 5},
            {'expected': [0x05, 0x09, 0x06, 0x02], 'mode': GpioPinChangeType.FALLING, 'pin': 6},
            {'expected': [0x05, 0x09, 0x07, 0x01], 'mode': GpioPinChangeType.RISING, 'pin': 7}
        ]

        for test in tests:
            with self.subTest(changetype=test['mode']):
                self.libmetawear.mbl_mw_gpio_set_pin_change_type(self.board, test['pin'], test['mode'])
                print("TestGpioDigitalConfig \n")
                self.assertEqual(self.command, test['expected'])

class TestGpioDigitalData(TestMetaWearBase):
    def test_pin_monitor(self):
        tests= [
            {'expected': 1, 'response': create_string_buffer(b'\x05\x0a\x00\x01', 4), 'change_type': "rising"},
            {'expected': 0, 'response': create_string_buffer(b'\x05\x0a\x00\x00', 4), 'change_type': "falling"}
        ]

        pin_monitor_signal= self.libmetawear.mbl_mw_gpio_get_pin_monitor_data_signal(self.board, 0);
        self.libmetawear.mbl_mw_datasignal_subscribe(pin_monitor_signal, None, self.sensor_data_handler)

        for test in tests:
            with self.subTest(changetype=test['change_type']):
                self.notify_mw_char(test['response'])
                print("TestGpioDigitalData \n")
                self.assertEqual(self.data_uint32.value, test['expected'])

    # Combining subscribe and unsubscribe since unsubscribing from pin monitoring does not 
    # issue any metawear commands 
    def test_pin_monitor_subscribe(self):
        expected= [0x5, 0xa, 0x1]

        pin_monitor_signal= self.libmetawear.mbl_mw_gpio_get_pin_monitor_data_signal(self.board, 1);
        self.libmetawear.mbl_mw_datasignal_subscribe(pin_monitor_signal, None, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_unsubscribe(pin_monitor_signal)
        print("TestGpioDigitalData \n")
        self.assertEqual(self.command, expected)

    def test_pin_monitor_start(self):
        expected= [0x05, 0x0b, 0x05, 0x01]

        self.libmetawear.mbl_mw_gpio_start_pin_monitoring(self.board, 5)
        print("TestGpioDigitalData \n")
        self.assertEqual(self.command, expected)

    def test_pin_monitor_stop(self):
        expected= [0x05, 0x0b, 0x06, 0x00]

        self.libmetawear.mbl_mw_gpio_stop_pin_monitoring(self.board, 6)
        print("TestGpioDigitalData \n")
        self.assertEqual(self.command, expected)

    def test_read_digital_input(self):
        expected= [0x05, 0x88, 0x04]

        di_signal= self.libmetawear.mbl_mw_gpio_get_digital_input_data_signal(self.board, 4)
        self.libmetawear.mbl_mw_datasignal_subscribe(di_signal, None, self.sensor_data_handler)
        self.libmetawear.mbl_mw_datasignal_read(di_signal)
        print("TestGpioDigitalData \n")
        self.assertEqual(self.command, expected)

    def test_read_digital_input_silent(self):
        expected= [0x05, 0xc8, 0x04]

        di_signal= self.libmetawear.mbl_mw_gpio_get_digital_input_data_signal(self.board, 4)
        self.libmetawear.mbl_mw_datasignal_read(di_signal)
        print("TestGpioDigitalData \n")
        self.assertEqual(self.command, expected)


    def test_handle_digital_state(self):
        tests= [
            {'expected': 1, 'response': create_string_buffer(b'\x05\x88\x07\x01', 4), 'state': "high" },
            {'expected': 0, 'response': create_string_buffer(b'\x05\x88\x07\x00', 4), 'state': "low"}
        ]

        pin_monitor_signal= self.libmetawear.mbl_mw_gpio_get_digital_input_data_signal(self.board, 7);
        self.libmetawear.mbl_mw_datasignal_subscribe(pin_monitor_signal, None, self.sensor_data_handler)

        for test in tests:
            with self.subTest(changetype=test['state']):
                self.notify_mw_char(test['response'])
                print("TestGpioDigitalData \n")
                self.assertEqual(self.data_uint32.value, test['expected'])

class TestGpioAnalogData(TestMetaWearBase):
    def test_read_analog_input(self):
        tests= [
            {'expected': [0x05, 0x86, 0x03], 'mode': GpioAnalogReadMode.ABS_REF, 'pin': 3},
            {'expected': [0x05, 0x87, 0x02], 'mode': GpioAnalogReadMode.ADC, 'pin': 2}
        ]

        for test in tests:
            with self.subTest(readmode=test['mode']):
                an_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, test['pin'], test['mode'])
                self.libmetawear.mbl_mw_datasignal_subscribe(an_signal, None, self.sensor_data_handler)
                self.libmetawear.mbl_mw_datasignal_read(an_signal)
                print("TestGpioAnalogData \n")
                self.assertEqual(self.command, test['expected'])

    def test_read_analog_input_silent(self):
        tests= [
            {'expected': [0x05, 0xc6, 0x03], 'mode': GpioAnalogReadMode.ABS_REF, 'pin': 3},
            {'expected': [0x05, 0xc7, 0x02], 'mode': GpioAnalogReadMode.ADC, 'pin': 2}
        ]

        for test in tests:
            with self.subTest(readmode=test['mode']):
                an_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, test['pin'], test['mode'])
                self.libmetawear.mbl_mw_datasignal_read(an_signal)
                print("TestGpioAnalogData \n")
                self.assertEqual(self.command, test['expected'])

    def test_read_analog_enhanced(self):
        parameters= GpioAnalogReadParameters()
        tests= [
            {'expected': [0x05, 0xc6, 0x03], 'mode': GpioAnalogReadMode.ABS_REF, 'pin': 3},
            {'expected': [0x05, 0xc7, 0x02], 'mode': GpioAnalogReadMode.ADC, 'pin': 2}
        ]

        for test in tests:
            with self.subTest(readmode=test['mode']):
                an_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, test['pin'], test['mode'])
                self.libmetawear.mbl_mw_datasignal_read_with_parameters(an_signal, byref(parameters))
                print("TestGpioAnalogData \n")
                self.assertEqual(self.command, test['expected'])

    def test_handle_analog_data(self):
        tests= [
            {'expected': 882, 'response': create_string_buffer(b'\x05\x87\x01\x72\x03', 5), 'mode': GpioAnalogReadMode.ADC},
            {'expected': 2498, 'response': create_string_buffer(b'\x05\x86\x01\xc2\x09', 5), 'mode': GpioAnalogReadMode.ABS_REF}
        ]

        for test in tests:
            with self.subTest(readmode=test['mode']):
                pin_monitor_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 1, test['mode']);
                self.libmetawear.mbl_mw_datasignal_subscribe(pin_monitor_signal, None, self.sensor_data_handler)

                self.notify_mw_char(test['response'])
                print("TestGpioAnalogData \n")
                self.assertEqual(self.data_uint32.value, test['expected'])

class TestGpioEnhancedAnalogRead(TestMetaWearBase):
    def setUp(self):
        self.metawear_motion_r_services[0x5]= create_string_buffer(b'\x05\x80\x00\x02', 4)
        super().setUp()

    def test_read_analog_no_parameters(self):
        tests= [
            {'expected': [0x05, 0xc6, 0x03, 0xff, 0xff, 0x00, 0xff], 'mode': GpioAnalogReadMode.ABS_REF, 'pin': 3},
            {'expected': [0x05, 0xc7, 0x02, 0xff, 0xff, 0x00, 0xff], 'mode': GpioAnalogReadMode.ADC, 'pin': 2}
        ]

        for test in tests:
            with self.subTest(readmode=test['mode']):
                an_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, test['pin'], test['mode'])
                self.libmetawear.mbl_mw_datasignal_read(an_signal)
                print("TestGpioEnhancedAnalogRead \n")
                self.assertEqual(self.command, test['expected'])

    def test_read_analog_with_parameters(self):
        tests= [
            {'expected': [0x05, 0x86, 0x03, 0x01, 0x02, 0x02, 0x15], 'mode': GpioAnalogReadMode.ABS_REF, 'pin': 3},
            {'expected': [0x05, 0x87, 0x02, 0x01, 0x02, 0x02, 0x15], 'mode': GpioAnalogReadMode.ADC, 'pin': 2}
        ]

        parameters= GpioAnalogReadParameters(pullup_pin= 1, pulldown_pin= 2, virtual_pin= 0x15, delay_us= 10)

        for test in tests:
            with self.subTest(readmode=test['mode']):
                an_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, test['pin'], test['mode'])
                self.libmetawear.mbl_mw_datasignal_subscribe(an_signal, None, self.sensor_data_handler)
                self.libmetawear.mbl_mw_datasignal_read_with_parameters(an_signal, byref(parameters))
                print("TestGpioEnhancedAnalogRead \n")
                self.assertEqual(self.command, test['expected'])

    def test_handle_analog_data(self):
        tests= [
            {'expected': 218, 'response': create_string_buffer(b'\x05\x87\x15\xda\x00', 5), 'mode': GpioAnalogReadMode.ADC},
            {'expected': 728, 'response': create_string_buffer(b'\x05\x86\x15\xd8\x02', 5), 'mode': GpioAnalogReadMode.ABS_REF}
        ]

        for test in tests:
            with self.subTest(readmode=test['mode']):
                virtual_signal= self.libmetawear.mbl_mw_gpio_get_analog_input_data_signal(self.board, 0x15, test['mode'])
                self.libmetawear.mbl_mw_datasignal_subscribe(virtual_signal, None, self.sensor_data_handler)

                self.notify_mw_char(test['response'])
                print("TestGpioEnhancedAnalogRead \n")
                self.assertEqual(self.data_uint32.value, test['expected'])
