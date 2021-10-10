#include "libstein.h"

#include <curl/curl.h>
#include <fmt/format.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <unordered_map>

extern "C"
{
#include <hiredis/hiredis.h>
}

using namespace libstein;

/**
 * Static initialization of settings.
 */
bool CachedRest::is_redis_enabled = false;
std::string CachedRest::is_redis_address = "127.0.0.1";
long CachedRest::is_delay_milisseconds = 0;

static size_t writeFunction(void *ptr, size_t size, size_t block_size, std::string* data)
{
    data->append((char*) ptr, size * block_size);
    return size * block_size;
}

CachedRest::CachedRest (const std::string &url,
                        const std::string &login,
                        const std::string &password)
{
    bool cached_data_found = false;
    bool redis_found = false;
    std::string url_hash = std::to_string(std::hash<std::string>{}(url));
    redisContext *redis = nullptr;

    // Try to locate Redis
    if (CachedRest::is_redis_enabled)
    {
        redis = redisConnect(CachedRest::is_redis_address.c_str(), 6379);

        redis_found = (redis && !redis->err);

        if (redis_found)
        {
            redisReply* reply = (redisReply*)redisCommand(redis, "GET %s", url_hash.c_str());

            if (    (reply->type == REDIS_REPLY_STRING)
                &&  (reply->len > 0))
            {
                this->status_code_ = 200;
                this->body_ = libstein::stringutils::base64_decode(reply->str);
                this->is_cached_ = true;

                cached_data_found = true;
            }

            freeReplyObject(reply);
        }
    }

    if (!cached_data_found)
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
            if (login.size() > 0)
            {
                curl_easy_setopt(curl, CURLOPT_USERNAME, login.c_str());
                curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "libstein/0.0.1");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 4L);
            // curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
            // curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

            std::string header_string;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &this->body_);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

            this->is_cached_ = false;

            // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            CURLcode status = curl_easy_perform(curl);
            if (CURLE_OK != status)
            {
                throw std::runtime_error(curl_easy_strerror(status));
            }

            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &this->status_code_);

            curl_easy_cleanup(curl);
            curl = nullptr;
        }
    }

    if (redis_found && !cached_data_found && this->status_code_ == 200)
    {
        auto encoded = libstein::stringutils::base64_encode(this->body_);
        redisReply* set_reply = (redisReply*)redisCommand(redis, "SET %s %s", url_hash.c_str(), encoded.c_str());

        if (    (set_reply->type == REDIS_REPLY_STRING)
            &&  (set_reply->len > 0))
        {
            this->is_cached_ = true;

            freeReplyObject(set_reply);

            // Set expiration for 4 hours.
            redisReply* expire_reply = (redisReply*)redisCommand(redis, "EXPIRE %s 14400", url_hash.c_str());
            freeReplyObject(expire_reply);
        }
    }

    if (redis_found)
        redisFree(redis);


    if (!redis_found && (CachedRest::is_delay_milisseconds > 0))
        std::this_thread::sleep_for(std::chrono::milliseconds(CachedRest::is_delay_milisseconds) );
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
