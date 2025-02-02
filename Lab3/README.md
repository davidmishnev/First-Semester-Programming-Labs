# Infix Notation Parser and Evaluator

## Task Description

The program must parse and evaluate an arithmetic expression written in **infix notation**.

### Input and Output Specifications

- **Input values, output values, and intermediate computations** must use **32-bit signed integers** in **decimal notation**.
- **Allowed operators and tokens**:
  - All **standard arithmetic operators** ([Arithmetic operators - cppreference.com](https://en.cppreference.com/w/c/language/operator_arithmetic)).
  - **Parentheses**: `(` and `)`.
  - **Exponentiation (`**`)**: 
    - Has the **highest precedence**.
    - Performs **integer exponentiation**.
  - **Integer arithmetic only** (no floating-point operations).

### Parsing Rules

- Expressions **must support operators of different precedence levels**.
- **Simple expressions** like `"3 + 5"` alone are **not sufficient** for program acceptance; it must correctly evaluate **complex expressions with mixed operator precedence**.
- **Operands and operators are always separated by spaces**, except for **unary operators**, where spaces may be omitted.

## Implementation Requirements

- The program takes a **single argument**: the arithmetic expression.

