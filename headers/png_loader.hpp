#pragma once
#include <png.h>

using pixel = unsigned short;

enum error {
    SUCCES = 0,
    FAIL
};

class Png_loader
{
private:
    int width = 0, height = 0;
    png_byte color_type = 0;
    png_byte bit_depth = 0;
    png_bytep *row_pointers = nullptr;
    pixel *pixel_data = nullptr;
    pixel *r_pixels = nullptr;
    pixel *g_pixels = nullptr;
    pixel *b_pixels = nullptr;
    pixel *a_pixels = nullptr;

    void clear_data();
    void separate_colors();
    void merge_colors();
    
public:
    Png_loader(const char *file_name, error &fail_status);
    Png_loader();
    ~Png_loader();
    error read_png_file(const char *file_name);
    error save_to_file(const char *output_file, bool use_separeted_rgba_layers = false);
    
    pixel* get_row_major_RGBA_pixel_data();
    pixel* get_row_major_R_pixel_data();
    pixel* get_row_major_G_pixel_data();
    pixel* get_row_major_B_pixel_data();
    pixel* get_row_major_A_pixel_data();

    error set_row_major_RGBA_pixel_data(pixel *RGBA_pixels);
    error set_row_major_R_pixel_data(pixel *R_pixels);
    error set_row_major_G_pixel_data(pixel *G_pixels);
    error set_row_major_B_pixel_data(pixel *B_pixels);
    error set_row_major_A_pixel_data(pixel *A_pixels);
    
    const int get_width();
    const int get_height();
    error set_image_size(int width, int height);
};
