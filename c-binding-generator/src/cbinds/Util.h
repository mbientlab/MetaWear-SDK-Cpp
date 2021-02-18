#pragma once

#include <string>
#include <vector>

namespace mbientlab {
namespace cbinds {

std::string strip(const std::string& original, const std::string& prefix);
std::vector<std::string> split(const std::string& original, char delim);
std::string join(const std::vector<std::string>& parts, char delim);
std::string join(const std::vector<std::string>& parts, char delim, size_t offset);

}
}