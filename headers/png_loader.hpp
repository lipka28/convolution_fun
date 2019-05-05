#pragma once
#include <png.h>

using pixel = unsigned char;

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

    void clear_data();
    
public:
    Png_loader(const char *file_name, error &fail_status);
    Png_loader();
    ~Png_loader();
    error read_png_file(const char *file_name);
    error save_to_file(const char *output_file);
    pixel* get_row_major_RGBA_pixel_data();
    int get_width() const;
    int get_height() const;
};
