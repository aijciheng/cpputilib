#include "net/httpclient.h"

#include <iostream>

int main() {
    std::string response;
    HttpClient httpclient;
    /* test get */
    if (httpclient.get("http://www.baidu.com", NULL, 0, &response) == HTTP_OK) {
        std::cout << response << std::endl;
    }

    /* test post */
    const char *headers[1];
    headers[0] = "Connection:close";
    if (httpclient.post("http://192.168.0.6:8868/simhash", headers, 0, "{\"content\":\"java开发工程师\"}", &response) == HTTP_OK) {
        std::cout << response << std::endl;
    } 
    return 0;
}
