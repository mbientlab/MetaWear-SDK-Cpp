/**
 * @copyright MbientLab License
 * @file gpio.h
 * @brief Interacts with the general purpose I/O pins on the board
 */
#pragma once

#include <stdint.h>

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"
#include "metawear/core/metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

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
 * Read analog input voltage
 * @param board     Board the pin is on
 * @param pin       GPIO pin to read
 * @param mode      Analog read mode
 */
METAWEAR_API void mbl_mw_gpio_read_analog_input(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioAnalogReadMode mode);

/**
 * Sets the pin pull mode
 * @param board     Board the pin is on
 * @param pin       GPIO pin to modify
 * @param mode      New pull mode
 */
METAWEAR_API void mbl_mw_gpio_set_pull_mode(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPullMode mode);
/**
 * Read digtal input state
 * @param board     Board the pin is on
 * @param pin       GPIO pin to read
 */
METAWEAR_API void mbl_mw_gpio_read_digital_input(const MblMwMetaWearBoard* board, uint8_t pin);
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
