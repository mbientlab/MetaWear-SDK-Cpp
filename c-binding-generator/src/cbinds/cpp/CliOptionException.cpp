#include "CliOptionException.h"

#include <sstream>

namespace mbientlab {
namespace cbinds {

using std::string;
using std::stringstream;

CliOptionException::CliOptionException(const string& msg, const string& function, int lineNo)  {
    stringstream formattedMsg;
    formattedMsg << "CliOptionException - " << msg << " (in " << function << " line " << lineNo << ")";
    this->msg= formattedMsg.str();
}

const char* CliOptionException::what() const throw() {
    return msg.c_str();
}

}   //namespace cbinds
}   //namespace mbientlab