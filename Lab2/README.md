# `sprintf` Implementation

## Tools and Requirements

- The task must be implemented in C (C11 or later).
- The server builds the project using C17.

## Task Description

The goal is to write a program that includes a function capable of converting numbers from one numeral system to another, with additional formatting.

- Standard functions may be used, but during evaluation, you must be able to quickly implement the necessary library functions manually.
- **Usage of `sprintf` and similar functions is prohibited**, as they cannot be quickly reimplemented.
- The number being converted is a **128-bit signed integer**, stored in **two’s complement** format.
- The input number may be in **binary (2), octal (8), decimal (10), or hexadecimal (16)** notation, as determined by its **prefix** according to C language rules.
- A leading `'-'` should be strictly interpreted as **"invert all bits of the input number and add 1"**, which is useful for representing negative numbers (but not the only way to obtain a negative value).
- Input letters (for hexadecimal values) may be in **any case**.

### Formatting Rules

- The output format follows the **`printf` formatting style**. Reference: [printf, snprintf, ... - cppreference.com](https://en.cppreference.com/w/c/io/fprintf).
- Supported **conversion specifiers**: `'b'`, `'o'`, `'d'`, `'x'`, `'X'`.
- Supported **flags**: `' '` (space), `'-'`, `'+'`, `'#'`, `'0'`, **field width**.
- **The output number must always include a sign**, regardless of the numeral system (unlike standard C behavior for non-decimal bases).
- The `'#'` flag must be ignored for **decimal (`'d'`) conversions** but must be respected for other numeral systems.

## Implementation Requirements

The function to be implemented must have the following prototype:

```c
void print(char *out_buf, const char *format, const char *number);
