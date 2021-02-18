#pragma once

#include "Type.h"
#include "Variable.h"

#include <iostream>
#include <string>
#include <vector>

namespace mbientlab {
namespace cbinds {

class Function {
public:
    virtual ~Function() = 0;
    virtual const std::string& getName() const = 0;
    virtual const std::vector<Variable*>& getParameters() const = 0;
    virtual const Type* getReturnType() const = 0;
    virtual const std::string& getComment() const = 0;
};

std::ostream& operator<<(std::ostream& os, const Function& fn);

}
}
