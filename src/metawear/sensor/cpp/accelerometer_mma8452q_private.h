#pragma once

#include <stdint.h>
#include <vector>

#include "metawear/core/metawearboard_fwd.h"

void init_accelerometer_mma8452q(MblMwMetaWearBoard *board);
void serialize_accelerometer_mma8452q_config(const MblMwMetaWearBoard* board, std::vector<uint8_t>& state);
void deserialize_accelerometer_mma8452q_config(MblMwMetaWearBoard* board, uint8_t** state_stream);
