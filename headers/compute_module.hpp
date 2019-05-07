#pragma once
#include <vector>

#include "globals.hpp"

using namespace std;

class Compute_module
{
private:
    int width = 0, height = 0;
    vector<pixel*> pixel_data;
    vector<pixel*> output_image;

public:
    Compute_module();
    ~Compute_module();
    error setup_compute_module(int height, int width, 
                               pixel *r_data, pixel *g_data,
                               pixel *b_data, pixel *a_data);
    vector<pixel*> get_RGBA_vect_data();
    error slow_cpu_edge_detection();
};
