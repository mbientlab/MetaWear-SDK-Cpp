#pragma once

#include <sstream>
#include "metawear/core/metawearboard_fwd.h"

void init_humidity_module(MblMwMetaWearBoard *board);
void create_humidity_uri(const MblMwDataSignal* signal, std::stringstream& uri);