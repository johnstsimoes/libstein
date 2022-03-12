FROM johnstsimoes/cpp-ready
#FROM alpine:latest

LABEL maintainer="John Simoes" \
      description="Basic C++ for CircleCi repo." \
      version="0.1.0"

# RUN apk update
# RUN apk upgrade
# RUN apk add --no-cache -f cmake make g++ bash
# RUN apk add --no-cache -f autoconf automake libtool git openssl-dev curl-dev curl-static curl
# RUN apk add --no-cache -f vim bash gtest-dev nlohmann-json
# RUN apk add --no-cache -f zlib-dev linux-headers
# RUN apk add --no-cache -f hiredis-dev redis fmt-dev

ADD . /libstein
RUN rm -rf /libstein/build
RUN mkdir -p /libstein/build && cd /libstein/build && cmake .. && make && make install
RUN redis-server --daemonize yes && \
    cd /libstein && \
    ctest --test-dir build/ --output-on-failure --verbose

