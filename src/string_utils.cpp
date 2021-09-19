#include <sstream>
#include <iterator>
#include <iomanip>
#include <numeric>

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

        // Source: M. Martinez at https://stackoverflow.com/a/34571089/5155484

        std::string base64_encode(const std::string &to_encode)
        {
            static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";//=
            std::string out;

            int val=0, valb=-6;
            for (unsigned char c : to_encode)
            {
                val = (val<<8) + c;
                valb += 8;
                while (valb>=0)
                {
                    out.push_back(b[(val>>valb)&0x3F]);
                    valb-=6;
                }
            }
            if (valb>-6) out.push_back(b[((val<<8)>>(valb+8))&0x3F]);
            while (out.size()%4) out.push_back('=');
            return out;
        }

        std::string base64_decode(const std::string &encoded)
        {
            static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";//=
            std::string out;

            std::vector<int> T(256,-1);
            for (int i=0; i<64; i++) T[b[i]] = i;

            int val=0, valb=-8;
            for (unsigned char c : encoded)
            {
                if (T[c] == -1) break;
                val = (val<<6) + T[c];
                valb += 6;
                if (valb>=0)
                {
                    out.push_back(char((val>>valb)&0xFF));
                    valb-=8;
                }
            }
            return out;
        }

        std::string join(const std::vector<std::string> &strings, const std::string &delimiter)
        {
            if (strings.size() == 0)
                return "";

            return std::accumulate(std::next(strings.begin()),
                                   strings.end(),
                                   strings[0],
                                   [&delimiter](std::string a, std::string b)
                                   {
                                       return a + delimiter + b;
                                   });
        }
    }
}
