#include "CliBuilder.h"

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>

namespace mbientlab {
namespace cbinds {

using std::cout;
using std::endl;
using std::exit;
using std::map;
using std::string;
using std::stringstream;
using std::unordered_map;

CliBuilder CliBuilder::instance;

CliBuilder* CliBuilder::getBuilder() {
    return &instance;
}

CliBuilder& CliBuilder::addOption(const Option& opt) {
    stringstream excep(stringstream::out);

    if (options.count(opt.optName) != 0) {
        excep << "Option " << opt.optName << " already registered";
        throw CliOptionException(excep.str(), __FUNCTION__, __LINE__);
    }
    
    options[opt.optName]= opt;
    if (!opt.longOpt.empty()) {
        if (options.count(opt.longOpt) != 0) {
            excep << "Option " << opt.optName << "(" << opt.longOpt << ") already registered";
            throw CliOptionException(excep.str(), __FUNCTION__, __LINE__);
        }
        options[opt.longOpt]= opt;
    }
    uniqueOpts.insert(&options[opt.optName]);
    return *this;
}

CliBuilder& CliBuilder::setUsage(const string& usage) {
    this->usage= usage;
    return *this;
}

void CliBuilder::parse(int argc, char **argv) {
    int i= 0;
    unordered_map<string, Option> optionsCpy(options);
    stringstream excep;

    while(i < argc) {
        if (argv[i][0] == '-') {
            if (options.count(argv[i]) == 0 && !excep.tellp()) {
                excep << "Invalid option read (" << argv[i] << ")";
            } else {
                Option *opt= &options[argv[i]];
                Arguments args;

                optionsCpy.erase(argv[i]);
                if (!opt->longOpt.empty()) {
                    optionsCpy.erase(opt->longOpt);
                }
                if (opt->args == Option::NARGS_UNLIMITED || opt->args > 0) {
                    i++;
                    if (i < argc && (argv[i][0] != '-' || options.count(argv[i]) == 0)) {
                        args.addArgs(argv[i], opt->separator);
                    } else {
                        i--;
                    }
                    if ((int) args.size() != opt->args && opt->args != Option::NARGS_UNLIMITED && !excep.tellp()) {
                        excep << "Option (" << opt->optName << ") requires " << opt->args << " arguments, was given " << args.size();
                    }
                }
                opt->callback(args);
            }
        }
        i++;
    }
    if (excep.tellp()) {
        throw CliOptionException(excep.str(), __FUNCTION__, __LINE__);
    }
    for(auto it= optionsCpy.begin(); it != optionsCpy.end(); it++) {
        if ((it->second).required) {
            throw CliOptionException("Required option was not used (" + it->second.optName + ")", __FUNCTION__, __LINE__);
        }
    }
}

void CliBuilder::displayUsage() {
    unsigned int maxOptionLength= 0;
    map<string,Option*> displayNames;
    auto displayOption= [](const Option& opt) -> string {
        string display= " ";
    
        display+= opt.optName;
        if (!opt.longOpt.empty()) {
            display+= ", " + opt.longOpt;
        }
        if (!opt.argName.empty()) {
            display+= " <" + opt.argName + ">";
        }
        return display;
    };

    for(auto opt: uniqueOpts) {
        string display(displayOption(*opt));
        displayNames[display]= opt;
        auto displayLength= display.size();
        maxOptionLength= maxOptionLength > displayLength ? maxOptionLength : displayLength;
    }
    cout << "usage: " << usage << endl;
    for(auto &names: displayNames) {
        string filler(maxOptionLength - names.first.length(), ' ');

        cout << names.first << filler << "\t" << names.second->description << endl;
    }
    exit(0);
}

CliBuilder::CliBuilder() {
}

}
}
