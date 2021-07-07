#include <string>
#include <regex>

#include "libstein/string_utils.h"

using namespace libstein::stringutils;

namespace libstein
{
    namespace stringutils
    {
        std::vector<std::string> split(const std::string str, const std::string regex_str)
        {
            return { std::sregex_token_iterator(str.begin(), str.end(), std::regex(regex_str), -1), std::sregex_token_iterator() };
        }
    }
}