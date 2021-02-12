#pragma once

#include <sstream>
#include <vector>

#include "metawear/core/metawearboard_fwd.h"

float bosch_gyro_get_data_scale(const MblMwMetaWearBoard *board);

void init_gyro_module(MblMwMetaWearBoard *board);
void free_gyro_module(MblMwMetaWearBoard *board);
void serialize_gyro_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state);
void deserialize_gyro_config(MblMwMetaWearBoard *board, uint8_t** state_stream);
void create_gyro_uri(const MblMwDataSignal* signal, std::stringstream& uri);