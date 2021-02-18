#pragma once

#include "Constant.h"
#include "Enum.h"
#include "Function.h"
#include "FunctionPointer.h"
#include "Struct.h"

#include <unordered_map>

namespace mbientlab {
namespace cbinds {

class CHeaderFile {
public:
    virtual ~CHeaderFile() = 0;

    virtual const std::unordered_map<std::string, Enum*>& getEnums() const = 0;
    virtual const std::unordered_map<std::string, FunctionPointer*>& getFunctionPointers() const = 0;
    virtual const std::unordered_map<std::string, Constant*>& getConstants() const = 0;
    virtual const std::unordered_map<std::string, Struct*>& getStructs() const = 0;
    virtual const std::unordered_map<std::string, Function*>& getFunctions() const = 0;
};

}
}