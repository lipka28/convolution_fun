#include "../headers/png_loader.hpp"

Png_loader::Png_loader(const char *file_name, error &fail_status)
{
   fail_status = read_png_file(file_name);
}

Png_loader::Png_loader()
{
}

error Png_loader::read_png_file(const char *file_name)
{
    clear_data();

    FILE *input_image = fopen(file_name, "rb");
    if(!input_image) return FAIL;
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                nullptr, nullptr, nullptr);
    
    if(!png) {fclose(input_image); return FAIL;}
    png_infop info = png_create_info_struct(png);
    if(!info) {fclose(input_image); return FAIL;}
    if(setjmp(png_jmpbuf(png))) {fclose(input_image); return FAIL;}

    png_init_io(png, input_image);
    png_read_info(png, info);

    width       = png_get_image_width(png, info);
    height      = png_get_image_height(png, info);
    color_type  = png_get_color_type(png, info);
    bit_depth   = png_get_bit_depth(png, info);

    if(bit_depth == 16)
    png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

    if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = new png_bytep[height]();
    for (int y = 0; y < height; y++){
        row_pointers[y] = new png_byte[png_get_rowbytes(png, info)]();
    }

    png_read_image(png, row_pointers);
    separate_colors();

    fclose(input_image);

    return SUCCES;
}

error Png_loader::save_to_file(const char *output_file, bool use_separeted_rgba_layers)
{
    if(use_separeted_rgba_layers) merge_colors(); // will merge color layers if available
    if(!row_pointers)return FAIL;

    FILE *output = fopen(output_file, "wb");
    if(!output) return FAIL;
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                            nullptr, nullptr, nullptr);
    if(!png) {fclose(output); return FAIL;}
    png_infop info = png_create_info_struct(png);
    if(!info) {fclose(output); return FAIL;}
    if(setjmp(png_jmpbuf(png))) {fclose(output); return FAIL;}

    png_init_io(png, output);
    png_set_IHDR(
        png,
        info,
        width,
        height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);
    png_write_image(png, row_pointers);
    png_write_end(png, info);

    fclose(output);

    return SUCCES;
}

void Png_loader::clear_data()
{
    if(row_pointers){
        for (int y = 0; y < height; y++){
            delete[] row_pointers[y];
        }
        delete[] row_pointers;
        row_pointers = nullptr;
    }
    if (r_pixels) {delete[] r_pixels; r_pixels = nullptr;}
    if (g_pixels) {delete[] g_pixels; g_pixels = nullptr;}
    if (b_pixels) {delete[] b_pixels; b_pixels = nullptr;}
    if (a_pixels) {delete[] a_pixels; a_pixels = nullptr;}
}

int Png_loader::get_height()
{
    return height;
}

int Png_loader::get_width()
{
    return width;
}

pixel* Png_loader::get_row_major_RGBA_pixel_data()
{
    if(!row_pointers) return nullptr;

    pixel_data = new pixel[width * height * 4]();

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width * 4; x++){
            pixel_data[height*y+x] = row_pointers[y][x];
        }
    }
    
    return pixel_data;
}

void Png_loader::separate_colors()
{
    if(row_pointers){

        if(r_pixels && g_pixels &&
            b_pixels && a_pixels){
            delete[] r_pixels;
            delete[] g_pixels;
            delete[] b_pixels;
            delete[] a_pixels;

        }

        r_pixels = new pixel[height*width]();
        g_pixels = new pixel[height*width]();
        b_pixels = new pixel[height*width]();
        a_pixels = new pixel[height*width]();
        
        for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                r_pixels[y*width+x] = row_pointers[y][x*4];
                g_pixels[y*width+x] = row_pointers[y][x*4+1];
                b_pixels[y*width+x] = row_pointers[y][x*4+2];
                a_pixels[y*width+x] = row_pointers[y][x*4+3];
            }
        }
    }

}

void Png_loader::merge_colors()
{
    if(r_pixels && g_pixels &&
        b_pixels && a_pixels && row_pointers){

        if(!row_pointers){
        row_pointers = new png_bytep[height]();
            for (int y = 0; y < height; y++){
                row_pointers[y] = new png_byte[width*4]();
            }
        }
            
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                row_pointers[y][x*4] = r_pixels[y*width+x];
                row_pointers[y][x*4+1] = g_pixels[y*width+x];
                row_pointers[y][x*4+2] = b_pixels[y*width+x];
                row_pointers[y][x*4+3] = a_pixels[y*width+x];
            }
        }

    }
}

pixel* Png_loader::get_row_major_R_pixel_data()
{
    pixel *pixel_data = new pixel[width*height]();
    for(int y = 0; y < width*height; y++){
        pixel_data[y] = r_pixels[y];
    }
    return pixel_data;
}

pixel* Png_loader::get_row_major_G_pixel_data()
{
    pixel *pixel_data = new pixel[width*height]();
    for(int y = 0; y < width*height; y++){
        pixel_data[y] = g_pixels[y];
    }
    return pixel_data;
}

pixel* Png_loader::get_row_major_B_pixel_data()
{
    pixel *pixel_data = new pixel[width*height]();
    for(int y = 0; y < width*height; y++){
        pixel_data[y] = b_pixels[y];
    }
    return pixel_data;
}

pixel* Png_loader::get_row_major_A_pixel_data()
{
    pixel *pixel_data = new pixel[width*height]();
    for(int y = 0; y < width*height; y++){
        pixel_data[y] = r_pixels[y];
    }
    return pixel_data;
}

error Png_loader::set_row_major_RGBA_pixel_data(pixel *RGBA_pixels)
{
    clear_data();

    row_pointers = new png_bytep[height]();
    for (int y = 0; y < height; y++){
        row_pointers[y] = new png_byte[width*4]();
    }

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            row_pointers[y][x] = pixel_data[height*y+x];
        }
    }

    separate_colors();
    
    return SUCCES;
}

error Png_loader::set_row_major_R_pixel_data(pixel *R_pixels)
{
    if(r_pixels) delete[] r_pixels;
    r_pixels = new pixel[height*width]();
    for(int y = 0; y < height*width; y++){
        r_pixels[y] = R_pixels[y];
    }
    
    return SUCCES;
}

error Png_loader::set_row_major_G_pixel_data(pixel *G_pixels)
{
    if(g_pixels) delete[] g_pixels;
    g_pixels = new pixel[height*width]();
    for(int y = 0; y < height*width; y++){
        g_pixels[y] = G_pixels[y];
    }
    return SUCCES;
}

error Png_loader::set_row_major_B_pixel_data(pixel *B_pixels)
{
    if(b_pixels) delete[] b_pixels;
    b_pixels = new pixel[height*width]();
    for(int y = 0; y < height*width; y++){
        b_pixels[y] = B_pixels[y];
    }
    return SUCCES;
}

error Png_loader::set_row_major_A_pixel_data(pixel *A_pixels)
{
    if(a_pixels) delete[] a_pixels;
    a_pixels = new pixel[height*width]();
    for(int y = 0; y < height*width; y++){
        a_pixels[y] = A_pixels[y];
    }
    return SUCCES;
}

error Png_loader::set_image_size(int width, int height)
{
    if(width <= 0 || height <= 0) return FAIL;

    this->width = width;
    this->height = height;

    return SUCCES;
}

Png_loader::~Png_loader()
{
    clear_data();
}