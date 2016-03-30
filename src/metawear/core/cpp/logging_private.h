#pragma once

#include <stdint.h>

void received_time_data(MblMwMetaWearBoard *board, const uint8_t* value, uint8_t length);
void init_logging(MblMwMetaWearBoard *board);
void tear_down_logging(MblMwMetaWearBoard *board);