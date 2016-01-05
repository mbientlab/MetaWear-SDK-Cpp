#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "metawear/core/types.h"

#include "datainterpreter.h"
#include "metawearboard_def.h"

using std::calloc;
using std::malloc;
using std::memcpy;
using std::memset;
using std::min;

static const float BMP280_SCALE= 256.f, TEMPERATURE_SCALE= 8.f, MMA8452Q_ACC_SCALE= 1000.f;

MblMwData* convert_to_int32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t *value = (int32_t*)malloc(sizeof(int32_t));

    if ((response[len - 1] & 0x80) == 0x80) {
        memset(value, 0xff, sizeof(int32_t));
    } else {
        memset(value, 0x0, sizeof(int32_t));
    }
    memcpy(value, response, min(len, (uint8_t) sizeof(value)));

    CREATE_MESSAGE(MBL_MW_DT_ID_INT32);
}

MblMwData* convert_to_uint32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    uint32_t *value= (uint32_t*) calloc(1, sizeof(uint32_t));
    memcpy(value, response, min(len, (uint8_t) sizeof(value)));

    CREATE_MESSAGE(MBL_MW_DT_ID_UINT32);
}

MblMwData* convert_to_temperature(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int16_t unscaled;
    memcpy(&unscaled, response, sizeof(unscaled));

    float *value= (float*) malloc(sizeof(float));
    *value= unscaled / TEMPERATURE_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
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

MblMwData* convert_to_mma8452q_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    CartesianShort milliG;
    memcpy(&milliG, response, sizeof(milliG));

    CartesianFloat *value= (CartesianFloat*) malloc(sizeof(CartesianFloat));
    value->x= milliG.x / MMA8452Q_ACC_SCALE;
    value->y= milliG.y / MMA8452Q_ACC_SCALE;
    value->z= milliG.z / MMA8452Q_ACC_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_CARTESIAN_FLOAT);
}

MblMwData* convert_to_mma8452q_acceleration_single_axis(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    int32_t unscaled;
    if ((response[len - 1] & 0x80) == 0x80) {
        unscaled= -1;
    } else {
        unscaled= 0;
    }
    memcpy(&unscaled, response, min(len, (uint8_t) sizeof(unscaled)));

    float *value = (float*) malloc(sizeof(float));
    *value = unscaled / MMA8452Q_ACC_SCALE;

    CREATE_MESSAGE(MBL_MW_DT_ID_FLOAT);
}
