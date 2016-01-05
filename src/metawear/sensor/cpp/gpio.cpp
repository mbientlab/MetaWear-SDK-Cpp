#include "metawear/sensor/gpio.h"
#include "gpio_register.h"

#include "metawear/core/cpp/connection_def.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

using std::forward_as_tuple;
using std::piecewise_construct;

static inline void insert_gpio_data_signal(MblMwMetaWearBoard* board, ResponseHeader& header, uint8_t data_len, bool read) {
    if (read) {
        header.register_id = READ_REGISTER(header.register_id);
    }

    if (board->sensor_data_signals.count(header) == 0) {
        MblMwDataSignal *gpio_signal= new MblMwDataSignal(header, board, ResponseConvertor::UINT32, 1, data_len, 0, 0);
        board->sensor_data_signals[header]= gpio_signal;
    }
}

MblMwDataSignal* mbl_mw_gpio_get_analog_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioAnalogReadMode mode) {
    uint8_t analogReadRegister = -1;

    switch (mode) {
    case MBL_MW_GPIO_ANALOG_READ_MODE_ABS_REF:
        analogReadRegister = ORDINAL(GpioRegister::READ_AI_ABS_REF);
        break;
    case MBL_MW_GPIO_ANALOG_READ_MODE_ADC:
        analogReadRegister = ORDINAL(GpioRegister::READ_AI_ADC);
        break;
    }

    ResponseHeader header(MBL_MW_MODULE_GPIO, analogReadRegister, pin);

    insert_gpio_data_signal(board, header, 2, true);
    return board->sensor_data_signals.at(header);
}

MblMwDataSignal* mbl_mw_gpio_get_digital_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin) {
    ResponseHeader header(MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::READ_DI), pin);

    insert_gpio_data_signal(board, header, 1, true);
    return board->sensor_data_signals.at(header);
}

MblMwDataSignal* mbl_mw_gpio_get_pin_monitor_data_signal(MblMwMetaWearBoard* board, uint8_t pin) {
    ResponseHeader header(MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY), pin);

    insert_gpio_data_signal(board, header, 1, false);
    return board->sensor_data_signals.at(header);
}

void mbl_mw_gpio_read_analog_input(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioAnalogReadMode mode) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, 0, pin};

    switch(mode) {
        case MBL_MW_GPIO_ANALOG_READ_MODE_ABS_REF:
            command[1]= READ_REGISTER(ORDINAL(GpioRegister::READ_AI_ABS_REF));
            send_command_wrapper(board, command, sizeof(command));
            break;
        case MBL_MW_GPIO_ANALOG_READ_MODE_ADC:
            command[1]= READ_REGISTER(ORDINAL(GpioRegister::READ_AI_ADC));
            send_command_wrapper(board, command, sizeof(command));
            break;
    }
}

void mbl_mw_gpio_set_pull_mode(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPullMode mode) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, 0, pin};

    switch(mode) {
        case MBL_MW_GPIO_PULL_MODE_UP:
            command[1]= ORDINAL(GpioRegister::PULL_UP_DI);
            send_command_wrapper(board, command, sizeof(command));
            break;
        case MBL_MW_GPIO_PULL_MODE_DOWN:
            command[1]= ORDINAL(GpioRegister::PULL_DOWN_DI);
            send_command_wrapper(board, command, sizeof(command));
            break;
        case MBL_MW_GPIO_PULL_MODE_NONE:
            command[1]= ORDINAL(GpioRegister::NO_PULL_DI);
            send_command_wrapper(board, command, sizeof(command));
            break;
    }
}

void mbl_mw_gpio_read_digital_input(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_DI)), pin};
    send_command_wrapper(board, command, sizeof(command));
}

void mbl_mw_gpio_set_digital_output(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::SET_DO), pin};
    send_command_wrapper(board, command, sizeof(command));
}

void mbl_mw_gpio_clear_digital_output(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[3]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::CLEAR_DO), pin};
    send_command_wrapper(board, command, sizeof(command));
}

void mbl_mw_gpio_set_pin_change_type(const MblMwMetaWearBoard* board, uint8_t pin, MblMwGpioPinChangeType type) {
    uint8_t command[4]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE), pin, (uint8_t) type};
    send_command_wrapper(board, command, sizeof(command));
}

void mbl_mw_gpio_start_pin_monitoring(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[4]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY_ENABLE), pin, 1};
    send_command_wrapper(board, command, sizeof(command));
}

void mbl_mw_gpio_stop_pin_monitoring(const MblMwMetaWearBoard* board, uint8_t pin) {
    uint8_t command[4]= {MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY_ENABLE), pin, 0};
    send_command_wrapper(board, command, sizeof(command));
}
