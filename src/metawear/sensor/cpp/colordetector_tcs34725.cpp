#include "metawear/sensor/colordetector_tcs34725.h"
#include "colordetector_tcs34725_private.h"
#include "colordetector_tcs34725_register.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

#include <cstring>
#include <stdint.h>
#include <cstdlib>

using std::calloc;

const ResponseHeader CD_TCS34725_ADC_RESPONSE_HEADER(MBL_MW_MODULE_COLOR_DETECTOR, READ_REGISTER(ORDINAL(ColorDetectorTcs34725Register::RGB_COLOR)));

struct Tcs34725Config {
    uint8_t integration_time;
    uint8_t gain:2;
    uint8_t :6;
    uint8_t illuminator;
};

void init_colordetector_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_COLOR_DETECTOR) && board->module_info.at(MBL_MW_MODULE_COLOR_DETECTOR).present) {
        if (!board->module_config.count(MBL_MW_MODULE_COLOR_DETECTOR)) {
            Tcs34725Config* new_config = (Tcs34725Config*)calloc(1, sizeof(Tcs34725Config));
            new_config->integration_time = 0xff;
            board->module_config.emplace(MBL_MW_MODULE_COLOR_DETECTOR, new_config);
        }

        if (!board->module_events.count(CD_TCS34725_ADC_RESPONSE_HEADER)) {
            board->module_events[CD_TCS34725_ADC_RESPONSE_HEADER] = new MblMwDataSignal(CD_TCS34725_ADC_RESPONSE_HEADER, board,
                DataInterpreter::TCS34725_COLOR_ADC, 4, 2, 0, 0);
        }
        board->responses[CD_TCS34725_ADC_RESPONSE_HEADER]= response_handler_data_no_id;
    }
}

void serialize_colordetector_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state) {
    SERIALIZE_MODULE_CONFIG(Tcs34725Config, MBL_MW_MODULE_COLOR_DETECTOR);
}

void deserialize_colordetector_config(MblMwMetaWearBoard *board, uint8_t** state_stream) {
    DESERIALIZE_MODULE_CONFIG(Tcs34725Config, MBL_MW_MODULE_COLOR_DETECTOR);
}

MblMwDataSignal* mbl_mw_cd_tcs34725_get_adc_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(CD_TCS34725_ADC_RESPONSE_HEADER);
}

void mbl_mw_cd_tcs34725_set_integration_time(MblMwMetaWearBoard *board, float time) {
    ((Tcs34725Config*) board->module_config.at(MBL_MW_MODULE_COLOR_DETECTOR))->integration_time= (uint8_t) (256.f - time / 2.4f);
}

void mbl_mw_cd_tcs34725_set_gain(MblMwMetaWearBoard *board, MblMwColorDetectorTcs34725Gain gain) {
    ((Tcs34725Config*) board->module_config.at(MBL_MW_MODULE_COLOR_DETECTOR))->gain= gain;
}

void mbl_mw_cd_tcs34725_enable_illuminator_led(MblMwMetaWearBoard *board) {
    ((Tcs34725Config*) board->module_config.at(MBL_MW_MODULE_COLOR_DETECTOR))->illuminator= 1;
}

void mbl_mw_cd_tcs34725_disable_illuminator_led(MblMwMetaWearBoard *board) {
    ((Tcs34725Config*) board->module_config.at(MBL_MW_MODULE_COLOR_DETECTOR))->illuminator= 0;
}

void mbl_mw_cd_tcs34725_write_config(const MblMwMetaWearBoard *board) {
    uint8_t command[5]= { MBL_MW_MODULE_COLOR_DETECTOR, ORDINAL(ColorDetectorTcs34725Register::MODE) };
    memcpy(command + 2, board->module_config.at(MBL_MW_MODULE_COLOR_DETECTOR), sizeof(Tcs34725Config));

    SEND_COMMAND;
}
