#pragma once

#include <stdint.h>

#include "metawear/sensor/accelerometer_bosch.h"

float bosch_get_data_scale(const MblMwMetaWearBoard *board);
void init_accelerometer_bmi160(MblMwMetaWearBoard *board);
void init_accelerometer_bma255(MblMwMetaWearBoard *board);
