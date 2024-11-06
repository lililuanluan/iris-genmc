#ifndef MYHELP_H
#define MYHELP_H

#include <stddef.h>

void to_string(char* str, int i) {
    size_t len = 0;
    char temp[100]; 

    if (i == 0) {
        str[len++] = '0';
        str[len] = '\0';
        return;
    }

    while (i > 0) {
        temp[len++] = (i % 10) + '0';
        i /= 10;
    }
    for (size_t j = 0; j < len; ++j) {
        str[j] = temp[len - j - 1];
    }
    str[len] = '\0';  
}


size_t Strlen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

void* Memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}


#endif