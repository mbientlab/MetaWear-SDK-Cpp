#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace mbientlab {
namespace cbinds {

struct EnumEntry {
    std::string comment;
    std::string name;
    std::int32_t value;
};

class Enum {
public:
    virtual ~Enum() = 0;
    virtual const std::string& getName() const = 0;
    virtual const std::vector<EnumEntry>& getEntries() const = 0;
    virtual const std::string& getComment() const = 0;
};

std::ostream& operator<<(std::ostream& os, const EnumEntry& entry);
std::ostream& operator<<(std::ostream& os, const Enum& e);

}
}
