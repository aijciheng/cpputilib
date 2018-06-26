#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include <string.h>

#include <iostream>

static uint8_t utf8_prefix[16] = { 
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    2, 2, 3, 4
};

inline int utf8_word_len(uint8_t prefix) {
    return utf8_prefix[prefix >> 4]; 
}

inline int utf8_word_count(const char *line) {
    if (line == NULL) {
        return 0;
    }

    int count = 0;
    int word_len = 0;
    for (int i = 0; i < strlen(line); ) {
        word_len = utf8_word_len(line[i]);
        if (word_len < 0) {
            return -1;
        }

        count++;
        i += word_len;
    }

    return count;
}
#endif
