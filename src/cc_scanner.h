#ifndef CC_SCANNER_H
#define CC_SCANNER_H

#include "cc_common.h"

typedef enum {
    // control
    TOKEN_NULL = 0,
    TOKEN_EOF,
    TOKEN_ERROR,
    // literals
    TOKEN_CONSTANT,
    TOKEN_IDENTIFIER,
    // symbols
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_TILDE,
    TOKEN_MINUS,
    // keywords
    TOKEN_INT = 256,
    TOKEN_VOID,
    TOKEN_RETURN,
} TokenType;

typedef struct {
    TokenType type;
    i32 line;
    i32 length;
    char const *start;
} Token;


void init_scanner(char const *source);
Token next_token();

// debug functions
void print_all_tokens(char const *source);

char const *token_type_to_string(TokenType type);

#endif // CC_SCANNER_H
