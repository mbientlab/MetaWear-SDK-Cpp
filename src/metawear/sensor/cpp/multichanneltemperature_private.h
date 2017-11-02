#pragma once

#include "metawear/core/metawearboard_fwd.h"

#include <sstream>

void init_multichannel_temp_module(MblMwMetaWearBoard *board);
void create_temp_uri(const MblMwDataSignal* signal, std::stringstream& uri);