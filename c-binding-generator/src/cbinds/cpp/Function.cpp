#include "cbinds/Function.h"

using std::ostream;

namespace mbientlab {
namespace cbinds {

Function::~Function() { }

ostream& operator<<(ostream& os, const Function& fn) {
    os << "{name: " << fn.getName() << ", parameters: [";

    bool first = true;
    for(auto it: fn.getParameters()) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << *it;
    }
    os << "], return: " << *(fn.getReturnType()) << ", comment: " << fn.getComment() << "}";

    return os;
}

}
}
