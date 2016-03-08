#include "metawear/sensor/gpio.h"
#include "gpio_register.h"

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

using std::forward_as_tuple;
using std::piecewise_construct;

static inline MblMwDataSignal* insert_gpio_data_signal(MblMwMetaWearBoard* board, GpioRegister gpioRegister, uint8_t pin, uint8_t data_len) {
    ResponseHeader header(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(gpioRegister)), pin);

    if (board->sensor_data_signals.count(header) == 0) {
        MblMwDataSignal *gpio_signal= new MblMwDataSignal(header, board, DataInterpreter::UINT32, 1, data_len, 0, 0);
        board->sensor_data_signals[header]= gpio_signal;
    }
    return board->sensor_data_signals.at(header);
}

static void gpio_pin_monitor_subscribe(MblMwDataSignal *source) {
    uint8_t enable_notify[3]= { source->header.module_id, source->header.register_id, 1};
    send_command(source->owner, enable_notify, sizeof(enable_notify));
}

static void gpio_pin_monitor_unsubscribe(MblMwDataSignal *source) {
}

MblMwDataSignal* mbl_mw_gpio_get_analog_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioAnalogReadMode mode) {
    GpioRegister analogReadRegister;

    switch (mode) {
    case MBL_MW_GPIO_ANALOG_READ_MODE_ABS_REF:
        analogReadRegister = GpioRegister::READ_AI_ABS_REF;
        break;
    case MBL_MW_GPIO_ANALOG_READ_MODE_ADC:
        analogReadRegister = GpioRegister::READ_AI_ADC;
        break;
    default:
        return nullptr;
    }

    return insert_gpio_data_signal(board, analogReadRegister, pin, 2);
}

MblMwDataSignal* mbl_mw_gpio_get_digital_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin) {
    return insert_gpio_data_signal(board, GpioRegister::READ_DI, pin, 1);
}

MblMwDataSignal* mbl_mw_gpio_get_pin_monitor_data_signal(MblMwMetaWearBoard* board, uint8_t pin) {
    ResponseHeader header(MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY), pin);

    if (board->sensor_data_signals.count(header) == 0) {
        MblMwDataSignal *gpio_signal= new MblMwDataSignal(header, board, DataInterpreter::UINT32, 1, 1, 0, 0, number_to_firmware_default, 
                gpio_pin_monitor_subscribe, gpio_pin_monitor_unsubscribe);
        board->sensor_data_signals[header]= gpio_signal;
    }

    return board->sensor_data_signals.at(header);
}

void mbl_mw_gpio_read_analog_input(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioAnalogReadMode mode) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, 0, pin};

    switch(mode) {
        case MBL_MW_GPIO_ANALOG_READ_MODE_ABS_REF:
            command[1]= READ_REGISTER(ORDINAL(GpioRegister::READ_AI_ABS_REF));
            send_command(board, command, sizeof(command));
            break;
        case MBL_MW_GPIO_ANALOG_READ_MODE_ADC:
            command[1]= READ_REGISTER(ORDINAL(GpioRegister::READ_AI_ADC));
            send_command(board, command, sizeof(command));
            break;
    }
}

void mbl_mw_gpio_set_pull_mode(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPullMode mode) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, 0, pin};

    switch(mode) {
        case MBL_MW_GPIO_PULL_MODE_UP:
            command[1]= ORDINAL(GpioRegister::PULL_UP_DI);
            send_command(board, command, sizeof(command));
            break;
        case MBL_MW_GPIO_PULL_MODE_DOWN:
            command[1]= ORDINAL(GpioRegister::PULL_DOWN_DI);
            send_command(board, command, sizeof(command));
            break;
        case MBL_MW_GPIO_PULL_MODE_NONE:
            command[1]= ORDINAL(GpioRegister::NO_PULL_DI);
            send_command(board, command, sizeof(command));
            break;
    }
}

void mbl_mw_gpio_read_digital_input(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_DI)), pin};
    send_command(board, command, sizeof(command));
}

void mbl_mw_gpio_set_digital_output(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::SET_DO), pin};
    send_command(board, command, sizeof(command));
}

void mbl_mw_gpio_clear_digital_output(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::CLEAR_DO), pin};
    send_command(board, command, sizeof(command));
}

void mbl_mw_gpio_set_pin_change_type(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPinChangeType type) {
    uint8_t command[4]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE), pin, (uint8_t) type};
    send_command(board, command, sizeof(command));
}

void mbl_mw_gpio_start_pin_monitoring(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[4]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY_ENABLE), pin, 1};
    send_command(board, command, sizeof(command));
}

void mbl_mw_gpio_stop_pin_monitoring(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[4]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY_ENABLE), pin, 0};
    send_command(board, command, sizeof(command));
}
