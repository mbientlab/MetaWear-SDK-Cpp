#include "metawear/sensor/humidity_bme280.h"
#include "humidity_bme280_private.h"
#include "humidity_bme280_register.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include <cstring>
#include <stdint.h>

const ResponseHeader HUMIDITY_BME280_ADC_RESPONSE_HEADER(MBL_MW_MODULE_HUMIDITY, READ_REGISTER(ORDINAL(HumidityBme280Register::HUMIDITY)));

void init_humidity_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_HUMIDITY) && board->module_info.at(MBL_MW_MODULE_HUMIDITY).present) {
        if (!board->module_events.count(HUMIDITY_BME280_ADC_RESPONSE_HEADER)) {
            board->module_events[HUMIDITY_BME280_ADC_RESPONSE_HEADER] = new MblMwDataSignal(HUMIDITY_BME280_ADC_RESPONSE_HEADER, board,
                DataInterpreter::BME280_HUMIDITY, FirmwareConverter::BME280_HUMIDITY, 1, 4, 0, 0);
        }
        board->responses[HUMIDITY_BME280_ADC_RESPONSE_HEADER]= response_handler_data_no_id;
    }
}

MblMwDataSignal* mbl_mw_humidity_bme280_get_percentage_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(HUMIDITY_BME280_ADC_RESPONSE_HEADER);
}

void mbl_mw_humidity_bme280_set_oversampling(const MblMwMetaWearBoard *board, MblMwHumidityBme280Oversampling oversampling) {
    uint8_t command[3]= { MBL_MW_MODULE_HUMIDITY, ORDINAL(HumidityBme280Register::MODE), static_cast<uint8_t>(oversampling) };
    SEND_COMMAND;
}
