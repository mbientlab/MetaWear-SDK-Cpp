#include "metawear/sensor/humidity_bme280.h"
#include "humidity_bme280_private.h"
#include "humidity_bme280_register.h"

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <cstring>
#include <stdint.h>

static float bme280_humidity_to_firmware(MblMwDataSignal* source, float value) {
    return (value * 1024.f);
}

void init_humidity_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_HUMIDITY) && board->module_info.at(MBL_MW_MODULE_HUMIDITY).present) {
        auto signal= new MblMwDataSignal(HUMIDITY_BME280_ADC_RESPONSE_HEADER, board, DataInterpreter::BME280_HUMIDITY, 1, 4, 0, 0);
        signal->number_to_firmware= bme280_humidity_to_firmware;

        board->sensor_data_signals[HUMIDITY_BME280_ADC_RESPONSE_HEADER]= signal;
        board->responses[HUMIDITY_BME280_ADC_RESPONSE_HEADER]= response_handler_data_no_id;
    }
}

MblMwDataSignal* mbl_mw_humidity_bme280_get_percentage_data_signal(const MblMwMetaWearBoard *board) {
    return board->sensor_data_signals.count(HUMIDITY_BME280_ADC_RESPONSE_HEADER) ? 
            board->sensor_data_signals.at(HUMIDITY_BME280_ADC_RESPONSE_HEADER) : 
            nullptr;
}

void mbl_mw_humidity_bme280_read_percentage(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= { HUMIDITY_BME280_ADC_RESPONSE_HEADER.module_id, HUMIDITY_BME280_ADC_RESPONSE_HEADER.register_id };
    SEND_COMMAND;
}

void mbl_mw_humidity_bme280_set_oversampling(const MblMwMetaWearBoard *board, MblMwHumidityBme280Oversampling oversampling) {
    uint8_t command[3]= { MBL_MW_MODULE_HUMIDITY, ORDINAL(HumidityBme280Register::MODE), oversampling };
    SEND_COMMAND;
}
