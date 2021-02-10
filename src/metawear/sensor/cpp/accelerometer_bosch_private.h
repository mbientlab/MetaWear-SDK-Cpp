#pragma once

#include <sstream>
#include <stdint.h>
#include <vector>

#include "metawear/core/metawearboard_fwd.h"

float bosch_get_data_scale(const MblMwMetaWearBoard *board);

void free_accelerometer_bosch(MblMwMetaWearBoard *board);
void init_accelerometer_bmi160(MblMwMetaWearBoard *board);
void init_accelerometer_bma255(MblMwMetaWearBoard *board);
void init_accelerometer_bmi270(MblMwMetaWearBoard *board);

void serialize_accelerometer_bmi160_config(const MblMwMetaWearBoard* board, std::vector<uint8_t>& state);
void serialize_accelerometer_bma255_config(const MblMwMetaWearBoard* board, std::vector<uint8_t>& state);
void serialize_accelerometer_bmi270_config(const MblMwMetaWearBoard* board, std::vector<uint8_t>& state);

void deserialize_accelerometer_bmi160_config(MblMwMetaWearBoard* board, uint8_t** state_stream);
void deserialize_accelerometer_bma255_config(MblMwMetaWearBoard* board, uint8_t** state_stream);
void deserialize_accelerometer_bmi270_config(MblMwMetaWearBoard* board, uint8_t** state_stream);

void read_accelerometer_bosch_acceleration_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed);
void create_acc_bosch_uri(const MblMwDataSignal* signal, std::stringstream& uri);
void create_acc_bmi270_uri(const MblMwDataSignal* signal, std::stringstream& uri);