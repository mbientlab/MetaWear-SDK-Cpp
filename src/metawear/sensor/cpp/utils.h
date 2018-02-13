#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

char* copy_string_index(const char* src, std::uint8_t i);
char* copy_string(const char* src);

uint8_t closest_index(const std::vector<float>& values, float key);
uint8_t closest_index(const float* values, std::size_t len, float key);