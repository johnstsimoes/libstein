#pragma once

#include <map>
#include <vector>
#include <string>

namespace libstein
{
    class Arguments
    {
    private:
        std::map<std::string, std::string> _values;

        void _build(std::vector<std::string> args);

    public:
        Arguments(int argc, char** argv);
        Arguments(std::vector<std::string> args);

        int size();
        
        std::string getParameter(std::string parameter);

        bool isSet(std::string parameter);
        bool hasValue(std::string parameter);
    };
}