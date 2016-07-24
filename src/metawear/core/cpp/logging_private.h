#pragma once

#include <stdint.h>
#include <vector>

void init_logging(MblMwMetaWearBoard *board);
void tear_down_logging(void *state, bool preserve_memory);
void serialize_logging(const MblMwMetaWearBoard* board, std::vector<uint8_t>& state);
void deserialize_logging(MblMwMetaWearBoard* board, uint8_t** state_stream);
