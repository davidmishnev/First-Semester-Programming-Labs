#include "handleArgs.h"

bool parse_file_extension(const char *argv) {
    size_t iter = 0;
    while (argv[iter] != '\0') {
        if (argv[iter] == '.') {
            if (argv[iter + 1] == 'p' && argv[iter + 2] == 'n' && argv[iter + 3] == 'g' && argv[iter + 4] == '\0') {
                return true;
            }
            return false;
        }
        iter++;
    }
    return false;
}

bool is_input_valid_png(const char *argv1) {
    FILE *input_file = fopen(argv1, "rb");
    if (!input_file) {
        return false;
    }
    unsigned char png_signature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    unsigned char buffer[8];
    fread(buffer, 1, 8, input_file);
    fclose(input_file);
    return signatureCompare(png_signature, buffer);
}

bool signatureCompare(const unsigned char str1[], const unsigned char str2[]) {
    for (size_t ptr = 0; ptr < 8; ptr++) {
        if (str1[ptr] != str2[ptr]) {
            return false;
        }
    }
    return true;
}

bool check_png_format(png_structp png_ptr, png_infop info_ptr, png_byte bit_depth, png_byte color_type) {
    if (bit_depth != 8 || (color_type != PNG_COLOR_TYPE_GRAY && color_type != PNG_COLOR_TYPE_RGB &&
                           color_type != PNG_COLOR_TYPE_PALETTE)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }
    return true;
}
