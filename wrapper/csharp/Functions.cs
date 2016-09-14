/// WARNING: This wrapper file is generated from the API header files and assumes it will be added to
/// your UWP app via the MetaWear.WinRT NuGet package: http://www.nuget.org/packages/MetaWear.WinRT/
using MbientLab.MetaWear.Core;
using MbientLab.MetaWear.Peripheral;
using MbientLab.MetaWear.Processor;
using MbientLab.MetaWear.Sensor;

using System;
using System.Runtime.InteropServices;

namespace MbientLab.MetaWear {
    public sealed class Functions {
        private const String METAWEAR_DLL= "MetaWear.WinRT.dll";
        /// <summary>
        /// Frees allocated memory
        /// </summary>
        /// <param name="ptr">Pointer to the memory to free</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_memory_free(IntPtr ptr);

        /// <summary>
        /// Subscribes to a data stream, processing messages with the given handler
        /// </summary>
        /// <param name="signal">Data signal to subscribe to</param>
        /// <param name="received_data">Callback function to handle data received from the signal</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_datasignal_subscribe(IntPtr signal, Fn_IntPtr received_data);

        /// <summary>
        /// Unsubscribes from a data stream
        /// </summary>
        /// <param name="signal">Data signal to unsubscribe from</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_datasignal_unsubscribe(IntPtr signal);

        /// <summary>
        /// Check if the data signal can be explicitly read
        /// </summary>
        /// <param name="signal">Data signal to check</param>
        /// <returns>Zero if not readable</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_datasignal_is_readable(IntPtr signal);

        /// <summary>
        /// Reads data from sensor represented by the data signal.  Data is forwarded to the callback function assigned by the mbl_mw_datasignal_subscribe function
        /// </summary>
        /// <param name="signal">Data signal to read from</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_datasignal_read(IntPtr signal);

        /// <summary>
        /// Reads data from sensor represented by the data signal.  Data is forwarded to the callback function assigned by the mbl_mw_datasignal_subscribe function.  This variant is for reads that require additional parameters.
        /// </summary>
        /// <param name="signal">Data signal to read from</param>
        /// <param name="parameters">Additional parameters required for the read operation</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_datasignal_read_with_parameters(IntPtr signal, IntPtr parameters);

        /// <summary>
        /// Creates an MblMwDataLogger for the signal
        /// </summary>
        /// <param name="signal">Data signal to log</param>
        /// <param name="logger_ready">Callback function to be executed when the logger is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_datasignal_log(IntPtr signal, Fn_IntPtr logger_ready);

        /// <summary>
        /// Issues a soft reset
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_debug_reset(IntPtr board);

        /// <summary>
        /// Restarts the board in bootloader mode
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_debug_jump_to_bootloader(IntPtr board);

        /// <summary>
        /// Instructs the board to terminate the connection
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_debug_disconnect(IntPtr board);

        /// <summary>
        /// Restarts the board after performing garbage collection
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_debug_reset_after_gc(IntPtr board);

        /// <summary>
        /// Retrieves the MblMwMetaWearBoard the event belongs to
        /// </summary>
        /// <param name="event">Event to lookup</param>
        /// <returns>Pointer to the owner</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_event_get_owner(IntPtr mw_event);

        /// <summary>
        /// Enables command recording.  All MetaWear commands called after this point will be executed when the owning event is fired
        /// </summary>
        /// <param name="event">Event to record commands for</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_event_record_commands(IntPtr mw_event);

        /// <summary>
        /// Ends command recording.  This function is non-blocking and will asynchronously alert the caller when the operation is completed.
        /// </summary>
        /// <param name="event">Event to end recording for</param>
        /// <param name="commands_recorded">Callback function to be executed when commands have been recorded</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_event_end_record(IntPtr mw_event, Fn_IntPtr_Int commands_recorded);

        /// <summary>
        /// Starts data logging
        /// </summary>
        /// <param name="board">Board to log data on</param>
        /// <param name="overwrite">Non</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_logging_start(IntPtr board, byte overwrite);

        /// <summary>
        /// Stops data logging
        /// </summary>
        /// <param name="board">Board to stop logging</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_logging_stop(IntPtr board);

        /// <summary>
        /// Clear the logger of saved entries
        /// </summary>
        /// <param name="board">Board to remove entries from</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_logging_clear_entries(IntPtr board);

        /// <summary>
        /// Downloads the log data
        /// </summary>
        /// <param name="board">Board to download the log data from</param>
        /// <param name="n_notifies">How many progress updates to send</param>
        /// <param name="handler">Handler for processing logger responses</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_logging_download(IntPtr board, byte n_notifies, ref LogDownloadHandler handler);

        /// <summary>
        /// Retrieves the id value identifying the logger
        /// </summary>
        /// <param name="logger">Logger to lookup</param>
        /// <returns>Numerical id of the logger</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte mbl_mw_logger_get_id(IntPtr logger);

        /// <summary>
        /// Retrieves the data signal the logger is recording data for
        /// </summary>
        /// <param name="logger">Logger to lookup</param>
        /// <returns>Pointer to owning MblMwDataSignal object</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_logger_get_signal(IntPtr logger);

        /// <summary>
        /// Looks up the MblMwDataLogger object corresponding to the id
        /// </summary>
        /// <param name="board">Board to search on</param>
        /// <param name="id">Numerical id to lookup</param>
        /// <returns>Logger object identified by the id</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_logger_lookup_id(IntPtr board, byte id);

        /// <summary>
        /// Removes the logger from the board
        /// </summary>
        /// <param name="logger">Logger to remove</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_logger_remove(IntPtr logger);

        /// <summary>
        /// Subscribes to responses from the data logger
        /// </summary>
        /// <param name="logger">Logger to subscribe to</param>
        /// <param name="received_data">Callback function to handle data received from the logger</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_logger_subscribe(IntPtr logger, Fn_IntPtr received_data);

        /// <summary>
        /// Retrieves an event pointer representing a disconnect event
        /// </summary>
        /// <param name="board">Board the event is fired on</param>
        /// <returns>Pointer to the disconnect event</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_settings_get_disconnect_event(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing battery state
        /// </summary>
        /// <param name="board">Board the battery state is from</param>
        /// <returns>Pointer to the battery state signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_settings_get_battery_state_data_signal(IntPtr board);

        /// <summary>
        /// Sets the advertisement name
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="device_name">Byte array containing the device name</param>
        /// <param name="len">Length of the array</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_settings_set_device_name(IntPtr board, byte[] device_name, byte len);

