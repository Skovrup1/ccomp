#include "cc_common.h"
#include "cc_scanner.h"
#include "cc_parser.h"
#include "cc_ast.h"
#include "cc_asm.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * @return caller owned buffer
 */
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
    char *source;

    if (argc == 2) {
        source = open_file(argv[1]);
    } else {
        fprintf(stderr, "invalid arguments\n");
        return 1;
    }

    print_all_tokens(source);

    init_scanner(source);

    init_parser();

    AstNode *ast = parse_program();

    print_ast(ast);

    AsmNode *_asm = ast_to_asm(ast);

    if (_asm == NULL) printf("null asm\n");

    print_asm(_asm);

    fflush(stdout);

    return 0;
}
