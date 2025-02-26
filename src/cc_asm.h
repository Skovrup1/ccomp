#ifndef CC_ASM_H
#define CC_ASM_H

#include "cc_common.h"
#include "cc_ast.h"

typedef enum {
    ASM_PROGRAM,
    ASM_FUNCTION,
    ASM_MOV,
    ASM_RET,
    ASM_IMM
} AsmNodeType;

typedef struct AsmNode {
    AsmNodeType type;

    union {
        struct {
            struct AsmNode *function;
        } program;
        struct {
            u32 name_length;
            u32 instruction_count;
            char *name;
            struct AsmNode **instructions;
        } function;
        struct {
            struct AsmNode *src;
            char *dst;
        } mov;
        struct {
            u32 value;
        } imm;
    };
} AsmNode;

AsmNode *ast_to_asm(AstNode *node);

void print_asm(AsmNode *node);

#endif // CC_ASM_H
