#pragma once

#include <vector>
#include <string>

namespace libstein
{
    namespace stringutils
    {
        std::vector<std::string> split(const std::string &string, const char delimiter);

        std::string get_environment(std::string environment_variable);
    }
}
