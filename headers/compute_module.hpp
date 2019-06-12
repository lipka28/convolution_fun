#pragma once
#include <vector>

#include "globals.hpp"

using namespace std;

class Compute_module
{
private:
    sp_Image original_image;
    sp_Image processed_image;


public:
    Compute_module(sp_Image const &source_image);
    ~Compute_module();
    sp_Image get_processed_image() const;
    error slow_cpu_edge_detection();
};
