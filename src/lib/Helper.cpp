//
// Created by tomas on 05.04.22.
//

#include <Helper.h>
#include <png.h>


//code edited by me, originally from https://gist.github.com/niw/5963798
std::vector<unsigned char> readPng(const std::string &path){
    FILE *fp = fopen(path.data(), "rb");
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);
    png_init_io(png, fp);
    png_read_info(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);
    png_read_update_info(png, info);
    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * png_get_image_height(png, info));
    for (int y = 0; y < png_get_image_height(png, info); y++) {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, row_pointers);
    std::vector<unsigned char> data;
    for (int y = 0; y < png_get_image_height(png, info); y++) {
        data.insert(data.end(), row_pointers[y], row_pointers[y] + png_get_rowbytes(png, info));
    }
    for (int y = 0; y < png_get_image_height(png, info); y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);
    return data;
}


