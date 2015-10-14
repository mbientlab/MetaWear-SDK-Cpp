#include <cstdlib>
#include <stdexcept>
#include <tuple>
#include <unordered_map>

#include "metawear/status.h"

#include "connection_def.h"
#include "datainterpreter.h"
#include "metawearboard_def.h"
#include "register.h"
#include "responseheader.h"

using std::forward_as_tuple;
using std::free;
using std::out_of_range;
using std::piecewise_construct;
using std::unordered_map;

/*
static const vector<vector<uint8_t>> MODULE_DISCOVERY_CMDS= {
    {ORDINAL(Module::SWITCH), INFO_REGISTER},
    {ORDINAL(Module::LED), INFO_REGISTER},
    {ORDINAL(Module::ACCELEROMETER), INFO_REGISTER}
    {ORDINAL(Module::TEMPERATURE), INFO_REGISTER},,
    {ORDINAL(Module::GPIO), INFO_REGISTER},
    {ORDINAL(Module::NEO_PIXEL), INFO_REGISTER},
    {ORDINAL(Module::IBEACON), INFO_REGISTER},
    {ORDINAL(Module::HAPTIC), INFO_REGISTER},
    {ORDINAL(Module::DATA_PROCESSOR), INFO_REGISTER},
    {ORDINAL(Module::EVENT), INFO_REGISTER},
    {ORDINAL(Module::LOGGING), INFO_REGISTER},
    {ORDINAL(Module::TIMER), INFO_REGISTER},
    {ORDINAL(Module::I2C), INFO_REGISTER},
    {ORDINAL(Module::MACRO), INFO_REGISTER},
    {ORDINAL(Module::GSR), INFO_REGISTER},
    {ORDINAL(Module::SETTINGS), INFO_REGISTER},
    {ORDINAL(Module::BAROMETER), INFO_REGISTER},
    {ORDINAL(Module::GYRO), INFO_REGISTER},
    {ORDINAL(Module::SWITCH), INFO_REGISTER},
    {ORDINAL(Module::AMBIENT_LIGHT), INFO_REGISTER},
    {ORDINAL(Module::DEBUG), INFO_REGISTER}
};
*/

MblMwMetaWearBoard::MblMwMetaWearBoard() {
    ///<  elements added in the same order as the DataSignalId enum
    sensor_data_signals.emplace_back(SWITCH_RESPONSE_HEADER, MBL_MW_DT_ID_UINT32, 0, 1, 0, this);
    sensor_data_signals.emplace_back(MMA8452Q_ACCEL_RESPONSE_HEADER, MBL_MW_DT_ID_CARTESIAN_FLOAT, 1, 6, 0, this);
    sensor_data_signals.emplace_back(BMI160_ACCEL_RESPONSE_HEADER, MBL_MW_DT_ID_CARTESIAN_FLOAT, 1, 6, 0, this);
    sensor_data_signals.emplace_back(BARO_PRESSURE_RESPONSE_HEADER, MBL_MW_DT_ID_FLOAT, 0, 4, 0, this);
    sensor_data_signals.emplace_back(BARO_ALTITUDE_RESPONSE_HEADER, MBL_MW_DT_ID_FLOAT, 1, 4, 0, this);
    sensor_data_signals.emplace_back(GYRO_ROT_RESPONSE_HEADER, MBL_MW_DT_ID_CARTESIAN_FLOAT, 1, 6, 0, this);
    sensor_data_signals.emplace_back(LTR329_ILLUMINANCE_RESPONSE_HEADER, MBL_MW_DT_ID_UINT32, 0, 4, 0, this);

    response_processors.emplace(SWITCH_RESPONSE_HEADER, convert_to_uint32);
    response_processors.emplace(LTR329_ILLUMINANCE_RESPONSE_HEADER, convert_to_uint32);
    response_processors.emplace(BARO_PRESSURE_RESPONSE_HEADER, convert_to_bmp280_pressure);
    response_processors.emplace(BARO_ALTITUDE_RESPONSE_HEADER, convert_to_bmp280_altitude);
    response_processors.emplace(GYRO_ROT_RESPONSE_HEADER, convert_to_bmi160_rotation);
}

MblMwMetaWearBoard* mbl_mw_create_metawear_board() {
    return new MblMwMetaWearBoard;
}

void mbl_mw_free_metawear_board(MblMwMetaWearBoard *board) {
    delete board;
}

/*
void mbl_mw_metawearboard_discover_modules() {
    for(auto &cmd: MODULE_DISCOVERY_CMDS) {
        send_command_fn(cmd.data(), cmd.size());
    }
}
*/

int32_t mbl_mw_metawearboard_handle_response(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint8_t resp_register_id= response[1] & 0x7f;

    if ((response[0] == ORDINAL(Module::SWITCH) && resp_register_id == ORDINAL(SwitchRegister::STATE)) || 
        (response[0] == ORDINAL(Module::ACCELEROMETER) && resp_register_id == ORDINAL(AccelerometerBmi160Register::DATA_INTERRUPT)) ||
        (response[0] == ORDINAL(Module::ACCELEROMETER) && resp_register_id == ORDINAL(AccelerometerMma8452qRegister::DATA_VALUE)) ||
        (response[0] == ORDINAL(Module::AMBIENT_LIGHT) && resp_register_id == ORDINAL(AmbientLightLtr329Register::OUTPUT)) || 
        (response[0] == ORDINAL(Module::BAROMETER) && resp_register_id == ORDINAL(BarometerBmp280Register::PRESSURE)) || 
        (response[0] == ORDINAL(Module::BAROMETER) && resp_register_id == ORDINAL(BarometerBmp280Register::ALTITUDE)) ||
        (response[0] == ORDINAL(Module::GYRO) && resp_register_id == ORDINAL(GyroBmi160Register::DATA))) {

        try {
            ResponseHeader header(response[0], resp_register_id);
            MblMwData* data= board->response_processors.at(header)(board, response + 2, len - 2);

            metawear_connection->received_sensor_data(board->active_data_signals.at(header), data);
            free(data->value);
            free(data);
        } catch (out_of_range) {
            return MBL_MW_STATUS_WARNING_UNEXPECTED_SENSOR_DATA;
        }
    } 

    return MBL_MW_STATUS_OK;
}
