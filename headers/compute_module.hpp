#pragma once
#include <vector>

#include "globals.hpp"

using namespace std;

class Compute_module
{
private:
    int width = 0, height = 0;
    vector<pixel*> pixel_data;

public:
    Compute_module();
    ~Compute_module();
    error setup_compute_module(int height, int width, 
                               pixel *r_data, pixel *g_data,
                               pixel *b_data, pixel *a_data);
};
