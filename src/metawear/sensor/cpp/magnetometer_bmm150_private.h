#pragma once

#include <sstream>

#include "metawear/core/metawearboard_fwd.h"

void init_magnetometer_module(MblMwMetaWearBoard *board);
void free_magnetometer_module(MblMwMetaWearBoard *board);
void create_magnetometer_uri(const MblMwDataSignal* signal, std::stringstream& uri);