#pragma once

#include <sstream>
#include <vector>

#include "metawear/core/metawearboard_fwd.h"

void init_sensor_fusion_module(MblMwMetaWearBoard* board);
void free_sensor_fusion_module(MblMwMetaWearBoard* board);
void serialize_sensor_fusion_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state);
void deserialize_sensor_fusion_config(MblMwMetaWearBoard *board, uint8_t** state_stream);
void create_sensor_fusion_uri(const MblMwDataSignal* signal, std::stringstream& uri);