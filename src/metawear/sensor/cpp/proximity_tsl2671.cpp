#include "metawear/sensor/proximity_tsl2671.h"
#include "proximity_tsl2671_private.h"
#include "proximity_tsl2671_register.h"
#include "utils.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include <cstring>
#include <stdint.h>
#include <cstdlib>

using std::calloc;
using std::stringstream;

const ResponseHeader PROXIMITY_TSL2671_ADC_RESPONSE_HEADER(MBL_MW_MODULE_PROXIMITY, READ_REGISTER(ORDINAL(ProximityTsl2671Register::PROXIMITY)));

struct Tsl2671Config {
    uint8_t integration_time;
    uint8_t n_pulses;
    uint8_t :4;
    uint8_t receiver_channel:2;
    uint8_t transmitter_current:2;
};

void init_proximity_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_PROXIMITY) && board->module_info.at(MBL_MW_MODULE_PROXIMITY).present) {
        if (!board->module_config.count(MBL_MW_MODULE_PROXIMITY)) {
            Tsl2671Config* new_config = (Tsl2671Config*)calloc(1, sizeof(Tsl2671Config));
            new_config->integration_time = 0xff;
            new_config->n_pulses = 1;
            new_config->receiver_channel = MBL_MW_PROXIMITY_TSL2671_CHANNEL_1;
            new_config->transmitter_current = MBL_MW_PROXIMITY_TSL2671_CURRENT_25mA;
            board->module_config.emplace(MBL_MW_MODULE_PROXIMITY, new_config);
        }

        if (!board->module_events.count(PROXIMITY_TSL2671_ADC_RESPONSE_HEADER)) {
            board->module_events[PROXIMITY_TSL2671_ADC_RESPONSE_HEADER] = new MblMwDataSignal(PROXIMITY_TSL2671_ADC_RESPONSE_HEADER, board,
                DataInterpreter::UINT32, 1, 2, 0, 0);
        }
        board->responses[PROXIMITY_TSL2671_ADC_RESPONSE_HEADER]= response_handler_data_no_id;
    }
}

void serialize_proximity_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(Tsl2671Config, MBL_MW_MODULE_PROXIMITY);
}

void deserialize_proximity_config(MblMwMetaWearBoard *board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(Tsl2671Config, MBL_MW_MODULE_PROXIMITY);
}

MblMwDataSignal* mbl_mw_proximity_tsl2671_get_adc_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(PROXIMITY_TSL2671_ADC_RESPONSE_HEADER);
}

void mbl_mw_proximity_tsl2671_set_integration_time(MblMwMetaWearBoard *board, float time) {
    ((Tsl2671Config*) board->module_config.at(MBL_MW_MODULE_PROXIMITY))->integration_time= (uint8_t) (256.f - time / 2.72f);
}

void mbl_mw_proximity_tsl2671_set_n_pulses(MblMwMetaWearBoard *board, uint8_t n_pulses) {
    ((Tsl2671Config*) board->module_config.at(MBL_MW_MODULE_PROXIMITY))->n_pulses= n_pulses;
}

void mbl_mw_proximity_tsl2671_set_receiver_channel(MblMwMetaWearBoard *board, MblMwProximityTsl2671Channel channel) {
    ((Tsl2671Config*) board->module_config.at(MBL_MW_MODULE_PROXIMITY))->receiver_channel= channel;
}

void mbl_mw_proximity_tsl2671_set_transmitter_current(MblMwMetaWearBoard *board, MblMwProximityTsl2671Current current) {
    ((Tsl2671Config*) board->module_config.at(MBL_MW_MODULE_PROXIMITY))->transmitter_current= current;
}

void mbl_mw_proximity_tsl2671_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[5]= { MBL_MW_MODULE_PROXIMITY, ORDINAL(ProximityTsl2671Register::MODE) };
    memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_PROXIMITY), sizeof(Tsl2671Config));

    SEND_COMMAND;
}

void create_proximity_uri(const MblMwDataSignal* signal, stringstream& uri) {
    switch(CLEAR_READ(signal->header.register_id)) {
    case ORDINAL(ProximityTsl2671Register::PROXIMITY):
        uri << "proximity";
        break;
    }
}