        /// <summary>
        /// Sets the advertising interval
        /// </summary>
        /// <param name="board">Board to set the ad interval</param>
        /// <param name="interval">Advertisement interval</param>
        /// <param name="timeout">Advertisement timeout</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_settings_set_ad_interval(IntPtr board, ushort interval, byte timeout);

        /// <summary>
        /// Sets advertising transmitting power.  If a non valid value is set, the nearest valid value will be used instead
        /// </summary>
        /// <param name="board">Board to set the TX power</param>
        /// <param name="tx_power">Valid values are</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_settings_set_tx_power(IntPtr board, sbyte tx_power);

        /// <summary>
        /// Starts advertising
        /// </summary>
        /// <param name="board">Board to start btle advertisement</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_settings_start_advertising(IntPtr board);

        /// <summary>
        /// Sets scan response
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="response">Scan response as a byte array</param>
        /// <param name="len">Length of the array</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_settings_set_scan_response(IntPtr board, byte[] response, byte len);

        /// <summary>
        /// Sets connection parameters
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="min_conn_interval">Connection interval lower bound</param>
        /// <param name="max_conn_interval">Connection interval upper bound</param>
        /// <param name="latency">Number of connection intervals to skip</param>
        /// <param name="timeout">Max time between data exchanges until the connection is considered to be lost</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_settings_set_connection_parameters(IntPtr board, float min_conn_interval, float max_conn_interval, ushort latency, ushort timeout);

        /// <summary>
        /// Creates a timer that will run for a set number of repetitions.  A pointer representing the timer will be passed to the user through a callback function
        /// </summary>
        /// <param name="board">Board the timer belongs to</param>
        /// <param name="period">How often to</param>
        /// <param name="repetitions">Number of events the timer will fire</param>
        /// <param name="delay">Zero if the tiemr should immediately fire</param>
        /// <param name="received_timer">Callback function to be executed when the timer is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_timer_create(IntPtr board, uint period, ushort repetitions, byte delay, Fn_IntPtr received_timer);

        /// <summary>
        /// Creates a timer that will run indefinitely.  A pointer representing the timer will be passed to the user through a callback function
        /// </summary>
        /// <param name="board">Board the timer belongs to</param>
        /// <param name="period">How often to</param>
        /// <param name="delay">Zero if the tiemr should immediately fire</param>
        /// <param name="received_timer">Callback function to be executed when the timer is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_timer_create_indefinite(IntPtr board, uint period, byte delay, Fn_IntPtr received_timer);

        /// <summary>
        /// Retrieves the id value identifying the timer
        /// </summary>
        /// <param name="timer">Timer to lookup</param>
        /// <returns>Numerical id of the timer</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte mbl_mw_timer_get_id(IntPtr timer);

        /// <summary>
        /// Looks up the MblMwTimer object corresponding to the id
        /// </summary>
        /// <param name="board">Board to search on</param>
        /// <param name="id">Numerical id to lookup</param>
        /// <returns>Timer object identified by the id</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_timer_lookup_id(IntPtr board, byte id);

        /// <summary>
        /// Starts a timer
        /// </summary>
        /// <param name="timer">Timer to start</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_timer_start(IntPtr timer);

        /// <summary>
        /// Stops a timer
        /// </summary>
        /// <param name="timer">Timer to stop</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_timer_stop(IntPtr timer);

        /// <summary>
        /// Removes the timer from the board
        /// </summary>
        /// <param name="timer">Timer to remove</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_timer_remove(IntPtr timer);

        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_connection_notify_char_changed(IntPtr board, byte[] value, byte len);

        /// <summary>
        /// Handles changes from the MetaWear notify characteristic.  All characteristic changes from the notify characteristic must be forwarded to this function
        /// </summary>
        /// <param name="board">Board the characteristic change is from</param>
        /// <param name="value">Byte array containing the new characteristic value</param>
        /// <param name="len">Length of the array</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_metawearboard_notify_char_changed(IntPtr board, byte[] value, byte len);

        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_connection_char_read(IntPtr board, IntPtr characteristic, byte[] value, byte length);

        /// <summary>
        /// Handles responses from a gatt characteristic read issued by a MblMwMetaWearBoard object.  All characteristic values read must be forwarded to this function.
        /// </summary>
        /// <param name="board">Board the response is from</param>
        /// <param name="characteristic">Characteristic that was read</param>
        /// <param name="value">Byte array containing the characteristic value</param>
        /// <param name="length">Length of the array</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_metawearboard_char_read(IntPtr board, IntPtr characteristic, byte[] value, byte length);

        /// <summary>
        /// Creates an instance of the MblMwMetaWearBoard struct
        /// </summary>
        /// <param name="connection">Connection struct the new MblMwMetaWearBoard variable will use for btle communication</param>
        /// <returns>Pointer to the newly created struct</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_metawearboard_create(ref BtleConnection connection);

        /// <summary>
        /// Frees the memory allocated for the struct
        /// </summary>
        /// <param name="board">Pointer to the memory to free</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_metawearboard_free(IntPtr board);

        /// <summary>
        /// Sets how long the API should wait before a required response is received.  You should increase this value if operations such as API initialization, creating timer, loggers, and data processors, and recording commands consistently time out.
        /// </summary>
        /// <param name="board">Board to configure</param>
        /// <param name="response_time_ms">How long to wait</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_metawearboard_set_time_for_response(IntPtr board, ushort response_time_ms);

        /// <summary>
        /// Initialize the API's internal state.  This function is non-blocking and will alert the caller when the operation is complete.
        /// </summary>
        /// <param name="board">Board to initialize</param>
        /// <param name="initialized">Callback function to be executed when the board is initialized</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_metawearboard_initialize(IntPtr board, Fn_IntPtr_Int initialized);

        /// <summary>
        /// Removes all data processors and timers from the MetaWear board
        /// </summary>
        /// <param name="board">Board to tear down</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_metawearboard_tear_down(IntPtr board);

        /// <summary>
        /// Checks if the board is initialized
        /// </summary>
        /// <param name="board">Board to check</param>
        /// <returns>Zero if not initialized</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_metawearboard_is_initialized(IntPtr board);

        /// <summary>
        /// Checks module type i.e. what kind of accelerometer is being used
        /// </summary>
        /// <param name="board">Board to check</param>
        /// <param name="module">Module to lookup</param>
        /// <returns>Module type used by the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_metawearboard_lookup_module(IntPtr board, Module module);

        /// <summary>
        /// Serializes the API state.  The memory allocated by the function must be freed by calling mbl_mw_memory_free.
        /// </summary>
        /// <param name="board">Board to serialize</param>
        /// <param name="size">Pointer to where the size of the returned byte array will be written to</param>
        /// <returns>Byte array of the serialized state</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_metawearboard_serialize(IntPtr board, ref uint size);

