#pragma once

#include <stddef.h>

struct String {
    char *data;
    size_t size;
};
typedef struct String String;

String string_alloc(size_t size);
String string_create(char const *literal);
