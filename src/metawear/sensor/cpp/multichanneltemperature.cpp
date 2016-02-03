#include <stdexcept>

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "metawear/sensor/multichanneltemperature.h"
#include "multichanneltemperature_register.h"

using std::forward_as_tuple;
using std::out_of_range;
using std::piecewise_construct;

static float temp_to_firmware(MblMwDataSignal* source, float value) {
    return (value * 8.f);
}

void init_multichannel_temp_module(MblMwMetaWearBoard *board) {
    for (auto extra_byte : board->module_info.at(MBL_MW_MODULE_TEMPERATURE).extra) {
        ResponseHeader header(MBL_MW_MODULE_TEMPERATURE, ORDINAL(MultiChannelTempRegister::TEMPERATURE), extra_byte);
        header.register_id = READ_REGISTER(header.register_id);

        MblMwDataSignal *temp_signal= new MblMwDataSignal(header, board, DataInterpreter::TEMPERATURE, 1, 2, 1, 0);
        temp_signal->number_to_firmware= temp_to_firmware;
        board->sensor_data_signals[header]= temp_signal;
    }

    board->responses[TEMPERATURE_RESPONSE_HEADER] = response_handler_data_with_id;
}

MblMwDataSignal* mbl_mw_multi_chnl_temp_get_temperature_data_signal(const MblMwMetaWearBoard *board, uint8_t channel) {
    ResponseHeader header(MBL_MW_MODULE_TEMPERATURE, READ_REGISTER(ORDINAL(MultiChannelTempRegister::TEMPERATURE)), channel);

    try {
        return board->sensor_data_signals.at(header);
    } catch (out_of_range) {
        return NULL;
    }
}

void mbl_mw_multi_chnl_temp_configure_ext_thermistor(const MblMwMetaWearBoard *board, uint8_t channel, uint8_t data_pin, 
        uint8_t pulldown_pin, uint8_t active_high) {
    uint8_t command[6]= {MBL_MW_MODULE_TEMPERATURE, ORDINAL(MultiChannelTempRegister::MODE), channel, data_pin, pulldown_pin, active_high};
    SEND_COMMAND;
}

void mbl_mw_multi_chnl_temp_read_temperature(const MblMwMetaWearBoard *board, uint8_t channel) {
    uint8_t command[3]= {MBL_MW_MODULE_TEMPERATURE, READ_REGISTER(ORDINAL(MultiChannelTempRegister::TEMPERATURE)), channel};
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
