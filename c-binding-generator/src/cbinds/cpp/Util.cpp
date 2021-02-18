#include "cbinds/Util.h"

#include <numeric>

using std::accumulate;
using std::next;
using std::string;
using std::vector;

namespace mbientlab {
namespace cbinds {

string strip(const string& original, const string& prefix) {
    auto pos = original.find(prefix);
    return pos == 0 ? original.substr(pos + prefix.length(), string::npos) : original;
}

vector<string> split(const string& original, char delim) {
    vector<string> result;
    string part;

    for(auto ch: original) {
        if (ch != delim) {
            part+= ch;
        } else {
            result.push_back(part);
            part.clear();
        }
    }
    if (!part.empty()) {
        result.push_back(part);
    }

    return result;
}

string join(const vector<string>& parts, char delim) {
    return join(parts, delim, 0);
}

string join(const vector<string>& parts, char delim, size_t offset) {
    auto first = parts.begin();
    for(size_t i = 0; i <= offset; i++) {
        first = next(first);
    }
    return accumulate(first, parts.end(), parts[offset], [delim](string acc, string elem) { return acc + delim + elem; });
}

}
}
