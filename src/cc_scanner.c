#include "cc_scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    i32 line;
    char const *start;
    char const *current;
} Scanner;

Scanner scanner;

void init_scanner(char const *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static Token make_token(TokenType type) {
    Token token = {
        .type = type,
        .line = scanner.line,
        .length = (i32)(scanner.current - scanner.start),
        .start = scanner.start,
    };

    return token;
}

static Token error_token(char const *message) {
    Token token = {
        .type = TOKEN_ERROR,
        .line = scanner.line,
        .length = (i32)strlen(message),
        .start = message,
    };

    return token;
}

static bool is_at_end() { return *scanner.current == '\0'; }

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_digit(char c) { return c >= '0' && c <= '9'; }

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static char peek() { return *scanner.current; }

static char peek_next() {
    if (is_at_end())
        return '\0';

    return scanner.current[1];
}

static Token constant() {
    while (is_digit(peek()))
        advance();

    if (peek() == '.' && is_digit(peek_next())) {
        advance();

        while (is_digit(peek()))
            advance();
    }

    return make_token(TOKEN_CONSTANT);
}

static TokenType identifier_type() {
    char const *keywords[] = {
        "int",
        "void",
        "return",
    };

    u64 length = (u64)(scanner.current - scanner.start);

    for (u64 i = 0; i < ARRAY_SIZE(keywords); i++) {
        if (strncmp(keywords[i], scanner.start, length) == 0)
            // 256 is the starting value for keyword tokens
            return (TokenType)(256 + i);
    }

    return TOKEN_IDENTIFIER;
}

static Token identifer() {
    while (is_alpha(peek()) || is_digit(peek()))
        advance();

    return make_token(identifier_type());
}

static void skip_whitespace() {
    while (true) {
        char c = peek();
        switch (c) {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            scanner.line++;
            advance();
            break;
        case '/':
            if (peek_next() == '/') {
                while (peek() != '\n' && !is_at_end())
                    advance();
            }
            break;
        default:
            return;
        }
    }
}

Token next_token() {
    skip_whitespace();

    scanner.start = scanner.current;

    if (is_at_end())
        return make_token(TOKEN_EOF);

    char c = advance();

    if (is_alpha(c))
        return identifer();
    if (is_digit(c))
        return constant();

    switch (c) {
    case '(':
        return make_token(TOKEN_LPAREN);
    case ')':
        return make_token(TOKEN_RPAREN);
    case '{':
        return make_token(TOKEN_LBRACE);
    case '}':
        return make_token(TOKEN_RBRACE);
    case '[':
        return make_token(TOKEN_LBRACKET);
    case ']':
        return make_token(TOKEN_RBRACKET);
    case ';':
        return make_token(TOKEN_SEMICOLON);
    case '~':
        return make_token(TOKEN_TILDE);
    case '-':
        return make_token(TOKEN_MINUS);
    }

    return error_token("unexpected character");
}

void print_all_tokens(char const *source) {
    init_scanner(source);

    Token t = next_token();
    while (t.type != TOKEN_EOF) {
        char const *type_str = token_type_to_string(t.type);
        printf("%s", type_str);

        if (t.type == TOKEN_IDENTIFIER)
            printf("(%.*s)", (int)t.length, t.start);

        printf("\n");

        t = next_token();
    }

    printf("\n");
}

const char *token_type_to_string(TokenType type) {
    switch (type) {
        // control
        case TOKEN_NULL:       return "TOKEN_NULL";
        case TOKEN_EOF:        return "TOKEN_EOF";
        case TOKEN_ERROR:      return "TOKEN_ERROR";
        // literals
        case TOKEN_CONSTANT:   return "TOKEN_CONSTANT";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        // symbols
        case TOKEN_LPAREN:     return "TOKEN_LPAREN";
        case TOKEN_RPAREN:     return "TOKEN_RPAREN";
        case TOKEN_LBRACE:     return "TOKEN_LBRACE";
        case TOKEN_RBRACE:     return "TOKEN_RBRACE";
        case TOKEN_LBRACKET:   return "TOKEN_LBRACKET";
        case TOKEN_RBRACKET:   return "TOKEN_RBRACKET";
        case TOKEN_TILDE:      return "TOKEN_TILDE";
        case TOKEN_MINUS:      return "TOKEN_MINUS";
        case TOKEN_SEMICOLON:  return "TOKEN_SEMICOLON";
        // keywords
        case TOKEN_INT:        return "TOKEN_INT";
        case TOKEN_VOID:       return "TOKEN_VOID";
        case TOKEN_RETURN:     return "TOKEN_RETURN";
        // default case for unknown tokens
        default:               return "TOKEN_UKNOWN";
    }
}
