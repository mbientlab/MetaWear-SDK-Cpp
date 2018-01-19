#include <stdexcept>

#include "utils.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"

#include "metawear/sensor/conductance.h"
#include "conductance_private.h"
#include "conductance_register.h"

using std::forward_as_tuple;
using std::piecewise_construct;

void init_conductance_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_CONDUCTANCE) && board->module_info.at(MBL_MW_MODULE_CONDUCTANCE).present) {
        uint8_t num_channels = mbl_mw_conductance_get_num_channels(board);
        for(uint8_t channel = 0; channel < num_channels; channel++) {
            ResponseHeader header(MBL_MW_MODULE_CONDUCTANCE, READ_REGISTER(ORDINAL(ConductanceRegister::CONDUCTANCE)), channel);
            if (!board->module_events.count(header)) {
                board->module_events[header] = new MblMwDataSignal(header, board, DataInterpreter::UINT32, 
                        FirmwareConverter::DEFAULT, 1, 4, 0, 0);
            }
        }

        board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_CONDUCTANCE, READ_REGISTER(ORDINAL(ConductanceRegister::CONDUCTANCE))),
            forward_as_tuple(response_handler_data_with_id));
    }
}

MblMwDataSignal* mbl_mw_conductance_get_data_signal(const MblMwMetaWearBoard *board, uint8_t channel) {
    ResponseHeader header(MBL_MW_MODULE_CONDUCTANCE, READ_REGISTER(ORDINAL(ConductanceRegister::CONDUCTANCE)), channel);
    return board->module_events.count(header) ? dynamic_cast<MblMwDataSignal*>(board->module_events.at(header)) : nullptr;
}

uint8_t mbl_mw_conductance_get_num_channels(const MblMwMetaWearBoard *board) {
    return ORDINAL(board->module_info.at(MBL_MW_MODULE_CONDUCTANCE).extra[0]);
}

void mbl_mw_conductance_calibrate(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= {MBL_MW_MODULE_CONDUCTANCE, ORDINAL(ConductanceRegister::CALIBRATE)};
    SEND_COMMAND;
}

void mbl_mw_conductance_set_range(MblMwMetaWearBoard *board, MblMwConductanceRange range) {
    uint8_t command[3]= {MBL_MW_MODULE_CONDUCTANCE, ORDINAL(ConductanceRegister::MODE), ORDINAL(range)};
    SEND_COMMAND;
}
