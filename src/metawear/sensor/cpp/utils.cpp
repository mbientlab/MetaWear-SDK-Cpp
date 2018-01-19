#include "utils.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using std::distance;
using std::fabs;
using std::min_element;
using std::sprintf;
using std::strcpy;
using std::strlen;
using std::transform;
using std::vector;

char* copy_string_index(const char* src, std::uint8_t i) {
    char* buffer = (char*)std::malloc(strlen(src) + 6);
    sprintf(buffer, "%s[%d]", src, i);
    return buffer;
}

char* copy_string(const char* src) {
    char* buffer = (char*)std::malloc(strlen(src));
    strcpy(buffer, src);
    return buffer;
}

uint8_t closest_index(const vector<float>& values, float key) {
    return closest_index(values.data(), values.size(), key);
}

uint8_t closest_index(const float* values, size_t len, float key) {
    vector<float> differences;
    differences.resize(len);

    transform(values, values + len, differences.begin(), [key](const float val) {
        return fabs(val - key);
    });
    return distance(differences.begin(), min_element(differences.begin(), differences.end()));
}