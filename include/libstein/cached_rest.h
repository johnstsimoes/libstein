#pragma once

#include <string>

class CachedRest
{
private:
    int status_code_;
    std::string body_;
    bool is_cached_;

public:
    CachedRest(const std::string &url,
               const std::string &login = "",
               const std::string &password = "");

    int status_code();
    std::string body();
    bool is_cached();
};
