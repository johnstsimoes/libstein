#pragma once

#include <vector>
#include <string>

namespace libstein
{
    namespace stringutils
    {
        std::vector<std::string> split(const std::string &string, const char delimiter);

        std::string get_environment(std::string environment_variable);

        std::string url_encode(const std::string &value);

        std::string base64_encode(const std::string &to_encode);
        std::string base64_decode(const std::string &encoded);
    }
}
