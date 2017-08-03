#pragma once

#include <stdint.h>
#include "metawear/processor/dataprocessor_fwd.h"

uint8_t get_accounter_length(const MblMwDataProcessor* source);
uint8_t get_accounter_prescale(const MblMwDataProcessor* source);