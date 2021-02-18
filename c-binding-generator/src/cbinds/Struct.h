#pragma once

#include "Variable.h"

#include <iostream>
#include <string>
#include <vector>

namespace mbientlab {
namespace cbinds {

class Struct {
public:
    virtual ~Struct() = 0;
    virtual const std::string& getName() const = 0;
    virtual const std::vector<Variable*>& getFields() const = 0;
    virtual bool isForwardDecl() const = 0;
    virtual const std::string& getComment() const = 0;
};

std::ostream& operator<<(std::ostream& os, const Struct& fn);

}
}
