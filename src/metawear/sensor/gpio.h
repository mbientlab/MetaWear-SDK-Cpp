/**
 * @copyright MbientLab License
 * @file gpio.h
 * @brief Interacts with the general purpose I/O pins on the board
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/** Pin value indicating the pin setting is not used */
const uint8_t MBL_MW_GPIO_UNUSED_PIN = 0xff;

/**
 * Pin configuration types
 */
typedef enum {
    MBL_MW_GPIO_PULL_MODE_UP= 0,
    MBL_MW_GPIO_PULL_MODE_DOWN,
    MBL_MW_GPIO_PULL_MODE_NONE
} MblMwGpioPullMode;

/**
 * Read modes for analog input
 */
typedef enum {
    MBL_MW_GPIO_ANALOG_READ_MODE_ABS_REF= 0,        ///< Read input voltage as an absolute reference
    MBL_MW_GPIO_ANALOG_READ_MODE_ADC                ///< Read input voltage as a supply ratio
} MblMwGpioAnalogReadMode;

/**
 * Pin change types
 */
typedef enum {
    MBL_MW_GPIO_PIN_CHANGE_TYPE_RISING= 1,          ///< Notify on rising edge of a change
    MBL_MW_GPIO_PIN_CHANGE_TYPE_FALLING,            ///< Notify on falling edge of a change
    MBL_MW_GPIO_PIN_CHANGE_TYPE_ANY                 ///< Notify on any edge of a change
} MblMwGpioPinChangeType;

/**
 * Additional parameters required for an enhanced analog read.
 * This struct is to be used with mbl_mw_datasignal_read_with_parameters
 * @author Eric Tsai
 */
typedef struct {
    uint8_t pullup_pin;                 ///< GPIO pin to be pulled up before the read, set to MBL_MW_GPIO_UNUSED_PIN if not used
    uint8_t pulldown_pin;               ///< GPIO pin to be pulled down before the read, set to MBL_MW_GPIO_UNUSED_PIN if not used
    uint8_t virtual_pin;                ///< GPIO pin the data identifies as, must match pin value for mbl_mw_gpio_get_analog_input_data_signal if used or set to MBL_MW_GPIO_UNUSED_PIN if not used
    uint16_t delay_us;                  ///< How long to wait before reading from the pin, between [0, 1020]us, set to 0 if not used
} MblMwGpioAnalogReadParameters;

/**
 * Retrieves a data signal representing analog data
 * @param board     Board to receive data from
 * @param pin       GPIO pin to read
 * @param mode      Read mode to use
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gpio_get_analog_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioAnalogReadMode mode);
/**
 * Retrieves a data signal representing digital data
 * @param board     Board to receive data from
 * @param pin       GPIO pin to read
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gpio_get_digital_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin);
/**
 * Retrieves a data signal representing changes in digital data
 * @param board     Board to receive data from
 * @param pin       GPIO pin to monitor
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gpio_get_pin_monitor_data_signal(MblMwMetaWearBoard* board, uint8_t pin);

/**
 * Sets the pin pull mode
 * @param board     Board the pin is on
 * @param pin       GPIO pin to modify
 * @param mode      New pull mode
 */
METAWEAR_API void mbl_mw_gpio_set_pull_mode(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPullMode mode);
/**
 * Sets the digital output state
 * @param board     Board the pin is on
 * @param pin       GPIO pin to set
 */
METAWEAR_API void mbl_mw_gpio_set_digital_output(const MblMwMetaWearBoard* board, uint8_t pin);
/**
 * Clears the digital output state
 * @param board     Board the pin is on
 * @param pin       GPIO pin to clear
 */
METAWEAR_API void mbl_mw_gpio_clear_digital_output(const MblMwMetaWearBoard* board, uint8_t pin);

/**
 * Sets the pin change type to monitor
 * @param board     Board the pin is on
 * @param pin       GPIO pin to set
 * @param type      Change type to monitor
 */
METAWEAR_API void mbl_mw_gpio_set_pin_change_type(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPinChangeType type);
/**
 * Start pin monitoring
 * @param board     Board the pin is on
 * @param pin       GPIO pin to monitor
 */
METAWEAR_API void mbl_mw_gpio_start_pin_monitoring(const MblMwMetaWearBoard* board, uint8_t pin);
/**
 * Stop pin monitoring
 * @param board     Board the pin is on
 * @param pin       GPIO pin to stop monitoring
 */
METAWEAR_API void mbl_mw_gpio_stop_pin_monitoring(const MblMwMetaWearBoard* board, uint8_t pin);

#ifdef	__cplusplus
}
#endif
