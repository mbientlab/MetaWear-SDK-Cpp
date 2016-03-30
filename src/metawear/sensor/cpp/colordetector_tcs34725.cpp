#include "metawear/sensor/colordetector_tcs34725.h"
#include "colordetector_tcs34725_private.h"
#include "colordetector_tcs34725_register.h"

#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include <cstring>
#include <stdint.h>

using std::calloc;

struct Tcs34725Config {
    uint8_t integration_time;
    uint8_t gain:2;
    uint8_t illuminator;
};

void init_colordetector_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_COLOR_DETECTOR) && board->module_info.at(MBL_MW_MODULE_COLOR_DETECTOR).present) {
        Tcs34725Config* new_config= (Tcs34725Config*) calloc(1, sizeof(Tcs34725Config));
        new_config->integration_time= 0xff;
        board->module_config.emplace(MBL_MW_MODULE_COLOR_DETECTOR, new_config);

        board->sensor_data_signals[CD_TCS34725_ADC_RESPONSE_HEADER]= new MblMwDataSignal(CD_TCS34725_ADC_RESPONSE_HEADER, board, 
                DataInterpreter::TCS34725_COLOR_ADC, 4, 2, 0, 0);
        board->responses[CD_TCS34725_ADC_RESPONSE_HEADER]= response_handler_data_no_id;
    }
}

METAWEAR_API MblMwDataSignal* mbl_mw_cd_tcs34725_get_adc_data_signal(const MblMwMetaWearBoard *board) {
    return board->sensor_data_signals.count(CD_TCS34725_ADC_RESPONSE_HEADER) ? 
            board->sensor_data_signals.at(CD_TCS34725_ADC_RESPONSE_HEADER) : 
            nullptr;
}

METAWEAR_API void mbl_mw_cd_tcs34725_read_adc(const MblMwMetaWearBoard *board) {
    uint8_t command[2]= { CD_TCS34725_ADC_RESPONSE_HEADER.module_id, CD_TCS34725_ADC_RESPONSE_HEADER.register_id };
    SEND_COMMAND;
}

METAWEAR_API void mbl_mw_cd_tcs34725_set_integration_time(MblMwMetaWearBoard *board, float time) {
    ((Tcs34725Config*) board->module_config.at(MBL_MW_MODULE_COLOR_DETECTOR))->integration_time= (uint8_t) (256.f - time / 2.4f);
}

METAWEAR_API void mbl_mw_cd_tcs34725_set_gain(MblMwMetaWearBoard *board, MblMwColorDetectorTcs34725Gain gain) {
    ((Tcs34725Config*) board->module_config.at(MBL_MW_MODULE_COLOR_DETECTOR))->gain= gain;
}

METAWEAR_API void mbl_mw_cd_tcs34725_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[5]= { MBL_MW_MODULE_COLOR_DETECTOR, ORDINAL(ColorDetectorTcs34725Register::MODE) };
    memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_COLOR_DETECTOR), sizeof(Tcs34725Config));

    SEND_COMMAND;
}
