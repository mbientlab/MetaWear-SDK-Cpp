#pragma once

void init_macro_module(MblMwMetaWearBoard *board);
void free_macro_module(void *state);
void record_macro(const MblMwMetaWearBoard *board, const uint8_t* command, uint8_t len);
