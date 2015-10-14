#include "connection_def.h"
#include "metawear/ambientlight_ltr329.h"
#include "metawearboard_def.h"
#include "register.h"

#include <cstring>

using std::memcpy;
using std::memset;

Ltr329Config::Ltr329Config() {
    memset(this, 0, sizeof(Ltr329Config));

    als_measurement_rate= MBL_MW_ALS_LTR329_RATE_500MS;
}

void mbl_mw_als_ltr329_set_gain(MblMwMetaWearBoard *board, MblMwAlsLtr329Gain gain) {
    switch(gain) {
        case MBL_MW_ALS_LTR329_GAIN_48X:
        case MBL_MW_ALS_LTR329_GAIN_96X:
            board->ltr329_config.als_gain= gain + 2;
            break;
        default:
            board->ltr329_config.als_gain= gain;
            break;
    }
}

void mbl_mw_als_ltr329_set_integration_time(MblMwMetaWearBoard *board, MblMwAlsLtr329IntegrationTime integration_time) {
    board->ltr329_config.als_integration_time= integration_time;
}

void mbl_mw_als_ltr329_set_measurement_rate(MblMwMetaWearBoard *board, MblMwAlsLtr329MeasurementRate measurement_rate) {
    board->ltr329_config.als_measurement_rate= measurement_rate;
}

void mbl_mw_als_ltr329_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::AMBIENT_LIGHT), ORDINAL(AmbientLightLtr329Register::CONFIG)};
    memcpy(command + 2, &board->ltr329_config, sizeof(Ltr329Config));
    SEND_COMMAND;
}

void mbl_mw_als_ltr329_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::AMBIENT_LIGHT), ORDINAL(AmbientLightLtr329Register::ENABLE), 1};
    SEND_COMMAND;
}

void mbl_mw_als_ltr329_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= {ORDINAL(Module::AMBIENT_LIGHT), ORDINAL(AmbientLightLtr329Register::ENABLE), 0};
    SEND_COMMAND;
}
