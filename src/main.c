#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "alloc.h"
#include "cc_string.h"
#include "lexer.h"

#define MAX_MEMORY 1000

String open_file(char const *path);

// caller owns the String
String open_file(char const *path) {
    // open file for reading
    FILE *file = fopen(path, "r");

    assert(file && "fopen in open_file failed");

    // seek end
    fseek(file, 0, SEEK_END);

    // get file size
    uint32_t size = ftell(file);

    // seek back to start
    fseek(file, 0, 0);

    String buf = string_alloc(size);

    // read to end
    uint32_t ret = fread(buf.data, sizeof(*buf.data), buf.size, file);
    if (ret != buf.size) {
        fprintf(stderr, "fread() failed: %u\n", ret);
        exit(EXIT_FAILURE);
    }

    fclose(file);

    return buf;
}

int main(int argc, char **argv) {
    char const *path = "tests/return_val.c";

    if (argc == 2) {
        path = argv[1];
        printf("running on %s\n", path);
    }

    uint32_t mem_size = MAX_MEMORY;
    unsigned char *mem_buf = malloc(mem_size);
    mem_init(mem_buf, mem_size);

    uint32_t temp_size = MAX_MEMORY;
    unsigned char *temp_buf = malloc(temp_size);
    temp_init(temp_buf, temp_size);

    String source = open_file(path);

    for (size_t i = 0; i < source.size; i++) {
        printf("%c", source.data[i]);
    }
    printf("\n");

    Lexer l = {
        .source = source,
    };

    TokenArray tokens = lexer_scan(&l);

    printf("tokens:\n");
    for (size_t i = 0; i < tokens.size; i++) {
        printf("    %s\n", token_type_to_string(tokens.data[i].type));
    }
    printf("\n");

    free(mem_buf);
    free(temp_buf);

    return 0;
}
