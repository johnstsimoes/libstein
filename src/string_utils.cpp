#include <sstream>
#include <iterator>

#include "libstein/string_utils.h"

using namespace libstein::stringutils;

namespace libstein
{
    namespace stringutils
    {
        std::vector<std::string> split(const std::string &string, const char delimiter)
        {
            std::stringstream ss(string);
            std::string token;
            std::vector<std::string> result;
            while (std::getline(ss, token, delimiter))
            {
                result.push_back(std::move(token));
            }
            return result;
        }

        std::string get_environment(std::string environment_variable)
        {
            const char* env_var = std::getenv(environment_variable.c_str());

            std::string result = (NULL == env_var)? "" : env_var;
            return result;
        }
    }
}
