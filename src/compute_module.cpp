#include "../headers/compute_module.hpp"

Compute_module::Compute_module()
{
    pixel_data.reserve(4);
    pixel_data[0] = nullptr;
    pixel_data[1] = nullptr;
    pixel_data[2] = nullptr;
    pixel_data[3] = nullptr;
}

Compute_module::~Compute_module()
{
    for(auto data : pixel_data){
        if(data) delete[] data;
    }

}

error Compute_module::setup_compute_module(int height, int width,
                                           pixel *r_data, pixel *g_data,
                                           pixel *b_data, pixel *a_data)
{
    if(height <= 0 || width <= 0) return FAIL;
    if(!r_data || !g_data || !b_data || !a_data) return FAIL;

    this->height = height;
    this->width = width;
    pixel_data[0] = r_data;
    pixel_data[1] = g_data;
    pixel_data[2] = b_data;
    pixel_data[3] = a_data;

    return SUCCES;    
}