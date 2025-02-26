#include "cc_string.h"

/*
 * @brief converts string to unsigned integer,
 *        does not handle signed values
 */
u32 str_to_u32(char const *string, u32 length) {
    u32 result = 0;
    for (u32 i = 0; i < length; i++) {
        result *= 10;
        result += string[i] - '0';
    }

    return result;
}

