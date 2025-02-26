#include "cc_parser.h"
#include "cc_scanner.h"
#include "cc_string.h"
#include "cc_ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Token previous_token;
Token current_token;

static void advance() {
    previous_token = current_token;
    current_token = next_token();
}

static void expect(TokenType type) {
    if (current_token.type != type) {
        printf("Error: Expected %d, got %d\n", type, current_token.type);
        exit(1);
    }

    advance();
}

static AstNode *parse_expr() {
    AstNode *node = create_node(AST_EXPRESSION);
    expect(TOKEN_CONSTANT);
    node->expr.value = str_to_u32(previous_token.start, previous_token.length);

    return node;
}

static AstNode *parse_statement() {
    AstNode *node = create_node(AST_STATEMENT);
    expect(TOKEN_RETURN);
    node->statement.expr = parse_expr();
    expect(TOKEN_SEMICOLON);

    return node;
}

static AstNode *parse_function() {
    AstNode *node = create_node(AST_FUNCTION);
    expect(TOKEN_INT);
    node->function.return_type = "int";

    expect(TOKEN_IDENTIFIER);
    node->function.name_length = previous_token.length;
    node->function.name = previous_token.start;
    expect(TOKEN_LPAREN);
    expect(TOKEN_VOID);
    expect(TOKEN_RPAREN);
    expect(TOKEN_LBRACE);
    node->function.body = parse_statement();
    expect(TOKEN_RBRACE);

    return node;
}

void init_parser() {
    advance();
}

AstNode *parse_program() {
    AstNode *node = create_node(AST_PROGRAM);
    node->program.function = parse_function();

    return node;
}
