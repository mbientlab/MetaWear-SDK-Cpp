#include "Option.h"

namespace mbientlab {
namespace cbinds {

Option::Option() {
}

Option::Option(const std::string& optName, const OptCallback& callback) : 
    optName(optName), separator(0), args(0), callback(callback), required(false) {
}

Option::Option(const std::string& optName, char separator, int args, const OptCallback& callback) : 
    optName(optName), separator(separator), args(args), callback(callback), required(false) {
}
    
Option& Option::operator=(const Option &rhs) {
    if (this != &rhs) {
        this->optName= rhs.optName;
        this->separator= rhs.separator;
        this->args= rhs.args;
        this->required= rhs.required;
        this->description= rhs.description;
        this->argName= rhs.argName;
        this->longOpt= rhs.longOpt;
        this->callback= rhs.callback;
    }

    return *this;
}

Option& Option::withDescription(const std::string& description) {
    this->description= description;
    return *this;
}

Option& Option::withArgName(const std::string& argName) {
    this->argName= argName;
    return *this;
}

Option& Option::withLongOpt(const std::string& longOpt) {
    this->longOpt= longOpt;
    return *this;
}

Option& Option::withRequired(bool required) {
    this->required= required;
    return *this;
}

}
}
