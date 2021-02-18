#include "cbinds/Struct.h"

using std::ostream;

namespace mbientlab {
namespace cbinds {

Struct::~Struct() { }

ostream& operator<<(ostream& os, const Struct& fn) {
    os << "{name: " << fn.getName() << ", forward_decl: " << fn.isForwardDecl() << ", fields: [";

    bool first = true;
    for(auto it: fn.getFields()) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << *it;
    }
    os << "], comment: " << fn.getComment() << "}";

    return os;
}

}
}
