#pragma once

#include <vector>

#include "metawear/core/metawearboard_fwd.h"

void init_proximity_module(MblMwMetaWearBoard *board);
void serialize_proximity_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state);
void deserialize_proximity_config(MblMwMetaWearBoard *board, uint8_t** state_stream);