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

void Lexer::add_token(TokenType type) {
    Token token = {.type = type, .i = 0};
    token_buf.push_back(token);
}

void Lexer::add_token_i(TokenType type, std::string word) {
    Token token = {.type = type,
                     .i = static_cast<uint32_t>(identifier_buf.size() - 1)};

    identifier_buf.push_back(word);
    token_buf.push_back(token);
}

void Lexer::add_token_k(TokenType type, uint32_t i) {
    Token token = {.type = type, .i = i};

    token_buf.push_back(token);
}

void Lexer::add_token_c(TokenType type, uint32_t i) {
    Token token = {.type = type,
                     .i = static_cast<uint32_t>(constant_buf.size() - 1)};

    constant_buf.push_back(i);
    token_buf.push_back(token);
}

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
        int i = std::stoi(num);
        add_token_c(CONSTANT, i);
    }
}

void Lexer::word() {
    while (isalpha(peek())) {
        advance();
    }

    /*if (isgraph(peek())) {
        advance();

        while (isgraph(peek())) {
            advance();
        }
    }*/

    std::string word(file_buf.begin() + cursor_start,
                     file_buf.begin() + cursor);

    for (uint32_t i = 0; i < std::size(keyword_buf); i++) {
        if (word == keyword_buf[i]) {
            add_token(static_cast<TokenType>(i));
            return;
        }
    }

    add_token_i(IDENTIFIER, std::move(word.data()));
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
        add_token(LPAREN);
        break;
    case ')':
        add_token(RPAREN);
        break;
    case '{':
        add_token(LCURLYBRACKET);
        break;
    case '}':
        add_token(RCURLYBRACKET);
        break;
    case ';':
        add_token(SEMICOLON);
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

void print_tokens(Lexer &l) {
    for (uint32_t i = 0; i < l.token_buf.size(); i++) {
        switch (l.token_buf[i].type) {
        case CONSTANT:
            printf("CONSTANT");
            break;
        case IDENTIFIER:
            printf("IDENTIFIER(");
            printf("%s", l.identifier_buf[l.token_buf[i].i].c_str());
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
        }

        printf("\n");
    }
}

void Lexer::translate() {
    while (!at_end()) {
        cursor_start = cursor;
        scan_token();
    }

    print_tokens(*this);
}
