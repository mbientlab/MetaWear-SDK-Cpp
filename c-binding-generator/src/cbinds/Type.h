#pragma once

#include <iostream>
#include <string>

namespace mbientlab {
namespace cbinds {

class Type {
public:
    virtual ~Type() = 0;
    virtual const std::string& getName() const = 0;
    virtual bool isPointer() const = 0;
    virtual bool isArray() const = 0;
    virtual long long getArraySize() const = 0;
};

std::ostream& operator<<(std::ostream& os, const Type& type);

}
}
