#include <cstring>
#include "../headers/compute_module.hpp"

Compute_module::Compute_module(sp_Image const &source_image)
{
    original_image = source_image;
    processed_image = make_shared<Png_image>(*source_image);

}

Compute_module::~Compute_module()
{

}

error Compute_module::slow_cpu_edge_detection()
{
    const size_t height = original_image->height;
    const size_t width = original_image->width;
    const size_t size = width * height;


    int ed_ker[] = {1, 0, -1, 0, 0, 0, -1, 0, 1};

    if(height <= 0 || width <= 0) {
        return FAIL;
    }
    
    for(size_t i = 0; i < size; i++){
        if(i < width || i >= size-width){
            continue;
        }

        if( (i-1)%width == 0 || (i+1)%width == 0 ){
            continue;
        }

        pixel big = 0;
        pixel acum = (pixel)ed_ker[0] * original_image->r_pixels[i+1+width]
                        +ed_ker[1] * original_image->r_pixels[i+width]
                        +ed_ker[2] * original_image->r_pixels[i-1+width]
                        +ed_ker[3] * original_image->r_pixels[i+1]
                        +ed_ker[4] * original_image->r_pixels[i]
                        +ed_ker[5] * original_image->r_pixels[i-1]
                        +ed_ker[6] * original_image->r_pixels[i+1-width]
                        +ed_ker[7] * original_image->r_pixels[i-width]
                        +ed_ker[8] * original_image->r_pixels[i-1-width];
        
        if(acum > 255) {
            acum = 0;
        }

        if(acum > big) {
            big = acum;
        }

        acum = (pixel)ed_ker[0] * original_image->g_pixels[i+1+width]
                    +ed_ker[1] * original_image->g_pixels[i+width]
                    +ed_ker[2] * original_image->g_pixels[i-1+width]
                    +ed_ker[3] * original_image->g_pixels[i+1]
                    +ed_ker[4] * original_image->g_pixels[i]
                    +ed_ker[5] * original_image->g_pixels[i-1]
                    +ed_ker[6] * original_image->g_pixels[i+1-width]
                    +ed_ker[7] * original_image->g_pixels[i-width]
                    +ed_ker[8] * original_image->g_pixels[i-1-width];
        
        if(acum > 255) {
            acum = 0;
        }

        if(acum > big) {
            big = acum;
        }

        acum = (pixel)ed_ker[0] * original_image->b_pixels[i+1+width]
                    +ed_ker[1] * original_image->b_pixels[i+width]
                    +ed_ker[2] * original_image->b_pixels[i-1+width]
                    +ed_ker[3] * original_image->b_pixels[i+1]
                    +ed_ker[4] * original_image->b_pixels[i]
                    +ed_ker[5] * original_image->b_pixels[i-1]
                    +ed_ker[6] * original_image->b_pixels[i+1-width]
                    +ed_ker[7] * original_image->b_pixels[i-width]
                    +ed_ker[8] * original_image->b_pixels[i-1-width];
        
        if(acum > 255) {
            acum = 0;
        }

        if(acum > big) {
            big = acum;
        }

        processed_image->r_pixels[i] = processed_image->g_pixels[i] = processed_image->b_pixels[i] = big; 

    }

    return SUCCES;

}

sp_Image Compute_module::get_processed_image() const
{
    return processed_image;
}