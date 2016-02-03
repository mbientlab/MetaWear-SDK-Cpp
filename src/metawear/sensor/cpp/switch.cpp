#include "metawear/sensor/switch.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "switch_register.h"

MblMwDataSignal* mbl_mw_switch_get_state_data_signal(const MblMwMetaWearBoard *board) {
    return board->sensor_data_signals.at(SWITCH_RESPONSE_HEADER);
}
