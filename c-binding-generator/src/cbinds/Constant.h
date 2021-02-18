#pragma once

#include "Type.h"

#include <iostream>
#include <string>
#include <vector>

namespace mbientlab {
namespace cbinds {

struct Constant {
    virtual ~Constant() = 0;

    virtual const std::string& getName() const = 0;
    virtual const Type* getType() const = 0;
    virtual const std::string& getValue() const = 0;
    virtual const std::vector<std::string>& getValueComponents() const = 0;
    virtual const std::string& getComment() const = 0;
};

std::ostream& operator<<(std::ostream& os, const Constant& c);

}
}
