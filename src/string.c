#include "cc_string.h"

#include <string.h>

#include "alloc.h"

String string_alloc(size_t size) {
    String str = {
        .data = mem_alloc(size),
        .size = size,
    };
    return str;
}

String string_create(char const *literal) {
    size_t size = strlen(literal);
    String str = {
        .data = mem_alloc(size),
        .size = size,
    };
    return str;
}
