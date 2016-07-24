#include "metawear/sensor/magnetometer_bmm150.h"
#include "magnetometer_bmm150_register.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

const ResponseHeader BMM150_MAG_DATA_RESPONSE_HEADER(MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::MAG_DATA));

void init_magnetometer_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_MAGNETOMETER) && board->module_info.at(MBL_MW_MODULE_MAGNETOMETER).present) {
        if (!board->module_events.count(BMM150_MAG_DATA_RESPONSE_HEADER)) {
            board->module_events[BMM150_MAG_DATA_RESPONSE_HEADER] = new MblMwDataSignal(BMM150_MAG_DATA_RESPONSE_HEADER, board,
                DataInterpreter::BMM150_B_FIELD, FirmwareConverter::BOSCH_MAGNETOMETER, 3, 2, 1, 0);
        }
        board->responses[BMM150_MAG_DATA_RESPONSE_HEADER] = response_handler_data_no_id;
    }
}

MblMwDataSignal* mbl_mw_mag_bmm150_get_b_field_data_signal(const MblMwMetaWearBoard *board) {
    return board->module_events.count(BMM150_MAG_DATA_RESPONSE_HEADER) ?
            dynamic_cast<MblMwDataSignal*>(board->module_events.at(BMM150_MAG_DATA_RESPONSE_HEADER)) :
            nullptr;
}

void mbl_mw_mag_bmm150_set_power_preset(const MblMwMetaWearBoard *board, MblMwMagBmm150PowerPreset preset) {
    uint8_t data_rate, rep_xy, rep_z;
    bool write= true;

    switch (preset) {
    case MWL_MW_MAG_BMM_150_PP_LOW_POWER:
        data_rate= 0;
        rep_xy= 1;
        rep_z= 2;
        break;
    case MWL_MW_MAG_BMM_150_PP_REGULAR:
        data_rate= 0;
        rep_xy= 4;
        rep_z= 14;
        break;
    case MWL_MW_MAG_BMM_150_PP_ENHANCED_REGULAR:
        data_rate= 0;
        rep_xy= 7;
        rep_z= 26;
        break;
    case MWL_MW_MAG_BMM_150_PP_HIGH_ACCURACY:
        data_rate= 5;
        rep_xy= 23;
        rep_z= 82;
        break;
    default:
        write= false;
        break;
    }

    if (write) {
        uint8_t data_rep_cmd[4]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::DATA_REPETITIONS), rep_xy, rep_z };
        send_command(board, data_rep_cmd, sizeof(data_rep_cmd));

        uint8_t data_rate_cmd[3]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::DATA_RATE), data_rate };
        send_command(board, data_rate_cmd, sizeof(data_rate_cmd));
    }
}

void mbl_mw_mag_bmm150_enable_b_field_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::DATA_INTERRUPT_ENABLE), 1, 0 };
    SEND_COMMAND;
}

void mbl_mw_mag_bmm150_disable_b_field_sampling(const MblMwMetaWearBoard *board) {
    uint8_t command[4]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::DATA_INTERRUPT_ENABLE), 0, 1 };
    SEND_COMMAND;
}

void mbl_mw_mag_bmm150_start(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::POWER_MODE), 1 };
    SEND_COMMAND;
}

void mbl_mw_mag_bmm150_stop(const MblMwMetaWearBoard *board) {
    uint8_t command[3]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::POWER_MODE), 0 };
    SEND_COMMAND;
}
