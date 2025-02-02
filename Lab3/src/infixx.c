#include "infixx.h"

int exit_with_error(const int exit_code, const char *message) {
    fprintf(stderr, "%s\n", message);
    exit(exit_code);
    //TO DO: replace the exit with * errors!
}

AST *insertOperationNode(enum TokenType type, AST *left, AST *right) {
    AST *newNode = (AST *) malloc(sizeof(AST));
    if (!newNode) {
        return NULL;
    }
    newNode->type = type;
    switch (type) {
        case AST_add:
            newNode->data.AST_add.left = left;
            newNode->data.AST_add.right = right;
            break;
        case AST_sub:
            newNode->data.AST_sub.left = left;
            newNode->data.AST_sub.right = right;
            break;
        case AST_mul:
            newNode->data.AST_mul.left = left;
            newNode->data.AST_mul.right = right;
            break;
        case AST_div:
            newNode->data.AST_div.left = left;
            newNode->data.AST_div.right = right;
            break;
        case AST_exp:
            newNode->data.AST_exp.left = left;
            newNode->data.AST_exp.right = right;
            break;
        case AST_remain:
            newNode->data.AST_remain.left = left;
            newNode->data.AST_remain.right = right;
            break;
        case AST_bitwiseNOT:
            newNode->data.AST_bitwiseNOT.left = left;
            newNode->data.AST_bitwiseNOT.right = right;
            break;
        case AST_bitwiseAND:
            newNode->data.AST_bitwiseAND.left = left;
            newNode->data.AST_bitwiseAND.right = right;
            break;
        case AST_bitwiseOR:
            newNode->data.AST_bitwiseOR.left = left;
            newNode->data.AST_bitwiseOR.right = right;
            break;
        case AST_bitwiseXOR:
            newNode->data.AST_bitwiseXOR.left = left;
            newNode->data.AST_bitwiseXOR.right = right;
            break;
        case AST_bitwiseLSHIFT:
            newNode->data.AST_bitwiseLSHIFT.left = left;
            newNode->data.AST_bitwiseLSHIFT.right = right;
            break;
        case AST_bitwiseRSHIFT:
            newNode->data.AST_bitwiseRSHIFT.left = left;
            newNode->data.AST_bitwiseRSHIFT.right = right;
            break;
        default:
            free(newNode);
            return NULL;
    }
    return newNode;
}

bool is_digit(const char slice) {
    return slice >= '0' && slice <= '9';
}

size_t strlen_(const char *string) {
    size_t i = 0;
    while (string[i] != '\0') {
        i++;
    }
    return i;
}

bool is_operator(const char *c) {
    static const char *operators[] = {"+", "-", "*", "/", "%", "^", "~", "&", "|", "<<", ">>", "**"};
    size_t num_operators = sizeof(operators) / sizeof(operators[0]);

    for (size_t i = 0; i < num_operators; i++) {
        if (strcmp(c, operators[i]) == 0) {
            return true;
        }
    }
    return false;
}

AST *insertNumberNode(int32_t value) {
    AST *newNode = (AST *) malloc(sizeof(AST));
    if (!newNode) {
        return NULL;
    }
    newNode->type = AST_number;
    newNode->data.AST_number.number = value;
    return newNode;
}

void freeAST(AST *node) {
    if (!node) {
        return;
    }
    switch (node->type) {
        case AST_number:
            break;
        case AST_add:
            freeAST(node->data.AST_add.left);
            freeAST(node->data.AST_add.right);
            break;
        case AST_sub:
            freeAST(node->data.AST_sub.left);
            freeAST(node->data.AST_sub.right);
            break;
        case AST_mul:
            freeAST(node->data.AST_mul.left);
            freeAST(node->data.AST_mul.right);
            break;
        case AST_div:
            freeAST(node->data.AST_div.left);
            freeAST(node->data.AST_div.right);
            break;
        case AST_exp:
            freeAST(node->data.AST_exp.left);
            freeAST(node->data.AST_exp.right);
            break;
        case AST_remain:
            freeAST(node->data.AST_remain.left);
            freeAST(node->data.AST_remain.right);
            break;
        case AST_bitwiseNOT:
            freeAST(node->data.AST_bitwiseNOT.left);
            break;
        case AST_bitwiseAND:
        case AST_bitwiseOR:
        case AST_bitwiseXOR:
        case AST_bitwiseLSHIFT:
        case AST_bitwiseRSHIFT:
            freeAST(node->data.AST_bitwiseAND.left);
            freeAST(node->data.AST_bitwiseAND.right);
            break;
        default:
            break;
    }
    free(node);
}

