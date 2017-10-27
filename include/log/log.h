#ifndef LOG_LOG_H_
#define LOG_LOG_H_

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

#include <string>

extern thread_local std::string request_id;

#define LOG_INFO(format, ...)                                                                           \
{                                                                                                       \
    time_t t = time(0);                                                                                 \
    struct tm ttt = *localtime(&t);                                                                     \
    fprintf(stdout, "[INFO] [%s] [%lu %4d-%02d-%02d %02d:%02d:%02d] [%s:%s:%d] " format "",             \
            request_id.c_str(), pthread_self(), ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday,        \
            ttt.tm_hour, ttt.tm_min, ttt.tm_sec, __FILE__,  __FUNCTION__ , __LINE__, ##__VA_ARGS__);    \
}

#define LOG_ERR(format, ...)                                                                            \
{                                                                                                       \
    time_t t = time(0);                                                                                 \
    struct tm ttt = *localtime(&t);                                                                     \
    fprintf(stderr, "[ERROR] [%s] [%lu %4d-%02d-%02d %02d:%02d:%02d] [%s:%s:%d] " format "",            \
            request_id.c_str(), pthread_self(), ttt.tm_year + 1900, ttt.tm_mon + 1, ttt.tm_mday,        \
            ttt.tm_hour, ttt.tm_min, ttt.tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
}

#endif
