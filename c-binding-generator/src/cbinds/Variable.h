#pragma once

#include "Type.h"

#include <iostream>
#include <string>

namespace mbientlab {
namespace cbinds {

class Variable {
public:
    virtual ~Variable() = 0;
    virtual const std::string& getComment() const = 0;
    virtual const std::string& getName() const = 0;
    virtual const Type& getType() const = 0;
};

std::ostream& operator<<(std::ostream& os, const Variable& var);

}
}