        /// <summary>
        /// Deserializes API state.  This function must be executed before calling mbl_mw_metawearboard_initialize.
        /// </summary>
        /// <param name="board">Board to deserialize</param>
        /// <param name="state">Byte array holding the the information state</param>
        /// <param name="size">Byte array size</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_metawearboard_deserialize(IntPtr board, byte[] state, uint size);

        /// <summary>
        /// Drives a motor
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="duty_cycle_per">Strength of the motor</param>
        /// <param name="pulse_width_ms">How long to run the motor</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_haptic_start_motor(IntPtr board, float duty_cycle_per, ushort pulse_width_ms);

        /// <summary>
        /// Drives a buzzer
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="pulse_width_ms">How long to run the buzzer</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_haptic_start_buzzer(IntPtr board, ushort pulse_width_ms);

        /// <summary>
        /// Sets the iBeacon advertising major number
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="major">New advertising major number</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_ibeacon_set_major(IntPtr board, ushort major);

        /// <summary>
        /// Sets the iBeacon advertising minor number
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="minor">New advertising minor number</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_ibeacon_set_minor(IntPtr board, ushort minor);

        /// <summary>
        /// Sets the iBeacon advertising period
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="period">New advertising period</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_ibeacon_set_period(IntPtr board, ushort period);

        /// <summary>
        /// Sets the iBeacon advertising transmitting power
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="tx_power">New advertising transmitting power</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_ibeacon_set_tx_power(IntPtr board, sbyte tx_power);

        /// <summary>
        /// Sets the iBeacon advertising receiving power
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="rx_power">New advertising receiving power</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_ibeacon_set_rx_power(IntPtr board, sbyte rx_power);

        /// <summary>
        /// Sets the iBeacon advertising UUID
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="ad_uuid">Byte representation of the UUID in little endian ordering</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_ibeacon_set_uuid(IntPtr board, byte[] ad_uuid);

        /// <summary>
        /// Enables iBeacon mode.  You will need to disconnect from the board to advertise as an iBeacon
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_ibeacon_enable(IntPtr board);

        /// <summary>
        /// Disables iBeacon mode
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_ibeacon_disable(IntPtr board);

        /// <summary>
        /// Loads the struct with a preset configuration
        /// </summary>
        /// <param name="pattern">Pointer to the pattern to write the configuration to</param>
        /// <param name="preset">Preset pattern to load</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_led_load_preset_pattern(ref Led.Pattern pattern, Led.PatternPreset preset);

        /// <summary>
        /// Writes the led pattern to the board
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="pattern">Pointer to the pattern attributes to write to the board</param>
        /// <param name="color">Color the pattern is configuring</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_led_write_pattern(IntPtr board, ref Led.Pattern pattern, Led.Color color);

        /// <summary>
        /// Plays any programmed patterns, and immediately plays any patterns programmed later
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_led_autoplay(IntPtr board);

        /// <summary>
        /// Plays any programmed patterns
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_led_play(IntPtr board);

        /// <summary>
        /// Pauses the patterns
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_led_pause(IntPtr board);

        /// <summary>
        /// Stops playing LED patterns
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_led_stop(IntPtr board);

        /// <summary>
        /// Stops playing LED patterns and clears all pattern configurations
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_led_stop_and_clear(IntPtr board);

        /// <summary>
        /// Initializes memory on the MetaWear board for a NeoPixel strand with a slow operating speed (400 KHz)
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand number</param>
        /// <param name="ordering">Color ordering format</param>
        /// <param name="gpio_pin">GPIO pin the strand</param>
        /// <param name="n_pixels">Number of pixels to allocate memory for</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_init_slow_strand(IntPtr board, byte strand, byte gpio_pin, byte n_pixels, NeoPixel.Ordering ordering);

        /// <summary>
        /// Initializes memory on the MetaWear board for a NeoPixel strand with a fast operating speed (800 KHz)
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand number</param>
        /// <param name="ordering">Color ordering format</param>
        /// <param name="gpio_pin">GPIO pin the strand</param>
        /// <param name="n_pixels">Number of pixels to allocate memory for</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_init_fast_strand(IntPtr board, byte strand, byte gpio_pin, byte n_pixels, NeoPixel.Ordering ordering);

        /// <summary>
        /// Frees NeoPixel resources on the MetaWeard board for a specific strand
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand index to free</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_free_strand(IntPtr board, byte strand);

        /// <summary>
        /// Enables strand holding which will not refresh with any LED changes until the hold is disabled. This let you to form complex LED patterns without having the strand refresh with partial changes.
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand number</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_enable_hold(IntPtr board, byte strand);

        /// <summary>
        /// Disables strand holding.  The strand will be refreshed with any LED changes programmed while the hold was active
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand number</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_disable_hold(IntPtr board, byte strand);

        /// <summary>
        /// Clears pixel states on a strand
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand number to clear</param>
        /// <param name="start">Pixel index to start clearing from</param>
        /// <param name="end">Pixel index to clear to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_clear(IntPtr board, byte strand, byte start, byte end);

        /// <summary>
        /// Sets pixel color
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand number the pixel is on</param>
        /// <param name="pixel">Index of the pixel</param>
        /// <param name="red">Red value</param>
        /// <param name="green">Green value</param>
        /// <param name="blue">Blue value</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_set_color(IntPtr board, byte strand, byte pixel, byte red, byte green, byte blue);

        /// <summary>
        /// Rotates the pixels on a strand
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand to rotate</param>
        /// <param name="direction">Rotation direction</param>
        /// <param name="count">Number of times to repeat the rotation</param>
        /// <param name="period_ms">Amount of time</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_rotate(IntPtr board, byte strand, byte count, ushort period_ms, NeoPixel.Direction direction);

        /// <summary>
        /// Rotates the pixels on a strand indefinitely
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand to rotate</param>
        /// <param name="direction">Rotation direction</param>
        /// <param name="period_ms">Amount of time</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_rotate_indefinitely(IntPtr board, byte strand, byte period_ms, NeoPixel.Direction direction);

        /// <summary>
        /// Stops the pixel rotation
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        /// <param name="strand">Strand to stop LED rotation</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_neopixel_stop_rotation(IntPtr board, byte strand);

        /// <summary>
        /// Create an accumulator whose output is the same size as the input.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_accumulator_create(IntPtr source, Fn_IntPtr processor_created);

        /// <summary>
        /// Create an accumulator with a specific output size.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="output_size">Output size</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_accumulator_create_size(IntPtr source, byte output_size, Fn_IntPtr processor_created);

