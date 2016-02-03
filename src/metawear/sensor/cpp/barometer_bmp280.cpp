#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "metawear/sensor/barometer_bmp280.h"
#include "barometer_bmp280_register.h"

#include <cstring>

using std::forward_as_tuple;
using std::memcpy;
using std::memset;
using std::piecewise_construct;

static float bmp280_to_firmware(MblMwDataSignal* source, float value) {
    return (value * 256.f);
}

struct Bmp280Config {
    uint8_t:2;
    uint8_t pressure_oversampling:3;
    uint8_t:3;
    uint8_t:2;
    uint8_t iir_filter:3;
    uint8_t standby_time:3;
};

void init_barometer_module(MblMwMetaWearBoard *board) {
    Bmp280Config* new_config = (Bmp280Config*)malloc(sizeof(Bmp280Config));

    memset(new_config, 0, sizeof(Bmp280Config));
    new_config->pressure_oversampling = MBL_MW_BARO_BMP280_OVERSAMPLE_STANDARD;
    new_config->iir_filter = MBL_MW_BARO_BMP280_IIR_FILTER_OFF;
    new_config->standby_time = MBL_MW_BARO_BMP280_STANDBY_TIME_0_5MS;
    board->module_config.emplace(MBL_MW_MODULE_BAROMETER, new_config);

    MblMwDataSignal *baro_pa_signal= new MblMwDataSignal(BARO_PRESSURE_RESPONSE_HEADER, board, 
            DataInterpreter::BMP280_PRESSURE, 1, 4, 0, 0);
    baro_pa_signal->number_to_firmware = bmp280_to_firmware;
    board->sensor_data_signals[BARO_PRESSURE_RESPONSE_HEADER]= baro_pa_signal;
    board->responses[BARO_PRESSURE_RESPONSE_HEADER]= response_handler_data_no_id;

    MblMwDataSignal *baro_m_signal= new MblMwDataSignal(BARO_ALTITUDE_RESPONSE_HEADER, board, 
            DataInterpreter::BMP280_ALTITUDE, 1, 4, 1, 0);
    baro_m_signal->number_to_firmware = bmp280_to_firmware;
    board->sensor_data_signals[BARO_ALTITUDE_RESPONSE_HEADER]= baro_m_signal;
    board->responses[BARO_ALTITUDE_RESPONSE_HEADER]= response_handler_data_no_id;
}

MblMwDataSignal* mbl_mw_baro_bmp280_get_pressure_data_signal(const MblMwMetaWearBoard *board) {
    return board->sensor_data_signals.at(BARO_PRESSURE_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_baro_bmp280_get_altitude_data_signal(const MblMwMetaWearBoard *board) {
    return board->sensor_data_signals.at(BARO_ALTITUDE_RESPONSE_HEADER);
}

void mbl_mw_baro_bmp280_set_oversampling(MblMwMetaWearBoard *board, MblMwBaroBmp280Oversampling oversampling) {
    ((Bmp280Config*) board->module_config.at(MBL_MW_MODULE_BAROMETER))->pressure_oversampling= oversampling;
}

void mbl_mw_baro_bmp280_set_iir_filter(MblMwMetaWearBoard *board, MblMwBaroBmp280IirFilter iir_filter) {
    ((Bmp280Config*) board->module_config.at(MBL_MW_MODULE_BAROMETER))->iir_filter= iir_filter;
}

void mbl_mw_baro_bmp280_set_standby_time(MblMwMetaWearBoard *board, MblMwBaroBmp280StandbyTime standby_time) {
    ((Bmp280Config*) board->module_config.at(MBL_MW_MODULE_BAROMETER))->standby_time= standby_time;
}

void mbl_mw_baro_bmp280_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::CONFIG)};
    memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_BAROMETER), sizeof(Bmp280Config));
    SEND_COMMAND;
}

void mbl_mw_baro_bmp280_start(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::CYCLIC), 1, 1};
    SEND_COMMAND;
}

void mbl_mw_baro_bmp280_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::CYCLIC), 0, 0};
    SEND_COMMAND;
}
