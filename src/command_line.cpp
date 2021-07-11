#include <cstdlib>
#include <cassert>
#include "libstein/command_line.h"
#include "libstein/string_utils.h"

using namespace libstein;

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
        if (    (this->_parameters.find(parameter) != this->_parameters.end())
            ||  (this->_environment_parameters.find(parameter) != this->_environment_parameters.end()))
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
    auto tokens = stringutils::split(forms, ",");

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

    auto parameter = std::make_shared<CommandLineParameter>(CommandLineParameter{
                            form_short,
                            form_long,
                            form_environment,
                            description,
                            mandatory,
                            "",
                            false});
    
    safe_add_parameter(parameter);
 
    return *this;
}

void CommandLine::safe_add_parameter(std::shared_ptr<CommandLineParameter>& parameter)
{
    assert(parameter != nullptr);

    if (parameter->_short       != "") this->_parameters[parameter->_short] = parameter;
    if (parameter->_long        != "") this->_parameters[parameter->_long]  = parameter;

    if (parameter->_environment != "") this->_environment_parameters[parameter->_environment] = parameter;
}

std::string CommandLine::formatParameter(std::shared_ptr<CommandLineParameter>& parameter)
{
    std::string result = "\t";
    result += "-";
    result += parameter->_short;
    result += "\t";
    result += parameter->_description;

    if (parameter->_long != "")
    {
        result += "\n\t\tLong alternative: --";
        result += parameter->_long;
    }

    if (parameter->_environment != "")
    {
        result += "\n\t\tEnvironment Variable: ";
        result += parameter->_environment;
    }

    return result;
}

CommandLineResults CommandLine::eval(Arguments& arguments)
{
    CommandLineResults results;
    results.valid = true;

    // Since short,long and environment variable all point to the same CommandLineParameter
    // struct, we need to reduce the map to the unique values.
    std::set<std::shared_ptr<CommandLineParameter>> unique_parameters;
    for (auto const &parameters : this->_parameters)
    {
        unique_parameters.insert(parameters.second);
    }

    // Fill parameters with values from arguments.
    auto argument_parameters = arguments.getParameters();

    for (auto &parameter: argument_parameters)
    {
        auto command_line_parameters = this->_parameters.find(parameter.first);

        if (this->_parameters.end() != command_line_parameters)
        {
            command_line_parameters->second->_value = parameter.second;
            command_line_parameters->second->_is_set = true;
        }
        else // do not accept unknown parameters
        {
            results.valid = false;
            std::string error_message = "Undefined parameter:";
            results.output.push_back(error_message + parameter.first);
        }
    }

    // Check for missing mandatory parameters.
    for (auto parameters : unique_parameters)
    {
        if (parameters->_mandatory)
        {
            if (    !(arguments.isSet(parameters->_short))
                &&  !(arguments.isSet(parameters->_long))
                &&  !(stringutils::get_environment(parameters->_environment) != ""))
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

int CommandLine::count()
{
    std::string empty;
    std::set<std::shared_ptr<CommandLineParameter> > unique_values;

    for (auto const &parameter : this->_parameters)
    {
        if (parameter.second->_is_set)
            unique_values.insert(parameter.second);
    }
    return unique_values.size();
}

std::string CommandLine::getParameter(std::string parameter)
{
    std::string result = "";

    auto command_line_parameters = this->_parameters.find(parameter);
    if (this->_parameters.end() != command_line_parameters)
    {
        result = command_line_parameters->second->_value;
    }
    return result;
}

bool CommandLine::isSet(std::string parameter)
{
    bool result = false;

    auto command_line_parameters = this->_parameters.find(parameter);
    if (this->_parameters.end() != command_line_parameters)
    {
        result = command_line_parameters->second->_is_set;
    }
    return result;
}

bool CommandLine::hasValue(std::string parameter)
{
    bool result = false;

    auto command_line_parameters = this->_parameters.find(parameter);
    if (this->_parameters.end() != command_line_parameters)
    {
        result = command_line_parameters->second->_value != "";
    }
    return result;
}