        /// <summary>
        /// Overwrites the current running sum with a new value
        /// </summary>
        /// <param name="accumulator">Accumulator processor to modify</param>
        /// <param name="new_running_sum">New running sum of the accumulator</param>
        /// <returns> MBL_MW_STATUS_OK if processor state was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_set_accumulator_state(IntPtr accumulator, float new_running_sum);

        /// <summary>
        /// Create an average processor with a specific sample size.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="size">How many data samples to use for computing the running average.  Recommended                              to be a power of 2 for faster computation.</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_average_create(IntPtr source, byte size, Fn_IntPtr processor_created);

        /// <summary>
        /// Resets the running average
        /// </summary>
        /// <param name="average">Average processor to reset</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_average_reset(IntPtr average);

        /// <summary>
        /// Modifies the sample size of the average processor
        /// </summary>
        /// <param name="average">Average processor to modify</param>
        /// <param name="size">New sample size to use</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_average_modify_size(IntPtr average, byte size);

        /// <summary>
        /// Create a buffer processor.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_buffer_create(IntPtr source, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a comparator processor where signed/unsigned is inferred.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="op">Comparison operation to execute</param>
        /// <param name="reference">Reference value to compare the input to</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_comparator_create(IntPtr source, Comparator.Operation op, float reference, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a comparator processor specifically for a signed comparison.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="op">Comparison operation to execute</param>
        /// <param name="reference">Reference value to compare the input to</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_comparator_create_signed(IntPtr source, Comparator.Operation op, float reference, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a comparator processor specifically for an unsigned comparison.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="op">Comparison operation to execute</param>
        /// <param name="reference">Reference value to compare the input to</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_comparator_create_unsigned(IntPtr source, Comparator.Operation op, float reference, Fn_IntPtr processor_created);

        /// <summary>
        /// Modifies the comparator processor, changing the operation and reference value
        /// </summary>
        /// <param name="comparator">Comparator processor to modify</param>
        /// <param name="op">New comparison operation</param>
        /// <param name="reference">New reference value</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_comparator_modify(IntPtr comparator, Comparator.Operation op, float reference);

        /// <summary>
        /// Modifies the comparator processor for a feedback or feedforward loop
        /// </summary>
        /// <param name="comparator">Comparator processor to modify</param>
        /// <param name="op">New comparison operation</param>
        /// <param name="reference_signal">Data signal output to be used for the reference value</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_comparator_modify_signal(IntPtr comparator, Comparator.Operation op, IntPtr reference_signal);

        /// <summary>
        /// Create a multi-value comparator where a signed/unsigned comparison is inferred.  This feature is only available on firmware v1.2.3 and later. A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input</param>
        /// <param name="op">Comparison operation to execute</param>
        /// <param name="mode">Processor output mode</param>
        /// <param name="references">Array of reference values to compare against</param>
        /// <param name="references_length">Number of elements in the references array</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_multi_comparator_create(IntPtr source, Comparator.Operation op, Comparator.Mode mode, ref float references, byte references_length, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a multi-value comparator for signed comparisons.  This feature is only available on firmware v1.2.3 and later.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input</param>
        /// <param name="op">Comparison operation to execute</param>
        /// <param name="mode">Processor output mode</param>
        /// <param name="references">Array of reference values to compare against</param>
        /// <param name="references_length">Number of elements in the references array</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_multi_comparator_create_signed(IntPtr source, Comparator.Operation op, Comparator.Mode mode, ref float references, byte references_length, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a multi-value comparator for unsigned comparisons.  This feature is only available on firmware v1.2.3 and later.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input</param>
        /// <param name="op">Comparison operation to execute</param>
        /// <param name="mode">Processor output mode</param>
        /// <param name="references">Array of reference values to compare against</param>
        /// <param name="references_length">Number of elements in the references array</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_multi_comparator_create_unsigned(IntPtr source, Comparator.Operation op, Comparator.Mode mode, ref float references, byte references_length, Fn_IntPtr processor_created);

        /// <summary>
        /// Modifies the multi-value comparator, changing the operation and reference values.  This feature is only available on firmware v1.2.3 and later.
        /// </summary>
        /// <param name="comparator">Comparator processor to modify</param>
        /// <param name="op">New comparison operation</param>
        /// <param name="references">Array of new reference values</param>
        /// <param name="references_length">Number of elements in the references array</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_multi_comparator_modify(IntPtr comparator, Comparator.Operation op, ref float references, byte references_length);

        /// <summary>
        /// Create a counter with an output size of 1 byte.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Event the processor is counting</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_counter_create(IntPtr source, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a counter with a specific output size.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Event the processor is counting</param>
        /// <param name="size">Output size</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_counter_create_size(IntPtr source, byte size, Fn_IntPtr processor_created);

        /// <summary>
        /// Overwrites the current count with a new value
        /// </summary>
        /// <param name="counter">Counter processor to modify</param>
        /// <param name="new_count">New count value</param>
        /// <returns> MBL_MW_STATUS_OK if processor state was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_counter_set_state(IntPtr counter, uint new_count);

        /// <summary>
        /// Get the data signal representing a processor's internal state.  Processors that have an internal state are: accumulator, buffer, counter, delta, and passthrough.
        /// </summary>
        /// <param name="processor">Processor to access</param>
        /// <returns>Pointer to the data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_dataprocessor_get_state_data_signal(IntPtr processor);

        /// <summary>
        /// Removes a data processor and its consumers from the board
        /// </summary>
        /// <param name="processor">Processor to remove</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_dataprocessor_remove(IntPtr processor);

        /// <summary>
        /// Retrieves the id value identifying the processor
        /// </summary>
        /// <param name="processor">Processor to lookup</param>
        /// <returns>Numerical id of the processor</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte mbl_mw_dataprocessor_get_id(IntPtr processor);

        /// <summary>
        /// Looks up the MblMwDataProcessor object corresponding to the id
        /// </summary>
        /// <param name="board">Board to search on</param>
        /// <param name="id">Numerical id to lookup</param>
        /// <returns>Data processor object identified by the id</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_dataprocessor_lookup_id(IntPtr board, byte id);

        /// <summary>
        /// Create a delta processor.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="mode">Output mode of the processor</param>
        /// <param name="magnitude">Min distance from the reference value to allow the input to pass</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_delta_create(IntPtr source, Delta.Mode mode, float magnitude, Fn_IntPtr processor_created);

        /// <summary>
        /// Sets the reference value of the processor.
        /// </summary>
        /// <param name="delta">Delta processor to modify</param>
        /// <param name="previous_value">Min distance from the reference value to allow the input to pass</param>
        /// <returns>MBL_MW_STATUS_OK if processor state was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_delta_set_reference(IntPtr delta, float previous_value);

