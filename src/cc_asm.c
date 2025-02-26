#include "cc_asm.h"

#include <stdlib.h>
#include <stdio.h>

AsmNode *ast_to_asm(AstNode *node) {
    switch (node->type) {
    case AST_PROGRAM:
        AsmNode *prog = malloc(sizeof(AsmNode));
        prog->type = ASM_PROGRAM;
        prog->program.function = ast_to_asm(node->program.function);
        return prog;
    case AST_FUNCTION:
        AsmNode *func = malloc(sizeof(AsmNode));
        func->type = ASM_FUNCTION;
        func->function.name_length = node->function.name_length;
        func->function.name = node->function.name;

        AsmNode *body_mov = ast_to_asm(node->function.body);
        func->function.instructions = malloc(2 * sizeof(AsmNode *));
        func->function.instruction_count = 2;
        func->function.instructions[0] = body_mov;
        func->function.instructions[1] = malloc(sizeof(AsmNode));
        func->function.instructions[1]->type = ASM_RET;

        return func;
    case AST_STATEMENT:
        AsmNode *mov = malloc(sizeof(AsmNode));
        mov->type = ASM_MOV;
        mov->mov.src = ast_to_asm(node->statement.expr);
        mov->mov.dst = "eax";

        return mov;
    case AST_EXPRESSION:
        AsmNode *imm = malloc(sizeof(AsmNode));
        imm->type = ASM_IMM;
        imm->imm.value = node->expr.value;

        return imm;
    default:
        return NULL;
    }
}

static AsmNode *get_next(AsmNode *node) {
    switch (node->type) {
        case ASM_PROGRAM:
            return node->program.function;
        case ASM_FUNCTION:
        case ASM_MOV:
        case ASM_RET:
        case ASM_IMM:
    }

    return NULL;
}

static void print_indent(u32 indent) {
    printf("%*s", indent, "");
}

static void print_node(AsmNode *node) {
    switch (node->type) {
        case ASM_PROGRAM:
            // do nothing
            break;
        case ASM_FUNCTION:
            printf("%.*s():\n", node->function.name_length, node->function.name);

            for (u32 i = 0; i < node->function.instruction_count; i++) {
                print_indent(4);
                print_node(node->function.instructions[i]);
                printf("\n");
            }
            break;
        case ASM_MOV:
            printf("mov ");
            printf("%s, ", node->mov.dst);
            print_node(node->mov.src);
            break;
        case ASM_RET:
            printf("ret");
            break;
        case ASM_IMM:
            printf("%d", node->imm.value);
            break;
    }
}

void print_asm(AsmNode *node) {
    while (node != NULL) {
        print_node(node);

        printf("\n");

        node = get_next(node);
    }
}
