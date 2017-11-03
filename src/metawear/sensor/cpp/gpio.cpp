#include "metawear/sensor/gpio.h"
#include "gpio_private.h"
#include "gpio_register.h"
#include "utils.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <cstring>

using std::forward_as_tuple;
using std::memcpy;
using std::piecewise_construct;
using std::stringstream;

static MblMwGpioAnalogReadParameters default_read_parameters= {0xff, 0xff, 0xff, 0x00};
const uint8_t ENHANCED_ANALOG_REVISION= 2;

MblMwGpioPinNotifySignal::MblMwGpioPinNotifySignal(ResponseHeader header, MblMwMetaWearBoard* owner) : 
        MblMwDataSignal(header, owner, DataInterpreter::UINT32, 1, 1, 0, 0) {
}

MblMwGpioPinNotifySignal::MblMwGpioPinNotifySignal(uint8_t** state_stream, MblMwMetaWearBoard *owner) : MblMwDataSignal(state_stream, owner) {
}

void MblMwGpioPinNotifySignal::unsubscribe() {

}

MblMwGpioAnalogSignal::MblMwGpioAnalogSignal(ResponseHeader header, MblMwMetaWearBoard* owner) : 
        MblMwDataSignal(header, owner, DataInterpreter::UINT32, 1, 2, 0, 0) {
}

MblMwGpioAnalogSignal::MblMwGpioAnalogSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner) : MblMwDataSignal(state_stream, owner) {
}

void MblMwGpioAnalogSignal::read() const {
    if (owner->module_info.at(MBL_MW_MODULE_GPIO).revision < ENHANCED_ANALOG_REVISION) {
        MblMwDataSignal::read();
    } else {
        read(&default_read_parameters);
    }
}

void MblMwGpioAnalogSignal::read(const void* parameters) const {
    if (owner->module_info.at(MBL_MW_MODULE_GPIO).revision < ENHANCED_ANALOG_REVISION) {
        MblMwDataSignal::read();
    } else {
        uint8_t command[7]= { MBL_MW_MODULE_GPIO, header.register_id, header.data_id };
        const MblMwGpioAnalogReadParameters* read_params= (const MblMwGpioAnalogReadParameters*) parameters;

        command[3]= read_params->pullup_pin;
        command[4]= read_params->pulldown_pin;
        command[5]= read_params->delay_us >> 2;
        command[6]= read_params->virtual_pin;
        SEND_COMMAND_BOARD(owner);
    }
}

void init_gpio_module(MblMwMetaWearBoard *board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_AI_ABS_REF))),
        forward_as_tuple(response_handler_data_with_id));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_AI_ADC))),
        forward_as_tuple(response_handler_data_with_id));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_DI))),
        forward_as_tuple(response_handler_data_with_id));
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY)),
        forward_as_tuple(response_handler_data_with_id));
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

    ResponseHeader header(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(analogReadRegister)), pin);
    if (!board->module_events.count(header)) {
        board->module_events[header]= new MblMwGpioAnalogSignal(header, board);
    }

    return dynamic_cast<MblMwDataSignal*>(board->module_events.at(header));
}

MblMwDataSignal* mbl_mw_gpio_get_digital_input_data_signal(MblMwMetaWearBoard* board, uint8_t pin) {
    ResponseHeader header(MBL_MW_MODULE_GPIO, READ_REGISTER(ORDINAL(GpioRegister::READ_DI)), pin);

    if (!board->module_events.count(header)) {
        board->module_events[header]= new MblMwDataSignal(header, board, DataInterpreter::UINT32, 1, 1, 0, 0);
    }
    return dynamic_cast<MblMwDataSignal*>(board->module_events.at(header));
}

MblMwDataSignal* mbl_mw_gpio_get_pin_monitor_data_signal(MblMwMetaWearBoard* board, uint8_t pin) {
    ResponseHeader header(MBL_MW_MODULE_GPIO, ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY), pin);

    if (board->module_events.count(header) == 0) {
        board->module_events[header]= new MblMwGpioPinNotifySignal(header, board);
    }

    return dynamic_cast<MblMwDataSignal*>(board->module_events.at(header));
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

void create_gpio_uri(const MblMwDataSignal* signal, stringstream& uri) {
    switch(CLEAR_READ(signal->header.register_id)) {
    case ORDINAL(GpioRegister::READ_AI_ABS_REF):
        uri << "abs-ref[" << (int) signal->header.data_id << "]";
        break;
    case ORDINAL(GpioRegister::READ_AI_ADC):
        uri << "adc[" << (int) signal->header.data_id << "]";
        break;
    case ORDINAL(GpioRegister::READ_DI):
        uri << "digital[" << (int) signal->header.data_id << "]";
        break;
    case ORDINAL(GpioRegister::PIN_CHANGE_NOTIFY):
        uri << "pin-monitor[" << (int) signal->header.data_id << "]";
        break;
    }
}
