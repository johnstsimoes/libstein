#pragma once

#include <string>

namespace libstein
{
    class CachedRest
    {
    private:
        long status_code_ = 404;
        std::string body_ = "";
        bool is_cached_ = false;

    public:

        static bool is_redis_enabled;
        static std::string is_redis_address;
        static long is_delay_milisseconds;
        static long redis_expiration_seconds;

        CachedRest(const std::string &url,
                   const std::string &login = "",
                   const std::string &password = "");

        int status_code();
        std::string body();
        bool is_cached();
  };
}
