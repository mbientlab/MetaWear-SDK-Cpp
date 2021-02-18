#include "Arguments.h"
#include "cbinds/Util.h"

#include <cstdlib>

namespace mbientlab {
namespace cbinds {

using std::string;
using std::atoi;
using std::atof;

Arguments::Arguments() {
}

void Arguments::addArgs(const string& argStr, char separator) {
    argList = split(argStr, separator);
}

size_t Arguments::size() const {
    return argList.size();
}

double Arguments::asDouble(int index) const {
    return atof(argList[index].c_str());
}

int Arguments::asInteger(int index) const {
    return atoi(argList[index].c_str());
}

string Arguments::asString(int index) const {
    return argList[index];
}

}
}
