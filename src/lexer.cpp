#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>

char const *keyword_buf[] = {
    "int",
    "main",
    "return",
    "void",
};

char Lexer::peek() {
    if (cursor >= file_buf.size()) {
        return '\0';
    }

    return file_buf[cursor];
}

char Lexer::peek_next() {
    if (cursor + 1 >= file_buf.size()) {
        return '\0';
    }

    return file_buf[cursor + 1];
}

char Lexer::advance() { return file_buf[cursor++]; }

void Lexer::number() {
    while (isdigit(peek())) {
        advance();
    }

    bool is_float = false;
    if (peek() == '.' && isdigit(peek_next())) {
        is_float = true;

        advance();
        while (isdigit(peek())) {
            advance();
        }
    }

    std::string num(file_buf.begin() + cursor_start, file_buf.begin() + cursor);
    if (is_float) {
        printf("floats not supported yet\n");
    } else {
        uint64_t i = std::stoul(num);
        token_buf.push_back({.type = TokenType::CONSTANT, .integer_value = i});
    }
}

void Lexer::word() {
    while (isalpha(peek())) {
        advance();
    }

    std::string word(file_buf.begin() + cursor_start,
                     file_buf.begin() + cursor);

    for (uint32_t i = 0; i < std::size(keyword_buf); i++) {
        if (word == keyword_buf[i]) {
            token_buf.push_back({.type = static_cast<TokenType>(i)});
            return;
        }
    }

    token_buf.push_back({.type = IDENTIFIER, .name = word});
}

void Lexer::scan_token() {
    char c = advance();
    switch (c) {
    case '\0':
        break;
    case ' ':
        break;
    case '\n':
        cursor_line++;
        cursor_prev_line = cursor;
        break;
    case '(':
        token_buf.push_back({.type = LPAREN});
        break;
    case ')':
        token_buf.push_back({.type = RPAREN});
        break;
    case '{':
        token_buf.push_back({.type = LCURLYBRACKET});
        break;
    case '}':
        token_buf.push_back({.type = RCURLYBRACKET});
        break;
    case ';':
        token_buf.push_back({.type = SEMICOLON});
        break;
    case '0' ... '9':
        number();
        break;
    case 'a' ... 'z':
        [[fallthrough]];
    case 'A' ... 'Z':
        word();
        break;
    default:
        fprintf(stderr, "Error: unexpected character c='%d' at %d,%d\n", c,
                cursor_line, cursor - cursor_prev_line);
        break;
    }
}

bool Lexer::at_end() { return cursor >= file_buf.size(); }

void Lexer::translate() {
    while (!at_end()) {
        cursor_start = cursor;
        scan_token();
    }
}

void print_tokens(std::vector<Token> &token_buf) {
    for (uint32_t i = 0; i < token_buf.size(); i++) {
        switch (token_buf[i].type) {
        case CONSTANT:
            printf("CONSTANT");
            break;
        case IDENTIFIER:
            printf("IDENTIFIER(");
            printf(")");
            break;
        case LPAREN:
            printf("LPAREN");
            break;
        case RPAREN:
            printf("RPAREN");
            break;
        case LCURLYBRACKET:
            printf("LCURLYBRACKET");
            break;
        case RCURLYBRACKET:
            printf("RCURLYBRACKET");
            break;
        case SEMICOLON:
            printf("SEMICOLON");
            break;
        case INT:
            printf("INT");
            break;
        case MAIN:
            printf("MAIN");
            break;
        case RETURN:
            printf("RETURN");
            break;
        case VOID:
            printf("VOID");
            break;
        case ERROR:
            printf("ERROR");
            break;
        }

        printf("\n");
    }
}

