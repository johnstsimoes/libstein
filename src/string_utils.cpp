#include <sstream>
#include <iterator>
#include <iomanip>

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

        std::string url_encode(const std::string &value)
        {
            std::ostringstream escaped;
            escaped.fill('0');
            escaped << std::hex;

            for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
            {
                std::string::value_type c = (*i);

                // Keep alphanumeric and other accepted characters intact
                if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                    escaped << c;
                    continue;
                }

                // Any other characters are percent-encoded
                escaped << std::uppercase;
                escaped << '%' << std::setw(2) << int((unsigned char) c);
                escaped << std::nouppercase;
            }

            return escaped.str();
        }
    }
}
