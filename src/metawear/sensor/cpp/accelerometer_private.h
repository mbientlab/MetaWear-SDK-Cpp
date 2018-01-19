#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "metawear/core/metawearboard_fwd.h"

void init_accelerometer_module(MblMwMetaWearBoard *board);
void free_accelerometer_module(MblMwMetaWearBoard *board);
void serialize_accelerometer_config(const MblMwMetaWearBoard *board, std::vector<uint8_t>& state);
void deserialize_accelerometer_config(MblMwMetaWearBoard *board, uint8_t** state_stream);
void create_acc_uri(const MblMwDataSignal* signal, std::stringstream& uri);