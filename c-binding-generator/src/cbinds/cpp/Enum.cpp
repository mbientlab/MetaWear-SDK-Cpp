#include "cbinds/Enum.h"

using std::ostream;

namespace mbientlab {
namespace cbinds {

Enum::~Enum() { }

ostream& operator<<(ostream& os, const EnumEntry& entry) {
    os << "{name: " << entry.name << ", value: " << entry.value << ", comment: " << entry.comment << "}";
    return os;
}

ostream& operator<<(ostream& os, const Enum& e) {
    os << "{name: " << e.getName() << ", entries: [";

    bool first = true;
    for(auto it: e.getEntries()) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << it;
        
    }
    os << "], comment: " << e.getComment() << "}";
    return os;
}

}
}
