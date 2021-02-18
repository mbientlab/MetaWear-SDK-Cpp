#include "DummyGenerator.h"

using std::ostream;
using std::endl;

namespace mbientlab {
namespace cbinds {

DummyGenerator::~DummyGenerator() { }

void DummyGenerator::create(ostream& os, const CHeaderFile& header) {
    os << "Enum" << endl << "====" << endl;
    for(auto const& it : header.getEnums()) {
        os << *(it.second) << endl;
    }

    os << "Structs" << endl << "=======" << endl;
    for(auto const& it : header.getStructs()) {
        os << *(it.second) << endl;
    }

    os << "Constants" << endl << "=========" << endl;
    for(auto const& it : header.getConstants()) {
        os << *(it.second) << endl;
    }

    os << "Functions" << endl << "=========" << endl;
    for(auto const& it : header.getFunctions()) {
        os << *(it.second) << endl;
    }

    os << "Function Pointers" << endl << "=================" << endl;
    for(auto const& it : header.getFunctionPointers()) {
        os << *(it.second) << endl;
    }
}

}
}
