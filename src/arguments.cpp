#include <iostream>
#include "libstein/arguments.h"
#include "libstein/string_utils.h"

using namespace libstein;

void Arguments::_build(std::vector<std::string> args)
{
    bool waiting_rvalue = false;
    std::string previous_lvalue = "";

    for (auto &arg : args)
    {
        if (arg.length() == 0) continue;

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
        else
        {
            if (waiting_rvalue)
            {
                _values[previous_lvalue] = arg;
                waiting_rvalue = false;
            }
        }
    }

    if (waiting_rvalue)
    {
        _values[previous_lvalue] = ""; // may be a last flag
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