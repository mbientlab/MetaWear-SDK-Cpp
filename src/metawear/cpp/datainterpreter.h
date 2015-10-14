#pragma once

#include "metawear/data.h"
#include "metawear/metawearboard_fwd.h"

MblMwData* convert_to_uint32(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmp280_pressure(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmp280_altitude(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmi160_rotation(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_bmi160_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
MblMwData* convert_to_mma8452q_acceleration(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);
