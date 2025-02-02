#pragma once
#include <png.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool convert_png(png_byte color_type, png_uint_32 width, png_uint_32 height, png_infop info_ptr, png_structp png_ptr, FILE* output_file);
