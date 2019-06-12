#pragma once
#include <png.h>

#include "globals.hpp"

class Png_loader
{
private:
    /**
     * Fills Png_image stuct with seperated RGBA pixel data
     */
    static void separate_colors(sp_Image &image_data, png_bytepp const &row_data);
    static void merge_colors(png_bytepp &row_data, sp_Image const &image_data);
    
public:
    //Png_loader();
    //~Png_loader();
    static sp_Image get_png_image_data(const char *file_name);
    static error save_to_file(sp_Image const &img_data, const char *output_file);

};
