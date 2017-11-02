#pragma once

#include <stdint.h>

#define ORDINAL(x) static_cast<uint8_t>(x)
#define READ_REGISTER(x) (0x80 | x)
#define CLEAR_READ(x) (0x3f & x)