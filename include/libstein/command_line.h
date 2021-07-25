#pragma once

#include <vector>
#include <string>
#include <set>
#include <memory>

#include "arguments.h"

namespace libstein
{
    struct CommandLineResults
    {
        std::vector<std::string> output;
        bool valid = false;
    };

    class CommandLine
    {
    private:

        std::string _description;

        struct CommandLineParameter
        {
            std::string _short;
            std::string _long;
            std::string _environment;
            std::string _description;
            bool _mandatory;
            std::string _value;
            bool _is_set;
        };
        std::map<std::string, std::shared_ptr<CommandLineParameter>> _parameters;
        std::map<std::string, std::shared_ptr<CommandLineParameter>> _environment_parameters;

        void safe_add_parameter(std::shared_ptr<CommandLineParameter>& parameter);
        void verify_if_parameter_exists(std::string parameter);
        std::string formatParameter(std::shared_ptr<CommandLineParameter>& parameter);

    public:
        CommandLine();

        CommandLine& description(std::string description);
        CommandLine& parameter(std::string forms,
                             std::string description,
                             bool mandatory);
        
        CommandLineResults eval(Arguments& arguments);
        int count();
        
        std::string getParameter(std::string parameter);

        bool isSet(std::string parameter);
        bool hasValue(std::string parameter);
    };
}
