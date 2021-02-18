#pragma once

#include "Function.h"
#include "Type.h"

namespace mbientlab {
namespace cbinds {

class FunctionPointer : public Function, public Type {
public:
    virtual ~FunctionPointer() = 0;
    virtual const std::string& getName() const = 0;
    virtual const std::string& getComment() const = 0;
};

std::ostream& operator<<(std::ostream& os, const FunctionPointer& fn);

}
}
