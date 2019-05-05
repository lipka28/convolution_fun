#include <iostream>
#include <memory>
#include "../headers/png_loader.hpp"

using namespace std;

int main(int argc, char **argv){
  if (argc < 3) {cout << "not enugh arguments! " << endl;
    cout << " usage: " << endl;
    cout << "./convolution [input.png] [output.png]" << endl;
    return 1;
  }

  auto image = make_unique<Png_loader>();
  if(image->read_png_file(argv[1])) cout << "failed to read " << argv[1] << endl;
  if(image->save_to_file(argv[2])) cout << "failed to write " << argv[2] << endl;

  pixel *pixel_data  = image->get_row_major_RGBA_pixel_data();

  cout << "Width: " << image->get_width() << endl
       << " heigt: " << image->get_height() << endl
       << " total size of pixel data: " << pixel_data
       << endl << " counted size of pixel data: "
       << image->get_height() * image->get_width() * 4 << endl;

  return 0;
}