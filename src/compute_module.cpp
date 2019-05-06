#include <cstring>
#include "../headers/compute_module.hpp"

Compute_module::Compute_module()
{
    pixel_data.resize(4);
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

vector<pixel*> Compute_module::get_RGBA_vect_data()
{
    vector<pixel*> output;
    pixel *r = new pixel[width*height]();
    memcpy(r, pixel_data[0], sizeof(pixel)*width*height);
    pixel *g = new pixel[width*height]();
    memcpy(g, pixel_data[1], sizeof(pixel)*width*height);
    pixel *b = new pixel[width*height]();
    memcpy(b, pixel_data[2], sizeof(pixel)*width*height);
    pixel *a = new pixel[width*height]();
    memcpy(a, pixel_data[3], sizeof(pixel)*width*height);
    output.push_back(r);
    output.push_back(g);
    output.push_back(b);
    output.push_back(a);

    return output;
}

error Compute_module::slow_cpu_edge_detection()
{
    if(height <= 0 || width <= 0) return FAIL;
    if(!pixel_data[0] || !pixel_data[1] || 
       !pixel_data[2] || !pixel_data[3]) return FAIL;
    
    for(int i = 0; i < height*width; i++){
        if(i < width || i >= height*width-width)continue;
        if( (i-1)%width == 0 || (i+1)%width == 0 )continue;

        for(auto px : pixel_data){
            px[i] = 0;
        }
    }

    return SUCCES;

}