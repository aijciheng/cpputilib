#include "json/json.h"

#include <iostream>

int main() {
    /* json object */
    JsonObject jsonObject;
    jsonObject.loads("{\"content\":\"2234324324\", \"new_content\":{\"key\":\"112\"}, \"words\":[\"测试\", \"2017\"]}");

    json_error_type error;
    std::string value = jsonObject.string_value("content", &error);
    std::cout << value << std::endl;

    JsonObject *newObject = jsonObject.json_object("new_content");
    newObject = jsonObject.json_object("new_content");
    newObject = jsonObject.json_object("new_content");
    std::cout << newObject->string_value("key", &error) << std::endl;

    int index = 0;
    JsonObject *itemObject; 
    array_foreach(jsonObject.json_object("words"), index, itemObject) {
        std::cout << itemObject->string_value(&error) << std::endl;
    }

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
