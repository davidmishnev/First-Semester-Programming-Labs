#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AST AST;
typedef struct Token Token;
typedef struct TokenQueue TokenQueue;

struct AST {
    enum TokenType {
        AST_number,
        AST_add,
        AST_sub,
        AST_mul,
        AST_div,
        AST_exp,
        AST_remain,
        AST_bitwiseNOT,
        AST_bitwiseAND,
        AST_bitwiseOR,
        AST_bitwiseXOR,
        AST_bitwiseLSHIFT,
        AST_bitwiseRSHIFT,
        AST_unaryPLUS,
        AST_unaryMINUS
    } type;

    union {
        struct AST_number {
            int32_t number;
        } AST_number;

        struct AST_add {
            AST *left;
            AST *right;
        } AST_add;

        struct AST_sub {
            AST *left;
            AST *right;
        } AST_sub;

        struct AST_mul {
            AST *left;
            AST *right;
        } AST_mul;

        struct AST_div {
            AST *left;
            AST *right;
        } AST_div;

        struct AST_exp {
            AST *left;
            AST *right;
        } AST_exp;

        struct AST_remain {
            AST *left;
            AST *right;
        } AST_remain;

        struct AST_bitwiseNOT {
            AST *left;
            AST *right;
        } AST_bitwiseNOT;

        struct AST_bitwiseAND {
            AST *left;
            AST *right;
        } AST_bitwiseAND;

        struct AST_bitwiseOR {
            AST *left;
            AST *right;
        } AST_bitwiseOR;

        struct AST_bitwiseXOR {
            AST *left;
            AST *right;
        } AST_bitwiseXOR;

        struct AST_bitwiseLSHIFT {
            AST *left;
            AST *right;
        } AST_bitwiseLSHIFT;

        struct AST_bitwiseRSHIFT {
            AST *left;
            AST *right;
        } AST_bitwiseRSHIFT;

        struct AST_unaryPLUS {
            AST *operand;
        } AST_unaryPLUS;

        struct AST_unaryMINUS {
            AST *operand;
        } AST_unaryMINUS;
    } data;
};

struct Token {
    enum {
        TOKEN_NUMBER,
        TOKEN_OPERATOR,
        TOKEN_LPAREN,
        TOKEN_RPAREN
    } type;

    union {
        int32_t number;
        char operator[3];
    } value;
};

struct TokenQueue {
    struct Token tokens[256];
    size_t size;
};

size_t strlen_(const char *string);

bool is_digit(char slice);

bool is_operator(const char *c);

bool tokenize(const char *expression, TokenQueue *queue);

void freeAST(AST *node);

AST *insertOperationNode(enum TokenType type, AST *left, AST *right);

AST *insertNumberNode(int32_t value);

AST *parseExpression(TokenQueue *queue, size_t *pos);

AST *parseTerm(TokenQueue *queue, size_t *pos);

AST *parseExpression(TokenQueue *queue, size_t *pos);

AST *parse(const char *expression);

int32_t evaluateAST(const AST *node);

int exit_with_error(const int exit_code, const char *message);
