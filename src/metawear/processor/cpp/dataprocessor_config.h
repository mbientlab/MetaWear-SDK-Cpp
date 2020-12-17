#pragma once

#include "metawear/core/cpp/version.h"

#include "metawear/processor/dataprocessor_fwd.h"

#include <stdint.h>

const uint8_t ACCOUNTER_COUNT = 0, ACCOUNTER_TIME = 1;

struct AccounterConfig {
    uint8_t mode:4;
    uint8_t length:2;
    uint8_t :2;
    uint8_t prescale:4;
    uint8_t :4;
};

uint8_t get_accounter_type(const MblMwDataProcessor* source);
uint8_t get_accounter_length(const MblMwDataProcessor* source);
uint8_t get_accounter_prescale(const MblMwDataProcessor* source);

const uint8_t ACCUMULATOR_SUM = 0, ACCUMULATOR_COUNT = 1;

struct AccumulatorConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t mode:3;
};

const uint8_t HPF_REVISION = 2, USE_HPF = 1;

struct AverageConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t :1;
    uint8_t mode:1;
    uint8_t :2;
    uint8_t size;
    uint8_t count;
};

struct BufferConfig {
    uint8_t length:5;
};

const int8_t COMBINER_RMS= 0, COMBINER_RSS= 1;

struct CombinerConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t count:3;
    uint8_t is_signed:1;
    uint8_t mode;
};

const Version MULTI_COMPARE(1, 2, 3);

struct ComparatorConfig {
    uint8_t is_signed;
    uint8_t operation;
    uint8_t :8;
    uint8_t reference[4];
};

struct MultiComparatorConfig {
    uint8_t is_signed:1;
    uint8_t length:2;
    uint8_t operation:3;
    uint8_t mode:2;
    uint8_t references[12];
};

struct DeltaConfig {
    uint8_t length:2;
    uint8_t is_signed:1;
    uint8_t mode:3;
    uint8_t magnitude[4];
};

const Version MULTI_CHANNEL_MATH(1, 1, 0), CONST_MATH_OP(1, 1, 0);

struct MathConfig {
    uint8_t output:2;
    uint8_t input:2;
    uint8_t is_signed:1;
    uint8_t :3;
    uint8_t operation;
    uint8_t rhs[4];
    uint8_t n_channels;
};

struct PackerConfig {
    uint8_t length:5;
    uint8_t :3;
    uint8_t count:5;
    uint8_t :3;
};

uint8_t get_packer_count(const MblMwDataProcessor* source);
uint8_t get_packer_length(const MblMwDataProcessor* source);

struct PassthroughConfig {
    uint8_t mode;
    uint8_t count[2];
};

struct PulseDetectorConfig {
    uint8_t length;
    uint8_t trigger_mode;
    uint8_t output_mode;
    uint8_t threshold[4];
    uint8_t width[2];
};

struct SampleDelayConfig {
    uint8_t length;
    uint8_t bin_size;
};

struct ThresholdConfig {
    uint8_t length:2;
    uint8_t is_signed:1;
    uint8_t mode:3;
    uint8_t boundary[4];
    uint8_t hysteresis[2];
};

struct TimeDelayConfig {
    uint8_t length:3;
    uint8_t mode:3;
    uint8_t :2;
    uint8_t period[4];
};

struct FuseConfig {
    uint8_t count:4;
    uint8_t:4;
    uint8_t references[12];
};