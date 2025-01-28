#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "alloc.h"

#define MAX_MEMORY 1000

int main(int argc, char** argv) {
    uint32_t mem_backing_buflen = MAX_MEMORY;
    unsigned char mem_backing_buf[mem_backing_buflen];
    mem_init(mem_backing_buf, mem_backing_buflen);

    uint32_t temp_backing_buflen = MAX_MEMORY;
    unsigned char temp_backing_buf[temp_backing_buflen];
    temp_init(temp_backing_buf, temp_backing_buflen);

    return 0;
}
