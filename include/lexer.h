#pragma once

#include <stddef.h>
#include <stdint.h>

#include "cc_string.h"

enum TokenType {
    // keywords
    INT    = 0,
    MAIN   = 1,
    VOID   = 2,
    RETURN = 3,
    // control
    END,
    EMPTY,
    INVALID,
    // literals
    CONSTANT,
    IDENTIFIER,
    // symbols
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LCURLYBRACKET,
    RCURLYBRACKET,
};
typedef enum TokenType TokenType;

struct Token {
    TokenType type;

    uint32_t c0; // character start
    uint32_t c1; // character end
    uint32_t l0; // line start
    uint32_t l1; // line end

    union {
        char *identifier;
        uint64_t constant;
    };
};
typedef struct Token Token;

struct TokenArray {
    Token *data;
    size_t size;
    size_t capacity;
};
typedef struct TokenArray TokenArray;

struct Lexer {
    String source;
    uint32_t start;
    uint32_t cursor;
    uint32_t line_start;
    uint32_t line_cursor;
};
typedef struct Lexer Lexer;

TokenArray lexer_scan(Lexer *l);

char const *token_type_to_string(TokenType type);
