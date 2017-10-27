#include "json/json.h"

#include <iostream>

int main() {
    JsonObject jsonObject;
    jsonObject.loads("{\"content\":\"2234324324\", \"new_content\":{\"key\":\"112\"}}");

    std::string value = jsonObject.string_value("content");
    std::cout << value << std::endl;

    JsonObject *newObject = jsonObject.json_object("new_content");
    newObject = jsonObject.json_object("new_content");
    newObject = jsonObject.json_object("new_content");
    std::cout << newObject->string_value("key") << std::endl;

    /* json data */
    JsonData data;
    data["id"] = "12345";
    data["content"] = "测试";
    data["skills"][0] = "java";
    data["skills"][1] = "c++";
    data["skills"][2] = "asm";
    
    std::cout << data.dumps() << std::endl;
    return 0;
}
