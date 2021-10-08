#include "libstein.h"

#include <fmt/core.h>
#include <curl/curl.h>
#include <mutex>

#include <unordered_map>

extern "C"
{
#include <hiredis/hiredis.h>
}

using namespace libstein;

static size_t writeFunction(void *ptr, size_t size, size_t block_size, std::string* data)
{
    data->append((char*) ptr, size * block_size);
    return size * block_size;
}

CachedRest::CachedRest (const std::string &url,
                        const std::string &login,
                        const std::string &password)
{
    // Try to locate Redis
    redisContext *redis = redisConnect("127.0.0.1", 6379);

    std::string hash = std::to_string(std::hash<std::string>{}(url));

    const bool redis_found = (redis && !redis->err);
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
        // Ensures curl_global_init is called only once in app lifetime.
        // This is important since the function itself is not thread-safe.
        static std::once_flag flag;
        std::call_once(flag, [&]()
        {
            curl_global_init(CURL_GLOBAL_DEFAULT);
        });

        auto curl = curl_easy_init();

        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            // curl_easy_setopt(curl, CURLOPT_USERPWD, "user:pass");
            if (login.size() > 0)
            {
                curl_easy_setopt(curl, CURLOPT_USERNAME, login.c_str());
                curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "jiralab/0.0.1");
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

            std::string response_string;
            std::string header_string;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &this->body_);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

            this->is_cached_ = false;

            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &this->status_code_);

            curl_easy_cleanup(curl);
            curl = nullptr;
        }
        if (redis_found && this->status_code_ == 200)
        {
            auto encoded = libstein::stringutils::base64_encode(this->body_);
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

            // Set expiration for 4 hours.
            reply = (redisReply*)redisCommand(redis, "EXPIRE %s 14400", hash.c_str());
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
