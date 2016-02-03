#pragma once

#include <stdint.h>

#define ORDINAL(x) static_cast<uint8_t>(x)
#define READ_REGISTER(x) (0x80 | x)

const uint8_t INFO_REGISTER= 0x0, READ_INFO_REGISTER= READ_REGISTER(INFO_REGISTER);