        /// <summary>
        /// Modifies the magnitude that allows data through
        /// </summary>
        /// <param name="delta">Delta processor to modify</param>
        /// <param name="magnitude">Min distance from the reference value to allow the input to pass</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_delta_modify_magnitude(IntPtr delta, float magnitude);

        /// <summary>
        /// Create a math processor where signed/unsigned operation is inferred.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="op">Math operation to compute</param>
        /// <param name="rhs">Right hand side of the operation that requires 2 inputs</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_math_create(IntPtr source, Maths.Operation op, float rhs, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a math processor using signed operations.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="op">Math operation to compute</param>
        /// <param name="rhs">Right hand side of the operation that requires 2 inputs</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_math_create_signed(IntPtr source, Maths.Operation op, float rhs, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a math processor using unsigned operations.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="op">Math operation to compute</param>
        /// <param name="rhs">Right hand side of the operation that requires 2 inputs</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_math_create_unsigned(IntPtr source, Maths.Operation op, float rhs, Fn_IntPtr processor_created);

        /// <summary>
        /// Modify the configuration of a math processor, changing the right side value of the operation
        /// </summary>
        /// <param name="math">Math processor to modify</param>
        /// <param name="rhs">New right hand side of the operation</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_math_modify_rhs(IntPtr math, float rhs);

        /// <summary>
        /// Modify the configuration of a math processor for a feedback or feedforward loop
        /// </summary>
        /// <param name="math">Math processor to modify</param>
        /// <param name="rhs_signal">Data signal supplying the rhs value of the operation</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_math_modify_rhs_signal(IntPtr math, IntPtr rhs_signal);

        /// <summary>
        /// Create a passthrough processor.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="mode">Processor</param>
        /// <param name="count">Internal count to initial the processor with</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_passthrough_create(IntPtr source, Passthrough.Mode mode, ushort count, Fn_IntPtr processor_created);

        /// <summary>
        /// Modify the internal count of the passthrough processor
        /// </summary>
        /// <param name="passthrough">Passthrough processor to modify</param>
        /// <param name="new_count">New internal count</param>
        /// <returns>MBL_MW_STATUS_OK if processor state was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_passthrough_set_count(IntPtr passthrough, ushort new_count);

        /// <summary>
        /// Modify the passthrough configuration
        /// </summary>
        /// <param name="passthrough">Passthrough processor to update</param>
        /// <param name="mode">New operation mode to use</param>
        /// <param name="count">New initial count</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_passthrough_modify(IntPtr passthrough, Passthrough.Mode mode, ushort count);

        /// <summary>
        /// Create a pulse detector.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="output">Output type of the processor</param>
        /// <param name="threshold">Value the data must exceed for a valid pulse</param>
        /// <param name="width">Number of samples that must exceed the threshold for a valid pulse</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_pulse_create(IntPtr source, Pulse.Output output, float threshold, ushort width, Fn_IntPtr processor_created);

        /// <summary>
        /// Modify the configuration of a pulse detector
        /// </summary>
        /// <param name="pulse">Pulse detector to modify</param>
        /// <param name="threshold">New threshold of the detector</param>
        /// <param name="width">New width of the detector</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_pulse_modify(IntPtr pulse, float threshold, ushort width);

        /// <summary>
        /// Create an rms processor.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_rms_create(IntPtr source, Fn_IntPtr processor_created);

        /// <summary>
        /// Create an rss processor.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_rss_create(IntPtr source, Fn_IntPtr processor_created);

        /// <summary>
        /// Create a sample delay processor.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="bin_size">Number of samples to hold before letting data through</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_sample_create(IntPtr source, byte bin_size, Fn_IntPtr processor_created);

        /// <summary>
        /// Modify the bin size of a sample delay processor
        /// </summary>
        /// <param name="sample_delay">Sample processor to modify</param>
        /// <param name="bin_size">Number of samples to hold before letting data through</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_sample_modify_bin_size(IntPtr sample_delay, byte bin_size);

        /// <summary>
        /// Create a threshold processor.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="mode">Processor output mode</param>
        /// <param name="boundary">Limit that triggers an event when data crosses it</param>
        /// <param name="hysteresis">Min distance between the limit and value to signal a successful crossing</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_threshold_create(IntPtr source, Threshold.Mode mode, float boundary, float hysteresis, Fn_IntPtr processor_created);

        /// <summary>
        /// Modifies the threshold processor configuration
        /// </summary>
        /// <param name="threshold">Threshold processor to modify</param>
        /// <param name="boundary">Limit that triggers an event when data crosses it</param>
        /// <param name="hysteresis">Min distance between the limit and value to signal a successful crossing</param>
        /// <returns>MBL_MW_STATUS_OK if processor configuration was updated</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_threshold_modify_boundary(IntPtr threshold, float boundary, float hysteresis);

        /// <summary>
        /// Creates a time delay processor.  A pointer representing the processor will be passed back to the user via a callback function.
        /// </summary>
        /// <param name="source">Data signal providing the input for the processor</param>
        /// <param name="mode">Operation mode of the processor</param>
        /// <param name="period">How often to allow data through</param>
        /// <param name="processor_created">Callback function to be executed when the processor is created</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_time_create(IntPtr source, Time.Mode mode, uint period, Fn_IntPtr processor_created);

        /// <summary>
        /// Modify the configuration of the time delay processor
        /// </summary>
        /// <param name="time_delay">Time delay processor to modify</param>
        /// <param name="period">How often to allow data through</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int mbl_mw_dataprocessor_time_modify_period(IntPtr time_delay, uint period);

        /// <summary>
        /// Retrieves the data signal representing acceleration data
        /// </summary>
        /// <param name="board">Board to retrieve the signal from</param>
        /// <returns>Pointer to the acceleration data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_acc_get_acceleration_data_signal(IntPtr board);

        /// <summary>
        /// Retrieves a special data signal representing high frequency (>100Hz) acceleration data.  This signal is only for streaming and cannot use logging nor data processing.  To use those features with an acceleration data signal, use the signal from mbl_mw_acc_get_acceleration_data_signal.
        /// </summary>
        /// <param name="board">Board to retrieve the signal from</param>
        /// <returns>Pointer to a high frequency data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_acc_get_high_freq_acceleration_data_signal(IntPtr board);

        /// <summary>
        /// Sets the output data rate.  If an invalid odr is used, the closest valid value will be used.
        /// </summary>
        /// <param name="board">Board to configure</param>
        /// <param name="odr">Output data rate</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern float mbl_mw_acc_set_odr(IntPtr board, float odr);

