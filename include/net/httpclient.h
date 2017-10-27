#ifndef NET_HTTPCLIENT_H_
#define NET_HTTPCLIENT_H_

#include <string>

typedef enum {
    HTTP_OK = 0,
    HTTP_ERROR = -1
} HTTP_STATE;

class HttpClient {
 public:
    HttpClient();
    ~HttpClient() {};
    
    int get(const std::string &url, const char **headers,
            const int headers_count, std::string *content);
    int post(const std::string &url, const char **headers,
             const int headers_count, const std::string &post_data,
             std::string *content);

 private:
    volatile bool is_global_init_ = false;
    pthread_mutex_t global_init_lock_ = PTHREAD_MUTEX_INITIALIZER; 

};
#endif
