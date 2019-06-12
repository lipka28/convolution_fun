#include <iostream>

#include "../headers/png_loader.hpp"
#include "../headers/compute_module.hpp"

//using namespace std;
using std::cout;
using std::endl;

int main(int argc, char **argv){
  if (argc < 3) {cout << "not enough arguments! " << endl;
    cout << " usage: " << endl << endl;
    cout << "./convolution [input.png] [output.png]" << endl;
    return 1;
  }

  cout << "started loading image" << endl;
  auto png_image = Png_loader::get_png_image_data(argv[1]);
  if(png_image->width == 0 || png_image->height == 0){
    cout << "Something broke on image load" << endl;
    return 2;
  }
  cout << "finished loading image" << endl;

  cout << "creating compute modue from image" << endl;
  auto comp_module = make_unique<Compute_module>(png_image);
  cout << "finished creating compute modue from image" << endl;

  cout << "started cpu compute" << endl;
  comp_module->slow_cpu_edge_detection();
  png_image = comp_module->get_processed_image();
  cout << "finished cpu compute" << endl;
  cout << "starting saving image" << endl;

  if(Png_loader::save_to_file(png_image, argv[2]))
  {
    cout << "something broke on image save" << endl;
    return 3;
  }
  cout << "finished saving image" << endl;
  cout << "Done!!! Everything OK" << endl;

  return 0;
}