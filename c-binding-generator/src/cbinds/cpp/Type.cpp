#include "cbinds/Type.h"

using std::ostream;

namespace mbientlab {
namespace cbinds {

Type::~Type() { }

ostream& operator<<(ostream& os, const Type& type) {
    os << "{name: " << type.getName() 
        << ", is_pointer: " << type.isPointer() 
        << ", is_array: " << type.isArray() 
        << ", array_size: " << type.getArraySize() << "}";
    return os;
}

}
}
