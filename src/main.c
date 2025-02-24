#include "cc_common.h"
#include "cc_scanner.h"

#include <stdio.h>
#include <stdlib.h>

static char *open_file(char const *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "failed to open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0, SEEK_END);
    usize file_size = ftell(file);
    rewind(file);

    char *buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "out of memory to read \"%s\".\n", path);
        exit(74);
    }

    usize bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "could not read file \"%s\".\n", path);
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

i32 main(i32 argc, char *argv[]) {
    if (argc == 2) {
        char *buffer = open_file(argv[1]);
        lexical_scan(buffer);
    } else {
        fprintf(stderr, "invalid arguments\n");
        return 1;
    }

    return 0;
}
