#pragma once

#include <stdint.h>
#include <vector>

#include "metawear/core/anonymous_datasignal_fwd.h"

extern const double TICK_TIME_STEP;

void init_logging(MblMwMetaWearBoard *board);
void tear_down_logging(void *state, bool preserve_memory);
void serialize_logging(const MblMwMetaWearBoard* board, std::vector<uint8_t>& state);
void deserialize_logging(MblMwMetaWearBoard* board, uint8_t format, uint8_t** state_stream);
void disconnect_logging(MblMwMetaWearBoard* board);
void query_active_loggers(MblMwMetaWearBoard* board);
int64_t calculate_epoch(const MblMwMetaWearBoard* board, uint32_t tick);
void mbl_mw_logging_set_latest_reset_uid(const MblMwMetaWearBoard* board, uint8_t reset_uid);
