#pragma once

#include <stdint.h>
#include "version.h"

const uint8_t NO_DATA_ID= 0xff, DISABLE= 0, ENABLE= 1;
const uint8_t SETTINGS_CONN_PARAMS_REVISION= 1;
const uint8_t MW_CMD_MAX_LENGTH= 18;
const uint8_t PROCESSOR_MAX_LENGTH = 4;

const Version MULTI_CHANNEL_MATH(1, 1, 0), CONST_MATH_OP(1, 1, 0), BUFFER_PROCESSOR(1, 1, 0);