int32_t evaluateAST(const AST *node) {
    switch (node->type) {
        case AST_number:
            return node->data.AST_number.number;
        case AST_add:
            return evaluateAST(node->data.AST_add.left) + evaluateAST(node->data.AST_add.right);
        case AST_sub:
            return evaluateAST(node->data.AST_sub.left) - evaluateAST(node->data.AST_sub.right);
        case AST_mul:
            return evaluateAST(node->data.AST_mul.left) * evaluateAST(node->data.AST_mul.right);
        case AST_div: {
            int32_t right = evaluateAST(node->data.AST_div.right);
            if (right == 0) {
                exit_with_error(2, "Division by zero");
            }
            return evaluateAST(node->data.AST_div.left) / right;
        }
        case AST_exp: {
            int32_t base = evaluateAST(node->data.AST_exp.left);
            int32_t exp = evaluateAST(node->data.AST_exp.right);
            if (exp < 0) {
                exit_with_error(2, "Negative exponent");
            }
            int32_t result = 1;
            for (int32_t i = 0; i < exp; ++i) {
                result *= base;
            }
            return result;
        }
        case AST_remain:
            return evaluateAST(node->data.AST_remain.left) % evaluateAST(node->data.AST_remain.right);
        case AST_bitwiseNOT:
            return ~evaluateAST(node->data.AST_bitwiseNOT.left);
        case AST_bitwiseAND:
            return evaluateAST(node->data.AST_bitwiseAND.left) & evaluateAST(node->data.AST_bitwiseAND.right);
        case AST_bitwiseOR:
            return evaluateAST(node->data.AST_bitwiseOR.left) | evaluateAST(node->data.AST_bitwiseOR.right);
        case AST_bitwiseXOR:
            return evaluateAST(node->data.AST_bitwiseXOR.left) ^ evaluateAST(node->data.AST_bitwiseXOR.right);
        case AST_bitwiseLSHIFT:
            return evaluateAST(node->data.AST_bitwiseLSHIFT.left) << evaluateAST(node->data.AST_bitwiseLSHIFT.right);
        case AST_bitwiseRSHIFT:
            return evaluateAST(node->data.AST_bitwiseRSHIFT.left) >> evaluateAST(node->data.AST_bitwiseRSHIFT.right);
        default:
            exit_with_error(2, "Unknown operation encountered");
    }
}

bool tokenize(const char *expression, TokenQueue *queue) {
    queue->size = 0;
    size_t length = strlen_(expression);
    size_t parentheses_count = 0;
    for (size_t i = 0; i < length;) {
        char c = expression[i];
        if (c == ' ') {
            if (expression[i + 1] == ' ') {
                exit_with_error(3, "Wrong input");
            }
            i++;
            continue;
        }
        if (is_digit(c)) {
            int32_t number = 0;
            while (i < length && is_digit(expression[i])) {
                number = number * 10 + (expression[i] - '0');
                i++;
            }
            queue->tokens[queue->size++] = (Token) {.type = TOKEN_NUMBER, .value.number = number};
        } else if (is_operator((char[]) {c, '\0'})) {
            if (i + 1 < length) {
                char two_chars[3] = {c, expression[i + 1], '\0'};
                if (is_operator(two_chars)) {
                    if (i + 2 < length && expression[i + 2] != ' ') {
                        exit_with_error(3, "Wrong input");
                    }
                    queue->tokens[queue->size++] =
                            (Token) {.type = TOKEN_OPERATOR, .value.operator= {two_chars[0], two_chars[1]}};
                    i += 2;
                    continue;
                }
            }

            queue->tokens[queue->size++] = (Token) {.type = TOKEN_OPERATOR, .value.operator= {c, '\0'}};
            i++;
        } else if (c == '(') {
            parentheses_count++;
            queue->tokens[queue->size++] = (Token) {.type = TOKEN_LPAREN};
            i++;
        } else if (c == ')') {
            parentheses_count--;
            queue->tokens[queue->size++] = (Token) {.type = TOKEN_RPAREN};
            i++;
        } else {
            exit_with_error(1, "Unknown operation encountered");
        }
    }
    if (parentheses_count != 0) {
        exit_with_error(3, "Unmatched Parentheses");
    }
    return (parentheses_count == 0);
}

