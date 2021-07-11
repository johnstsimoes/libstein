#include "libstein/arguments.h"
#include "libstein/string_utils.h"

using namespace libstein;

void Arguments::_build(std::vector<std::string> args)
{
    bool waiting_rvalue = false;
    std::string previous_lvalue = "";

    // split all A=B into two items
    std::vector<std::string> expanded_args;
    for (const auto &arg : args)
    {
        std::vector<std::string> tokens = stringutils::split(arg, "=");
        for (const auto &token : tokens)
        {
            if (token.length() > 0)
                expanded_args.push_back(token);
        }
    }

    for (auto &arg : expanded_args)
    {
        if (arg.length() == 0) continue;
        
        auto equals_found = arg.find('=');

        if ('-' == arg[0])
        {
            if (waiting_rvalue)
            {
                _values[previous_lvalue] = "";
            }

            waiting_rvalue = true;
            arg.erase (0,1); // remove "-"
            if (arg[0] == '-') arg.erase (0,1); // in case of "--"
            previous_lvalue = arg;
        }
        else if (std::string::npos != equals_found)
        {
            if (waiting_rvalue)
            {
                _values[previous_lvalue] = "";
            }

            _values[arg.substr(0, equals_found)] = arg.substr(equals_found + 1);
            waiting_rvalue = false;
        }
        else
        {
            if (waiting_rvalue)
            {
                _values[previous_lvalue] = arg;
                waiting_rvalue = false;
            }
        }
    }
}

Arguments::Arguments(std::vector<std::string> args)
{
    _build(args);
}

Arguments::Arguments(int argc, char** argv)
{
    std::vector<std::string> args;

    for (int pos = 0; pos < argc; pos++)
    {
        args.push_back(argv[pos]);
    }

    _build(args);
}

int Arguments::size()
{
    return _values.size();
}


std::map<std::string, std::string> Arguments::getParameters()
{
    return this->_values;
}

std::string Arguments::getParameter(std::string parameter)
{
    if (parameter.length() > 0 && parameter[0] == '-')
        parameter.erase(0,1);

    std::string result = "";

    if (_values.find(parameter) != _values.end())
        result = _values [parameter];
    
    return result;
}

bool Arguments::isSet(std::string parameter)
{
    return (_values.find(parameter) != _values.end());
}

bool Arguments::hasValue(std::string parameter)
{
    auto value = _values.find(parameter);
    return (    (value != _values.end())
            &&  (!value->second.empty()));
}