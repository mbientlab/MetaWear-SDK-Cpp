#pragma once

#include <stdint.h>

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"
#include "metawear/core/metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_GPIO_PULL_MODE_UP= 0,
    MBL_MW_GPIO_PULL_MODE_DOWN,
    MBL_MW_GPIO_PULL_MODE_NONE
} MblMwGpioPullMode;

typedef enum {
    MBL_MW_GPIO_ANALOG_READ_MODE_ABS_REF= 0,
    MBL_MW_GPIO_ANALOG_READ_MODE_ADC
} MblMwGpioAnalogReadMode;

typedef enum {
    MBL_MW_GPIO_PIN_CHANGE_TYPE_RISING= 1,
    MBL_MW_GPIO_PIN_CHANGE_TYPE_FALLING,
    MBL_MW_GPIO_PIN_CHANGE_TYPE_ANY
} MblMwGpioPinChangeType;

METAWEAR_API MblMwDataSignal* mbl_mw_gpio_get_analog_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioAnalogReadMode mode);
METAWEAR_API MblMwDataSignal* mbl_mw_gpio_get_digital_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin);
METAWEAR_API MblMwDataSignal* mbl_mw_gpio_get_pin_monitor_data_signal(MblMwMetaWearBoard* board, uint8_t pin);

METAWEAR_API void mbl_mw_gpio_read_analog_input(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioAnalogReadMode mode);

METAWEAR_API void mbl_mw_gpio_set_pull_mode(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPullMode mode);

METAWEAR_API void mbl_mw_gpio_read_digital_input(const MblMwMetaWearBoard* board, uint8_t pin);

METAWEAR_API void mbl_mw_gpio_set_digital_output(const MblMwMetaWearBoard* board, uint8_t pin);
METAWEAR_API void mbl_mw_gpio_clear_digital_output(const MblMwMetaWearBoard* board, uint8_t pin);

METAWEAR_API void mbl_mw_gpio_set_pin_change_type(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPinChangeType type);

METAWEAR_API void mbl_mw_gpio_start_pin_monitoring(const MblMwMetaWearBoard* board, uint8_t pin);
METAWEAR_API void mbl_mw_gpio_stop_pin_monitoring(const MblMwMetaWearBoard* board, uint8_t pin);

#ifdef	__cplusplus
}
#endif
