#pragma once

#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, std::string> SHORTENED_PREFIXES;

std::string createPrefix(const std::string& in);