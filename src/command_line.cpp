#include <regex>
#include <cstdlib>
#include "libstein/command_line.h"

using namespace libstein;

std::vector<std::string> split(const std::string str, const std::string regex_str)
{
    return { std::sregex_token_iterator(str.begin(), str.end(), std::regex(regex_str), -1), std::sregex_token_iterator() };
}

CommandLine::CommandLine() {}

CommandLine& CommandLine::description(std::string description)
{
    this->_description = description;
    return *this;
}

void CommandLine::verify_if_parameter_exists(std::string parameter)
{
    if (parameter != "")
    {
        if (this->_parameters_already_set.find(parameter)
            != this->_parameters_already_set.end())
        {
            std::string message = "duplicate parameter:";
            throw std::runtime_error{message + parameter};
        }

    }
}

CommandLine& CommandLine::parameter(std::string forms,
                        std::string description,
                        bool mandatory)
{
    auto tokens = split(forms, ",");

    if (    (tokens.size() == 0)
        ||  (tokens.size() > 3)
        ||  (tokens[0] == ""))
    {
        std::string message = "failed to parse parameter:";
        throw std::runtime_error{message + forms};
    }

    auto pos = tokens.begin();

    std::string form_short = *pos++;
    std::string form_long;
    std::string form_environment;

    if (pos != tokens.end()) form_long        = *pos++;
    if (pos != tokens.end()) form_environment = *pos++;

    verify_if_parameter_exists(form_short);
    verify_if_parameter_exists(form_long);
    verify_if_parameter_exists(form_environment);

    // check if exists already
    this->_parameters_already_set.insert(form_short);
    this->_parameters_already_set.insert(form_long);
    this->_parameters_already_set.insert(form_environment);

    this->_parameters.push_back({
                            form_short,
                            form_long,
                            form_environment,
                            description,
                            mandatory});
    return *this;
}

std::string get_environment(std::string environment_variable)
{
    const char* env_var = std::getenv(environment_variable.c_str());

    std::string result = (NULL == env_var)? "" : env_var;
    return result;
}

std::string CommandLine::formatParameter(CommandLineParameter& parameter)
{
    std::string result = "\t";
    result += "-";
    result += parameter._short;
    result += "\t";
    result += parameter._description;

    if (parameter._long != "")
    {
        result += "\n\t\tLong alternative: --";
        result += parameter._long;
    }

    if (parameter._environment != "")
    {
        result += "\n\t\tEnvironment Variable: ";
        result += parameter._environment;
    }

    return result;
}

CommandLineResults CommandLine::eval(Arguments& arguments)
{
    CommandLineResults results;
    results.valid = true;

    for (auto parameters : this->_parameters)
    {
        if (parameters._mandatory)
        {
            if (    !(arguments.isSet(parameters._short))
                &&  !(arguments.isSet(parameters._long))
                &&  !(get_environment(parameters._environment) != ""))
            {
                results.valid = false;
                results.output.push_back(formatParameter (parameters));
            }
        }
    }

    // Builds header to be displayed to the user in "--help" fashion.
    if (false == results.valid)
    {
        auto pos = results.output.insert (results.output.begin(), this->_description + "\n");

        pos = results.output.insert (pos + 1, "Options:");
    }

    return results;
}
