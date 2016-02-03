#pragma once

#include <stdint.h>
#include <string>

struct Version {
    Version();
    Version(uint8_t major, uint8_t minor, uint8_t step);

    bool empty() const;
    void assign(const std::string new_version);
    Version& operator =(const Version& original);

    uint8_t major, minor, step;
};

bool operator <(const Version& left, const Version& right);
bool operator ==(const Version& left, const Version& right);
