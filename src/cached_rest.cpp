#include "libstein.h"

#include <fmt/core.h>
#include <cpr/cpr.h>
#include <unordered_map>

extern "C"
{
#include <hiredis/hiredis.h>
}

CachedRest::CachedRest (const std::string &url,
                        const std::string &login,
                        const std::string &password)
{
    // Try to locate Redis
    redisContext *redis = redisConnect("127.0.0.1", 6379);

    std::string hash = std::to_string(std::hash<std::string>{}(url));

    bool redis_found = (redis && !redis->err);
    bool data_found = false;

    if (redis_found)
    {
        redisReply* reply = (redisReply*)redisCommand(redis, "GET %s", hash.c_str());

        if (    (reply->type == REDIS_REPLY_STRING)
            &&  (reply->len > 0))
        {
            this->status_code_ = 200;
            this->body_ = libstein::stringutils::base64_decode(reply->str);
            this->is_cached_ = true;

            data_found = true;
        }

        freeReplyObject(reply);
    }

    if (!data_found)
    {
        cpr::Response response = cpr::Get(cpr::Url{url},
                                        cpr::Authentication{login, password});

        this->status_code_ = response.status_code;
        this->body_ = response.text;
        this->is_cached_ = false;

        if (response.status_code == 200)
        {
            auto encoded = libstein::stringutils::base64_encode(response.text);
            redisReply* reply = (redisReply*)redisCommand(redis, "SET %s %s", hash.c_str(), encoded.c_str());

            if (    (reply->type == REDIS_REPLY_STRING)
                &&  (reply->len > 0))
            {
                this->status_code_ = 200;
                this->body_ = reply->str;
                this->is_cached_ = true;

                data_found = true;
            }

            freeReplyObject(reply);

            // Set expiration for 1 hour.
            reply = (redisReply*)redisCommand(redis, "EXPIRE %s 3600", hash.c_str());
            freeReplyObject(reply);
        }
    }

    if (redis_found)
        redisFree(redis);
}

bool CachedRest::is_cached()
{
    return this->is_cached_;
}

int CachedRest::status_code()
{
    return this->status_code_;
}

std::string CachedRest::body()
{
    return this->body_;
}