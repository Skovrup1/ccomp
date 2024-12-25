#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>

#include "lexer.h"
#include "macro.h"

int main() {
    char const *F_PATH = "tests/return_val.c";
    FILE *file = fopen(F_PATH, "r");

    fseek(file, 0, SEEK_END);

    size_t file_size = ftell(file);
    printf("file_size = %lu\n", file_size);

    fseek(file, 0, 0);

    std::vector<char> buf(file_size + 1);
    fread(buf.data(), sizeof(char), buf.size(), file);

    fclose(file);

    printf("buf:\n%s", buf.data());

    Lexer lex{.file_buf = buf};
    lex.translate();

    print_tokens(lex.token_buf);

    return 0;
}