AST *parseTerm(TokenQueue *queue, size_t *pos) {
    Token token = queue->tokens[*pos];
    if (token.type == TOKEN_OPERATOR) {
        if (strcmp(token.value.operator, "-") == 0) {
            (*pos)++;
            AST *operand = parseTerm(queue, pos);
            if (!operand) {
                exit_with_error(3, "Invalid unary '-' usage");
            }
            AST *node = (AST *) malloc(sizeof(AST));
            node->type = AST_unaryMINUS;
            node->data.AST_unaryMINUS.operand = operand;
            return node;
        } else if (strcmp(token.value.operator, "+") == 0) {
            (*pos)++;
            return parseTerm(queue, pos);
        }
    }

    if (token.type == TOKEN_NUMBER) {
        (*pos)++;
        return insertNumberNode(token.value.number);
    } else if (token.type == TOKEN_LPAREN) {
        (*pos)++;
        AST *expr = parseExpression(queue, pos);
        if (*pos >= queue->size || queue->tokens[*pos].type != TOKEN_RPAREN) {
            exit_with_error(3, "Unmatched parentheses");
        }
        (*pos)++;
        return expr;
    }
    exit_with_error(3, "Unknown type");
}

AST *parseExpression(TokenQueue *queue, size_t *pos) {
    AST *left = parseTerm(queue, pos);
    while (*pos < queue->size) {
        Token token = queue->tokens[*pos];
        if (token.type != TOKEN_OPERATOR) {
            break;
        }
        enum TokenType type;
        if (strcmp(token.value.operator, "**") == 0) {
            type = AST_exp;
        } else if (strcmp(token.value.operator, "*") == 0) {
            type = AST_mul;
        } else if (strcmp(token.value.operator, "/") == 0) {
            type = AST_div;
        } else if (strcmp(token.value.operator, "%") == 0) {
            type = AST_remain;
        } else if (strcmp(token.value.operator, "+") == 0) {
            type = AST_add;
        } else if (strcmp(token.value.operator, "-") == 0) {
            type = AST_sub;
        } else if (strcmp(token.value.operator, "~") == 0) {
            type = AST_bitwiseNOT;
        } else if (strcmp(token.value.operator, "&") == 0) {
            type = AST_bitwiseAND;
        } else if (strcmp(token.value.operator, "|") == 0) {
            type = AST_bitwiseOR;
        } else if (strcmp(token.value.operator, "^") == 0) {
            type = AST_bitwiseXOR;
        } else if (strcmp(token.value.operator, "<<") == 0) {
            type = AST_bitwiseLSHIFT;
        } else if (strcmp(token.value.operator, ">>") == 0) {
            type = AST_bitwiseRSHIFT;
        } else {
            fprintf(stderr, "Unknown operator: %s", token.value.operator);
            freeAST(left);
            return NULL;
        }
        (*pos)++;
        AST *right = parseTerm(queue, pos);
        left = insertOperationNode(type, left, right);
    }
    return left;
}

AST *parse(const char *expression) {
    TokenQueue queue;
    tokenize(expression, &queue);
    size_t pos = 0;
    AST *root = parseExpression(&queue, &pos);
    return root;
}
