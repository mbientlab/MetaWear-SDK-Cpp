#include "CSharpGenerator.h"

#include <iostream>

using std::endl;
using std::ostream;
using namespace mbientlab::cbinds;

Generator* createCSharpGenerator() {
    return new CSharpGenerator();
}

CSharpGenerator::~CSharpGenerator() {
}

void CSharpGenerator::create(ostream& os, const CHeaderFile& header) {
    os << "This is a csharp generator" << endl;
}
