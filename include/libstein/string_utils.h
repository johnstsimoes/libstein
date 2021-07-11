#pragma once

#include <vector>
#include <string>

namespace libstein
{
    namespace stringutils
    {
        std::vector<std::string> split(const std::string str, const std::string regex_str);

        std::string get_environment(std::string environment_variable);
    }
}