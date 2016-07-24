#include <stdexcept>

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"

#include "metawear/sensor/multichanneltemperature.h"
#include "multichanneltemperature_private.h"
#include "multichanneltemperature_register.h"

using std::forward_as_tuple;
using std::out_of_range;
using std::piecewise_construct;

void init_multichannel_temp_module(MblMwMetaWearBoard *board) {
    for(uint8_t channel = 0; channel < (uint8_t) board->module_info.at(MBL_MW_MODULE_TEMPERATURE).extra.size(); channel++) {
        ResponseHeader header(MBL_MW_MODULE_TEMPERATURE, READ_REGISTER(ORDINAL(MultiChannelTempRegister::TEMPERATURE)), channel);
        if (!board->module_events.count(header)) {
             board->module_events[header] = new MblMwDataSignal(header, board, DataInterpreter::TEMPERATURE, 
                     FirmwareConverter::TEMPERATURE, 1, 2, 1, 0);
        }
    }

    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_TEMPERATURE, READ_REGISTER(ORDINAL(MultiChannelTempRegister::TEMPERATURE))),
        forward_as_tuple(response_handler_data_with_id));
}

MblMwDataSignal* mbl_mw_multi_chnl_temp_get_temperature_data_signal(const MblMwMetaWearBoard *board, uint8_t channel) {
    ResponseHeader header(MBL_MW_MODULE_TEMPERATURE, READ_REGISTER(ORDINAL(MultiChannelTempRegister::TEMPERATURE)), channel);
    return board->module_events.count(header) ? dynamic_cast<MblMwDataSignal*>(board->module_events.at(header)) : nullptr;
}

void mbl_mw_multi_chnl_temp_configure_ext_thermistor(const MblMwMetaWearBoard *board, uint8_t channel, uint8_t data_pin, 
        uint8_t pulldown_pin, uint8_t active_high) {
    uint8_t command[6]= {MBL_MW_MODULE_TEMPERATURE, ORDINAL(MultiChannelTempRegister::MODE), channel, data_pin, pulldown_pin, active_high};
    SEND_COMMAND;
}

MblMwTemperatureSource mbl_mw_multi_chnl_temp_get_source(const MblMwMetaWearBoard *board, uint8_t channel) {
    try {
        return (MblMwTemperatureSource) board->module_info.at(MBL_MW_MODULE_TEMPERATURE).extra.at(channel);
    } catch (out_of_range) {
        return MBL_MW_TEMP_SOURCE_INVALID;
    }
}

uint8_t mbl_mw_multi_chnl_temp_get_num_channels(const MblMwMetaWearBoard *board) {
    return (uint8_t) board->module_info.at(MBL_MW_MODULE_TEMPERATURE).extra.size();
}
