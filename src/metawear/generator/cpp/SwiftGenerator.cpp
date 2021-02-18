#include "SwiftGenerator.h"
#include "cbinds/Util.h"

#include <cctype>
#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::endl;
using std::function;
using std::isdigit;
using std::isupper;
using std::ostream;
using std::string;
using std::stringstream;
using std::toupper;
using std::unordered_map;
using std::unordered_set;

using namespace mbientlab::cbinds;

Generator* createSwiftGenerator() {
    return new SwiftGenerator();
}

SwiftGenerator::~SwiftGenerator() {

}

void SwiftGenerator::create(ostream& os, const CHeaderFile& header) {
    for(auto it: header.getConstants()) {
        if (!header.getStructs().count(it.second->getType()->getName())) {
            os << "public let " << strip(it.second->getName(), "MBL_MW_") << " = " << it.second->getValue() << endl;
        }
    }
}
