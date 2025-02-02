#include "infixx.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit_with_error(4, "Invalid number of arguments given");
    }
  
    if (strlen_(argv[1]) < 1) {
        exit_with_error(3, "Wrong input");
    }
  
    const char *expression = argv[1];
  
    AST *root = parse(expression);
  
    int32_t result = evaluateAST(root);
  
    printf("%d\n", result);
  
    freeAST(root);
    return 0;
}