        /// <summary>
        /// Sets the full scale range.  IF an invalid range is used, the closet valid value will be used.
        /// </summary>
        /// <param name="board">Board to configure</param>
        /// <param name="range">Sampling range</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern float mbl_mw_acc_set_range(IntPtr board, float range);

        /// <summary>
        /// Writes the acceleration settings to the board
        /// </summary>
        /// <param name="board">Board to configure</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_write_acceleration_config(IntPtr board);

        /// <summary>
        /// Switches the accelerometer to active mode
        /// </summary>
        /// <param name="board">Board the accelerometer is on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_start(IntPtr board);

        /// <summary>
        /// Switches the accelerometer to standby mode
        /// </summary>
        /// <param name="board">Board the accelerometer is on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_stop(IntPtr board);

        /// <summary>
        /// Enables acceleration sampling
        /// </summary>
        /// <param name="board">Board to enable acceleration sampling on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_enable_acceleration_sampling(IntPtr board);

        /// <summary>
        /// Disables acceleration sampling
        /// </summary>
        /// <param name="board">Board to disable acceleration sampling on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_disable_acceleration_sampling(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing acceleration data from a Bosch accelerometer
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_acc_bosch_get_acceleration_data_signal(IntPtr board);

        /// <summary>
        /// Retrieves a special data signal representing high frequency (>100Hz) acceleration data for a Bosch accelerometer.  This signal is only for streaming and cannot use logging nor data processing.  To use those features with an acceleration data signal, use the signal from mbl_mw_acc_bosch_get_acceleration_data_signal.
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to a high frequency data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_acc_bosch_get_high_freq_acceleration_data_signal(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing data from the BMI160 step counter
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_acc_bmi160_get_step_counter_data_signal(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing data from the BMI160 step detector
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_acc_bmi160_get_step_detector_data_signal(IntPtr board);

        /// <summary>
        /// Sets the output data rate for the BMI160 accelerometer
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="odr">Output data rate value to assign</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bmi160_set_odr(IntPtr board, AccelerometerBmi160.OutputDataRate odr);

        /// <summary>
        /// Sets the output data rate for the BMA255 accelerometer
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="odr">Output data rate value to assign</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bma255_set_odr(IntPtr board, AccelerometerBma255.OutputDataRate odr);

        /// <summary>
        /// Sets the acceleration range
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="range">Acceleration range to assign</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bosch_set_range(IntPtr board, AccelerometerBosch.FullScaleRange range);

        /// <summary>
        /// Writes the acceleration settings to the sensor
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bosch_write_acceleration_config(IntPtr board);

        /// <summary>
        /// Sets the operational mode of the step counter
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="mode">New operation mode</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bmi160_set_step_counter_mode(IntPtr board, AccelerometerBmi160.StepCounterMode mode);

        /// <summary>
        /// Enables the BMI160 step counter
        /// </summary>
        /// <param name="board">Board to modify</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bmi160_enable_step_counter(IntPtr board);

        /// <summary>
        /// Disables the BMI160 step counter
        /// </summary>
        /// <param name="board">Board to modify</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bmi160_disable_step_counter(IntPtr board);

        /// <summary>
        /// Writes the step counter configuration to the sensor
        /// </summary>
        /// <param name="board">Board to write to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bmi160_write_step_counter_config(IntPtr board);

        /// <summary>
        /// Resets the BMI160 step counter
        /// </summary>
        /// <param name="board">Board to reset</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bmi160_reset_step_counter(IntPtr board);

        /// <summary>
        /// Enables the BMI160 step detector
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bmi160_enable_step_detector(IntPtr board);

        /// <summary>
        /// Disables the BMI160 step detector
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bmi160_disable_step_detector(IntPtr board);

        /// <summary>
        /// Switches the accelerometer to active mode.  When in active mode, the accelerometer cannot be configured
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bosch_start(IntPtr board);

        /// <summary>
        /// Switches the accelerometer to standby mode
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bosch_stop(IntPtr board);

        /// <summary>
        /// Enables acceleration sampling
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bosch_enable_acceleration_sampling(IntPtr board);

        /// <summary>
        /// Disables acceleration sampling
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_bosch_disable_acceleration_sampling(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing acceleration data for the MMA8452Q accelerometer
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_acc_mma8452q_get_acceleration_data_signal(IntPtr board);

        /// <summary>
        /// Retrieves a special data signal representing high frequency (>100Hz) acceleration data for the MMA8452Q accelerometer.  This signal is only for streaming and cannot use logging nor data processing.  To use those features with an acceleration data signal, use the signal from mbl_mw_acc_mma8452q_get_acceleration_data_signal.
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to a high frequency data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_acc_mma8452q_get_high_freq_acceleration_data_signal(IntPtr board);

        /// <summary>
        /// Sets the output data rate
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="odr">Output data rate value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_mma8452q_set_odr(IntPtr board, AccelerometerMma8452q.OutputDataRate odr);

        /// <summary>
        /// Sets the acceleration range
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="range">Acceleration range value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_mma8452q_set_range(IntPtr board, AccelerometerMma8452q.FullScaleRange range);

        /// <summary>
        /// Writes the acceleration settings to the sensor
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_mma8452q_write_acceleration_config(IntPtr board);

        /// <summary>
        /// Switches the accelerometer to active mode.  When in active mode, the accelerometer cannot be configured
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_mma8452q_start(IntPtr board);

        /// <summary>
        /// Switches the accelerometer to standby mode
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_mma8452q_stop(IntPtr board);

        /// <summary>
        /// Enables acceleration sampling
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_mma8452q_enable_acceleration_sampling(IntPtr board);

        /// <summary>
        /// Disables acceleration sampling
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_acc_mma8452q_disable_acceleration_sampling(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing LTR329 illuminance data
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_als_ltr329_get_illuminance_data_signal(IntPtr board);

        /// <summary>
        /// Sets the sensor gain
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="gain">Sensor gain value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_als_ltr329_set_gain(IntPtr board, AmbientLightLtr329.SensorGain gain);

        /// <summary>
        /// Sets the sensor integration time
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="integration_time">Integration time value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_als_ltr329_set_integration_time(IntPtr board, AmbientLightLtr329.IntegrationTime integration_time);

        /// <summary>
        /// Sets the sensor measurement rate
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="measurement_rate">Measurement rate value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_als_ltr329_set_measurement_rate(IntPtr board, AmbientLightLtr329.MeasurementRate measurement_rate);

        /// <summary>
        /// Writes the configuration to the LTR329 sensor
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_als_ltr329_write_config(IntPtr board);

        /// <summary>
        /// Starts illuminance sampling
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_als_ltr329_start(IntPtr board);

        /// <summary>
        /// Stops illuminance sampling
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_als_ltr329_stop(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing pressure data from a Bosch barometer
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_baro_bosch_get_pressure_data_signal(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing a single read from a Bosch barometer
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_baro_bosch_get_pressure_read_data_signal(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing altitude data from a Bosch barometer
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_baro_bosch_get_altitude_data_signal(IntPtr board);

        /// <summary>
        /// Set the oversampling mode
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="oversampling">Oversampling value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_baro_bosch_set_oversampling(IntPtr board, BarometerBosch.Oversampling oversampling);

        /// <summary>
        /// Set the iir filter coefficient
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="iir_filter">IIR filter value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_baro_bosch_set_iir_filter(IntPtr board, BarometerBosch.InfiniteImpluseFilter iir_filter);

        /// <summary>
        /// Set the standby time for the BMP280 barometer
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="standby_time">Standby time value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_baro_bmp280_set_standby_time(IntPtr board, BarometerBmp280.Standby standby_time);

        /// <summary>
        /// Set the standby time for the BME280 barometer
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="standby_time">Standby time value to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_baro_bme280_set_standby_time(IntPtr board, BarometerBme280.Standby standby_time);

        /// <summary>
        /// Set the standby time.  If an invalid standby time is used, the closest valid value will be chosen
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="standby_time_ms">Standby time value to set</param>
        /// <returns>Selected standby time</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern float mbl_mw_baro_bosch_set_standby_time(IntPtr board, float standby_time_ms);

        /// <summary>
        /// Writes the configuration to the sensor
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_baro_bosch_write_config(IntPtr board);

        /// <summary>
        /// Start pressure and altitude sensing
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_baro_bosch_start(IntPtr board);

        /// <summary>
        /// Stop pressure and altitude sensing
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_baro_bosch_stop(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing detected ADC values for clear, red, green, and blue
        /// </summary>
        /// <param name="board">Board the sensor resides on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_cd_tcs34725_get_adc_data_signal(IntPtr board);

        /// <summary>
        /// Sets the integration time
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="time">New integration time to use</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_cd_tcs34725_set_integration_time(IntPtr board, float time);

        /// <summary>
        /// Sets the analog gain scale
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="gain">New gain scale to use</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_cd_tcs34725_set_gain(IntPtr board, ColorDetectorTcs34725.Gain gain);

        /// <summary>
        /// Enable the illuminator LED
        /// </summary>
        /// <param name="board">Board to modify</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_cd_tcs34725_enable_illuminator_led(IntPtr board);

        /// <summary>
        /// Disable the illuminator LED
        /// </summary>
        /// <param name="board">Board to modify</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_cd_tcs34725_disable_illuminator_led(IntPtr board);

        /// <summary>
        /// Writes the configuration to the sensor
        /// </summary>
        /// <param name="board">Board the sensor resides on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_cd_tcs34725_write_config(IntPtr board);

        /// <summary>
        /// Retrieves a data signal representing analog data
        /// </summary>
        /// <param name="board">Board to receive data from</param>
        /// <param name="pin">GPIO pin to read</param>
        /// <param name="mode">Read mode to use</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_gpio_get_analog_input_data_signal(IntPtr board, byte pin, Gpio.ReadMode mode);

        /// <summary>
        /// Retrieves a data signal representing digital data
        /// </summary>
        /// <param name="board">Board to receive data from</param>
        /// <param name="pin">GPIO pin to read</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_gpio_get_digital_input_data_signal(IntPtr board, byte pin);

        /// <summary>
        /// Retrieves a data signal representing changes in digital data
        /// </summary>
        /// <param name="board">Board to receive data from</param>
        /// <param name="pin">GPIO pin to monitor</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_gpio_get_pin_monitor_data_signal(IntPtr board, byte pin);

        /// <summary>
        /// Sets the pin pull mode
        /// </summary>
        /// <param name="board">Board the pin is on</param>
        /// <param name="pin">GPIO pin to modify</param>
        /// <param name="mode">New pull mode</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gpio_set_pull_mode(IntPtr board, byte pin, Gpio.PullMode mode);

        /// <summary>
        /// Sets the digital output state
        /// </summary>
        /// <param name="board">Board the pin is on</param>
        /// <param name="pin">GPIO pin to set</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gpio_set_digital_output(IntPtr board, byte pin);

        /// <summary>
        /// Clears the digital output state
        /// </summary>
        /// <param name="board">Board the pin is on</param>
        /// <param name="pin">GPIO pin to clear</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gpio_clear_digital_output(IntPtr board, byte pin);

        /// <summary>
        /// Sets the pin change type to monitor
        /// </summary>
        /// <param name="board">Board the pin is on</param>
        /// <param name="pin">GPIO pin to set</param>
        /// <param name="type">Change type to monitor</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gpio_set_pin_change_type(IntPtr board, byte pin, Gpio.PinChangeType type);

        /// <summary>
        /// Start pin monitoring
        /// </summary>
        /// <param name="board">Board the pin is on</param>
        /// <param name="pin">GPIO pin to monitor</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gpio_start_pin_monitoring(IntPtr board, byte pin);

        /// <summary>
        /// Stop pin monitoring
        /// </summary>
        /// <param name="board">Board the pin is on</param>
        /// <param name="pin">GPIO pin to stop monitoring</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gpio_stop_pin_monitoring(IntPtr board, byte pin);

        /// <summary>
        /// Retrieves the data signal representing BMI160 rotation data
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the board</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_gyro_bmi160_get_rotation_data_signal(IntPtr board);

        /// <summary>
        /// Retrieves a special data signal representing high frequency (>100Hz) rotation data for the BMI160 IMU.  This signal is only for streaming and cannot use logging nor data processing.  To use those features with a rotation data signal, use the signal from mbl_mw_gyro_bmi160_get_rotation_data_signal.
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to a high frequency data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal(IntPtr board);

        /// <summary>
        /// Sets the output data rate
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="odr">Output data rate value to assign</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gyro_bmi160_set_odr(IntPtr board, GyroBmi160.OutputDataRate odr);

        /// <summary>
        /// Sets the rotation range
        /// </summary>
        /// <param name="board">Pointer to the board to modify</param>
        /// <param name="range">New rotation range</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gyro_bmi160_set_range(IntPtr board, GyroBmi160.FullScaleRange range);

        /// <summary>
        /// Writes the configuration to the sendor
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gyro_bmi160_write_config(IntPtr board);

        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gyro_bmi160_start(IntPtr board);

        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gyro_bmi160_stop(IntPtr board);

        /// <summary>
        /// Enables rotation sampling
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gyro_bmi160_enable_rotation_sampling(IntPtr board);

        /// <summary>
        /// Disables rotation sampling
        /// </summary>
        /// <param name="board">Pointer to the board to send the command to</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_gyro_bmi160_disable_rotation_sampling(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing humidity data
        /// </summary>
        /// <param name="board">Board the humidity sensor resides on</param>
        /// <returns>Pointer to the data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_humidity_bme280_get_percentage_data_signal(IntPtr board);

        /// <summary>
        /// Sets the oversampling mode
        /// </summary>
        /// <param name="board">Board the humidity sensor resides on</param>
        /// <param name="oversampling">New oversampling mode</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_humidity_bme280_set_oversampling(IntPtr board, HumidityBme280.Oversampling oversampling);

        /// <summary>
        /// Retrieves the data signal representing i2c data.  The data signal is identified by the id value and if the id is not present, a new data signal will be created using the length parameter.
        /// </summary>
        /// <param name="board">Board the i2c bus resides on</param>
        /// <param name="length">Number of bytes to read</param>
        /// <param name="id">Numerical value identifying the data</param>
        /// <returns>Pointer to the i2c data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_i2c_get_data_signal(IntPtr board, byte length, byte id);

        /// <summary>
        /// Writes data via the i2c bus
        /// </summary>
        /// <param name="board">Board the i2c bus resides on</param>
        /// <param name="device_addr">Device to write to</param>
        /// <param name="register_addr">Address of the register to write</param>
        /// <param name="value">Payload</param>
        /// <param name="length">Number of bytes</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_i2c_write(IntPtr board, byte device_addr, byte register_addr, byte[] value, byte length);

        /// <summary>
        /// Retrieves the data signal representing B field strength
        /// </summary>
        /// <param name="board">Board the magnetometer resides on</param>
        /// <returns>Pointer to the data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_mag_bmm150_get_b_field_data_signal(IntPtr board);

        /// <summary>
        /// Sets the power mode to one of the recommended presets
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="preset">New preset power mode to use</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_mag_bmm150_set_power_preset(IntPtr board, MagnetometerBmm150.PowerPreset preset);

        /// <summary>
        /// Enable B field sampling
        /// </summary>
        /// <param name="board">Board the magnetometer resides on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_mag_bmm150_enable_b_field_sampling(IntPtr board);

        /// <summary>
        /// Disable B field sampling
        /// </summary>
        /// <param name="board">Board the magnetometer resides on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_mag_bmm150_disable_b_field_sampling(IntPtr board);

        /// <summary>
        /// Switches the magnetometer into normal mode
        /// </summary>
        /// <param name="board">Board the magnetometer resides on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_mag_bmm150_start(IntPtr board);

        /// <summary>
        /// Switches the magnetometer into sleep mode
        /// </summary>
        /// <param name="board">Board the magnetometer resides on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_mag_bmm150_stop(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing a temperature source
        /// </summary>
        /// <param name="board">Board to retrieve the signal from</param>
        /// <param name="channel">Channel ID of the temperature source</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_multi_chnl_temp_get_temperature_data_signal(IntPtr board, byte channel);

        /// <summary>
        /// Configure the external thermistor
        /// </summary>
        /// <param name="board">Board the external thermistor is attached to</param>
        /// <param name="channel">Channel ID of the external thermistor</param>
        /// <param name="data_pin">GPIO pin reading the data</param>
        /// <param name="pulldown_pin">GPIO pin the pulldown resistor is connected to</param>
        /// <param name="active_high">Zero if the pulldown pin is not active high</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_multi_chnl_temp_configure_ext_thermistor(IntPtr board, byte channel, byte data_pin, byte pulldown_pin, byte active_high);

        /// <summary>
        /// Retrieve the temperature source type corresponding to a channel ID
        /// </summary>
        /// <param name="board">Board to lookup the temperature source on</param>
        /// <param name="channel">Channel ID to lookup</param>
        /// <returns>Source type of the channel ID</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern MultiChannelTemperature.Source mbl_mw_multi_chnl_temp_get_source(IntPtr board, byte channel);

        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte mbl_mw_multi_chnl_temp_get_num_channels(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing ADC values for the light's colors
        /// </summary>
        /// <param name="board">Board the sensor resides on</param>
        /// <returns>Pointer to the data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_proximity_tsl2671_get_adc_data_signal(IntPtr board);

        /// <summary>
        /// Sets the integration time
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="time">New integration time to use</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_proximity_tsl2671_set_integration_time(IntPtr board, float time);

        /// <summary>
        /// Sets the pulse count.  Sensitivity increase by the sqrt of pulse count.
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="n_pulses">Number of pulses to use for proximity detection</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_proximity_tsl2671_set_n_pulses(IntPtr board, byte n_pulses);

        /// <summary>
        /// Sets the photodiode that responds to light
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="channel">New receiver channel to use</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_proximity_tsl2671_set_receiver_channel(IntPtr board, ProximityTsl2671.RecieverChannel channel);

        /// <summary>
        /// Sets the current driving the light transmitter.  For boards powered by the CR2032 battery, it is recommended that the current be 25mA or less.
        /// </summary>
        /// <param name="board">Board to modify</param>
        /// <param name="current">New driver current to use</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_proximity_tsl2671_set_transmitter_current(IntPtr board, ProximityTsl2671.TransmitterCurrent current);

        /// <summary>
        /// Writes the configuration to the sensor
        /// </summary>
        /// <param name="board">Board the sensor resides on</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_proximity_tsl2671_write_config(IntPtr board);

        /// <summary>
        /// Retrieves the data signal representing spi data.  The data signal is identified by the id value and if the id is not present, a new data signal will be created using the length parameter.
        /// </summary>
        /// <param name="board">Board to communicate with</param>
        /// <param name="length">Number of bytes to read</param>
        /// <param name="id">Numerical id identifying the data</param>
        /// <returns>Pointer to the spi data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_spi_get_data_signal(IntPtr board, byte length, byte id);

        /// <summary>
        /// Writes data via the spi bus
        /// </summary>
        /// <param name="board">Board to communicate with</param>
        /// <param name="parameters">Parameters configuring the read</param>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mbl_mw_spi_write(IntPtr board, IntPtr parameters);

        /// <summary>
        /// Retrieves the data signal representing switch state data
        /// </summary>
        /// <param name="board">Pointer to the board to retrieve the signal from</param>
        /// <returns>Pointer to the switch data signal</returns>
        [DllImport(METAWEAR_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr mbl_mw_switch_get_state_data_signal(IntPtr board);
    };
}
