#pragma once
#include <png.h>
#include <vector>

#include "globals.hpp"

using std::vector;

struct Png_image
{
    size_t width;
    size_t height;
    png_byte color_type;
    png_byte bit_depth;

    vector<pixel> r_pixels;
    vector<pixel> g_pixels;
    vector<pixel> b_pixels;
    vector<pixel> a_pixels;
};

class Png_loader
{
private:
    /**
     * Fills Png_image stuct with seperated RGBA pixel data
     */
    static void separate_colors(Png_image &image_data, png_bytepp const &row_data);
    static void merge_colors(png_bytepp &row_data, Png_image const &image_data);
    
public:
    //Png_loader();
    //~Png_loader();
    static Png_image get_png_image_data(const char *file_name);
    static error save_to_file(Png_image const &img_data, const char *output_file);

};
