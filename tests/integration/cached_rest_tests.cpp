#include <gtest/gtest.h>
#include <fmt/core.h>
#include <chrono>
#include "libstein/cached_rest.h"

using namespace libstein;

TEST(CachedRestTest, basics_redis_disabled)
{
    CachedRest::is_delay_milisseconds = 150;

    auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    auto url = fmt::format("https://networkcalc.com/api/binary/{}?from=10&to=16", timestamp);

    CachedRest::is_redis_enabled = false;
    CachedRest api_convert(url);

    EXPECT_EQ(api_convert.status_code(), 200);
    EXPECT_EQ(api_convert.is_cached(), false);
    EXPECT_NE(api_convert.body().find("converted"), std::string::npos);

    // Calling the same url without redis will perform the call again without caching.
    CachedRest cached_api_convert(url);
    EXPECT_EQ(cached_api_convert.status_code(), 200);
    EXPECT_EQ(cached_api_convert.is_cached(), false);
    EXPECT_NE(cached_api_convert.body().find("converted"), std::string::npos);
}

TEST(CachedRestTest, basics_redis_enabled)
{
    CachedRest::is_delay_milisseconds = 0;

    auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    auto url = fmt::format("https://networkcalc.com/api/binary/{}?from=10&to=16", timestamp);

    CachedRest::is_redis_enabled = true;
    CachedRest api_convert(url);

    EXPECT_EQ(api_convert.status_code(), 200);
    EXPECT_EQ(api_convert.is_cached(), false);
    EXPECT_NE(api_convert.body().find("converted"), std::string::npos);

    // Calling the same url will retrieve a cached result.
    CachedRest cached_api_convert(url);
    EXPECT_EQ(cached_api_convert.status_code(), 200);
    EXPECT_EQ(cached_api_convert.is_cached(), true);
    EXPECT_NE(cached_api_convert.body().find("converted"), std::string::npos);
}
