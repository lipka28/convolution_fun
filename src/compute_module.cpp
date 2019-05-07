#include <cstring>
#include "../headers/compute_module.hpp"

Compute_module::Compute_module()
{
    pixel_data.resize(4);
    pixel_data[0] = nullptr;
    pixel_data[1] = nullptr;
    pixel_data[2] = nullptr;
    pixel_data[3] = nullptr;

    output_image.resize(4);
    output_image[0] = nullptr;
    output_image[1] = nullptr;
    output_image[2] = nullptr;
    output_image[3] = nullptr;
}

Compute_module::~Compute_module()
{
    for(auto data : pixel_data){
        if(data) delete[] data;
    }

    for(auto data : output_image){
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

    pixel *r = new pixel[width*height]();
    memcpy(r, pixel_data[0], sizeof(pixel)*width*height);
    pixel *g = new pixel[width*height]();
    memcpy(g, pixel_data[1], sizeof(pixel)*width*height);
    pixel *b = new pixel[width*height]();
    memcpy(b, pixel_data[2], sizeof(pixel)*width*height);
    pixel *a = new pixel[width*height]();
    memcpy(a, pixel_data[3], sizeof(pixel)*width*height);
    output_image[0] = r;
    output_image[1] = g;
    output_image[2] = b;
    output_image[3] = a;

    return SUCCES;    
}

vector<pixel*> Compute_module::get_RGBA_vect_data()
{
    vector<pixel*> output;
    pixel *r = new pixel[width*height]();
    memcpy(r, output_image[0], sizeof(pixel)*width*height);
    pixel *g = new pixel[width*height]();
    memcpy(g, output_image[1], sizeof(pixel)*width*height);
    pixel *b = new pixel[width*height]();
    memcpy(b, output_image[2], sizeof(pixel)*width*height);
    pixel *a = new pixel[width*height]();
    memcpy(a, output_image[3], sizeof(pixel)*width*height);
    output.push_back(r);
    output.push_back(g);
    output.push_back(b);
    output.push_back(a);

    return output;
}

error Compute_module::slow_cpu_edge_detection()
{
    int ed_ker[] = {1, 0, -1, 0, 0, 0, -1, 0, 1};
    if(height <= 0 || width <= 0) return FAIL;
    if(!pixel_data[0] || !pixel_data[1] || 
       !pixel_data[2] || !pixel_data[3]) return FAIL;
    
    for(int i = 0; i < height*width; i++){
        if(i < width || i >= height*width-width)continue;
        if( (i-1)%width == 0 || (i+1)%width == 0 )continue;
        int big = 0;
        for(int layer = 0; layer < 3; layer++){
            pixel acum = (pixel)ed_ker[0] * pixel_data[layer][i+1+width]
                        +ed_ker[1] * pixel_data[layer][i+width]
                        +ed_ker[2] * pixel_data[layer][i-1+width]
                        +ed_ker[3] * pixel_data[layer][i+1]
                        +ed_ker[4] * pixel_data[layer][i]
                        +ed_ker[5] * pixel_data[layer][i-1]
                        +ed_ker[6] * pixel_data[layer][i+1-width]
                        +ed_ker[7] * pixel_data[layer][i-width]
                        +ed_ker[8] * pixel_data[layer][i-1-width];
            if(acum > 255) acum = 0;
            if(acum > big) big = acum;
            //if(layer > 0 && output_image[layer-1][i] != acum) acum = output_image[layer-1][i]; 
            //output_image[layer][i] = acum;
        }

        output_image[0][i] = output_image[1][i] = output_image[2][i] = big;

    }

    return SUCCES;

}