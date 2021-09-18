#include <gtest/gtest.h>
#include <fmt/core.h>
#include <chrono>
#include "libstein/cached_rest.h"

using namespace libstein;

TEST(CachedRestTest, basics)
{
    // TODO this isn't unit testing, but E2E. Using an external API which can go offline.
    auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    auto url = fmt::format("https://hexabin.herokuapp.com/api/decimal/{}/convert/hex", timestamp);

    CachedRest api_convert(url);

    EXPECT_EQ(api_convert.status_code(), 200);
    EXPECT_EQ(api_convert.is_cached(), false);
    EXPECT_NE(api_convert.body().find("result"), std::string::npos);

    // Calling the same url will retrieve a cached result.
    CachedRest cached_api_convert(url);
    EXPECT_EQ(cached_api_convert.status_code(), 200);
    EXPECT_EQ(cached_api_convert.is_cached(), true);
    EXPECT_NE(cached_api_convert.body().find("result"), std::string::npos);
}
