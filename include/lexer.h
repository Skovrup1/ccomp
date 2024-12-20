#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {
    CONSTANT,
    IDENTIFIER,
    KEYWORD,
    LPAREN,
    RPAREN,
    LCURLYBRACKET,
    RCURLYBRACKET,
    SEMICOLON,
} tokentype_t;

typedef struct {
    tokentype_t type;
    uint32_t i;
} token_t;

void lex(char *buf, uint32_t n);

