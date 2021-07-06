#pragma once

#include <vector>
#include <string>

namespace libstein
{
    namespace stringutils
    {
        std::vector<std::string> split(const std::string &string, char delimiter);
    }
}