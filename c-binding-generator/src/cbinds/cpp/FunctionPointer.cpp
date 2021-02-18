#include "cbinds/FunctionPointer.h"

using std::ostream;

namespace mbientlab {
namespace cbinds {

FunctionPointer::~FunctionPointer() { }

ostream& operator<<(ostream& os, const FunctionPointer& fPointer) {
    os << "{name: " << fPointer.getName() << ", is_pointer: " << fPointer.isPointer() << ", parameters: [";

    bool first = true;
    for(auto it: fPointer.getParameters()) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << *it;
    }

    os << "], return: ";
    auto retType = fPointer.getReturnType();
    if(retType == nullptr) {
        os << "void";
    } else {
        os << *retType;
     }
     os << ", comment: " << fPointer.getComment() << "}";

    return os;
}

}
}
