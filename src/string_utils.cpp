#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "libstein/string_utils.h"

using namespace libstein::stringutils;

namespace libstein
{
    namespace stringutils
    {
        template <typename Out>
        void split(const std::string &s, char delim, Out result)
        {
            std::istringstream iss(s);
            std::string item;
            while (std::getline(iss, item, delim))
            {
                *result++ = item;
            }
        }

        std::vector<std::string> split(const std::string &s, char delim)
        {
            std::vector<std::string> elems;
            split(s, delim, std::back_inserter(elems));
            return elems;
        }

    }
}