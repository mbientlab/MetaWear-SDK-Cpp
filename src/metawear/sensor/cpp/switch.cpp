#include "metawear/sensor/switch.h"
#include "switch_register.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

const ResponseHeader SWITCH_RESPONSE_HEADER(MBL_MW_MODULE_SWITCH, ORDINAL(SwitchRegister::STATE));

MblMwDataSignal* mbl_mw_switch_get_state_data_signal(const MblMwMetaWearBoard *board) {
    return dynamic_cast<MblMwDataSignal*>(board->module_events.at(SWITCH_RESPONSE_HEADER));
}

void init_switch_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_SWITCH) && board->module_info.at(MBL_MW_MODULE_SWITCH).present) {
        if (!board->module_events.count(SWITCH_RESPONSE_HEADER)) {
            board->module_events[SWITCH_RESPONSE_HEADER] = new MblMwDataSignal(SWITCH_RESPONSE_HEADER, board, DataInterpreter::UINT32,
                1, 1, 0, 0);
        }

        board->responses[SWITCH_RESPONSE_HEADER] = response_handler_data_no_id;
    }
}
