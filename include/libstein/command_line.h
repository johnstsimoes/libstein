#pragma once

#include <vector>
#include <string>

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

        struct CommandLineParameter
        {
            std::string _short;
            std::string _long;
            std::string _environment;
            std::string _description;
            bool _mandatory;
        };
        std::vector<CommandLineParameter> _parameters;

        std::string _description;

    public:
        CommandLine();

        CommandLine& description(std::string description);
        CommandLine& parameter(std::string forms,
                             std::string description,
                             bool mandatory);
        
        CommandLineResults eval(Arguments& arguments);
    };
}
