#include "net/httpclient.h"

#include <pthread.h>
#include <curl/curl.h>

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::string *r = (std::string*)userdata;
    r->append(ptr, size * nmemb);
    return size * nmemb;
}

HttpClient::HttpClient() {
    if (!is_global_init_) {
        pthread_mutex_lock(&global_init_lock_);
        if (!is_global_init_) {
            curl_global_init(CURL_GLOBAL_ALL);
            is_global_init_ = true;
        }
        pthread_mutex_unlock(&global_init_lock_);
    }
}

int HttpClient::get(const std::string &url, const char **headers,
                    const int headers_count, std::string *content) {
    CURLcode res;
    long code = HTTP_ERROR;
    struct curl_slist *hds = NULL;
    CURL *curl = curl_easy_init(); 
    if (curl == NULL) {
       return HTTP_ERROR;      
    } 

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)content);

    /* headers */
    if (headers && headers_count > 0) {
        for (int i = 0; i < headers_count; i++) {
            hds = curl_slist_append(hds, headers[i]);
        } 
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hds);
    }

    /* do transfer */
    res = curl_easy_perform(curl); 
    if (hds) curl_slist_free_all(hds);
    curl_easy_reset(curl);
    curl_easy_cleanup(curl);
    if (CURLE_OK != res) {
        return HTTP_ERROR;
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if (200 != code) {
        return HTTP_ERROR;
    }
    return HTTP_OK;
}

int HttpClient::post(const std::string &url, const char **headers,
                     const int headers_count, const std::string &post_data,
                     std::string *content) {
    CURLcode res;
    long code = HTTP_ERROR;
    struct curl_slist *hds = NULL;
    CURL *curl = curl_easy_init(); 
    if (curl == NULL) {
       return HTTP_ERROR;      
    } 

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)content);
    curl_easy_setopt(curl, CURLOPT_POST, 1);  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

    /* headers */
    if (headers && headers_count > 0) {
        for (int i = 0; i < headers_count; i++) {
            hds = curl_slist_append(hds, headers[i]);
        } 
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hds);
    }

    /* do transfer */
    res = curl_easy_perform(curl); 
    if (hds) curl_slist_free_all(hds);
    curl_easy_reset(curl);
    curl_easy_cleanup(curl);
    if (CURLE_OK != res) {
        return HTTP_ERROR;
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if (200 != code) {
        return HTTP_ERROR;
    }
    return HTTP_OK;
}
