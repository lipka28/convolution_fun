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
    size_t size = original_image->width * original_image->height;
    cpu_edge_detection_workload(original_image, processed_image, 0, original_image->width, 0);

    return SUCCES;

}

error Compute_module::fast_cpu_edge_detection(int num_of_threads)
{
    const int max_threads = thread::hardware_concurrency();

    if (num_of_threads == 0 || num_of_threads > max_threads)
    {
        num_of_threads = max_threads;
    }

    vector<thread> threads(num_of_threads);
    size_t width = original_image->width;
    size_t stride = width * (num_of_threads-1);

    for (size_t i = 0; i < num_of_threads; i++)
    {
        threads[i] = thread(cpu_edge_detection_workload,ref(original_image), ref(processed_image),
                   i*width, i*width + width, stride);
    }



    for (auto &t : threads)
    {
        t.join();
    }
    
    return SUCCES;
}

sp_Image Compute_module::get_processed_image() const
{
    return processed_image;
}

void Compute_module::cpu_edge_detection_workload(sp_Image const &original_image,
                                                sp_Image &processed_image,
                                                size_t start_index,
                                                size_t end_index,
                                                const size_t stride)
{
    const size_t height = original_image->height;
    const size_t width = original_image->width;
    const size_t size = width * height;

    const int ed_ker[] = {1, 0, -1, 0, 0, 0, -1, 0, 1};

    for(size_t i = start_index; i < end_index; i++){
        if(i < width || i >= size-width){
            
            if (i == end_index - 1 && (start_index + stride) < size)
            {
            start_index = end_index + stride;
            end_index += end_index + stride;
            i = start_index;
            }
            continue;
        }

        if( (i-1)%width == 0 || (i+1)%width == 0 ){
            if (i == end_index-1 && (start_index + stride) < size)
            {
            start_index = end_index + stride;
            end_index += end_index + stride;
            i = start_index;
            }
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

}