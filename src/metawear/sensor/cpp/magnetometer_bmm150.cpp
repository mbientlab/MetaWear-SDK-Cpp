#include "metawear/sensor/magnetometer_bmm150.h"
#include "magnetometer_bmm150_register.h"
#include "utils.h"

#include "metawear/core/module.h"
#include "metawear/core/cpp/datasignal_private.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/metawearboard_macro.h"
#include "metawear/core/cpp/register.h"
#include "metawear/core/cpp/responseheader.h"

using std::stringstream;

#define CREATE_BFIELD_SIGNAL_SINGLE(offset) CREATE_BFIELD_SIGNAL(DataInterpreter::BMM150_B_FIELD_SINGLE_AXIS, 1, offset)
#define CREATE_BFIELD_SIGNAL(interpreter, channels, offset) new MblMwDataSignal(BMM150_MAG_DATA_RESPONSE_HEADER, board, interpreter, \
        FirmwareConverter::BOSCH_MAGNETOMETER, channels, 2, 1, offset)

const uint8_t PACKED_MAG_REVISION = 1, SUSPEND_REVISION = 2;
const ResponseHeader BMM150_MAG_DATA_RESPONSE_HEADER(MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::MAG_DATA)),
        BMM150_MAG_PACKED_DATA_RESPONSE_HEADER(MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::PACKED_MAG_DATA));;

void init_magnetometer_module(MblMwMetaWearBoard *board) {
    if (board->module_info.count(MBL_MW_MODULE_MAGNETOMETER) && board->module_info.at(MBL_MW_MODULE_MAGNETOMETER).present) {
        MblMwDataSignal* bfield;
        if (board->module_events.count(BMM150_MAG_DATA_RESPONSE_HEADER)) {
            bfield = dynamic_cast<MblMwDataSignal*>(board->module_events[BMM150_MAG_DATA_RESPONSE_HEADER]);
        } else {
            bfield = CREATE_BFIELD_SIGNAL(DataInterpreter::BMM150_B_FIELD, 3, 0);
            board->module_events[BMM150_MAG_DATA_RESPONSE_HEADER] = bfield;
        }
        if (!bfield->components.size()) {
            bfield->components.push_back(CREATE_BFIELD_SIGNAL_SINGLE(0));
            bfield->components.push_back(CREATE_BFIELD_SIGNAL_SINGLE(2));
            bfield->components.push_back(CREATE_BFIELD_SIGNAL_SINGLE(4));
        }

        board->responses[BMM150_MAG_DATA_RESPONSE_HEADER] = response_handler_data_no_id;

        if (board->module_info.at(MBL_MW_MODULE_MAGNETOMETER).revision >= PACKED_MAG_REVISION) {
            if (!board->module_events.count(BMM150_MAG_PACKED_DATA_RESPONSE_HEADER)) {
                board->module_events[BMM150_MAG_PACKED_DATA_RESPONSE_HEADER]= new MblMwDataSignal(BMM150_MAG_PACKED_DATA_RESPONSE_HEADER, board, 
                    DataInterpreter::BMM150_B_FIELD, FirmwareConverter::BOSCH_MAGNETOMETER, 3, 2, 1, 0);
            }
            board->responses[BMM150_MAG_PACKED_DATA_RESPONSE_HEADER]= response_handler_packed_data;
        }
    }
}

MblMwDataSignal* mbl_mw_mag_bmm150_get_b_field_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(BMM150_MAG_DATA_RESPONSE_HEADER);
}

MblMwDataSignal* mbl_mw_mag_bmm150_get_packed_b_field_data_signal(const MblMwMetaWearBoard *board) {
    GET_DATA_SIGNAL(BMM150_MAG_PACKED_DATA_RESPONSE_HEADER);
}

void mbl_mw_mag_bmm150_configure(const MblMwMetaWearBoard *board, uint16_t xy_reps, uint16_t z_reps, MblMwMagBmm150Odr odr) {
    if (board->module_info.at(MBL_MW_MODULE_MAGNETOMETER).revision >= SUSPEND_REVISION) {
        mbl_mw_mag_bmm150_stop(board);
    }

    uint8_t data_rep_cmd[4]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::DATA_REPETITIONS), 
            static_cast<uint8_t>((xy_reps - 1) / 2), static_cast<uint8_t>(z_reps - 1) };
    send_command(board, data_rep_cmd, sizeof(data_rep_cmd));

    uint8_t data_rate_cmd[3]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::DATA_RATE), static_cast<uint8_t>(odr) };
    send_command(board, data_rate_cmd, sizeof(data_rate_cmd));
}

void mbl_mw_mag_bmm150_set_preset(const MblMwMetaWearBoard *board, MblMwMagBmm150Preset preset) {
    switch (preset) {
    case MBL_MW_MAG_BMM150_PRESET_LOW_POWER:
        mbl_mw_mag_bmm150_configure(board, 3, 3, MBL_MW_MAG_BMM150_ODR_10Hz);
        break;
    case MBL_MW_MAG_BMM150_PRESET_REGULAR:
        mbl_mw_mag_bmm150_configure(board, 9, 15, MBL_MW_MAG_BMM150_ODR_10Hz);
        break;
    case MBL_MW_MAG_BMM150_PRESET_ENHANCED_REGULAR:
        mbl_mw_mag_bmm150_configure(board, 15, 27, MBL_MW_MAG_BMM150_ODR_10Hz);
        break;
    case MBL_MW_MAG_BMM150_PRESET_HIGH_ACCURACY:
        mbl_mw_mag_bmm150_configure(board, 47, 83, MBL_MW_MAG_BMM150_ODR_20Hz);
        break;
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

void mbl_mw_mag_bmm150_suspend(const MblMwMetaWearBoard *board) {
    if (board->module_info.at(MBL_MW_MODULE_MAGNETOMETER).revision >= SUSPEND_REVISION) {
        uint8_t command[3]= { MBL_MW_MODULE_MAGNETOMETER, ORDINAL(MagnetometerBmm150Register::POWER_MODE), 2 };
        SEND_COMMAND;
    }
}

void create_magnetometer_uri(const MblMwDataSignal* signal, stringstream& uri) {
    switch(CLEAR_READ(signal->header.register_id)) {
    case ORDINAL(MagnetometerBmm150Register::MAG_DATA):
        uri << "magnetic-field";
        if (signal->length() <= 2) {
            uri << "[" << (int) (signal->offset >> 1) << "]";
        }
    }
}
