#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define kSize 16

struct uint128_t {
    uint8_t digits[kSize];
};

struct FormattingArgs {
    uint16_t length;
    uint16_t spaces;
    uint16_t width;
    bool is_negative;
    bool is_positive;
    bool length_modifier;
    bool show_prefix;
    bool is_binary;
    bool is_oct;
    bool is_hex_lowercase;
    bool is_hex_uppercase;
    bool show_leading_zeros;
};

struct NumberArgs {
    bool sign;
    uint8_t base;
    uint16_t length;
};

bool is_digit(const char slice) {
    if (slice == '0' || slice == '1' || slice == '2' || slice == '3' || slice == '4' || slice == '5' || slice == '6' ||
        slice == '7' || slice == '8' || slice == '9') {
        return true;
    }
    return false;
}

bool is_number_signed(const char *number) {
    if (number[0] == '-') {
        return true;
    }
    return false;
}

bool is_hex_digit(const char slice) {
    if (slice == 'A' || slice == 'B' || slice == 'C' || slice == 'D' || slice == 'E' || slice == 'F' || slice == 'a' ||
        slice == 'b' || slice == 'c' || slice == 'd' || slice == 'e' || slice == 'f') {
        return true;
    }
    return false;
}

uint16_t strlen_(const char *string) {
    uint16_t i = 0;
    while (string[i] != '\0') {
        i++;
    }
    return i;
}

uint16_t find_number_prefix(const char *number) {
    bool is_prefix_found = false;
    uint16_t iter = 0;

    while (!is_prefix_found && iter < strlen_(number)) {
        if (number[iter] == 'x' || number[iter] == 'X' || number[iter] == 'b' || number[iter] == 'o') {
            is_prefix_found = true;
            if (number[iter] == 'x' || number[iter] == 'X') {
                return 16;
            } else if (number[iter] == 'b') {
                return 2;
            } else if (number[iter] == 'o') {
                return 8;
            }
        }
        iter++;
    }
    if (number[0] == '0' || (number[0] == '-' && number[1] == '0')) {
        return 8;
    }
    return 10;
}

int exit_with_error(const char *message) {
    fprintf(stderr, "An error has occurred: %s\n", message);
    return 1;
}

void add_uint128_t(struct uint128_t *num, uint8_t digit) {
    uint16_t carry = digit;
    for (int i = 0; i < kSize; i++) {
        carry += num->digits[i];
        num->digits[i] = carry % 256;
        carry /= 256;
        if (carry == 0) {
            break;
        }
    }
}

void multiply_uint128_t(struct uint128_t *num, uint8_t base) {
    uint16_t carry = 0;
    for (int i = 0; i < kSize; i++) {
        carry += num->digits[i] * base;
        num->digits[i] = carry % 256;
        carry /= 256;
    }
}

struct uint128_t to_uint128_t(const char *number, bool sign, uint8_t base) {
    struct uint128_t result = {0};
    if (sign) {
        number++;
    }
    if (base == 16 && strlen_(number) > 2 && number[0] == '0' && (number[1] == 'x' || number[1] == 'X')) {
        number += 2;
    } else if (base == 2 && strlen_(number) > 2 && number[0] == '0' && (number[1] == 'b' || number[1] == 'B')) {
        number += 2;
    } else if (base == 8 && strlen_(number) > 1 && number[0] == '0') {
        number += 1;
    }
    for (size_t i = 0; i < strlen_(number); i++) {
        char c = number[i];
        uint8_t value;

        if (is_digit(c)) {
            value = c - '0';
        } else if (is_hex_digit(c)) {
            value = c - 'A' + 10;
        }
        multiply_uint128_t(&result, base);
        add_uint128_t(&result, value);
    }
    return result;
}

bool is_zero(const struct uint128_t *num) {
    for (int i = 0; i < kSize; i++) {
        if (num->digits[i] != 0) {
            return false;
        }
    }
    return true;
}

