#pragma once
#include "pngConverter.h"

bool is_input_valid_png(const char* argv1);

bool parse_file_extension(const char* argv);

bool signatureCompare(const unsigned char str1[], const unsigned char str2[]);

bool check_png_format(png_structp png_ptr, png_infop info_ptr, png_byte bit_depth, png_byte color_type);
