#ifndef CC_AST_H
#define CC_AST_H

#include "cc_common.h"

typedef enum {
    AST_PROGRAM,
    AST_FUNCTION,
    AST_STATEMENT,
    AST_EXPRESSION,
} AstNodeType;

typedef struct AstNode {
    AstNodeType type;

    union {
        struct {
            struct AstNode *function;
        } program;
        struct {
            u32 name_length;
            char* name;
            char* return_type;
            struct AstNode *body;
        } function;
        struct {
            struct AstNode *expr;
        } statement;
        struct {
            i32 value;
        } expr;
    };
} AstNode;

AstNode *create_node(AstNodeType type);

void print_ast(AstNode *node);

#endif // CC_AST_H
