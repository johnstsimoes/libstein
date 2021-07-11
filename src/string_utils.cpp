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

        std::string get_environment(std::string environment_variable)
        {
            const char* env_var = std::getenv(environment_variable.c_str());

            std::string result = (NULL == env_var)? "" : env_var;
            return result;
        }
    }
}