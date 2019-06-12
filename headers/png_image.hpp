#pragma once
#include <vector>

typedef unsigned char png_byte;

using std::vector;
using pixel = unsigned short;

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