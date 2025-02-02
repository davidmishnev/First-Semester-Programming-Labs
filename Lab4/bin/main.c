#include "handleArgs.h"
#include "pngConverter.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "A few/Too much arguments given.\n");
        return 1;
    }
    if (!is_input_valid_png(argv[1])) {
        fprintf(stderr, "Wrong input file given.\n");
        return 1;
    }
    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file) {
        fprintf(stderr, "Failed to open input file.\n");
        return 1;
    }
    if (!parse_file_extension(argv[2])) {
        fclose(input_file);
        fprintf(stderr, "Wrong output file given.\n");
        return 1;
    }
    FILE *output_file = fopen(argv[2], "wb");
    if (!output_file) {
        fclose(input_file);
        fprintf(stderr, "Failed to open output file.\n");
        return 1;
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(input_file);
        fclose(output_file);
        fprintf(stderr, "Failed to allocate read struct.\n");
        return 1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(input_file);
        fclose(output_file);
        fprintf(stderr, "Failed to allocate info struct.\n");
        return 1;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(input_file);
        fclose(output_file);
        fprintf(stderr, "Failed to allocate end info struct.\n");
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(input_file);
        fclose(output_file);
        fprintf(stderr, "Error during PNG processing.\n");
        return 1;
    }

    png_init_io(png_ptr, input_file);
    //    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);

    if (!check_png_format(png_ptr, info_ptr, bit_depth, color_type)) {
        fclose(input_file);
        fclose(output_file);
        fprintf(stderr, "Unsupported PNG format.\n");
        return 1;
    }

    if (!convert_png(color_type, width, height, info_ptr, png_ptr, output_file)) {
        fclose(input_file);
        fclose(output_file);
        fprintf(stderr, "Failed to allocate output file read/info struct.\n");
        return 1;
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
