#pragma once

#include <sstream>
#include <vector>

#include "metawear/core/metawearboard_fwd.h"

void init_colordetector_module(MblMwMetaWearBoard *board);
void serialize_colordetector_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state);
void deserialize_colordetector_config(MblMwMetaWearBoard *board, uint8_t** state_stream);
void create_colordetector_uri(const MblMwDataSignal* signal, std::stringstream& uri);