void print(char *out_buf, const char *format, const char *number) {
    struct FormattingArgs Params = {0};
    struct NumberArgs ProvidedNumber = {0};
    struct uint128_t EvaluatedNumber = {0};
    Params.length = strlen_(format);
    for (uint16_t i = Params.length - 1; i >= 1; i--) {
        if (format[i] == ' ') {
            Params.spaces++;
        } else if (format[i] == '+') {
            Params.is_positive = true;
        } else if (format[i] == '-') {
            Params.is_negative = true;
        } else if (format[i] == '#') {
            Params.show_prefix = true;
        } else if (format[i] == 'd') {
            Params.is_decimal = true;
        } else if (format[i] == 'o') {
            Params.is_oct = true;
        } else if (format[i] == 'x') {
            Params.is_hex_lowercase = true;
        } else if (format[i] == 'X') {
            Params.is_hex_uppercase = true;
        } else if (format[i] == 'b') {
            Params.is_binary = true;
        } else if (is_digit(format[i])) {
            uint16_t multiplier = 1;
            while (is_digit(format[i])) {
                if (!is_digit(format[i - 1]) && format[i] == '0') {
                    Params.show_leading_zeros = true;
                }
                Params.width += (format[i] - '0') * multiplier;
                multiplier *= 10;
                i--;
            }
        }
    }
    ProvidedNumber.base = find_number_prefix(number);
    ProvidedNumber.sign = is_number_signed(number);
    ProvidedNumber.length = strlen_(number);
    EvaluatedNumber = to_uint128_t(number, ProvidedNumber.sign, ProvidedNumber.base);
    char temp[512] = {0};
    int temp_index = 0;
    if (Params.show_prefix) {
        if (Params.is_binary) {
            temp[temp_index++] = '0';
            temp[temp_index++] = 'b';
        } else if (Params.is_oct) {
            temp[temp_index++] = '0';
        } else if (Params.is_hex_lowercase || Params.is_hex_uppercase) {
            temp[temp_index++] = '0';
            temp[temp_index++] = (Params.is_hex_uppercase ? 'X' : 'x');
        }
    }
    struct uint128_t num = EvaluatedNumber;
    if (is_zero(&num)) {
        puts("0");
        return;
    }
    int digits[512];
    int digit_count = 0;
    uint8_t base = 10;
    if (Params.is_binary) {
        base = 2;
    } else if (Params.is_oct) {
        base = 8;
    } else if (Params.is_hex_lowercase || Params.is_hex_uppercase) {
        base = 16;
    }
    while (!is_zero(&num)) {
        int remainder = 0;
        for (int i = kSize - 1; i >= 0; i--) {
            uint16_t current = (remainder << 8) | num.digits[i];
            num.digits[i] = current / base;
            remainder = current % base;
        }
        digits[digit_count++] = remainder;
    }
    for (int i = digit_count - 1; i >= 0; i--) {
        if (digits[i] < 10) {
            temp[temp_index++] = '0' + digits[i];
        } else {
            temp[temp_index++] = (Params.is_hex_uppercase ? 'A' : 'a') + (digits[i] - 10);
        }
    }
    while (Params.show_leading_zeros && temp_index < Params.width) {
        for (int i = temp_index; i > 0; i--) {
            temp[i] = temp[i - 1];
        }
        temp[0] = '0';
        temp_index++;
    }
    temp[temp_index] = '\0';
    if (ProvidedNumber.sign == true && is_zero(&num)) {
        printf("-");
    }
    if (ProvidedNumber.sign == false && Params.is_positive == true) {
        printf("+");
    }
    if (temp_index > Params.width) {
        Params.width = 0;
        Params.is_negative = false;
        Params.show_leading_zeros = false;
    }

    for (int i = 0; i < 256; i++) {
        out_buf[i] = '\0';
    }

    int temp_len = strlen_(temp);
    int sign_len = 0;
    int padding_len = 0;

    if (Params.is_negative) {
        sign_len = 1;
    } else if (Params.is_positive) {
        sign_len = 1;
    } else if (Params.spaces > 0) {
        sign_len = 1;
    }

    if (Params.width > sign_len + temp_len) {
        padding_len = Params.width - sign_len - temp_len;
    } else if (Params.spaces > 0) {
        padding_len = Params.spaces;
    }

    int pos = 0;

    if (!Params.is_negative) {
        char pad_char = Params.show_leading_zeros ? '0' : ' ';
        for (int i = 0; i < padding_len; i++) {
            out_buf[pos++] = pad_char;
        }
    }
    if (Params.is_negative) {
        out_buf[pos++] = '-';
    } else if (Params.is_positive) {
        out_buf[pos++] = '+';
    } else if (Params.spaces > 0) {
        out_buf[pos++] = ' ';
    }
    for (int i = 0; i < temp_len; i++) {
        out_buf[pos++] = temp[i];
    }
    if (Params.is_negative) {
        for (int i = 0; i < padding_len; i++) {
            out_buf[pos++] = ' ';
        }
    }
    out_buf[pos] = '\0';
    puts(out_buf);
}
