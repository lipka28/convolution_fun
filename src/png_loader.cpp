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

    fclose(input_image);

    return SUCCES;
}

error Png_loader::save_to_file(const char *output_file)
{
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
    }
}

int Png_loader::get_height() const
{
    return height;
}

int Png_loader::get_width() const
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

Png_loader::~Png_loader()
{
    clear_data();
}