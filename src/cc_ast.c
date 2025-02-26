#include "cc_ast.h"

#include <stdio.h>
#include <stdlib.h>

AstNode *create_node(AstNodeType type) {
    AstNode *node = malloc(sizeof(AstNode));
    node->type = type;

    return node;
}

static AstNode *get_next(AstNode *node) {
    switch (node->type) {
    case AST_PROGRAM:
        return node->program.function;
    case AST_FUNCTION:
        return node->function.body;
    case AST_STATEMENT:
        return node->statement.expr;
    default:
        return NULL;
    }
}

u32 depth;
static void print_depth() {
    printf("%*s", depth, "");
}

static void print_node(AstNode *node) {
    switch (node->type) {
    case AST_PROGRAM:
        print_depth();
        printf("AST_PROGRAM\n");
        break;
    case AST_FUNCTION:
        print_depth();
        printf("AST_FUNCTION\n");
        print_depth();
        printf("name: %.*s\n", node->function.name_length, node->function.name);
        print_depth();
        printf("return_type: %s\n", node->function.return_type);
        break;
    case AST_STATEMENT:
        print_depth();
        printf("AST_STATEMENT\n");
        break;
    case AST_EXPRESSION:
        print_depth();
        printf("AST_EXPRESSION\n");
        print_depth();
        printf("value: %d\n", node->expr.value);
        break;
    default:
        fprintf(stderr, "unknown AST node type\n");
        exit(1);
    }
}

void print_ast(AstNode *node) {
    depth = 0;
    while (node != NULL) {
        print_node(node);

        depth += 2;

        node = get_next(node);
    }
}
