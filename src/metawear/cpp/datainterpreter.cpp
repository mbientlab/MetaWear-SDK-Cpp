#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "metawear/types.h"

#include "datainterpreter.h"
#include "metawearboard_def.h"

using std::calloc;
using std::malloc;
using std::memcpy;
using std::memset;
using std::min;

#define CREATE_MESSAGE(dt_id) MblMwData *msg= (MblMwData*) malloc(sizeof(MblMwData));\
    msg->value= value;\
    msg->type_id= dt_id;\
    return msg

static const float BMP280_SCALE= 256.f;

MblMwData* convert_to_uint32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t *value= (uint32_t*) calloc(1, sizeof(uint32_t));
    memcpy(value, response, min(len, (uint8_t) sizeof(value)));

    CREATE_MESSAGE(MBL_MW_DT_ID_UINT32);
}

MblMwData* convert_to_bmp280_pressure(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / BMP280_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

MblMwData* convert_to_bmp280_altitude(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / BMP280_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}

MblMwData* convert_to_bmi160_rotation(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    CartesianFloat *value= (CartesianFloat*) malloc(sizeof(CartesianFloat));
    value->x= unscaled.x / board->gyro_bmi160_config.get_scale();
    value->y= unscaled.y / board->gyro_bmi160_config.get_scale();
    value->z= unscaled.z / board->gyro_bmi160_config.get_scale();

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

MblMwData* convert_to_bmi160_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    CartesianFloat *value= (CartesianFloat*) malloc(sizeof(CartesianFloat));
    value->x= unscaled.x / board->acc_bmi160_config.get_scale();
    value->y= unscaled.y / board->acc_bmi160_config.get_scale();
    value->z= unscaled.z / board->acc_bmi160_config.get_scale();

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

MblMwData* convert_to_mma8452q_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort milliG;
    memcpy(&milliG, response, sizeof(milliG));

    CartesianFloat *value= (CartesianFloat*) malloc(sizeof(CartesianFloat));
    value->x= milliG.x / 1000.f;
    value->y= milliG.y / 1000.f;
    value->z= milliG.z / 1000.f;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}
