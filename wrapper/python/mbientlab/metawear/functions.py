# WARNING: This file is generated from the API header files
# It is used to set the appropriate return and arguments types of the exposed functions 
# from the MetaWear shared library so Python can correctly call into the library
from ctypes import *
from .core import *
from .peripheral import Led
from .sensor import SPI

def setup_libmetawear(libmetawear):
    libmetawear.mbl_mw_memory_free.restype= None
    libmetawear.mbl_mw_memory_free.argtypes= [c_void_p]

    libmetawear.mbl_mw_datasignal_subscribe.restype= None
    libmetawear.mbl_mw_datasignal_subscribe.argtypes= [c_void_p, Fn_DataPtr]

    libmetawear.mbl_mw_datasignal_unsubscribe.restype= None
    libmetawear.mbl_mw_datasignal_unsubscribe.argtypes= [c_void_p]

    libmetawear.mbl_mw_datasignal_is_readable.restype= c_int
    libmetawear.mbl_mw_datasignal_is_readable.argtypes= [c_void_p]

    libmetawear.mbl_mw_datasignal_read.restype= None
    libmetawear.mbl_mw_datasignal_read.argtypes= [c_void_p]

    libmetawear.mbl_mw_datasignal_read_with_parameters.restype= None
    libmetawear.mbl_mw_datasignal_read_with_parameters.argtypes= [c_void_p, c_void_p]

    libmetawear.mbl_mw_datasignal_log.restype= None
    libmetawear.mbl_mw_datasignal_log.argtypes= [c_void_p, Fn_VoidPtr]

    libmetawear.mbl_mw_debug_reset.restype= None
    libmetawear.mbl_mw_debug_reset.argtypes= [c_void_p]

    libmetawear.mbl_mw_debug_jump_to_bootloader.restype= None
    libmetawear.mbl_mw_debug_jump_to_bootloader.argtypes= [c_void_p]

    libmetawear.mbl_mw_debug_disconnect.restype= None
    libmetawear.mbl_mw_debug_disconnect.argtypes= [c_void_p]

    libmetawear.mbl_mw_debug_reset_after_gc.restype= None
    libmetawear.mbl_mw_debug_reset_after_gc.argtypes= [c_void_p]

    libmetawear.mbl_mw_event_get_owner.restype= c_void_p
    libmetawear.mbl_mw_event_get_owner.argtypes= [c_void_p]

    libmetawear.mbl_mw_event_record_commands.restype= None
    libmetawear.mbl_mw_event_record_commands.argtypes= [c_void_p]

    libmetawear.mbl_mw_event_end_record.restype= None
    libmetawear.mbl_mw_event_end_record.argtypes= [c_void_p, Fn_VoidPtr_Int]

    libmetawear.mbl_mw_logging_start.restype= None
    libmetawear.mbl_mw_logging_start.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_logging_stop.restype= None
    libmetawear.mbl_mw_logging_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_logging_clear_entries.restype= None
    libmetawear.mbl_mw_logging_clear_entries.argtypes= [c_void_p]

    libmetawear.mbl_mw_logging_download.restype= None
    libmetawear.mbl_mw_logging_download.argtypes= [c_void_p, c_ubyte, POINTER(LogDownloadHandler)]

    libmetawear.mbl_mw_logger_get_id.restype= c_ubyte
    libmetawear.mbl_mw_logger_get_id.argtypes= [c_void_p]

    libmetawear.mbl_mw_logger_get_signal.restype= c_void_p
    libmetawear.mbl_mw_logger_get_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_logger_lookup_id.restype= c_void_p
    libmetawear.mbl_mw_logger_lookup_id.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_logger_remove.restype= None
    libmetawear.mbl_mw_logger_remove.argtypes= [c_void_p]

    libmetawear.mbl_mw_logger_subscribe.restype= None
    libmetawear.mbl_mw_logger_subscribe.argtypes= [c_void_p, Fn_DataPtr]

    libmetawear.mbl_mw_settings_get_disconnect_event.restype= c_void_p
    libmetawear.mbl_mw_settings_get_disconnect_event.argtypes= [c_void_p]

    libmetawear.mbl_mw_settings_get_battery_state_data_signal.restype= c_void_p
    libmetawear.mbl_mw_settings_get_battery_state_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_settings_set_device_name.restype= None
    libmetawear.mbl_mw_settings_set_device_name.argtypes= [c_void_p, c_void_p, c_ubyte]

    libmetawear.mbl_mw_settings_set_ad_interval.restype= None
    libmetawear.mbl_mw_settings_set_ad_interval.argtypes= [c_void_p, c_ushort, c_ubyte]

    libmetawear.mbl_mw_settings_set_tx_power.restype= None
    libmetawear.mbl_mw_settings_set_tx_power.argtypes= [c_void_p, c_byte]

    libmetawear.mbl_mw_settings_start_advertising.restype= None
    libmetawear.mbl_mw_settings_start_advertising.argtypes= [c_void_p]

    libmetawear.mbl_mw_settings_set_scan_response.restype= None
    libmetawear.mbl_mw_settings_set_scan_response.argtypes= [c_void_p, c_void_p, c_ubyte]

    libmetawear.mbl_mw_settings_set_connection_parameters.restype= None
    libmetawear.mbl_mw_settings_set_connection_parameters.argtypes= [c_void_p, c_float, c_float, c_ushort, c_ushort]

    libmetawear.mbl_mw_timer_create.restype= None
    libmetawear.mbl_mw_timer_create.argtypes= [c_void_p, c_uint, c_ushort, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_timer_create_indefinite.restype= None
    libmetawear.mbl_mw_timer_create_indefinite.argtypes= [c_void_p, c_uint, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_timer_get_id.restype= c_ubyte
    libmetawear.mbl_mw_timer_get_id.argtypes= [c_void_p]

    libmetawear.mbl_mw_timer_lookup_id.restype= c_void_p
    libmetawear.mbl_mw_timer_lookup_id.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_timer_start.restype= None
    libmetawear.mbl_mw_timer_start.argtypes= [c_void_p]

    libmetawear.mbl_mw_timer_stop.restype= None
    libmetawear.mbl_mw_timer_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_timer_remove.restype= None
    libmetawear.mbl_mw_timer_remove.argtypes= [c_void_p]

    libmetawear.mbl_mw_connection_notify_char_changed.restype= c_int
    libmetawear.mbl_mw_connection_notify_char_changed.argtypes= [c_void_p, c_void_p, c_ubyte]

    libmetawear.mbl_mw_metawearboard_notify_char_changed.restype= c_int
    libmetawear.mbl_mw_metawearboard_notify_char_changed.argtypes= [c_void_p, c_void_p, c_ubyte]

    libmetawear.mbl_mw_connection_char_read.restype= None
    libmetawear.mbl_mw_connection_char_read.argtypes= [c_void_p, c_void_p, c_void_p, c_ubyte]

    libmetawear.mbl_mw_metawearboard_char_read.restype= None
    libmetawear.mbl_mw_metawearboard_char_read.argtypes= [c_void_p, c_void_p, c_void_p, c_ubyte]

    libmetawear.mbl_mw_metawearboard_create.restype= c_void_p
    libmetawear.mbl_mw_metawearboard_create.argtypes= [POINTER(BtleConnection)]

    libmetawear.mbl_mw_metawearboard_free.restype= None
    libmetawear.mbl_mw_metawearboard_free.argtypes= [c_void_p]

    libmetawear.mbl_mw_metawearboard_set_time_for_response.restype= None
    libmetawear.mbl_mw_metawearboard_set_time_for_response.argtypes= [c_void_p, c_ushort]

    libmetawear.mbl_mw_metawearboard_initialize.restype= None
    libmetawear.mbl_mw_metawearboard_initialize.argtypes= [c_void_p, Fn_VoidPtr_Int]

    libmetawear.mbl_mw_metawearboard_tear_down.restype= None
    libmetawear.mbl_mw_metawearboard_tear_down.argtypes= [c_void_p]

    libmetawear.mbl_mw_metawearboard_is_initialized.restype= c_int
    libmetawear.mbl_mw_metawearboard_is_initialized.argtypes= [c_void_p]

    libmetawear.mbl_mw_metawearboard_lookup_module.restype= c_int
    libmetawear.mbl_mw_metawearboard_lookup_module.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_metawearboard_serialize.restype= c_void_p
    libmetawear.mbl_mw_metawearboard_serialize.argtypes= [c_void_p, POINTER(c_uint)]

    libmetawear.mbl_mw_metawearboard_deserialize.restype= None
    libmetawear.mbl_mw_metawearboard_deserialize.argtypes= [c_void_p, c_void_p, c_uint]

    libmetawear.mbl_mw_haptic_start_motor.restype= None
    libmetawear.mbl_mw_haptic_start_motor.argtypes= [c_void_p, c_float, c_ushort]

    libmetawear.mbl_mw_haptic_start_buzzer.restype= None
    libmetawear.mbl_mw_haptic_start_buzzer.argtypes= [c_void_p, c_ushort]

    libmetawear.mbl_mw_ibeacon_set_major.restype= None
    libmetawear.mbl_mw_ibeacon_set_major.argtypes= [c_void_p, c_ushort]

    libmetawear.mbl_mw_ibeacon_set_minor.restype= None
    libmetawear.mbl_mw_ibeacon_set_minor.argtypes= [c_void_p, c_ushort]

    libmetawear.mbl_mw_ibeacon_set_period.restype= None
    libmetawear.mbl_mw_ibeacon_set_period.argtypes= [c_void_p, c_ushort]

    libmetawear.mbl_mw_ibeacon_set_tx_power.restype= None
    libmetawear.mbl_mw_ibeacon_set_tx_power.argtypes= [c_void_p, c_byte]

    libmetawear.mbl_mw_ibeacon_set_rx_power.restype= None
    libmetawear.mbl_mw_ibeacon_set_rx_power.argtypes= [c_void_p, c_byte]

    libmetawear.mbl_mw_ibeacon_set_uuid.restype= None
    libmetawear.mbl_mw_ibeacon_set_uuid.argtypes= [c_void_p, c_void_p]

    libmetawear.mbl_mw_ibeacon_enable.restype= None
    libmetawear.mbl_mw_ibeacon_enable.argtypes= [c_void_p]

    libmetawear.mbl_mw_ibeacon_disable.restype= None
    libmetawear.mbl_mw_ibeacon_disable.argtypes= [c_void_p]

    libmetawear.mbl_mw_led_load_preset_pattern.restype= None
    libmetawear.mbl_mw_led_load_preset_pattern.argtypes= [POINTER(Led.Pattern), c_int]

    libmetawear.mbl_mw_led_write_pattern.restype= None
    libmetawear.mbl_mw_led_write_pattern.argtypes= [c_void_p, POINTER(Led.Pattern), c_int]

    libmetawear.mbl_mw_led_autoplay.restype= None
    libmetawear.mbl_mw_led_autoplay.argtypes= [c_void_p]

    libmetawear.mbl_mw_led_play.restype= None
    libmetawear.mbl_mw_led_play.argtypes= [c_void_p]

    libmetawear.mbl_mw_led_pause.restype= None
    libmetawear.mbl_mw_led_pause.argtypes= [c_void_p]

    libmetawear.mbl_mw_led_stop.restype= None
    libmetawear.mbl_mw_led_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_led_stop_and_clear.restype= None
    libmetawear.mbl_mw_led_stop_and_clear.argtypes= [c_void_p]

    libmetawear.mbl_mw_neopixel_init_slow_strand.restype= None
    libmetawear.mbl_mw_neopixel_init_slow_strand.argtypes= [c_void_p, c_ubyte, c_ubyte, c_ubyte, c_int]

    libmetawear.mbl_mw_neopixel_init_fast_strand.restype= None
    libmetawear.mbl_mw_neopixel_init_fast_strand.argtypes= [c_void_p, c_ubyte, c_ubyte, c_ubyte, c_int]

    libmetawear.mbl_mw_neopixel_free_strand.restype= None
    libmetawear.mbl_mw_neopixel_free_strand.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_neopixel_enable_hold.restype= None
    libmetawear.mbl_mw_neopixel_enable_hold.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_neopixel_disable_hold.restype= None
    libmetawear.mbl_mw_neopixel_disable_hold.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_neopixel_clear.restype= None
    libmetawear.mbl_mw_neopixel_clear.argtypes= [c_void_p, c_ubyte, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_neopixel_set_color.restype= None
    libmetawear.mbl_mw_neopixel_set_color.argtypes= [c_void_p, c_ubyte, c_ubyte, c_ubyte, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_neopixel_rotate.restype= None
    libmetawear.mbl_mw_neopixel_rotate.argtypes= [c_void_p, c_ubyte, c_ubyte, c_ushort, c_int]

    libmetawear.mbl_mw_neopixel_rotate_indefinitely.restype= None
    libmetawear.mbl_mw_neopixel_rotate_indefinitely.argtypes= [c_void_p, c_ubyte, c_ubyte, c_int]

    libmetawear.mbl_mw_neopixel_stop_rotation.restype= None
    libmetawear.mbl_mw_neopixel_stop_rotation.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_accumulator_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_accumulator_create.argtypes= [c_void_p, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_accumulator_create_size.restype= c_int
    libmetawear.mbl_mw_dataprocessor_accumulator_create_size.argtypes= [c_void_p, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_set_accumulator_state.restype= c_int
    libmetawear.mbl_mw_dataprocessor_set_accumulator_state.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_dataprocessor_average_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_average_create.argtypes= [c_void_p, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_average_reset.restype= c_int
    libmetawear.mbl_mw_dataprocessor_average_reset.argtypes= [c_void_p]

    libmetawear.mbl_mw_dataprocessor_average_modify_size.restype= c_int
    libmetawear.mbl_mw_dataprocessor_average_modify_size.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_buffer_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_buffer_create.argtypes= [c_void_p, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_comparator_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_comparator_create.argtypes= [c_void_p, c_int, c_float, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_comparator_create_signed.restype= c_int
    libmetawear.mbl_mw_dataprocessor_comparator_create_signed.argtypes= [c_void_p, c_int, c_float, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_comparator_create_unsigned.restype= c_int
    libmetawear.mbl_mw_dataprocessor_comparator_create_unsigned.argtypes= [c_void_p, c_int, c_float, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_comparator_modify.restype= c_int
    libmetawear.mbl_mw_dataprocessor_comparator_modify.argtypes= [c_void_p, c_int, c_float]

    libmetawear.mbl_mw_dataprocessor_comparator_modify_signal.restype= c_int
    libmetawear.mbl_mw_dataprocessor_comparator_modify_signal.argtypes= [c_void_p, c_int, c_void_p]

    libmetawear.mbl_mw_dataprocessor_multi_comparator_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_multi_comparator_create.argtypes= [c_void_p, c_int, c_int, c_void_p, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_multi_comparator_create_signed.restype= c_int
    libmetawear.mbl_mw_dataprocessor_multi_comparator_create_signed.argtypes= [c_void_p, c_int, c_int, c_void_p, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_multi_comparator_create_unsigned.restype= c_int
    libmetawear.mbl_mw_dataprocessor_multi_comparator_create_unsigned.argtypes= [c_void_p, c_int, c_int, c_void_p, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_multi_comparator_modify.restype= c_int
    libmetawear.mbl_mw_dataprocessor_multi_comparator_modify.argtypes= [c_void_p, c_int, c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_counter_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_counter_create.argtypes= [c_void_p, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_counter_create_size.restype= c_int
    libmetawear.mbl_mw_dataprocessor_counter_create_size.argtypes= [c_void_p, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_counter_set_state.restype= c_int
    libmetawear.mbl_mw_dataprocessor_counter_set_state.argtypes= [c_void_p, c_uint]

    libmetawear.mbl_mw_dataprocessor_get_state_data_signal.restype= c_void_p
    libmetawear.mbl_mw_dataprocessor_get_state_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_dataprocessor_remove.restype= None
    libmetawear.mbl_mw_dataprocessor_remove.argtypes= [c_void_p]

    libmetawear.mbl_mw_dataprocessor_get_id.restype= c_ubyte
    libmetawear.mbl_mw_dataprocessor_get_id.argtypes= [c_void_p]

    libmetawear.mbl_mw_dataprocessor_lookup_id.restype= c_void_p
    libmetawear.mbl_mw_dataprocessor_lookup_id.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_delta_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_delta_create.argtypes= [c_void_p, c_int, c_float, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_delta_set_reference.restype= c_int
    libmetawear.mbl_mw_dataprocessor_delta_set_reference.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_dataprocessor_delta_modify_magnitude.restype= c_int
    libmetawear.mbl_mw_dataprocessor_delta_modify_magnitude.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_dataprocessor_math_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_math_create.argtypes= [c_void_p, c_int, c_float, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_math_create_signed.restype= c_int
    libmetawear.mbl_mw_dataprocessor_math_create_signed.argtypes= [c_void_p, c_int, c_float, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_math_create_unsigned.restype= c_int
    libmetawear.mbl_mw_dataprocessor_math_create_unsigned.argtypes= [c_void_p, c_int, c_float, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_math_modify_rhs.restype= c_int
    libmetawear.mbl_mw_dataprocessor_math_modify_rhs.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_dataprocessor_math_modify_rhs_signal.restype= c_int
    libmetawear.mbl_mw_dataprocessor_math_modify_rhs_signal.argtypes= [c_void_p, c_void_p]

    libmetawear.mbl_mw_dataprocessor_passthrough_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_passthrough_create.argtypes= [c_void_p, c_int, c_ushort, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_passthrough_set_count.restype= c_int
    libmetawear.mbl_mw_dataprocessor_passthrough_set_count.argtypes= [c_void_p, c_ushort]

    libmetawear.mbl_mw_dataprocessor_passthrough_modify.restype= c_int
    libmetawear.mbl_mw_dataprocessor_passthrough_modify.argtypes= [c_void_p, c_int, c_ushort]

    libmetawear.mbl_mw_dataprocessor_pulse_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_pulse_create.argtypes= [c_void_p, c_int, c_float, c_ushort, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_pulse_modify.restype= c_int
    libmetawear.mbl_mw_dataprocessor_pulse_modify.argtypes= [c_void_p, c_float, c_ushort]

    libmetawear.mbl_mw_dataprocessor_rms_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_rms_create.argtypes= [c_void_p, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_rss_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_rss_create.argtypes= [c_void_p, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_sample_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_sample_create.argtypes= [c_void_p, c_ubyte, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_sample_modify_bin_size.restype= c_int
    libmetawear.mbl_mw_dataprocessor_sample_modify_bin_size.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_dataprocessor_threshold_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_threshold_create.argtypes= [c_void_p, c_int, c_float, c_float, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_threshold_modify_boundary.restype= c_int
    libmetawear.mbl_mw_dataprocessor_threshold_modify_boundary.argtypes= [c_void_p, c_float, c_float]

    libmetawear.mbl_mw_dataprocessor_time_create.restype= c_int
    libmetawear.mbl_mw_dataprocessor_time_create.argtypes= [c_void_p, c_int, c_uint, Fn_VoidPtr]

    libmetawear.mbl_mw_dataprocessor_time_modify_period.restype= c_int
    libmetawear.mbl_mw_dataprocessor_time_modify_period.argtypes= [c_void_p, c_uint]

    libmetawear.mbl_mw_acc_get_acceleration_data_signal.restype= c_void_p
    libmetawear.mbl_mw_acc_get_acceleration_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_get_high_freq_acceleration_data_signal.restype= c_void_p
    libmetawear.mbl_mw_acc_get_high_freq_acceleration_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_set_odr.restype= c_float
    libmetawear.mbl_mw_acc_set_odr.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_acc_set_range.restype= c_float
    libmetawear.mbl_mw_acc_set_range.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_acc_write_acceleration_config.restype= None
    libmetawear.mbl_mw_acc_write_acceleration_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_start.restype= None
    libmetawear.mbl_mw_acc_start.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_stop.restype= None
    libmetawear.mbl_mw_acc_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_enable_acceleration_sampling.restype= None
    libmetawear.mbl_mw_acc_enable_acceleration_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_disable_acceleration_sampling.restype= None
    libmetawear.mbl_mw_acc_disable_acceleration_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal.restype= c_void_p
    libmetawear.mbl_mw_acc_bosch_get_acceleration_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal.restype= c_void_p
    libmetawear.mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_get_step_counter_data_signal.restype= c_void_p
    libmetawear.mbl_mw_acc_bmi160_get_step_counter_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_get_step_detector_data_signal.restype= c_void_p
    libmetawear.mbl_mw_acc_bmi160_get_step_detector_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_set_odr.restype= None
    libmetawear.mbl_mw_acc_bmi160_set_odr.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bma255_set_odr.restype= None
    libmetawear.mbl_mw_acc_bma255_set_odr.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bosch_set_range.restype= None
    libmetawear.mbl_mw_acc_bosch_set_range.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bosch_write_acceleration_config.restype= None
    libmetawear.mbl_mw_acc_bosch_write_acceleration_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_set_step_counter_mode.restype= None
    libmetawear.mbl_mw_acc_bmi160_set_step_counter_mode.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_acc_bmi160_enable_step_counter.restype= None
    libmetawear.mbl_mw_acc_bmi160_enable_step_counter.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_disable_step_counter.restype= None
    libmetawear.mbl_mw_acc_bmi160_disable_step_counter.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_write_step_counter_config.restype= None
    libmetawear.mbl_mw_acc_bmi160_write_step_counter_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_reset_step_counter.restype= None
    libmetawear.mbl_mw_acc_bmi160_reset_step_counter.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_enable_step_detector.restype= None
    libmetawear.mbl_mw_acc_bmi160_enable_step_detector.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bmi160_disable_step_detector.restype= None
    libmetawear.mbl_mw_acc_bmi160_disable_step_detector.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bosch_start.restype= None
    libmetawear.mbl_mw_acc_bosch_start.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bosch_stop.restype= None
    libmetawear.mbl_mw_acc_bosch_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bosch_enable_acceleration_sampling.restype= None
    libmetawear.mbl_mw_acc_bosch_enable_acceleration_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_bosch_disable_acceleration_sampling.restype= None
    libmetawear.mbl_mw_acc_bosch_disable_acceleration_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal.restype= c_void_p
    libmetawear.mbl_mw_acc_mma8452q_get_acceleration_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal.restype= c_void_p
    libmetawear.mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_set_odr.restype= None
    libmetawear.mbl_mw_acc_mma8452q_set_odr.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_acc_mma8452q_set_range.restype= None
    libmetawear.mbl_mw_acc_mma8452q_set_range.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config.restype= None
    libmetawear.mbl_mw_acc_mma8452q_write_acceleration_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_start.restype= None
    libmetawear.mbl_mw_acc_mma8452q_start.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_stop.restype= None
    libmetawear.mbl_mw_acc_mma8452q_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_enable_acceleration_sampling.restype= None
    libmetawear.mbl_mw_acc_mma8452q_enable_acceleration_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_acc_mma8452q_disable_acceleration_sampling.restype= None
    libmetawear.mbl_mw_acc_mma8452q_disable_acceleration_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_als_ltr329_get_illuminance_data_signal.restype= c_void_p
    libmetawear.mbl_mw_als_ltr329_get_illuminance_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_als_ltr329_set_gain.restype= None
    libmetawear.mbl_mw_als_ltr329_set_gain.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_als_ltr329_set_integration_time.restype= None
    libmetawear.mbl_mw_als_ltr329_set_integration_time.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_als_ltr329_set_measurement_rate.restype= None
    libmetawear.mbl_mw_als_ltr329_set_measurement_rate.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_als_ltr329_write_config.restype= None
    libmetawear.mbl_mw_als_ltr329_write_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_als_ltr329_start.restype= None
    libmetawear.mbl_mw_als_ltr329_start.argtypes= [c_void_p]

    libmetawear.mbl_mw_als_ltr329_stop.restype= None
    libmetawear.mbl_mw_als_ltr329_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal.restype= c_void_p
    libmetawear.mbl_mw_baro_bosch_get_pressure_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_baro_bosch_get_pressure_read_data_signal.restype= c_void_p
    libmetawear.mbl_mw_baro_bosch_get_pressure_read_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_baro_bosch_get_altitude_data_signal.restype= c_void_p
    libmetawear.mbl_mw_baro_bosch_get_altitude_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_baro_bosch_set_oversampling.restype= None
    libmetawear.mbl_mw_baro_bosch_set_oversampling.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_baro_bosch_set_iir_filter.restype= None
    libmetawear.mbl_mw_baro_bosch_set_iir_filter.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_baro_bmp280_set_standby_time.restype= None
    libmetawear.mbl_mw_baro_bmp280_set_standby_time.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_baro_bme280_set_standby_time.restype= None
    libmetawear.mbl_mw_baro_bme280_set_standby_time.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_baro_bosch_set_standby_time.restype= c_float
    libmetawear.mbl_mw_baro_bosch_set_standby_time.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_baro_bosch_write_config.restype= None
    libmetawear.mbl_mw_baro_bosch_write_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_baro_bosch_start.restype= None
    libmetawear.mbl_mw_baro_bosch_start.argtypes= [c_void_p]

    libmetawear.mbl_mw_baro_bosch_stop.restype= None
    libmetawear.mbl_mw_baro_bosch_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_cd_tcs34725_get_adc_data_signal.restype= c_void_p
    libmetawear.mbl_mw_cd_tcs34725_get_adc_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_cd_tcs34725_set_integration_time.restype= None
    libmetawear.mbl_mw_cd_tcs34725_set_integration_time.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_cd_tcs34725_set_gain.restype= None
    libmetawear.mbl_mw_cd_tcs34725_set_gain.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_cd_tcs34725_enable_illuminator_led.restype= None
    libmetawear.mbl_mw_cd_tcs34725_enable_illuminator_led.argtypes= [c_void_p]

    libmetawear.mbl_mw_cd_tcs34725_disable_illuminator_led.restype= None
    libmetawear.mbl_mw_cd_tcs34725_disable_illuminator_led.argtypes= [c_void_p]

    libmetawear.mbl_mw_cd_tcs34725_write_config.restype= None
    libmetawear.mbl_mw_cd_tcs34725_write_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_gpio_get_analog_input_data_signal.restype= c_void_p
    libmetawear.mbl_mw_gpio_get_analog_input_data_signal.argtypes= [c_void_p, c_ubyte, c_int]

    libmetawear.mbl_mw_gpio_get_digital_input_data_signal.restype= c_void_p
    libmetawear.mbl_mw_gpio_get_digital_input_data_signal.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gpio_get_pin_monitor_data_signal.restype= c_void_p
    libmetawear.mbl_mw_gpio_get_pin_monitor_data_signal.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gpio_set_pull_mode.restype= None
    libmetawear.mbl_mw_gpio_set_pull_mode.argtypes= [c_void_p, c_ubyte, c_int]

    libmetawear.mbl_mw_gpio_set_digital_output.restype= None
    libmetawear.mbl_mw_gpio_set_digital_output.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gpio_clear_digital_output.restype= None
    libmetawear.mbl_mw_gpio_clear_digital_output.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gpio_set_pin_change_type.restype= None
    libmetawear.mbl_mw_gpio_set_pin_change_type.argtypes= [c_void_p, c_ubyte, c_int]

    libmetawear.mbl_mw_gpio_start_pin_monitoring.restype= None
    libmetawear.mbl_mw_gpio_start_pin_monitoring.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gpio_stop_pin_monitoring.restype= None
    libmetawear.mbl_mw_gpio_stop_pin_monitoring.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal.restype= c_void_p
    libmetawear.mbl_mw_gyro_bmi160_get_rotation_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal.restype= c_void_p
    libmetawear.mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_set_odr.restype= None
    libmetawear.mbl_mw_gyro_bmi160_set_odr.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_gyro_bmi160_set_range.restype= None
    libmetawear.mbl_mw_gyro_bmi160_set_range.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_gyro_bmi160_write_config.restype= None
    libmetawear.mbl_mw_gyro_bmi160_write_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_start.restype= None
    libmetawear.mbl_mw_gyro_bmi160_start.argtypes= [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_stop.restype= None
    libmetawear.mbl_mw_gyro_bmi160_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_enable_rotation_sampling.restype= None
    libmetawear.mbl_mw_gyro_bmi160_enable_rotation_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_gyro_bmi160_disable_rotation_sampling.restype= None
    libmetawear.mbl_mw_gyro_bmi160_disable_rotation_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_humidity_bme280_get_percentage_data_signal.restype= c_void_p
    libmetawear.mbl_mw_humidity_bme280_get_percentage_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_humidity_bme280_set_oversampling.restype= None
    libmetawear.mbl_mw_humidity_bme280_set_oversampling.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_i2c_get_data_signal.restype= c_void_p
    libmetawear.mbl_mw_i2c_get_data_signal.argtypes= [c_void_p, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_i2c_write.restype= None
    libmetawear.mbl_mw_i2c_write.argtypes= [c_void_p, c_ubyte, c_ubyte, c_void_p, c_ubyte]

    libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal.restype= c_void_p
    libmetawear.mbl_mw_mag_bmm150_get_b_field_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_set_power_preset.restype= None
    libmetawear.mbl_mw_mag_bmm150_set_power_preset.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_mag_bmm150_enable_b_field_sampling.restype= None
    libmetawear.mbl_mw_mag_bmm150_enable_b_field_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_disable_b_field_sampling.restype= None
    libmetawear.mbl_mw_mag_bmm150_disable_b_field_sampling.argtypes= [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_start.restype= None
    libmetawear.mbl_mw_mag_bmm150_start.argtypes= [c_void_p]

    libmetawear.mbl_mw_mag_bmm150_stop.restype= None
    libmetawear.mbl_mw_mag_bmm150_stop.argtypes= [c_void_p]

    libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal.restype= c_void_p
    libmetawear.mbl_mw_multi_chnl_temp_get_temperature_data_signal.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_multi_chnl_temp_configure_ext_thermistor.restype= None
    libmetawear.mbl_mw_multi_chnl_temp_configure_ext_thermistor.argtypes= [c_void_p, c_ubyte, c_ubyte, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_multi_chnl_temp_get_source.restype= c_int
    libmetawear.mbl_mw_multi_chnl_temp_get_source.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_multi_chnl_temp_get_num_channels.restype= c_ubyte
    libmetawear.mbl_mw_multi_chnl_temp_get_num_channels.argtypes= [c_void_p]

    libmetawear.mbl_mw_proximity_tsl2671_get_adc_data_signal.restype= c_void_p
    libmetawear.mbl_mw_proximity_tsl2671_get_adc_data_signal.argtypes= [c_void_p]

    libmetawear.mbl_mw_proximity_tsl2671_set_integration_time.restype= None
    libmetawear.mbl_mw_proximity_tsl2671_set_integration_time.argtypes= [c_void_p, c_float]

    libmetawear.mbl_mw_proximity_tsl2671_set_n_pulses.restype= None
    libmetawear.mbl_mw_proximity_tsl2671_set_n_pulses.argtypes= [c_void_p, c_ubyte]

    libmetawear.mbl_mw_proximity_tsl2671_set_receiver_channel.restype= None
    libmetawear.mbl_mw_proximity_tsl2671_set_receiver_channel.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_proximity_tsl2671_set_transmitter_current.restype= None
    libmetawear.mbl_mw_proximity_tsl2671_set_transmitter_current.argtypes= [c_void_p, c_int]

    libmetawear.mbl_mw_proximity_tsl2671_write_config.restype= None
    libmetawear.mbl_mw_proximity_tsl2671_write_config.argtypes= [c_void_p]

    libmetawear.mbl_mw_spi_get_data_signal.restype= c_void_p
    libmetawear.mbl_mw_spi_get_data_signal.argtypes= [c_void_p, c_ubyte, c_ubyte]

    libmetawear.mbl_mw_spi_write.restype= None
    libmetawear.mbl_mw_spi_write.argtypes= [c_void_p, POINTER(SPI.Parameters)]

    libmetawear.mbl_mw_switch_get_state_data_signal.restype= c_void_p
    libmetawear.mbl_mw_switch_get_state_data_signal.argtypes= [c_void_p]
