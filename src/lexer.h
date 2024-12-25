#pragma once

#include <stdint.h>
#include <string>
#include <string_view>
#include <vector>

typedef enum {
    // keywords
    INT,
    MAIN,
    RETURN,
    VOID,
    // other tokens
    CONSTANT,
    IDENTIFIER,
    LPAREN,
    RPAREN,
    LCURLYBRACKET,
    RCURLYBRACKET,
    SEMICOLON,
    ERROR,
} TokenType;

struct Token {
    TokenType type;

    union {
        uint64_t integer_value;
        float f32_value;
        double f64_value;
        std::string_view name;
    };
};

struct Lexer {
    std::vector<char> file_buf;
    std::vector<Token> token_buf = std::vector<Token>();

    uint32_t cursor = 0;
    uint32_t cursor_start = 0;
    uint32_t cursor_line = 1;
    uint32_t cursor_prev_line = 0;

    void translate();

    char peek();
    char peek_next();
    char advance();
    void number();
    void word();
    void scan_token();
    bool at_end();
};

void print_tokens(std::vector<Token> &token_buf);
