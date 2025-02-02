#include "pngConverter.h"

bool convert_png(png_byte color_type, png_uint_32 width, png_uint_32 height, png_infop info_ptr, png_structp png_ptr,
                 FILE *output_file) {
    png_bytep *row = malloc(sizeof(png_bytep) * height);
    if (!row) {
        return false;
    }

    for (png_uint_32 i = 0; i < height; i++) {
        row[i] = (png_bytep) malloc(png_get_rowbytes(png_ptr, info_ptr));
        if (!row[i]) {
            for (png_uint_32 j = 0; j < i; j++) {
                free(row[j]);
            }
            free(row);
            return false;
        }
    }

    png_read_image(png_ptr, row);

    png_structp png_ptr_out = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr_out) {
        for (png_uint_32 i = 0; i < height; i++) {
            free(row[i]);
        }
        free(row);
        return false;
    }

    png_infop info_ptr_out = png_create_info_struct(png_ptr_out);
    if (!info_ptr_out) {
        png_destroy_write_struct(&png_ptr_out, NULL);
        for (png_uint_32 i = 0; i < height; i++) {
            free(row[i]);
        }
        free(row);
        return false;
    }

    png_init_io(png_ptr_out, output_file);

    if (setjmp(png_jmpbuf(png_ptr_out))) {
        png_destroy_write_struct(&png_ptr_out, &info_ptr_out);
        for (png_uint_32 i = 0; i < height; i++) {
            free(row[i]);
        }
        free(row);
        return false;
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
        png_set_IHDR(png_ptr_out, info_ptr_out, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    } else if (color_type == PNG_COLOR_TYPE_RGB) {
        png_set_rgb_to_gray(png_ptr, 1, -1, -1);
        png_set_IHDR(png_ptr_out, info_ptr_out, width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    } else if (color_type == PNG_COLOR_TYPE_GRAY) {
        png_set_IHDR(png_ptr_out, info_ptr_out, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    }

    png_set_rows(png_ptr_out, info_ptr_out, row);
    png_write_png(png_ptr_out, info_ptr_out, PNG_TRANSFORM_IDENTITY, NULL);
    png_write_end(png_ptr_out, NULL);

    for (png_uint_32 i = 0; i < height; i++) {
        free(row[i]);
    }
    free(row);

    png_destroy_write_struct(&png_ptr_out, &info_ptr_out);
    return true;
}
