#include "string/string_utils.h"

#include <string.h>

#include <iostream>

int main() {
    const char *line = "测2试3";
    std::cout << "word : " << line << std::endl;
    std::cout << "word size : " << strlen(line) << std::endl;
    std::cout << "first word len : " << utf8_word_len(line[0]) << std::endl;
    std::cout << "word count : " << utf8_word_count(line) << std::endl; 
    return 0;
}
