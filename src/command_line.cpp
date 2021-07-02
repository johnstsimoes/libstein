#include <regex>

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

CommandLine& CommandLine::parameter(std::string forms,
                        std::string description,
                        bool mandatory)
{
    auto tokens = split(forms, " ");

    //TODO validate between 1-3

    std::string form_short = tokens[0];
    std::string form_long;
    std::string form_environment;

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
    std::string result;
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
                results.output.push_back(parameters._description);
            }
        }
    }

    if (false == results.valid)
    {
        auto pos = results.output.insert (results.output.begin(), this->_description);
        results.output.insert (pos, "\n");
    }

    return results;
}
