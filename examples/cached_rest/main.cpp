#include <iostream>
#include <libstein.h>
#include <chrono>

int main(int argc, char** argv)
{
    libstein::CachedRest::is_delay_milisseconds = 150;

    auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    auto url = std::string("https://networkcalc.com/api/binary/") + std::to_string(timestamp) + "?from=10&to=16";

    libstein::CachedRest::is_redis_enabled = false;
    libstein::CachedRest api_convert(url);

    std::cout << "Returned with code: " << api_convert.status_code() << std::endl;
    std::cout << "Response:" << std::endl << api_convert.body();

    exit(EXIT_SUCCESS);
}
