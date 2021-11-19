#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include "metawear/sensor/barometer_bosch.h"
#include "barometer_bosch_private.h"
#include "barometer_bosch_register.h"
#include "utils.h"

#include <cstdlib>
#include <cstring>

using std::calloc;
using std::forward_as_tuple;
using std::memcpy;
using std::piecewise_construct;
using std::vector;

const uint8_t MBL_MW_MODULE_BARO_TYPE_BMP280 = 0;            ///< Constant identifying the BMP280 barometer module type
const uint8_t MBL_MW_MODULE_BARO_TYPE_BME280 = 1;            ///< Constant identifying the BME280 barometer module type

const vector<float> BMP280_STANDBY_VALUES= {0.5f, 62.5f, 125.f, 250.f, 500.f, 1000.f, 2000.f, 4000.f},
        BME280_STANDBY_VALUES= {0.5f, 62.5f, 125.f, 250.f, 500.f, 1000.f, 10.f, 20.f};

const ResponseHeader BARO_PRESSURE_RESPONSE_HEADER(MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::PRESSURE)),
        BARO_ALTITUDE_RESPONSE_HEADER(MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::ALTITUDE)),
        BARO_PRESSURE_READ_RESPONSE_HEADER(MBL_MW_MODULE_BAROMETER, READ_REGISTER(ORDINAL(BarometerBmp280Register::PRESSURE)));

struct BoschBaroConfig {
    uint8_t:2;
    uint8_t pressure_oversampling:3;
    uint8_t temperature_oversampling:3;
    uint8_t:2;
    uint8_t iir_filter:3;
    uint8_t standby_time:3;
};

void init_barometer_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_BAROMETER) && board->module_info.at(MBL_MW_MODULE_BAROMETER).present) {
        if (!board->module_config.count(MBL_MW_MODULE_BAROMETER)) {
            BoschBaroConfig* new_config = (BoschBaroConfig*)calloc(1, sizeof(BoschBaroConfig));

            new_config->pressure_oversampling = MBL_MW_BARO_BOSCH_OVERSAMPLING_STANDARD;
            new_config->iir_filter = MBL_MW_BARO_BOSCH_IIR_FILTER_OFF;
            new_config->standby_time = 0;       ///< Set standby time to 0.5ms, which is enum 0 for both bmp280 and bme280
            new_config->temperature_oversampling = MBL_MW_BARO_BOSCH_OVERSAMPLING_ULTRA_LOW_POWER;

            board->module_config.emplace(MBL_MW_MODULE_BAROMETER, new_config);
        }

        if (!board->module_events.count(BARO_PRESSURE_RESPONSE_HEADER)) {
            board->module_events[BARO_PRESSURE_RESPONSE_HEADER] = new MblMwDataSignal(BARO_PRESSURE_RESPONSE_HEADER, board, 
                DataInterpreter::BOSCH_PRESSURE, FirmwareConverter::BOSCH_BAROMETER, 1, 4, 0, 0);
        }
        board->responses[BARO_PRESSURE_RESPONSE_HEADER]= response_handler_data_no_id;

        if (!board->module_events.count(BARO_ALTITUDE_RESPONSE_HEADER)) {
            board->module_events[BARO_ALTITUDE_RESPONSE_HEADER] = new MblMwDataSignal(BARO_ALTITUDE_RESPONSE_HEADER, board, 
                DataInterpreter::BOSCH_ALTITUDE, FirmwareConverter::BOSCH_BAROMETER, 1, 4, 1, 0);
        }
        board->responses[BARO_ALTITUDE_RESPONSE_HEADER]= response_handler_data_no_id;
        
        if (!board->module_events.count(BARO_PRESSURE_READ_RESPONSE_HEADER)) {
            board->module_events[BARO_PRESSURE_READ_RESPONSE_HEADER] = new MblMwDataSignal(BARO_PRESSURE_READ_RESPONSE_HEADER, board,
                DataInterpreter::BOSCH_PRESSURE, FirmwareConverter::BOSCH_BAROMETER, 1, 4, 0, 0);
        }
        board->responses[BARO_PRESSURE_READ_RESPONSE_HEADER]= response_handler_data_no_id;
    }
}


