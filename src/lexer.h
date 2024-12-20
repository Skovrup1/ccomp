#pragma once

#include <stdint.h>
#include <string>
#include <vector>

typedef enum : uint32_t {
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
} TokenType;

struct Token {
    TokenType type;
    uint32_t i;
};

struct Lexer {
    std::vector<char> file_buf;
    std::vector<int> constant_buf = std::vector<int>();
    std::vector<std::string> identifier_buf = std::vector<std::string>();
    std::vector<Token> token_buf = std::vector<Token>();

    uint32_t cursor = 0;
    uint32_t cursor_start = 0;
    uint32_t cursor_line = 1;
    uint32_t cursor_prev_line = 0;

    void translate();

    char peek();
    char peek_next();
    char advance();
    void add_token(TokenType type);
    void add_token_i(TokenType type, std::string word);
    void add_token_k(TokenType type, uint32_t i);
    void add_token_c(TokenType type, uint32_t i);
    void number();
    void word();
    void scan_token();
    bool at_end();
};
