#include "connection_def.h"
#include "metawearboard_def.h"
#include "metawear/barometer_bmp280.h"
#include "register.h"

#include <cstring>

using std::memcpy;
using std::memset;

Bmp280Config::Bmp280Config() {
    memset(this, 0, sizeof(Bmp280Config));

    pressure_oversampling= MBL_MW_BARO_BMP280_OVERSAMPLE_STANDARD;
    iir_filter= MBL_MW_BARO_BMP280_IIR_FILTER_OFF;
    standby_time= MBL_MW_BARO_BMP280_STANDBY_TIME_0_5MS;
}

void mbl_mw_baro_bmp280_set_oversampling(MblMwMetaWearBoard *board, MblMwBaroBmp280Oversampling oversampling) {
    board->bmp280_config.pressure_oversampling= oversampling;
}

void mbl_mw_baro_bmp280_set_iir_filter(MblMwMetaWearBoard *board, MblMwBaroBmp280IirFilter iir_filter) {
    board->bmp280_config.iir_filter= iir_filter;
}

void mbl_mw_baro_bmp280_set_standby_time(MblMwMetaWearBoard *board, MblMwBaroBmp280StandbyTime standby_time) {
    board->bmp280_config.standby_time= standby_time;
}

void mbl_mw_baro_bmp280_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::BAROMETER), ORDINAL(BarometerBmp280Register::CONFIG)};
    memcpy(command + 2, &board->bmp280_config, sizeof(Bmp280Config));
    SEND_COMMAND;
}

void mbl_mw_baro_bmp280_start(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::BAROMETER), ORDINAL(BarometerBmp280Register::CYCLIC), 1, 1};
    SEND_COMMAND;
}

void mbl_mw_baro_bmp280_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {ORDINAL(Module::BAROMETER), ORDINAL(BarometerBmp280Register::CYCLIC), 0, 0};
    SEND_COMMAND;
}
