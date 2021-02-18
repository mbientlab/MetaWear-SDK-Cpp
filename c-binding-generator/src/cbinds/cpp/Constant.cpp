#include "cbinds/Constant.h"

using std::ostream;

namespace mbientlab {
namespace cbinds {

Constant::~Constant() { }

ostream& operator<<(ostream& os, const Constant& c) {
    os << "{name: " << c.getName() << ", type: " << *(c.getType()) << ", value: " << c.getValue();
    
    os << ", value_components: [";
    bool first = true;
    for(auto it: c.getValueComponents()) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << it;
        
    }
    os << "], comment: " << c.getComment() << "}";
    return os;
}

}
}
