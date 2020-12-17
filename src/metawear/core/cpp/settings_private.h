#pragma once

#include "metawear/core/metawearboard_fwd.h"
#include <sstream>

void init_settings_module(MblMwMetaWearBoard *board);
void free_settings_module(MblMwMetaWearBoard* board);
void create_settings_uri(const MblMwDataSignal* signal, std::stringstream& uri);