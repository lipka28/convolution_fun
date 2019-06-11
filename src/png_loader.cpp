#include "../headers/png_loader.hpp"

Png_image Png_loader::get_png_image_data(const char *file_name)
{
    //clear_data();
    Png_image image_data = {0, 0, 0, 0};

    FILE *input_image = fopen(file_name, "rb");
    
    if(!input_image) {
        return image_data;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                nullptr, nullptr, nullptr);
    
    if(!png) {
        fclose(input_image); 
        return image_data;
    }

    png_infop info = png_create_info_struct(png);
    if(!info) {
        fclose(input_image);
        return image_data;
    }

    if(setjmp(png_jmpbuf(png))) {
        fclose(input_image);
         return image_data;
    }

    png_init_io(png, input_image);
    png_read_info(png, info);

    image_data.width       = png_get_image_width(png, info);
    image_data.height      = png_get_image_height(png, info);
    image_data.color_type  = png_get_color_type(png, info);
    image_data.bit_depth   = png_get_bit_depth(png, info);

    if(image_data.bit_depth == 16) {
        png_set_strip_16(png);
    }

    if(image_data.color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }

    if(image_data.color_type == PNG_COLOR_TYPE_GRAY && image_data.bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if(png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }

    if(image_data.color_type == PNG_COLOR_TYPE_RGB 
        || image_data.color_type == PNG_COLOR_TYPE_GRAY 
        || image_data.color_type == PNG_COLOR_TYPE_PALETTE) {

        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    if(image_data.color_type == PNG_COLOR_TYPE_GRAY 
        || image_data.color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    png_bytep *row_pointers = new png_bytep[image_data.height]();
    for (size_t y = 0; y < image_data.height; y++){
        row_pointers[y] = new png_byte[png_get_rowbytes(png, info)]();
    }

    png_read_image(png, row_pointers);
    separate_colors(image_data, row_pointers);

    if(row_pointers){
        for (size_t y = 0; y < image_data.height; y++){
            delete[] row_pointers[y];
        }
        delete[] row_pointers;
    }

    fclose(input_image);

    return image_data;
}

error Png_loader::save_to_file(Png_image const &img_data, const char *output_file)
{
    if(img_data.height == 0 || img_data.width == 0)
    {
        return FAIL;
    }

    png_bytep *row_pointers = new png_bytep[img_data.height]();
    for (size_t y = 0; y < img_data.height; y++){
        row_pointers[y] = new png_byte[img_data.width*4]();
    }

    merge_colors(row_pointers, img_data);

    FILE *output = fopen(output_file, "wb");
    if(!output) {
        return FAIL;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                            nullptr, nullptr, nullptr);
    
    if(!png) {
        fclose(output); 
        return FAIL;
    }

    png_infop info = png_create_info_struct(png);
    if(!info) {
        fclose(output); 
        return FAIL;
    }

    if(setjmp(png_jmpbuf(png))) {
        fclose(output); 
        return FAIL;
    }

    png_init_io(png, output);
    png_set_IHDR(
        png,
        info,
        img_data.width,
        img_data.height,
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

    if(row_pointers){
        for (size_t y = 0; y < img_data.height; y++){
            delete[] row_pointers[y];
        }
        delete[] row_pointers;
    }

    return SUCCES;
}

void Png_loader::separate_colors(Png_image &image_data, png_bytepp const &row_data)
{
    const size_t width = image_data.width;
    const size_t height = image_data.height;

    image_data.r_pixels.reserve(width*height);
    image_data.g_pixels.reserve(width*height);
    image_data.b_pixels.reserve(width*height);
    image_data.a_pixels.reserve(width*height);

    for (size_t y = 0; y < height; y++){
        for (size_t x = 0; x < width; x++){
            image_data.r_pixels[y*width+x] = row_data[y][x*4];
            image_data.g_pixels[y*width+x] = row_data[y][x*4+1];
            image_data.b_pixels[y*width+x] = row_data[y][x*4+2];
            image_data.a_pixels[y*width+x] = row_data[y][x*4+3];
        }
    }

}

void Png_loader::merge_colors(png_bytepp &row_data, Png_image const &image_data)
{
    const size_t width = image_data.width;
    const size_t height = image_data.height;
    
    for(size_t y = 0; y < height; y++){
        for(size_t x = 0; x < width; x++){
            row_data[y][x*4] = image_data.r_pixels[y*width+x];
            row_data[y][x*4+1] = image_data.g_pixels[y*width+x];
            row_data[y][x*4+2] = image_data.b_pixels[y*width+x];
            row_data[y][x*4+3] = image_data.a_pixels[y*width+x];
        }
    }

}