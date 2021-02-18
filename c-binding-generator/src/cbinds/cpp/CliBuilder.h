#pragma once

#include "CliOptionException.h"
#include "Option.h"

#include <unordered_map>
#include <unordered_set>
#include <string>

namespace mbientlab {
namespace cbinds {

/**
 * Builder for processing command line arguments
 * @author etsai
 */
class CliBuilder {
public:
    /**
     * Get the builder object
     * @return  CliBuilder instance
     */
    static CliBuilder* getBuilder();

    /**
     * Add an option to the collection
     * @param   opt     Option to add
     * @return  Reference to the calling object
     * @throw   CliOptionException   If a duplicate option has already been acted
     */
    CliBuilder& addOption(const Option& opt);
    /**
     * Set the usage message
     * @param   usage   Usage description
     * @return  Reference to the calling object
     */
    CliBuilder& setUsage(const std::string& usage);

    /**
     * Parse the command line arguments and execute the callback functions
     * @param   argc    Number of arguments
     * @param   argv    Array of arguments passed in
     * @throw   CliOptionException   If invalid option was passed in or not enough 
     * arguments were given for an option
     */
    void parse(int argc, char **argv);
    /**
     * Print the usage message, and then terminate
     */
    void displayUsage();

private:
    CliBuilder();                               ///< Default constructor.  Hidden to make singleton

    static CliBuilder instance;                 ///< Single instance of the class
    std::unordered_map<std::string, Option> options;      ///< Maps unique names to option parameters
    std::unordered_set<Option*> uniqueOpts;          ///< Set of unique options
    std::string usage;                               ///< Usage message for the program
};  //class CliBuilder

}   //namesapce cpputilities
}   //namespace etsai
