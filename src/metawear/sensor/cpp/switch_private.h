#pragma once

#include "metawear/core/metawearboard_fwd.h"
#include <sstream>

void init_switch_module(MblMwMetaWearBoard *board);
void create_switch_uri(const MblMwDataSignal* signal, std::stringstream& uri);