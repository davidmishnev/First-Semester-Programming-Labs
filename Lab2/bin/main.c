#include "sprintff.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        return exit_with_error("Invalid number of arguments");
    }
  
    const char *format = argv[1];
    const char *number = argv[2];
  
    uint16_t format_length = strlen_(format);
    uint16_t number_length = strlen_(number);
  
    if (format_length < 2 || format[0] != '%' ||
        (format[format_length - 1] != 'x' && format[format_length - 1] != 'X' && format[format_length - 1] != 'b' &&
         format[format_length - 1] != 'd' && format[format_length - 1] != 'o')) {
        return exit_with_error("Invalid format given");
    }
  
    if (number_length == 0 || (number_length == 1 && (!is_digit(number[0]) && !is_hex_digit(number[0])))) {
        return exit_with_error("Invalid number given");
    }
  
    char out_buf[256];
  
    print(out_buf, format, number);
  
    return 0;
}
