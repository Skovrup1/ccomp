#include "lexer.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "common.h"
#include "string.h"

char const *keywords[] = {
    "int",
    "main",
    "void",
    "return",
};


char const *token_type_to_string(TokenType type);

TokenArray token_array_init(size_t capacity);
void token_array_push(TokenArray *arr, Token value);
Token token_array_get(TokenArray *arr, size_t i);

bool is_ident_char(char c);

bool lexer_at_end(Lexer *l);

Token lexer_create_token(Lexer *l, TokenType type);
Token lexer_next_token(Lexer *l);

char lexer_next(Lexer *l);
char lexer_peek(Lexer *l);
char lexer_peek_next(Lexer *l);

Token lexer_constants(Lexer *l);

TokenType keyword_type(char const *str, size_t len);
Token lexer_identifiers(Lexer *l);

Token lexer_scan_token(Lexer *l);


char const *token_type_to_string(TokenType type) {
    switch (type) {
    case INT:
        return "INT";
    case MAIN:
        return "MAIN";
    case VOID:
        return "VOID";
    case RETURN:
        return "RETURN";
    case END:
        return "END";
    case INVALID:
        return "INVALID";
    case CONSTANT:
        return "CONSTANT";
    case IDENTIFIER:
        return "IDENTIFIER";
    case SEMICOLON:
        return "SEMICOLON";
    case LPAREN:
        return "LPAREN";
    case RPAREN:
        return "RPAREN";
    case LBRACKET:
        return "LBRACKET";
    case RBRACKET:
        return "RBRACKET";
    case LCURLYBRACKET:
        return "LCURLYBRACKET";
    case RCURLYBRACKET:
        return "RCURLYBRACKET";
    default:
        return "UNKNOWN_TOKEN";
    }
}

TokenArray token_array_init(size_t capacity) {
    TokenArray arr = {
        .data = mem_alloc(capacity),
        .size = 0,
        .capacity = capacity,
    };
    return arr;
}

void token_array_push(TokenArray *arr, Token value) {
    // resize if out of capacity
    if (arr->size == arr->capacity) {
        arr->data = mem_resize(arr->data, arr->capacity, arr->capacity * 2);
        arr->capacity *= 2;
    }

    arr->data[arr->size++] = value;
}

Token token_array_get(TokenArray *arr, size_t i) {
    assert(i < arr->size && "oops, out of bounds\n");

    return arr->data[i];
}

// has to follow an alphabetic character
bool is_ident_char(char c) {
    char extra[] = { '_' };

    for (size_t i = 0; i < ARRAY_SIZE(extra); i++) {
        if (extra[i] == c)
            return true;
    }

    if (isalpha(c))
        return true;

    return false;
}

bool lexer_at_end(Lexer *l) { return l->cursor >= l->source.size; }

Token lexer_create_token(Lexer *l, TokenType type) {
    Token t = {
        .type = type,
        .c0 = l->start,
        .c1 = l->cursor,
        .l0 = l->line_start,
        .l1 = l->line_cursor,
    };

    return t;
}

Token lexer_next_token(Lexer *l) {
    Token t = lexer_scan_token(l);

    while (t.type == EMPTY) {
        t = lexer_scan_token(l);
    }

    return t;
}

// Token lexer_peek_token(Lexer *l) {}

// Token lexer_expect_token(Lexer *l) {}

char lexer_next(Lexer *l) {
    if (lexer_at_end(l))
        return '\0';

    return l->source.data[l->cursor++];
}

char lexer_peek(Lexer *l) {
    if (lexer_at_end(l))
        return '\0';

    return l->source.data[l->cursor];
}

char lexer_peek_next(Lexer *l) {
    if (l->cursor + 1 >= l->source.size)
        return '\0';

    return l->source.data[l->cursor + 1];
}

// char lexer_expect(Lexer *l) {}

Token lexer_constants(Lexer *l) {
    while (isdigit(lexer_peek(l))) {
        lexer_next(l);
    }

    bool is_float = false;
    if (lexer_peek(l) == '.' && isdigit(lexer_peek_next(l))) {
        is_float = true;

        lexer_next(l);
        while (isdigit(lexer_peek(l))) {
            lexer_next(l);
        }
    }

    // length is equal to the number of chars
    // that have been scanned
    size_t len = l->cursor - l->start;
    // we only need the string allocated until
    // it is converted into a number
    char *constant = temp_alloc(len + 1); // also add space for null char
    strncpy(constant, l->source.data + l->start, len);
    constant[len] = '\0';

    if (is_float) {
        assert(0 && "floats are todo!");
    } else {
        // convert string to number
        int number = atol(constant);

        Token t = lexer_create_token(l, CONSTANT);
        t.constant = number;

        return t;
    }
}

TokenType keyword_type(char const *str, size_t len) {
    for (size_t i = 0; i < ARRAY_SIZE(keywords); i++) {
        if (!strncmp(str, keywords[i], len))
            return (TokenType)i;
    }

    return INVALID;
}

Token lexer_identifiers(Lexer *l) {
    while (isalpha(lexer_peek(l))) {
        lexer_next(l);
    }

    if (is_ident_char(lexer_peek(l))) {
        lexer_next(l);

        while (is_ident_char(lexer_peek(l))) {
            lexer_next(l);
        }
    }

    // length is equal to the number of chars
    // that have been scanned
    size_t len = l->cursor - l->start;
    char *ident = mem_alloc(len + 1); // also add space for null char
    strncpy(ident, l->source.data + l->start, len);
    ident[len] = '\0';

    Token t = lexer_create_token(l, keyword_type(ident, len + 1));
    if (t.type == INVALID) {
        t.type = IDENTIFIER;
        t.identifier = ident;
    }

    return t;
}

Token lexer_scan_token(Lexer *l) {
    l->start = l->cursor;
    l->line_start = l->line_cursor;

    char c = lexer_next(l);
    switch (c) {
    case ';':
        return lexer_create_token(l, SEMICOLON);
        break;
    case '(':
        return lexer_create_token(l, LPAREN);
        break;
    case ')':
        return lexer_create_token(l, RPAREN);
        break;
    case '[':
        return lexer_create_token(l, LBRACKET);
        break;
    case ']':
        return lexer_create_token(l, RBRACKET);
        break;
    case '{':
        return lexer_create_token(l, LCURLYBRACKET);
        break;
    case '}':
        return lexer_create_token(l, RCURLYBRACKET);
        break;
    case '\0':
        return lexer_create_token(l, END);
        break;
    case ' ':
        return lexer_create_token(l, EMPTY);
        break;
    case '\n':
        l->line_cursor++;
        return lexer_create_token(l, EMPTY);
        break;
    default:
        if (isdigit(c)) {
            return lexer_constants(l);
        } else if (isalpha(c)) {
            return lexer_identifiers(l);
        } else {
            fprintf(stderr, "invalid character %%d = %d, at c%u, l%u\n", (int)c, l->cursor, l->line_cursor);
            return lexer_create_token(l, INVALID);
        }

        break;
    }
}

TokenArray lexer_scan(Lexer *l) {
    TokenArray tokens = token_array_init(20);

    while (!lexer_at_end(l)) {
        token_array_push(&tokens, lexer_next_token(l));
    }

    return tokens;
}
