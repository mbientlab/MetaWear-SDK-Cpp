#pragma once

#include <exception>
#include <string>

namespace mbientlab {
namespace cbinds {

/**
 * Indicates an error with a command line option.
 * @author etsai
 */
class CliOptionException : public std::exception {
public:
    /**
     * Constructs a CliOptionException 
     * @param   msg         Error message
     * @param   function    Function name the exception was thrown at
     * @param   lineNo      Line nuimber the exception was thrown at
     */
    CliOptionException(const std::string& msg, const std::string& function, int lineNo);

    /**
     * Get the error message
     * @return Error message for the exception
     */
    const char* what() const throw();

private:
    std::string msg;             ///< Error message for the exception
};

}   //namespace cbinds
}   //namespace mbientlab