void serialize_barometer_config(const MblMwMetaWearBoard *board, vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(BoschBaroConfig, MBL_MW_MODULE_BAROMETER);
}

void deserialize_barometer_config(MblMwMetaWearBoard *board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(BoschBaroConfig, MBL_MW_MODULE_BAROMETER);
}

MblMwDataSignal* mbl_mw_baro_bosch_get_pressure_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(BARO_PRESSURE_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_baro_bosch_get_pressure_read_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(BARO_PRESSURE_READ_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_baro_bosch_get_altitude_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(BARO_ALTITUDE_RESPONSE_HEADER);
}

void mbl_mw_baro_bosch_set_oversampling(MblMwMetaWearBoard *board, MblMwBaroBoschOversampling oversampling) {
    auto config= (BoschBaroConfig*) board->module_config.at(MBL_MW_MODULE_BAROMETER);

    config->pressure_oversampling= oversampling;
    if (oversampling == MBL_MW_BARO_BOSCH_OVERSAMPLING_ULTRA_HIGH) {
        config->temperature_oversampling= MBL_MW_BARO_BOSCH_OVERSAMPLING_LOW_POWER;
    }
}

void mbl_mw_baro_bosch_set_iir_filter(MblMwMetaWearBoard *board, MblMwBaroBoschIirFilter iir_filter) {
    ((BoschBaroConfig*) board->module_config.at(MBL_MW_MODULE_BAROMETER))->iir_filter= iir_filter;
}

void mbl_mw_baro_bmp280_set_standby_time(MblMwMetaWearBoard *board, MblMwBaroBmp280StandbyTime standby_time) {
    ((BoschBaroConfig*) board->module_config.at(MBL_MW_MODULE_BAROMETER))->standby_time= standby_time;
}

void mbl_mw_baro_bme280_set_standby_time(MblMwMetaWearBoard *board, MblMwBaroBme280StandbyTime standby_time) {
    ((BoschBaroConfig*) board->module_config.at(MBL_MW_MODULE_BAROMETER))->standby_time= standby_time;
}

float mbl_mw_baro_bosch_set_standby_time(MblMwMetaWearBoard *board, float standby_time_ms) {
    uint8_t index;

    switch (board->module_info.at(MBL_MW_MODULE_BAROMETER).implementation) {
    case MBL_MW_MODULE_BARO_TYPE_BMP280:
        index= closest_index(BMP280_STANDBY_VALUES, standby_time_ms);
        mbl_mw_baro_bmp280_set_standby_time(board, (MblMwBaroBmp280StandbyTime) index);
        return BMP280_STANDBY_VALUES[index];
    case MBL_MW_MODULE_BARO_TYPE_BME280:
        index= closest_index(BME280_STANDBY_VALUES, standby_time_ms);
        mbl_mw_baro_bme280_set_standby_time(board, (MblMwBaroBme280StandbyTime) index);
        return BME280_STANDBY_VALUES[index];
    default:
        return -1;
    }
}

void mbl_mw_baro_bosch_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::CONFIG)};
    memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_BAROMETER), sizeof(BoschBaroConfig));
    SEND_COMMAND;
}

void mbl_mw_baro_bosch_start(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::CYCLIC), 1, 1};
    SEND_COMMAND;
}

void mbl_mw_baro_bosch_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= {MBL_MW_MODULE_BAROMETER, ORDINAL(BarometerBmp280Register::CYCLIC), 0, 0};
    SEND_COMMAND;
}

void create_barometer_uri(const MblMwDataSignal* signal, std::stringstream& uri) {
    switch(CLEAR_READ(signal->header.register_id)) {
    case ORDINAL(BarometerBmp280Register::PRESSURE):
        uri << "pressure";
        break;
    case ORDINAL(BarometerBmp280Register::ALTITUDE):
        uri << "altitude";
        break;
    }
}
