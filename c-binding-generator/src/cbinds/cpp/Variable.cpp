#include "cbinds/Variable.h"

using std::ostream;

namespace mbientlab {
namespace cbinds {

Variable::~Variable() { }

ostream& operator<<(ostream& os, const Variable& var) {
    os << "{name: " << var.getName() << ", type: " << var.getType() << ", comment: " << var.getComment() << "}";
    return os;
}

}
}
