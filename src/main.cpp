#include <iostream>
#include <memory>

#include "../headers/png_loader.hpp"
#include "../headers/compute_module.hpp"

using namespace std;

int main(int argc, char **argv){
  if (argc < 3) {cout << "not enugh arguments! " << endl;
    cout << " usage: " << endl << endl;
    cout << "./convolution [input.png] [output.png]" << endl;
    return 1;
  }

  auto image = make_unique<Png_loader>();
  auto comp_module = make_unique<Compute_module>();

  if(image->read_png_file(argv[1])) cout << "failed to read " << argv[1] << endl;
  if(comp_module->setup_compute_module(image->get_height(),
                                       image->get_width(),
                                       image->get_row_major_R_pixel_data(),
                                       image->get_row_major_G_pixel_data(),
                                       image->get_row_major_B_pixel_data(),
                                       image->get_row_major_A_pixel_data())) 
    cout << "failed to setup compute module" << endl;

  if(comp_module->slow_cpu_edge_detection())cout << "failed to run task" << endl;
  
  vector<pixel*> data = comp_module->get_RGBA_vect_data();
  image->set_row_major_R_pixel_data(data[0]);
  image->set_row_major_G_pixel_data(data[1]);
  image->set_row_major_B_pixel_data(data[2]);
  image->set_row_major_A_pixel_data(data[3]);

  if(image->save_to_file(argv[2],true)) cout << "failed to write " << argv[2] << endl;
  cout << "done!" << endl;

  return 0;
}